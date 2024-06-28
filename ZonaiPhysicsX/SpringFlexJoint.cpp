#include <assert.h>
#include "PxConstraint.h"

#include "SpringFlexJoint.h"

using namespace physx;

namespace ZonaiPhysics
{
	static PxU32 solverPrep(Px1DConstraint* constraints,
		PxVec3p& body0WorldOffset,
		PxU32 maxConstraints,
		PxConstraintInvMassScale& invMassScale,
		const void* constantBlock,
		const PxTransform& bA2w,
		const PxTransform& bB2w,
		bool useExtendedLimits,
		PxVec3p& cA2wOut, PxVec3p& cB2wOut)
	{
		PX_UNUSED(maxConstraints);

		const SpringFlexJoint::CustomSpringJointData& data = *reinterpret_cast<const SpringFlexJoint::CustomSpringJointData*>(constantBlock);

		PxTransform cA2w = bA2w.transform(data.c2b[0]);
		PxTransform cB2w = bB2w.transform(data.c2b[1]);

		cA2wOut = cA2w.p;
		cB2wOut = cB2w.p;

		body0WorldOffset = cB2w.p - bA2w.p;

		Px1DConstraint* c = constraints;

		// 선형 스프링과 구 형태 제한 적용
		PxVec3 linearDirection = cB2w.p - cA2w.p;
		PxReal linearDistance = linearDirection.magnitude();
		PxReal linearError = 0.0f;

		if (linearDistance > data.linearLimit) 
		{
			// 위치가 제한을 초과할 경우 제한을 넘은 거리 계산
			linearError = data.linearLimit - linearDistance;
			linearDirection.normalize();	// 방향 벡터 정규화
		}

		c->linear0 = linearDirection;
		c->angular0 = (cA2w.p - bA2w.p).cross(c->linear0);		// 각속도 계산
		c->linear1 = -linearDirection;
		c->angular1 = (cB2w.p - bB2w.p).cross(c->linear1);
		c->geometricError = linearError;
		c->mods.spring.stiffness = data.stiffness;				// 스프링 강성
		c->mods.spring.damping = data.damping;					// 스프링 감쇠
		c->minImpulse = -PX_MAX_F32;
		c->maxImpulse = PX_MAX_F32;
		c->flags = Px1DConstraintFlag::eSPRING;					// 스프링 플래그 설정
		++c;

		// 제한 없는 각 스프링 적용
		PxQuat relativeRotation = cB2w.q.getConjugate() * cA2w.q;
		PxVec3 angularAxis;
		PxReal angularAngle;
		relativeRotation.toRadiansAndUnitAxis(angularAngle, angularAxis);	// 각도와 축을 계산

		c->linear0 = PxVec3(0.0f);
		c->angular0 = angularAxis;								// 회전 축 설정
		c->linear1 = PxVec3(0.0f);
		c->angular1 = -angularAxis;								// 반대 회전 축 설정
		c->geometricError = -angularAngle;						// 각도 에러 설정
		c->mods.spring.stiffness = data.stiffness;				// 스프링 강성
		c->mods.spring.damping = data.damping;					// 스프링 감쇠
		c->minImpulse = -PX_MAX_F32;
		c->maxImpulse = PX_MAX_F32;
		c->flags = Px1DConstraintFlag::eSPRING;					// 스프링 플래그 설정

		return 2;	// 생성된 제약 조건 수 반환							
	}

	static void visualize(PxConstraintVisualizer& viz,
		const void* constantBlock,
		const PxTransform& body0Transform,
		const PxTransform& body1Transform,
		PxU32 flags)
	{
		PX_UNUSED(flags);
		const SpringFlexJoint::CustomSpringJointData& data = *reinterpret_cast<const SpringFlexJoint::CustomSpringJointData*>(constantBlock);

		PxTransform cA2w = body0Transform * data.c2b[0];
		PxTransform cB2w = body1Transform * data.c2b[1];
		viz.visualizeJointFrames(cA2w, cB2w);
	}

	static PxConstraintShaderTable sShaderTable = { solverPrep, visualize, PxConstraintFlag::Enum(0) };

	PxConstraintSolverPrep SpringFlexJoint::getPrep() const { return solverPrep; }

	SpringFlexJoint::SpringFlexJoint(physx::PxPhysics& physics, PxRigidBody& body0, const PxTransform& localFrame0,
		PxRigidBody& body1, const PxTransform& localFrame1)
	{
		mConstraint = physics.createConstraint(&body0, &body1, *this, sShaderTable, sizeof(CustomSpringJointData));

		mBody[0] = &body0;
		mBody[1] = &body1;

		mLocalPose[0] = localFrame0.getNormalized();
		mLocalPose[1] = localFrame1.getNormalized();

		mData.stiffness = 1000.0f;
		mData.damping = 10.0f;
		mData.linearLimit = 1.0f;
		mData.c2b[0] = body0.getCMassLocalPose().transformInv(mLocalPose[0]);
		mData.c2b[1] = body1.getCMassLocalPose().transformInv(mLocalPose[1]);
	}

	void SpringFlexJoint::release()
	{
		mConstraint->release();
	}

	void SpringFlexJoint::setSpringStiffness(physx::PxReal stiffness)
	{
		mData.stiffness = stiffness;
		mConstraint->markDirty();
	}

	physx::PxReal SpringFlexJoint::getSpringStiffness() const
	{
		return mData.stiffness;
	}

	void SpringFlexJoint::setSpringDamping(physx::PxReal damping)
	{
		mData.damping = damping;
		mConstraint->markDirty();
	}

	physx::PxReal SpringFlexJoint::getSpringDamping() const
	{
		return mData.damping;
	}

	void SpringFlexJoint::setLinearLimit(physx::PxReal limit)
	{
		mData.linearLimit = limit;
		mConstraint->markDirty();
	}

	physx::PxReal SpringFlexJoint::getLinearLimit() const
	{
		return mData.linearLimit;
	}

	void* SpringFlexJoint::prepareData()
	{
		return &mData;
	}

	void SpringFlexJoint::onConstraintRelease()
	{
		delete this;
	}

	void SpringFlexJoint::onComShift(PxU32 actor)
	{
		mData.c2b[actor] = mBody[actor]->getCMassLocalPose().transformInv(mLocalPose[actor]);
		mConstraint->markDirty();
	}

	void SpringFlexJoint::onOriginShift(const PxVec3& shift)
	{
		// Handle any necessary adjustments for origin shifts
	}

	void* SpringFlexJoint::getExternalReference(PxU32& typeID)
	{
		typeID = TYPE_ID;
		return this;
	}
}