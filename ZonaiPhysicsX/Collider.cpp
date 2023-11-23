#include "Collider.h"

#include <windows.h>

#include "ZonaiMath.h"
#include "PxPhysicsAPI.h"

namespace ZonaiPhysics
{
	Collider::Collider(physx::PxPhysics*& _factory) noexcept
	{
		using namespace physx;
		shape_ = _factory->createShape(

		)
	}

	Collider::~Collider() noexcept
	{

	}

	Vector3D Collider::GetPosition() const noexcept
	{
		using namespace physx;
		PxVec3 v = rigid_->getGlobalPose().p;

		return Vector3D(v.x, v.y, v.z);
	}

	void Collider::SetPosition(const Vector3D& _position) noexcept
	{

	}

	Vector3D Collider::GetLocalPosition() const noexcept
	{
	}

	void Collider::SetLocalPosition(const Vector3D& _position) noexcept
	{
	}

	Quaternion Collider::GetQuaternion() const noexcept
	{

	}

	void Collider::SetQuaternion(const Quaternion& _quaternion) noexcept
	{

	}

	Quaternion Collider::GetLocalQuaternion() const noexcept
	{
	}

	void Collider::SetLocalQuaternion(const Quaternion& _quaternion) noexcept
	{
	}

	void* Collider::GetUserData() const noexcept
	{

	}

	void Collider::SetUserData(void* _userData) noexcept
	{

	}
} // namespace ZonaiPhysics