#include "RigidBody.h"

#include "PhysicsSystem.h"

namespace PurahEngine
{
	RigidBody::RigidBody() : body()
	{

	}

	RigidBody::~RigidBody()
	{

	}

	void RigidBody::Awake()
	{
		// body = PhysicsSystem::GetInstance().CreateRigidBody();
	}

	Eigen::Vector3f RigidBody::GetPosition() const
	{
		return body->GetPosition();
	}

	void RigidBody::SetPosition(const Eigen::Vector3f& _position) noexcept
	{
		body->SetPosition(_position);
	}

	Eigen::Quaternionf RigidBody::GetQuaternion() const noexcept
	{
		return body->GetQuaternion();
	}

	void RigidBody::SetQuaternion(const Eigen::Quaternionf& _quaternion) noexcept
	{
		body->SetQuaternion(_quaternion);
	}

	void RigidBody::WakeUp() noexcept
	{
		body->WakeUp();
	}

	bool RigidBody::IsSleeping() const noexcept
	{
		return body->IsSleeping();
	}

	ZonaiPhysics::DynamicLocks RigidBody::GetDynamicLockFlags() const noexcept
	{
		return body->GetDynamicLockFlags();
	}

	void RigidBody::SetDynamicLockFlag(ZonaiPhysics::DynamicLock flag, bool value) noexcept
	{
		body->SetDynamicLockFlag(flag, value);
	}

	void RigidBody::SetDynamicLockFlags(ZonaiPhysics::DynamicLocks flags) noexcept
	{
		body->SetDynamicLockFlags(flags);
	}

	float RigidBody::GetMass() const noexcept
	{
		return  body->GetMass();
	}

	void RigidBody::SetMass(float _mass) noexcept
	{
		body->SetMass(_mass);
	}

	float RigidBody::GetLinearDamping() const noexcept
	{
		return body->GetLinearDamping();
	}

	void RigidBody::SetLinearDamping(float _damping) noexcept
	{
		body->SetLinearDamping(_damping);
	}

	float RigidBody::GetAngularDamping() const noexcept
	{
		return body->GetAngularDamping();
	}

	void RigidBody::SetAngularDamping(float _damping) noexcept
	{
		body->SetAngularDamping(_damping);
	}

	Eigen::Vector3f RigidBody::GetLinearVelocity() const noexcept
	{
		return body->GetLinearVelocity();
	}

	void RigidBody::SetLinearVelocity(const Eigen::Vector3f& _velocity) noexcept
	{
		body->SetLinearVelocity(_velocity);
	}

	Eigen::Vector3f RigidBody::GetAngularVelocity() const noexcept
	{
		return body->GetAngularVelocity();
	}

	void RigidBody::SetAngularVelocity(const Eigen::Vector3f& _velocity) noexcept
	{
		body->SetAngularVelocity(_velocity);
	}

	float RigidBody::GetMaxLinearVelocity() const noexcept
	{
		return body->GetMaxLinearVelocity();
	}

	void RigidBody::SetMaxLinearVelocity(const float& _velo) noexcept
	{
		body->SetMaxLinearVelocity(_velo);
	}

	float RigidBody::GetMaxAngularVelocity() const noexcept
	{
		return body->GetMaxAngularVelocity();
	}

	void RigidBody::SetMaxAngularVelocity(const float& _velo) noexcept
	{
		body->SetMaxAngularVelocity(_velo);
	}

	void RigidBody::AddForce(const Eigen::Vector3f& _force, ZonaiPhysics::ForceType _type) noexcept
	{
		body->AddForce(_force, _type);
	}

	void RigidBody::ClearForce() noexcept
	{
		body->ClearForce();
	}

	void RigidBody::AddTorque(const Eigen::Vector3f& _torque, ZonaiPhysics::ForceType _type) noexcept
	{
		body->AddTorque(_torque, _type);
	}

	void RigidBody::ClearTorque() noexcept
	{
		body->ClearTorque();
	}
}
