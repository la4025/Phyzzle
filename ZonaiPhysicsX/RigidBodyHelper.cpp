#include <PxRigidDynamic.h>
#include <extensions/PxRigidBodyExt.h>

#include "ZnUtil.h"
#include "RigidBody.h"

#include "RigidBodyHelper.h"

namespace ZonaiPhysics
{
	void RigidBodyHelper::Detach(void* _body, void* _shape) noexcept
	{
		assert(_body != nullptr);
		assert(_shape != nullptr);

		const auto pxBody = static_cast<physx::PxRigidDynamic*>(_body);
		const auto pxShape = static_cast<physx::PxShape*>(_shape);

		pxBody->detachShape(*pxShape);
	}

	void RigidBodyHelper::Attach(void* _body, void* _shape) noexcept
	{
		assert(_body != nullptr);
		assert(_shape != nullptr);

		const auto pxBody = static_cast<physx::PxRigidDynamic*>(_body);
		const auto pxShape = static_cast<physx::PxShape*>(_shape);

		pxBody->attachShape(*pxShape);
	}

	void RigidBodyHelper::Release(void* _body) noexcept
	{
		assert(_body != nullptr);

		static_cast<physx::PxRigidDynamic*>(_body)->release();
	}

	void RigidBodyHelper::WakeUp(void* _body) noexcept
	{
		assert(_body != nullptr);

		static_cast<physx::PxRigidDynamic*>(_body)->wakeUp();
	}

	bool RigidBodyHelper::IsSleeping(void* _body) noexcept
	{
		assert(_body != nullptr);

		return static_cast<physx::PxRigidDynamic*>(_body)->isSleeping();
	}

	void RigidBodyHelper::UseGravity(void* _body, bool _value) noexcept
	{
		assert(_body != nullptr);

		static_cast<physx::PxRigidDynamic*>(_body)->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, !_value);
	}

	void RigidBodyHelper::SetKinematic(void* _body, bool _value) noexcept
	{
		assert(_body != nullptr);

		static_cast<physx::PxRigidDynamic*>(_body)->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, _value);
	}

	void RigidBodyHelper::UpdateInertiaTensor(void* _body) noexcept
	{
		assert(_body != nullptr);

		physx::PxRigidBodyExt::updateMassAndInertia(*static_cast<physx::PxRigidDynamic*>(_body), 1.f);
	}

	void RigidBodyHelper::Disable(void* _body, bool _value)
	{
		assert(_body != nullptr);

		static_cast<physx::PxRigidDynamic*>(_body)->setActorFlag(physx::PxActorFlag::eDISABLE_SIMULATION, !_value);
	}

	uint8_t RigidBodyHelper::GetDynamicLockFlags(void* _body) noexcept
	{
		assert(_body != nullptr);

		return static_cast<physx::PxRigidDynamic*>(_body)->getRigidDynamicLockFlags();
	}

	void RigidBodyHelper::SetDynamicLockFlag(void* _body, uint8_t _flag, bool _value) noexcept
	{
		assert(_body != nullptr);

		static_cast<physx::PxRigidDynamic*>(_body)->setRigidDynamicLockFlag(
			static_cast<physx::PxRigidDynamicLockFlag::Enum>(_flag),
			_value
		);
	}

	void RigidBodyHelper::SetDynamicLockFlags(void* _body, uint8_t _flags) noexcept
	{
		assert(_body != nullptr);

		static_cast<physx::PxRigidDynamic*>(_body)->setRigidDynamicLockFlags(
			static_cast<physx::PxRigidDynamicLockFlags>(_flags)
		);
	}

	void* RigidBodyHelper::GetUserData(void* _body) noexcept
	{
		assert(_body != nullptr);

		return static_cast<physx::PxRigidDynamic*>(_body)->userData;
	}

	void RigidBodyHelper::SetUserData(void* _body, void* _userData) noexcept
	{
		assert(_body != nullptr);

		static_cast<physx::PxRigidDynamic*>(_body)->userData = _userData;
	}

	float RigidBodyHelper::GetMass(void* _body) noexcept
	{
		assert(_body != nullptr);

		return static_cast<physx::PxRigidDynamic*>(_body)->getMass();
	}

	void RigidBodyHelper::SetMass(void* _body, float _mass) noexcept
	{
		assert(_body != nullptr);

		const auto pxBody = static_cast<physx::PxRigidDynamic*>(_body);
		pxBody->setMass(_mass);
		physx::PxRigidBodyExt::setMassAndUpdateInertia(*pxBody, _mass);
	}

	float RigidBodyHelper::GetInvMass(void* _body) noexcept
	{
		assert(_body != nullptr);

		return static_cast<physx::PxRigidDynamic*>(_body)->getInvMass();
	}

	Eigen::Vector3f RigidBodyHelper::GetInertiaTensor(void* _body) noexcept
	{
		assert(_body != nullptr);

		return PhysxToEigen(static_cast<physx::PxRigidDynamic*>(_body)->getMassSpaceInertiaTensor());
	}

	void RigidBodyHelper::SetInertiaTensor(void* _body, const Eigen::Vector3f& _tensor) noexcept
	{
		assert(_body != nullptr);

		static_cast<physx::PxRigidDynamic*>(_body)->setMassSpaceInertiaTensor(EigenToPhysx(_tensor));
	}

	float RigidBodyHelper::GetLinearDamping(void* _body) noexcept
	{
		assert(_body != nullptr);

		return static_cast<physx::PxRigidDynamic*>(_body)->getLinearDamping();
	}

	void RigidBodyHelper::SetLinearDamping(void* _body, float _damping) noexcept
	{
		assert(_body != nullptr);

		static_cast<physx::PxRigidDynamic*>(_body)->setLinearDamping(_damping);
	}

	float RigidBodyHelper::GetAngularDamping(void* _body) noexcept
	{
		assert(_body != nullptr);

		return static_cast<physx::PxRigidDynamic*>(_body)->getAngularDamping();
	}

	void RigidBodyHelper::SetAngularDamping(void* _body, float _damping) noexcept
	{
		assert(_body != nullptr);

		static_cast<physx::PxRigidDynamic*>(_body)->setAngularDamping(_damping);
	}

	Eigen::Vector3f RigidBodyHelper::GetLinearVelocity(void* _body) noexcept
	{
		assert(_body != nullptr);

		return PhysxToEigen(static_cast<physx::PxRigidDynamic*>(_body)->getLinearVelocity());
	}

	void RigidBodyHelper::SetLinearVelocity(void* _body, const Eigen::Vector3f& _velo) noexcept
	{
		assert(_body != nullptr);

		static_cast<physx::PxRigidDynamic*>(_body)->setLinearVelocity(EigenToPhysx(_velo));
	}

	Eigen::Vector3f RigidBodyHelper::GetAngularVelocity(void* _body) noexcept
	{
		assert(_body != nullptr);

		return PhysxToEigen(static_cast<physx::PxRigidDynamic*>(_body)->getAngularVelocity());
	}

	void RigidBodyHelper::SetAngularVelocity(void* _body, const Eigen::Vector3f& _velo) noexcept
	{
		assert(_body != nullptr);

		static_cast<physx::PxRigidDynamic*>(_body)->setAngularVelocity(EigenToPhysx(_velo));
	}

	float RigidBodyHelper::GetMaxLinearVelocity(void* _body) noexcept
	{
		assert(_body != nullptr);

		return static_cast<physx::PxRigidDynamic*>(_body)->getMaxLinearVelocity();
	}

	void RigidBodyHelper::SetMaxLinearVelocity(void* _body, const float& _velo) noexcept
	{
		assert(_body != nullptr);

		static_cast<physx::PxRigidDynamic*>(_body)->setMaxLinearVelocity(_velo);
	}

	float RigidBodyHelper::GetMaxAngularVelocity(void* _body) noexcept
	{
		assert(_body != nullptr);

		return static_cast<physx::PxRigidDynamic*>(_body)->getMaxAngularVelocity();
	}

	void RigidBodyHelper::SetMaxAngularVelocity(void* _body, const float& _velo) noexcept
	{
		assert(_body != nullptr);

		static_cast<physx::PxRigidDynamic*>(_body)->setMaxAngularVelocity(_velo);
	}

	Eigen::Vector3f RigidBodyHelper::GetPosition(void* _body) noexcept
	{
		assert(_body != nullptr);

		return PhysxToEigen(static_cast<physx::PxRigidDynamic*>(_body)->getGlobalPose().p);
	}

	void RigidBodyHelper::SetPosition(void* _body, const Eigen::Vector3f& _pos) noexcept
	{
		assert(_body != nullptr);

		const auto pxBody = static_cast<physx::PxRigidDynamic*>(_body);
		physx::PxTransform t = pxBody->getGlobalPose();
		t.p = EigenToPhysx(_pos);
		pxBody->setGlobalPose(t);
	}

	Eigen::Quaternionf RigidBodyHelper::GetQuaternion(void* _body) noexcept
	{
		assert(_body != nullptr);

		return PhysxToEigen(static_cast<physx::PxRigidDynamic*>(_body)->getGlobalPose().q);
	}

	void RigidBodyHelper::SetQuaternion(void* _body, const Eigen::Quaternionf& _quat) noexcept
	{
		assert(_body != nullptr);

		const auto pxBody = static_cast<physx::PxRigidDynamic*>(_body);
		physx::PxTransform t = pxBody->getGlobalPose();
		t.q = EigenToPhysx(_quat);
		pxBody->setGlobalPose(t);
	}

	void RigidBodyHelper::SetForceAndTorque(void* _body, const Eigen::Vector3f& _force,
		const Eigen::Vector3f& _torque, ForceType _type) noexcept
	{
		assert(_body != nullptr);

		static_cast<physx::PxRigidDynamic*>(_body)->setForceAndTorque(
			EigenToPhysx(_force),
			EigenToPhysx(_torque),
			static_cast<physx::PxForceMode::Enum>(_type)
		);
	}

	void RigidBodyHelper::AddForce(void* _body, const Eigen::Vector3f& _force, ForceType _type) noexcept
	{
		assert(_body != nullptr);

		static_cast<physx::PxRigidDynamic*>(_body)->addForce(
			EigenToPhysx(_force),
			static_cast<physx::PxForceMode::Enum>(_type)
		);
	}

	void RigidBodyHelper::ClearForce(void* _body) noexcept
	{
		assert(_body != nullptr);

		static_cast<physx::PxRigidDynamic*>(_body)->clearForce();
	}

	void RigidBodyHelper::AddTorque(void* _body, const Eigen::Vector3f& _torque, ForceType _type) noexcept
	{
		assert(_body != nullptr);

		static_cast<physx::PxRigidDynamic*>(_body)->addTorque(
			EigenToPhysx(_torque),
			static_cast<physx::PxForceMode::Enum>(_type)
		);
	}

	void RigidBodyHelper::ClearTorque(void* _body) noexcept
	{
		assert(_body != nullptr);

		static_cast<physx::PxRigidDynamic*>(_body)->clearTorque();
	}
}
