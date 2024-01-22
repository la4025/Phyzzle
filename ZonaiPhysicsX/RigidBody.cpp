#include <Eigen/Dense>

#include "PxPhysicsAPI.h"
#include "ZnUtil.h"

#include "RigidBody.h"


namespace ZonaiPhysics
{
	RigidBody::RigidBody(physx::PxPhysics*& _factory) noexcept :
		pxBody()
	{
		using namespace physx;
		pxBody = _factory->createRigidDynamic(PxTransform(PxVec3(0,0,0)));

		assert(pxBody != nullptr, "ZonaiPhysicsX :: RigidBody Initialize Error");

		pxBody->userData = this;
	}

	RigidBody::~RigidBody() noexcept
	{
		pxBody->release();
		pxBody = nullptr;
	}

	void RigidBody::WakeUp() noexcept
	{
		assert(pxBody != nullptr);

		pxBody->wakeUp();
	}

	bool RigidBody::IsSleeping() const noexcept
	{
		assert(pxBody != nullptr);

		return pxBody->isSleeping();
	}

	void RigidBody::UseGravity(bool value) noexcept
	{
		assert(pxBody != nullptr);

		pxBody->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, !value);
	}

	void RigidBody::SetKinematic(bool value) noexcept
	{
		assert(pxBody != nullptr);

		pxBody->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, value);
	}

	void RigidBody::UpdateInertiaTensor() noexcept
	{
		assert(pxBody != nullptr);

		using namespace physx;
	}

	void RigidBody::CanSimulate(bool value) const noexcept
	{
		assert(pxBody != nullptr);

		pxBody->setActorFlag(physx::PxActorFlag::eDISABLE_SIMULATION, !value);
	}

	DynamicLocks RigidBody::GetDynamicLockFlags() const noexcept
	{
		assert(pxBody != nullptr);

		return static_cast<uint8_t>(pxBody->getRigidDynamicLockFlags());
	}

	void RigidBody::SetDynamicLockFlag(DynamicLock _flag, bool _value) noexcept
	{
		assert(pxBody != nullptr);

		pxBody->setRigidDynamicLockFlag(
			static_cast<physx::PxRigidDynamicLockFlag::Enum>(_flag), 
			_value
		);
	}

	void RigidBody::SetDynamicLockFlags(DynamicLocks _flags) noexcept
	{
		assert(pxBody != nullptr);

		pxBody->setRigidDynamicLockFlags(
			static_cast<physx::PxRigidDynamicLockFlags>(_flags)
		);
	}

	void* RigidBody::GetUserData() const noexcept
	{
		return this->userData;
	}

	void RigidBody::SetUserData(void* _userData) noexcept
	{
		this->userData = _userData;
	}

	float RigidBody::GetMass() const noexcept
	{
		assert(pxBody != nullptr);

		return pxBody->getMass();
	}

	void RigidBody::SetMass(float _mass) noexcept
	{
		assert(pxBody != nullptr);

		pxBody->setMass(_mass);
		pxBody->setMassSpaceInertiaTensor(pxBody->getCMassLocalPose().p);
	}

	float RigidBody::GetInvMass() const noexcept
	{
		assert(pxBody != nullptr);

		return pxBody->getInvMass();
	}

	Eigen::Vector3f RigidBody::GetInertiaTensor() const noexcept
	{
		assert(pxBody != nullptr);

		return PhysxToEigen(pxBody->getMassSpaceInertiaTensor());
	}

	void RigidBody::SetInertiaTensor(const Eigen::Vector3f& _I) noexcept
	{
		assert(pxBody != nullptr);

		pxBody->setMassSpaceInertiaTensor(EigenToPhysx(_I));
	}

	float RigidBody::GetLinearDamping() const noexcept
	{
		assert(pxBody != nullptr);

		return pxBody->getLinearDamping();
	}

	void RigidBody::SetLinearDamping(float _damping) noexcept
	{
		assert(pxBody != nullptr);

		pxBody->setLinearDamping(_damping);
	}

	float RigidBody::GetAngularDamping() const noexcept
	{
		assert(pxBody != nullptr);

		return pxBody->getAngularDamping();
	}

	void RigidBody::SetAngularDamping(float _damping) noexcept
	{
		assert(pxBody != nullptr);

		pxBody->setAngularDamping(_damping);
	}

	Eigen::Vector3f RigidBody::GetLinearVelocity() const noexcept
	{
		assert(pxBody != nullptr);

		return PhysxToEigen(pxBody->getLinearVelocity());
	}

	void RigidBody::SetLinearVelocity(const Eigen::Vector3f& _velocity) noexcept
	{
		assert(pxBody != nullptr);

		pxBody->setLinearVelocity(EigenToPhysx(_velocity));
	}

	Eigen::Vector3f RigidBody::GetAngularVelocity() const noexcept
	{
		assert(pxBody != nullptr);

		return PhysxToEigen(pxBody->getAngularVelocity());
	}

	void RigidBody::SetAngularVelocity(const Eigen::Vector3f& _velocity) noexcept
	{
		assert(pxBody != nullptr);

		pxBody->setAngularVelocity(EigenToPhysx(_velocity));
	}

	float RigidBody::GetMaxLinearVelocity() const noexcept
	{
		assert(pxBody != nullptr);

		return pxBody->getMaxLinearVelocity();
	}

	void RigidBody::SetMaxLinearVelocity(const float& _maxVelocity) noexcept
	{
		assert(pxBody != nullptr);

		pxBody->setMaxLinearVelocity(_maxVelocity);
	}

	float RigidBody::GetMaxAngularVelocity() const noexcept
	{
		assert(pxBody != nullptr);

		return pxBody->getMaxAngularVelocity();
	}

	void RigidBody::SetMaxAngularVelocity(const float& _maxVelocity) noexcept
	{
		assert(pxBody != nullptr);

		pxBody->setMaxAngularVelocity(_maxVelocity);
	}

	Eigen::Vector3f RigidBody::GetPosition() const noexcept
	{
		assert(pxBody != nullptr);

		return PhysxToEigen(pxBody->getGlobalPose().p);
	}

	void RigidBody::SetPosition(const Eigen::Vector3f& _position) noexcept
	{
		assert(pxBody != nullptr);

		using namespace physx;
		PxTransform t = pxBody->getGlobalPose();
		t.p = EigenToPhysx(_position);
		pxBody->setGlobalPose(t);
	}

	Eigen::Quaternionf RigidBody::GetQuaternion() const noexcept
	{
		assert(pxBody != nullptr);

		return PhysxToEigen(pxBody->getGlobalPose().q);
	}

	void RigidBody::SetQuaternion(const Eigen::Quaternionf& _quaternion) noexcept
	{
		assert(pxBody != nullptr);

		using namespace physx;
		PxTransform t = pxBody->getGlobalPose();
		t.q = EigenToPhysx(_quaternion);
		pxBody->setGlobalPose(t);
	}

	void RigidBody::SetForceAndTorque(const Eigen::Vector3f& _force, const Eigen::Vector3f& _torque,
		ForceType _type) noexcept
	{
		assert(pxBody != nullptr);

		pxBody->setForceAndTorque(
			EigenToPhysx(_force),
			EigenToPhysx(_torque),
			static_cast<physx::PxForceMode::Enum>(_type)
		);
	}

	void RigidBody::AddForce(const Eigen::Vector3f& _force, ForceType _type) noexcept
	{
		assert(pxBody != nullptr);

		pxBody->addForce(
			EigenToPhysx(_force), 
			static_cast<physx::PxForceMode::Enum>(_type)
		);
	}

	void RigidBody::ClearForce() noexcept
	{
		assert(pxBody != nullptr);

		pxBody->clearForce();
	}

	void RigidBody::AddTorque(const Eigen::Vector3f& _torque, ForceType _type) noexcept
	{
		assert(pxBody != nullptr);

		pxBody->addTorque(
			EigenToPhysx(_torque), 
			static_cast<physx::PxForceMode::Enum>(_type)
		);
	}

	void RigidBody::ClearTorque() noexcept
	{
		assert(pxBody != nullptr);

		pxBody->clearTorque();
	}

	//physx::PxRigidDynamic* RigidBody::getRigidDynamic() const noexcept
	//{
	//	return pxBody;
	//}
} // namespace ZonaiPhysics