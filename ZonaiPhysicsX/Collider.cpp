#include "RigidBody.h"
#include "PxPhysicsAPI.h"
#include "ZnLayer.h"


#include "ZnUtil.h"
#include "ColliderHelper.h"

#include "Collider.h"

namespace ZonaiPhysics
{
	Collider::Collider(physx::PxPhysics*& _factory, RigidBody* _rigid) noexcept :
		rigidbody(_rigid), shape()
	{

	}

	void Collider::SetTrigger(bool _flag) noexcept
	{
		// rigidbody->pxBody->detachShape(*shape);

		ColliderHelper::SetTrigger(shape, _flag);

		// rigidbody->pxBody->attachShape(*shape);
	}

	void Collider::SetLayerData(const uint32_t& _id) noexcept
	{
		ColliderHelper::SetLayer(shape, _id);
	}

	/// <summary>
	/// 이건 강체의 위치를 바꾸는건데...
	/// 이게 맞을까?
	/// </summary>
	Eigen::Vector3f Collider::GetPosition() const noexcept
	{
		assert(rigidbody != nullptr);

		using namespace physx;
		return rigidbody->GetPosition();
	}

	void Collider::SetPosition(const Eigen::Vector3f& _position) noexcept
	{
		assert(rigidbody != nullptr);

		using namespace physx;
		rigidbody->SetPosition(_position);
	}

	Eigen::Vector3f Collider::GetLocalPosition() const noexcept
	{
		assert(shape != nullptr);

		return ColliderHelper::GetLocalPosition(shape);
	}

	void Collider::SetLocalPosition(const Eigen::Vector3f& _position) noexcept
	{
		assert(rigidbody != nullptr);

		using namespace physx;
		rigidbody->pxBody->detachShape(*shape);

		PxTransform t = shape->getLocalPose();
		// t.p.x = _position.x();
		// t.p.y = _position.y();
		// t.p.z = _position.z();
		t.p = EigenToPhysx(_position);
		shape->setLocalPose(t);

		rigidbody->pxBody->attachShape(*shape);

		UpdateInertiaTensor();
	}

	Eigen::Quaternionf Collider::GetQuaternion() const noexcept
	{
		return rigidbody->GetQuaternion();
	}

	void Collider::SetQuaternion(const Eigen::Quaternionf& _quaternion) noexcept
	{
		rigidbody->SetQuaternion(_quaternion);
	}

	Eigen::Quaternionf Collider::GetLocalQuaternion() const noexcept
	{
		return ColliderHelper::GetLocalQuaternion(shape);
	}

	void Collider::SetLocalQuaternion(const Eigen::Quaternionf& _quaternion) noexcept
	{
		// rigidbody->pxBody->detachShape(*shape);

		ColliderHelper::SetLocalQuaternion(shape, _quaternion);

		// rigidbody->pxBody->attachShape(*shape);

		UpdateInertiaTensor();
	}

	void* Collider::GetUserData() const noexcept
	{
		return  userData;
	}

	void Collider::SetUserData(void* _userData) noexcept
	{
		userData = _userData;
	}

	void Collider::UpdateInertiaTensor() const noexcept
	{
		assert(shape != nullptr && rigidbody != nullptr);

		// auto pos = shape->getLocalPose().p;
		// rigidbody->pxBody->setCMassLocalPose(shape->getLocalPose());
	}
} // namespace ZonaiPhysics