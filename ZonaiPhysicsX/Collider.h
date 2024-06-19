#pragma once
#pragma warning(push)
#pragma warning(disable: 33010 26495 4819)
#include <Eigen/Dense>
#pragma warning (pop)

#include "ZnCollider.h"

namespace physx
{
	class PxPhysics;
	class PxShape;
	class PxRigidDynamic;
	class PxRigidActor;
}

namespace ZonaiPhysics
{
	struct ZnTransform;
	class RigidBody;

	class Collider : public ZnCollider
	{
	public:
									Collider() = delete;
									Collider(physx::PxShape*, RigidBody*);
									//Collider(physx::PxPhysics*&, RigidBody*);
									~Collider() override;

	public:
		void						DisableQuery(bool) final;
		void						DisableSimulation(bool) final;
		void						SetTrigger(bool) final;

		void						SetLayerData(const uint32_t&) final;

		/**
		위치
		*/
		Eigen::Vector3f				GetPosition() const final;
		void						SetPosition(const Eigen::Vector3f& _position) final;
		Eigen::Vector3f				GetLocalPosition() const final;
		void						SetLocalPosition(const Eigen::Vector3f& _position) final;

		/**
		회전
		*/
		Eigen::Quaternionf			GetQuaternion() const final;
		void						SetQuaternion(const Eigen::Quaternionf& _quaternion) final;
		Eigen::Quaternionf			GetLocalQuaternion() const final;
		void						SetLocalQuaternion(const Eigen::Quaternionf& _quaternion) final;

		/**
		유저 데이터
		*/
		void*						GetUserData() const final;
		void						SetUserData(void* _userData) final;

		ZnBound3					GetBoundingBox(const ZnTransform& _trans) override;

	public:

		void						UpdateInertiaTensor() const;

	protected:
		friend class ZnPhysicsX;
		RigidBody* znBody;

	public:
		physx::PxShape* pxShape;
	};
} // namespace ZonaiPhysics

