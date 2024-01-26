#pragma once
#include <string>
#include <Eigen/Dense>
#include "ZnBase.h"

namespace ZonaiPhysics
{
	class ZnSimulationCallback;
	class ZnPrismaticJoint;
	class ZnHingeJoint;
	class ZnDistanceJoint;
	class ZnSphericalJoint;
	class ZnFixedJoint;
}

namespace ZonaiPhysics
{
	class FixedJoint;

	class ZnBase;
	class ZnRigidBody;
	class ZnSoftBody;
	class ZnCollider;
	class ZnJoint;

	class ZnTransform;
	struct ZnRaycastInfo;

	class ZnPhysicsBase : public ZnBase
	{
	public:
		ZnPhysicsBase() noexcept = default;
		virtual ~ZnPhysicsBase() noexcept = default;
	
	public:
		/// <summary>
		/// 
		/// </summary>
		virtual void				Initialize(ZnSimulationCallback* = nullptr) noexcept = 0;
		virtual void				Simulation(float _dt) noexcept = 0;
		virtual void				Finalize() noexcept = 0;

		virtual void				SetGravity(const Eigen::Vector3f&) noexcept = 0;

	public:
		///	wstring은 void*로 변경될 예정.

		/// <summary>
		/// Create RigidBoby
		/// </summary>
		virtual ZnRigidBody*		CreateRigidBody(const std::wstring&) noexcept = 0;

		/// <summary>
		/// Create Collider
		/// </summary>
		virtual ZnCollider*			CreateBoxCollider(const std::wstring&, float x, float y, float z) noexcept = 0;
		virtual ZnCollider*			CreateSphereCollider(const std::wstring&, float radius) noexcept = 0;
		virtual ZnCollider*			CreateCapsuleCollider(const std::wstring&, float radius, float height) noexcept = 0;
		// virtual ZnCollider*		CreateCustomCollider(const std::wstring&) noexcept = 0;

		/// <summary>
		/// Create Joint
		/// </summary>
		// 
		// virtual ZnD6Joint*		CreatD6Joint(ZnRigidBody*, ZnTransform, ZnRigidBody*, ZnTransform) noexcept = 0;			// D6 조인트		*사실 뭔지 모름
		// 고정 조인트
		virtual ZnFixedJoint*		CreateFixedJoint(ZnRigidBody*, const ZnTransform&, ZnRigidBody*, const ZnTransform&) noexcept = 0;
		// 거리 조인트
		virtual ZnDistanceJoint*	CreateDistanceJoint(ZnRigidBody*, const ZnTransform&, ZnRigidBody*, const ZnTransform&) noexcept = 0;
		// 구형 조인트
		virtual ZnSphericalJoint*	CreateSphericalJoint(ZnRigidBody*, const ZnTransform&, ZnRigidBody*, const ZnTransform&) noexcept = 0;
		// 특정 축에 회전만 허용하는 조인트 (이동 X)
		virtual ZnHingeJoint*		CreateHingeJoint(ZnRigidBody*, const ZnTransform&, ZnRigidBody*, const ZnTransform&) noexcept = 0;
		// 특정 축에 이동만 허용하는 조인트 (회전 X)
		virtual ZnPrismaticJoint*	CreatePrismaticJoint(ZnRigidBody*, const ZnTransform&, ZnRigidBody*, const ZnTransform&) noexcept = 0;

	public:
		virtual bool Raycast(const Eigen::Vector3f& _from, const Eigen::Vector3f& _to, float _distance, ZnRaycastInfo& _out) noexcept = 0;
	};
}
