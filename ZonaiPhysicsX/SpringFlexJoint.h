#pragma once

#include "PxPhysicsAPI.h"

namespace ZonaiPhysics
{
	class SpringFlexJoint : public physx::PxConstraintConnector
	{
	public:

		static const physx::PxU32 TYPE_ID = physx::PxConcreteType::eFIRST_USER_EXTENSION;

		SpringFlexJoint(physx::PxPhysics& physics,
			physx::PxRigidBody& body0, const physx::PxTransform& localFrame0,
			physx::PxRigidBody& body1, const physx::PxTransform& localFrame1);

		void release();

		// Attribute accessor and mutators
		void setSpringStiffness(physx::PxReal stiffness);
		physx::PxReal getSpringStiffness() const;

		void setSpringDamping(physx::PxReal damping);
		physx::PxReal getSpringDamping() const;

		void setLinearLimit(physx::PxReal limit);
		physx::PxReal getLinearLimit() const;

		// PxConstraintConnector boilerplate
		void* prepareData();
		void onConstraintRelease();
		void onComShift(physx::PxU32 actor);
		void onOriginShift(const physx::PxVec3& shift);
		void* getExternalReference(physx::PxU32& typeID);

		bool updatePvdProperties(physx::pvdsdk::PvdDataStream&,
			const physx::PxConstraint*,
			physx::PxPvdUpdateType::Enum) const {
			return true;
		}
		void updateOmniPvdProperties() const { }
		physx::PxBase* getSerializable() { return NULL; }

		virtual physx::PxConstraintSolverPrep getPrep() const;
		virtual const void* getConstantBlock() const { return &mData; }

		struct CustomSpringJointData
		{
			physx::PxTransform c2b[2];
			physx::PxReal stiffness;
			physx::PxReal damping;
			physx::PxReal linearLimit;
		};

		physx::PxRigidBody* mBody[2];
		physx::PxTransform mLocalPose[2];

		physx::PxConstraint* mConstraint;
		CustomSpringJointData mData;

		~SpringFlexJoint() {}
	};
}

