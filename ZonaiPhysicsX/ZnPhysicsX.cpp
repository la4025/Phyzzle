#include "RigidBody.h"

#include "BoxCollider.h"
#include "SphereCollider.h"
#include "CapsuleCollider.h"

#include "FixedJoint.h"
#include "DistanceJoint.h"
#include "HingeJoint.h"
#include "PrismaticJoint.h"
#include "SphericalJoint.h"

#include "ZnRaycastInfo.h"
#include "FilterCallback.h"

#include "ZnPhysicsX.h"

#include <ranges>

#include "ZnWorld.h"


namespace ZonaiPhysics
{
	ZnPhysicsX* ZnPhysicsX::instance = nullptr;

	void ZnPhysicsX::Initialize(ZnSimulationCallback* _instance) noexcept
	{
		assert(_instance);

		// SDK 생성
		{
			ZnFactoryX::CreatePhysxFactory();
		}

		{
			ZnFactoryX::SetSimulationCallback(_instance);
		}

		// 레이어 설정
		{
			ZnLayer::Clear();
			ZnLayer::SetCollisionData(0, {0, 1, 2, 3});
		}

		defaultMaterial = ZnFactoryX::CreateMaterial(0.5f, 0.5f, 0.6f);
	}

	void ZnPhysicsX::Simulation(float _dt) noexcept
	{
		ZnWorld::Run(_dt);
	}

	void ZnPhysicsX::Finalize() noexcept
	{
		ZnWorld::Release();
		ZnFactoryX::Release();
	}

	ZnPhysicsX* ZnPhysicsX::Instance()
	{
		assert(!instance);

		instance = new ZnPhysicsX();

		return instance;
	}

	// 유저의 Scene 포인터를 key로 PxScene을 만든다.
	void ZnPhysicsX::CreateScene(void* _userScene, const Eigen::Vector3f& _gravity) noexcept
	{
		assert(_userScene != nullptr);

		ZnWorld::AddScene(_userScene, ZnFactoryX::CreateScene(_userScene, _gravity));
	}

	void ZnPhysicsX::LoadScene(void* _userScene) noexcept
	{
		assert(_userScene != nullptr);

		ZnWorld::LoadScene(_userScene);
	}

	void ZnPhysicsX::UnloadScene(void* _userScene) noexcept
	{
		assert(_userScene != nullptr);

		ZnWorld::UnloadScene(_userScene);
	}

	void ZnPhysicsX::SetGravity(const Vector3f& _gravity, void* _userScene) noexcept
	{
		ZnWorld::SetGravity(_gravity, _userScene);
	}

	void ZnPhysicsX::SetCollisionLayerData(uint32_t _layer, const std::initializer_list<uint32_t>& _data) noexcept
	{
		ZnLayer::SetCollisionData(_layer, _data);
	}

	/// <summary>
	/// 강체를 만들어서 반환
	/// </summary>
	ZnRigidBody* ZnPhysicsX::CreateRigidBody(void* _userData, void* _userScene) noexcept
	{
		// 유저의 (컴포넌트) 포인터를 키값으로 먼저 생성이 되어있는지 확인하고
		// 생성이 되어 있으면 반환
		// 생성이 되어있지 않으면 생성하고 반환
		// 생성한 다음 World에 추가를 하는데
		// userScene이 nullptr이면 현재 Scene에 추가하고
		// nullptr이 아니면 userScene에 해당하는 Scene에 추가함.
		const auto znBody = ZnFactoryX::CreateRigidBody(_userData);
		ZnWorld::AddBody(_userScene, znBody);

		return znBody;
	}

	/// <summary>
	/// 강체를 찾아서 거기에 콜라이더를 붙임.
	/// </summary>
	ZnCollider* ZnPhysicsX::CreateBoxCollider(void* _userData, float _x, float _y, float _z) noexcept
	{
		// userData를 키값으로 rigidbody가 생성이 되어있는지 체크하고
		// 생성이 되어 있으면 rigidbody에 collider를 추가
		// 생성이 되어 있지 않으면 rigidbody를 생성하고 collider를 추가.

		auto collider = ZnFactoryX::CreateBoxCollider(_userData, _x, _y, _z, nullptr);
	}

	ZnCollider* ZnPhysicsX::CreateSphereCollider(void* _userData, float _radius) noexcept
	{
		auto collider = ZnFactoryX::CreateSphereCollider(_userData, _radius, nullptr);
	}

	ZnCollider* ZnPhysicsX::CreateCapsuleCollider(void* _userData, float _radius, float _height) noexcept
	{
		auto collider = ZnFactoryX::CreateCapsuleCollider(_userData, _radius, _height, nullptr);
	}

	// 	ZnCollider* ZnPhysicsX::CreateCustomCollider(const std::wstring&) noexcept
	// 	{
	// 
	// 	}

	ZnFixedJoint* ZnPhysicsX::CreateFixedJoint(ZnRigidBody* _object0, const ZnTransform& _transform0,
	                                           ZnRigidBody* _object1, const ZnTransform& _transform1) noexcept
	{


		auto ob0 = dynamic_cast<RigidBody*>(_object0);
		auto ob1 = dynamic_cast<RigidBody*>(_object1);

		const auto joint = ZnFactoryX::CreateFixedJoint(nullptr, _transform0, nullptr, _transform1);
		return joint;
	}

	ZnDistanceJoint* ZnPhysicsX::CreateDistanceJoint(ZnRigidBody* _object0, const ZnTransform& _transform0,
	                                                 ZnRigidBody* _object1, const ZnTransform& _transform1) noexcept
	{
		auto ob0 = dynamic_cast<RigidBody*>(_object0);
		auto ob1 = dynamic_cast<RigidBody*>(_object1);

		const auto joint = ZnFactoryX::CreateDistanceJoint(nullptr, _transform0, nullptr, _transform1);

		return joint;
	}

	ZnSphericalJoint* ZnPhysicsX::CreateSphericalJoint(ZnRigidBody* _object0, const ZnTransform& _transform0,
	                                                   ZnRigidBody* _object1, const ZnTransform& _transform1) noexcept
	{
		auto ob0 = dynamic_cast<RigidBody*>(_object0);
		auto ob1 = dynamic_cast<RigidBody*>(_object1);

		const auto joint = ZnFactoryX::CreateSphericalJoint(nullptr, _transform0, nullptr, _transform1);
		return joint;
	}

	ZnHingeJoint* ZnPhysicsX::CreateHingeJoint(ZnRigidBody* _object0, const ZnTransform& _transform0,
	                                           ZnRigidBody* _object1, const ZnTransform& _transform1) noexcept
	{
		auto ob0 = dynamic_cast<RigidBody*>(_object0);
		auto ob1 = dynamic_cast<RigidBody*>(_object1);

		const auto joint = ZnFactoryX::CreateHingeJoint(nullptr, _transform0, nullptr, _transform1);
		return joint;
	}

	ZnPrismaticJoint* ZnPhysicsX::CreatePrismaticJoint(ZnRigidBody* _object0, const ZnTransform& _transform0,
	                                                   ZnRigidBody* _object1, const ZnTransform& _transform1) noexcept
	{
		auto ob0 = dynamic_cast<RigidBody*>(_object0);
		auto ob1 = dynamic_cast<RigidBody*>(_object1);

		const auto joint = ZnFactoryX::CreatePrismaticJoint(nullptr, _transform0, nullptr, _transform1);
		return joint;
	}

	bool ZnPhysicsX::Raycast(const Vector3f& _from, const Vector3f& _to, float _distance, ZnRaycastInfo& _out) noexcept
	{
		return ZnWorld::Raycast(_from, _to, _distance, _out);
	}

	extern "C" {
	ZnPhysicsBase* CreatePhysics()
	{
		return ZnPhysicsX::Instance();
	}
	}
} // namespace ZonaiPhysics
