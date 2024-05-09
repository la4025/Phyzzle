#pragma once
#include <string>
#include <Eigen/Dense>

#include "MaterialEnum.h"
#include "ZnResourceID.h"
#include "ZnMaterial.h"
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

	struct MaterialDesc;

	class ZnPhysicsBase : public ZnBase
	{
	public:
		virtual						~ZnPhysicsBase() = default;
	
	public:
		virtual void				Initialize(ZnSimulationCallback* = nullptr) = 0;
		virtual void				Simulation(float _dt) = 0;
		virtual void				Finalize() = 0;

		virtual ZnMaterialID		AddMaterial(const MaterialDesc& _desc) = 0;

		virtual void				CreateScene(void* _userScene, const Eigen::Vector3f& _gravity) = 0;
		virtual void				LoadScene(void* _userScene) = 0;
		virtual void				UnloadScene(void* _userScene) = 0;

	public:
		virtual Eigen::Vector3f		GetGravity(void* _userScene = nullptr) = 0;
		virtual void				SetGravity(const Eigen::Vector3f& _gravity, void* _userScene = nullptr) = 0;
		virtual void				SetCollisionLayer(uint32_t _layer, uint32_t _collision, bool _value) = 0;
		virtual void				SetCollisionLayerData(uint32_t _layer, const std::initializer_list<uint32_t>& _data) = 0;

	public:
		virtual void				ReleaseRigidBody(ZnRigidBody*, void* _userData, void* _userScene = nullptr) = 0;
		virtual void				ReleaseCollider(ZnCollider*, void* _userData, void* _userScene = nullptr) = 0;
		virtual void				ReleaseJoint(ZnJoint*, void* _userData, void* _userScene = nullptr) = 0;

	public:
		/// <summary>
		/// Create RigidBoby
		/// </summary>
		virtual ZnRigidBody*		CreateRigidBody(void* _userData, void* _userScene = nullptr) = 0;

		/// <summary>
		/// Create Collider
		/// </summary>
		virtual ZnCollider*			CreateBoxCollider(
			void* _userData, 
			const Eigen::Vector3f& extend, 
			const ZnMaterialID& _material, 
			void* userScene = nullptr) = 0;

		virtual ZnCollider*			CreateSphereCollider(
			void* _userData, 
			float _radius, 
			const ZnMaterialID& _material, 
			void* userScene = nullptr) = 0;

		virtual ZnCollider*			CreateCapsuleCollider(
			void* _userData, 
			float _radius, 
			float _height, 
			const ZnMaterialID& _material, 
			void* userScene = nullptr) = 0;

		virtual ZnCollider*			CreateMeshCollider(
			void* _userData, 
			const ZnMeshID&,
			const Eigen::Quaternionf& _rot,
			const Eigen::Vector3f&,
			const ZnMaterialID&, 
			void* userScene = nullptr) = 0;

		virtual ZnCollider*			CreateConvexCollider(
			void* _userData, 
			const ZnConvexID&,
			const Eigen::Quaternionf&,
			const Eigen::Vector3f&,
			const ZnMaterialID&, 
			void* userScene = nullptr) = 0;

		/// <summary>
		/// Create Joint
		/// </summary>
		// 고정 조인트
		virtual ZnFixedJoint*		CreateFixedJoint(ZnRigidBody*, const ZnTransform&, ZnRigidBody*, const ZnTransform&) = 0;
		// 거리 조인트
		virtual ZnDistanceJoint*	CreateDistanceJoint(ZnRigidBody*, const ZnTransform&, ZnRigidBody*, const ZnTransform&) = 0;
		// 구형 조인트
		virtual ZnSphericalJoint*	CreateSphericalJoint(ZnRigidBody*, const ZnTransform&, ZnRigidBody*, const ZnTransform&) = 0;
		// 특정 축에 회전만 허용하는 조인트 (이동 X)
		virtual ZnHingeJoint*		CreateHingeJoint(ZnRigidBody*, const ZnTransform&, ZnRigidBody*, const ZnTransform&) = 0;
		// 특정 축에 이동만 허용하는 조인트 (회전 X)
		virtual ZnPrismaticJoint*	CreatePrismaticJoint(ZnRigidBody*, const ZnTransform&, ZnRigidBody*, const ZnTransform&) = 0;

	public:
		virtual bool				Raycast(const Eigen::Vector3f& _from, const Eigen::Vector3f& _to, float _distance, ZnRaycastInfo& _out) = 0;
	};
}
