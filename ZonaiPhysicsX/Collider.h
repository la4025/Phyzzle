#pragma once
#include "ZnCollider.h"

namespace physx
{
	class PxPhysics;

	class PxShape;
	class PxRigidDynamic;
	class PxRigidActor;
}


namespace ZonaiMath
{
	class Vector3D;
	class Quaternion;
}

namespace ZonaiPhysics
{
	class Collider : public ZnCollider
	{
	public:
		Collider() noexcept = delete;
		Collider(physx::PxPhysics*&) noexcept;
		virtual				~Collider() noexcept;

	public:
		/**
		위치
		*/
		virtual Vector3D	GetPosition() const noexcept override;
		virtual void		SetPosition(const Vector3D& _position) noexcept override;
		virtual Vector3D	GetLocalPosition() const noexcept override;
		virtual void		SetLocalPosition(const Vector3D& _position) noexcept override;

		/**
		회전
		*/
		virtual Quaternion	GetQuaternion() const noexcept override;
		virtual void		SetQuaternion(const Quaternion& _quaternion) noexcept override;
		virtual Quaternion	GetLocalQuaternion() const noexcept override;
		virtual void		SetLocalQuaternion(const Quaternion& _quaternion) noexcept override;


		/**
		유저 데이터
		*/
		virtual void*		GetUserData() const noexcept override;
		virtual void		SetUserData(void* _userData) noexcept override;

	public:
		__declspec(property(get = GetPosition, put = SetPosition))
			Vector3D position;

		__declspec(property(get = GetQuaternion, put = SetQuaternion))
			Quaternion quaternion;

	private:
		physx::PxRigidDynamic* rigid_;
		physx::PxShape* shape_;
	};
} // namespace ZonaiPhysics

