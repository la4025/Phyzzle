#include "World.h"
#include "RigidBody.h"


namespace ZonaiPhysics
{
	void World::Initialize() noexcept
	{
		using namespace physx;
		foundation = PxCreateFoundation(PX_PHYSICS_VERSION, allocator, errorCallback);
		pvd = PxCreatePvd(*foundation);
		PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
		pvd->connect(*transport, PxPvdInstrumentationFlag::eALL);
<<<<<<< HEAD
=======

		physics = PxCreatePhysics(PX_PHYSICS_VERSION, *foundation, PxTolerancesScale(), true, pvd);
		PxInitExtensions(*physics, pvd);

		PxSceneDesc sceneDesc(physics->getTolerancesScale());
		sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
		dispatcher = PxDefaultCpuDispatcherCreate(2);
		sceneDesc.cpuDispatcher = dispatcher;
		sceneDesc.filterShader = PxDefaultSimulationFilterShader;
		scene = physics->createScene(sceneDesc);

		PxPvdSceneClient* pvdClient = scene->getScenePvdClient();
		if (pvdClient)
		{
			pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
			pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
			pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
		}
	}

	void World::Simulation(float _dt) noexcept
	{
		scene->simulate(_dt);
		scene->fetchResults(true);
	}

	void World::Finalize() noexcept
	{
>>>>>>> 49329f7c854b5a1bc6f7f78c07b81085dead6dbe

		physics = PxCreatePhysics(PX_PHYSICS_VERSION, *foundation, PxTolerancesScale(), true, pvd);
		PxInitExtensions(*physics, pvd);

		PxSceneDesc sceneDesc(physics->getTolerancesScale());
		sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
		dispatcher = PxDefaultCpuDispatcherCreate(2);
		sceneDesc.cpuDispatcher = dispatcher;
		sceneDesc.filterShader = PxDefaultSimulationFilterShader;
		scene = physics->createScene(sceneDesc);

		PxPvdSceneClient* pvdClient = scene->getScenePvdClient();
		if (pvdClient)
		{
			pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
			pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
			pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
		}

		first = true;
	}

	void World::Simulation(float _dt) noexcept
	{
		if (first)
		{
			first = false;
		}
		scene->simulate(_dt);
		scene->fetchResults(true);
	}

	void World::Finalize() noexcept
	{
		using namespace physx;
		PX_RELEASE(scene);
		PX_RELEASE(dispatcher);
		PxCloseExtensions();
		PX_RELEASE(physics);
		if (pvd)
		{
			PxPvdTransport* transport = pvd->getTransport();
			pvd->release();	pvd = NULL;
			PX_RELEASE(transport);
		}
		PX_RELEASE(foundation);
	}

	ZnRigidBody* World::CreateRigidBody() noexcept
	{
		return new RigidBody(physics);
	}

<<<<<<< HEAD
	ZnRigidBody* World::CreateRigidBody(const std::wstring& _id) noexcept
	{
		auto itr = bodies.find(_id);
		if (itr != bodies.end())
		{
			return itr->second;
		}

		RigidBody* newRigidBody = new RigidBody(physics);
		bodies.insert(std::make_pair(_id, newRigidBody));
		return newRigidBody;
	}

	ZnCollider* World::CreatBoxCollider(float width, float height) noexcept
	{
		auto itr = bodies.find(_id);
		if (itr != bodies.end())
		{
			return itr->second;
		}
	}

	ZnCollider* World::CreatPlaneCollider() noexcept
	{

	}

	ZnCollider* World::CreatSphereCollider() noexcept
	{

	}

	ZnCollider* World::CreateCapsuleCollider() noexcept
	{

	}

=======
>>>>>>> 49329f7c854b5a1bc6f7f78c07b81085dead6dbe
} // namespace ZonaiPhysics