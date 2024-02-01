#include <Eigen/Dense>

#include "PxPhysicsAPI.h"


#include "ZnUtil.h"
#include "RigidBodyHelper.h"
#include "RigidBody.h"


namespace ZonaiPhysics
{
	RigidBody::RigidBody() noexcept : pxBody()
	{}

	RigidBody::~RigidBody() noexcept
	{
		RigidBodyHelper::Release(pxBody);
		pxBody = nullptr;
	}

	void RigidBody::WakeUp() noexcept
	{
		RigidBodyHelper::WakeUp(pxBody);
	}

	bool RigidBody::IsSleeping() const noexcept
	{
		return RigidBodyHelper::IsSleeping(pxBody);
	}

	void RigidBody::UseGravity(bool value) noexcept
	{
		RigidBodyHelper::UseGravity(pxBody, value);
	}

	void RigidBody::SetKinematic(bool value) noexcept
	{
		RigidBodyHelper::SetKinematic(pxBody, value);
	}

	void RigidBody::Disable(bool value) const noexcept
	{
		RigidBodyHelper::Disable(pxBody, value);
	}

	uint8_t RigidBody::GetDynamicLockFlags() const noexcept
	{
		return RigidBodyHelper::GetDynamicLockFlags(pxBody);
	}

	void RigidBody::SetDynamicLockFlag(FreezeFlag _flag, bool _value) noexcept
	{
		RigidBodyHelper::SetDynamicLockFlag(pxBody, _flag, _value);
	}

	void RigidBody::SetDynamicLockFlags(uint8_t _flags) noexcept
	{
		RigidBodyHelper::SetDynamicLockFlags(pxBody, _flags);
	}

	void* RigidBody::GetUserData() const noexcept
	{
		return RigidBodyHelper::GetUserData(pxBody);
	}

	void RigidBody::SetUserData(void* _userData) noexcept
	{
		RigidBodyHelper::SetUserData(pxBody, _userData);
	}

	float RigidBody::GetMass() const noexcept
	{
		return RigidBodyHelper::GetMass(pxBody);
	}

	void RigidBody::SetMass(float _mass) noexcept
	{
		assert(pxBody != nullptr);

		RigidBodyHelper::SetMass(pxBody, _mass);
	}

	float RigidBody::GetInvMass() const noexcept
	{
		return RigidBodyHelper::GetInvMass(pxBody);
	}

	Eigen::Vector3f RigidBody::GetInertiaTensor() const noexcept
	{
		return RigidBodyHelper::GetInertiaTensor(pxBody);
	}

	float RigidBody::GetLinearDamping() const noexcept
	{
		return RigidBodyHelper::GetLinearDamping(pxBody);
	}

	void RigidBody::SetLinearDamping(float _damping) noexcept
	{
		RigidBodyHelper::SetLinearDamping(pxBody, _damping);
	}

	float RigidBody::GetAngularDamping() const noexcept
	{
		return RigidBodyHelper::GetAngularDamping(pxBody);
	}

	void RigidBody::SetAngularDamping(float _damping) noexcept
	{
		RigidBodyHelper::SetAngularDamping(pxBody, _damping);
	}

	Eigen::Vector3f RigidBody::GetLinearVelocity() const noexcept
	{
		return RigidBodyHelper::GetLinearVelocity(pxBody);

		return PhysxToEigen(pxBody->getLinearVelocity());
	}

	void RigidBody::SetLinearVelocity(const Eigen::Vector3f& _velocity) noexcept
	{
		RigidBodyHelper::SetLinearVelocity(pxBody, _velocity);
	}

	Eigen::Vector3f RigidBody::GetAngularVelocity() const noexcept
	{
		return RigidBodyHelper::GetAngularVelocity(pxBody);
	}

	void RigidBody::SetAngularVelocity(const Eigen::Vector3f& _velocity) noexcept
	{
		RigidBodyHelper::SetAngularVelocity(pxBody, _velocity);
	}

	float RigidBody::GetMaxLinearVelocity() const noexcept
	{
		return RigidBodyHelper::GetMaxLinearVelocity(pxBody);
	}

	void RigidBody::SetMaxLinearVelocity(const float& _maxVelocity) noexcept
	{
		RigidBodyHelper::SetMaxLinearVelocity(pxBody, _maxVelocity);
	}

	float RigidBody::GetMaxAngularVelocity() const noexcept
	{
		return RigidBodyHelper::GetMaxAngularVelocity(pxBody);
	}

	void RigidBody::SetMaxAngularVelocity(const float& _maxVelocity) noexcept
	{
		RigidBodyHelper::SetMaxAngularVelocity(pxBody, _maxVelocity);
	}

	Eigen::Vector3f RigidBody::GetPosition() const noexcept
	{
		return RigidBodyHelper::GetPosition(pxBody);
	}

	void RigidBody::SetPosition(const Eigen::Vector3f& _position) noexcept
	{
		RigidBodyHelper::SetPosition(pxBody, _position);
	}

	Eigen::Quaternionf RigidBody::GetQuaternion() const noexcept
	{
		return RigidBodyHelper::GetQuaternion(pxBody);
	}

	void RigidBody::SetQuaternion(const Eigen::Quaternionf& _quaternion) noexcept
	{
		RigidBodyHelper::SetQuaternion(pxBody, _quaternion);
	}

	void RigidBody::AddForce(const Eigen::Vector3f& _force, ForceType _type) noexcept
	{
		RigidBodyHelper::AddForce(pxBody, _force, _type);
	}

	void RigidBody::ClearForce() noexcept
	{
		RigidBodyHelper::ClearForce(pxBody);
	}

	void RigidBody::AddTorque(const Eigen::Vector3f& _torque, ForceType _type) noexcept
	{
		RigidBodyHelper::AddTorque(pxBody, _torque, _type);
	}

	void RigidBody::ClearTorque() noexcept
	{
		RigidBodyHelper::ClearTorque(pxBody);
	}
} // namespace ZonaiPhysics