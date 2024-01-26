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

		using namespace physx;

		// SDK 생성
		{
			ZnFactoryX::CreatePhysxFactory();
		// 	foundation = PxCreateFoundation(PX_PHYSICS_VERSION, allocator, errorCallback);
		// 	pvd = PxCreatePvd(*foundation);
		// 	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
		// 	pvd->connect(*transport, PxPvdInstrumentationFlag::eALL);
		// 
		// 	physics = PxCreatePhysics(PX_PHYSICS_VERSION, *foundation, PxTolerancesScale(), true, pvd);
		// 	PxInitExtensions(*physics, pvd);
		}

		// 씬 생성
		{
			PxSceneDesc sceneDesc(physics->getTolerancesScale());
			sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
			dispatcher = PxDefaultCpuDispatcherCreate(2);
			sceneDesc.cpuDispatcher = dispatcher;

			eventCallback.setInstance(_instance);
			sceneDesc.simulationEventCallback = &eventCallback;
			sceneDesc.filterShader = FilterShader;
			// sceneDesc.filterCallback = &filterCallback;

			scene = physics->createScene(sceneDesc);
			scene->setVisualizationParameter(PxVisualizationParameter::eJOINT_LIMITS, 3.f);
			scene->setVisualizationParameter(PxVisualizationParameter::eJOINT_LOCAL_FRAMES, 3.f);
		}

		// 레이어 설정
		{
			ZnLayer::Clear();
			ZnLayer::SetCollisionData(0, { 0, 1, 2, 3 });
		}

		{
			PxPvdSceneClient* pvdClient = scene->getScenePvdClient();
			if (pvdClient)
			{
				pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
				pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
				pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
			}
			pvdClient->updateCamera()
		}

		material = physics->createMaterial(0.5f, 0.5f, 0.6f);
	}

	void ZnPhysicsX::Simulation(float _dt) noexcept
	{
		assert(scene);

		ZnWorld::Run(_dt);
		// scene->simulate(_dt);
		// scene->fetchResults(true);
	}

	void ZnPhysicsX::Finalize() noexcept
	{
		using namespace physx;
		for (auto& body : bodies | std::views::values)
		{
			delete body;
			body = nullptr;
		}
		bodies.clear();


		PX_RELEASE(scene);
		PX_RELEASE(dispatcher);

		ZnFactoryX::Release();
		// PxCloseExtensions();
		// PX_RELEASE(physics);
		// if (pvd)
		// {
		// 	PxPvdTransport* transport = pvd->getTransport();
		// 	pvd->release();
		// 	pvd = nullptr;
		// 	PX_RELEASE(transport);
		// }
		// PX_RELEASE(foundation);
	}

	ZnPhysicsX* ZnPhysicsX::Instance()
	{
		assert(!instance);

		instance = new ZnPhysicsX();

		return instance;
	}

	void ZnPhysicsX::SetGravity(const Vector3f& _gravity) noexcept
	{
		assert(scene != nullptr);

		scene->setGravity({ _gravity.x(), _gravity.y(), _gravity.z() });
	}

	void ZnPhysicsX::SceneClear()
	{

	}

	void ZnPhysicsX::SetCollisionLayerData(uint32_t _layer, const std::initializer_list<uint32_t>& _data) noexcept
	{
		ZnLayer::SetCollisionData(_layer, _data);
	}

	/// <summary>
	/// 강체를 만들어서 반환
	/// </summary>
	ZnRigidBody* ZnPhysicsX::CreateRigidBody(const std::wstring& _id) noexcept
	{
		RigidBody* result = FindRigidBody(_id);

		if (result == nullptr)
		{
			result = new RigidBody();
			ZnFactoryX::CreateRigidBody(_id);
			bodies.insert(std::make_pair(_id, result));
			ZnWorld::AddBody(scene, result->pxBody);
			// scene->addActor(*result->pxBody);
		}

		result->CanSimulate(true);

		return result;
	}

	/// <summary>
	/// 강체를 찾아서 거기에 콜라이더를 붙임.
	/// </summary>
	ZnCollider* ZnPhysicsX::CreateBoxCollider(const std::wstring& _id, float x, float y, float z) noexcept
	{
		RigidBody* body = FindRigidBody(_id);

		if (body == nullptr)
		{
			body = new RigidBody(physics);
			body->CanSimulate(false);
			bodies.insert(std::make_pair(_id, body));
			scene->addActor(*body->pxBody);
		}

		Collider* newRigidBody = new BoxCollider(physics, body, Vector3f(x, y, z), material);

		return newRigidBody;
	}

	ZnCollider* ZnPhysicsX::CreateSphereCollider(const std::wstring& _id, float radius) noexcept
	{
		RigidBody* body = FindRigidBody(_id);

		if (body == nullptr)
		{
			body = new RigidBody(physics);
			body->CanSimulate(false);
			bodies.insert(std::make_pair(_id, body));
			scene->addActor(*body->pxBody);
		}

		Collider* newRigidBody = new SphereCollider(physics, body, radius, material);

		return newRigidBody;
	}

	ZnCollider* ZnPhysicsX::CreateCapsuleCollider(const std::wstring& _id, float _radius, float _height) noexcept
	{
		RigidBody* body = FindRigidBody(_id);

		if (body == nullptr)
		{
			body = new RigidBody(physics);
			body->CanSimulate(false);
			bodies.insert(std::make_pair(_id, body));
			scene->addActor(*body->pxBody);
		}

		Collider* newRigidBody = new CapsuleCollider(physics, body, _radius, _height, material);

		return newRigidBody;
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

		auto* joint = new FixedJoint(physics, ob0, _transform0, ob1, _transform1);

		return joint;
	}

	ZnDistanceJoint* ZnPhysicsX::CreateDistanceJoint(ZnRigidBody* _object0, const ZnTransform& _transform0,
	                                                 ZnRigidBody* _object1, const ZnTransform& _transform1) noexcept
	{
		auto ob0 = dynamic_cast<RigidBody*>(_object0);
		auto ob1 = dynamic_cast<RigidBody*>(_object1);

		auto* joint = new DistanceJoint(physics, ob0, _transform0, ob1, _transform1);

		return joint;
	}

	ZnSphericalJoint* ZnPhysicsX::CreateSphericalJoint(ZnRigidBody* _object0, const ZnTransform& _transform0,
	                                                   ZnRigidBody* _object1, const ZnTransform& _transform1) noexcept
	{
		auto ob0 = dynamic_cast<RigidBody*>(_object0);
		auto ob1 = dynamic_cast<RigidBody*>(_object1);

		auto* joint = new SphericalJoint(physics, ob0, _transform0, ob1, _transform1);

		return joint;
	}

	ZnHingeJoint* ZnPhysicsX::CreateHingeJoint(ZnRigidBody* _object0, const ZnTransform& _transform0,
	                                           ZnRigidBody* _object1, const ZnTransform& _transform1) noexcept
	{
		auto ob0 = dynamic_cast<RigidBody*>(_object0);
		auto ob1 = dynamic_cast<RigidBody*>(_object1);

		auto* joint = new HingeJoint(physics, ob0, _transform0, ob1, _transform1);

		return joint;
	}

	ZnPrismaticJoint* ZnPhysicsX::CreatePrismaticJoint(ZnRigidBody* _object0, const ZnTransform& _transform0,
	                                                   ZnRigidBody* _object1, const ZnTransform& _transform1) noexcept
	{
		auto ob0 = dynamic_cast<RigidBody*>(_object0);
		auto ob1 = dynamic_cast<RigidBody*>(_object1);

		auto* joint = new PrismaticJoint(physics, ob0, _transform0, ob1, _transform1);

		return joint;
	}

	bool ZnPhysicsX::Raycast(const Vector3f& _from, const Vector3f& _to, float _distance, ZnRaycastInfo& _out) noexcept
	{
		physx::PxRaycastBuffer temp;

		if (bool hit = scene->raycast({_from.x(), _from.y(), _from.z()}, {_to.x(), _to.y(), _to.z()}, _distance, temp))
		{
			_out.bodyData = static_cast<ZnRigidBody*>(temp.block.actor->userData)->GetUserData();
			_out.colliderData = static_cast<ZnCollider*>(temp.block.shape->userData)->GetUserData();
			_out.position = {temp.block.position.x, temp.block.position.y, temp.block.position.z};
			_out.distance = temp.block.distance;

			return true;
		}
		return false;
	}

	RigidBody* ZnPhysicsX::FindRigidBody(const std::wstring& _id) noexcept
	{
		auto itr = bodies.find(_id);
		if (itr != bodies.end())
		{
			return itr->second;
		}

		return nullptr;
	}

	extern "C" {
	ZnPhysicsBase* CreatePhysics()
	{
		return ZnPhysicsX::Instance();
	}
	}
} // namespace ZonaiPhysics
