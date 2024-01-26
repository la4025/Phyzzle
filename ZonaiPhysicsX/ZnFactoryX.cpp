#include <PxPhysics.h>

#include "RigidBody.h"
#include "ZnFactoryX.h"

namespace ZonaiPhysics
{
	void ZnFactoryX::CreatePhysxFactory()
	{
		/// SDK »ý¼º
		{
			foundation = PxCreateFoundation(PX_PHYSICS_VERSION, allocator, errorCallback);

#ifdef _DEBUG
			pxPvd = PxCreatePvd(*foundation);
			physx::PxPvdTransport* transport = physx::PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
			pxPvd->connect(*transport, physx::PxPvdInstrumentationFlag::eALL);
#endif

			pxFactory = PxCreatePhysics(PX_PHYSICS_VERSION, *foundation, physx::PxTolerancesScale(), true, pxPvd);

#ifdef _DEBUG
			PxInitExtensions(*pxFactory, pxPvd);
#endif
		}
	}

	void ZnFactoryX::Release()
	{
		PxCloseExtensions();
		PX_RELEASE(pxFactory);
		if (pxPvd)
		{
			physx::PxPvdTransport* transport = pxPvd->getTransport();
			pxPvd->release();
			pxPvd = nullptr;
			PX_RELEASE(transport);
		}
		PX_RELEASE(foundation);
	}

	physx::PxScene* ZnFactoryX::CreateScene(const physx::PxSceneDesc& _desc)
	{
		const auto scene = pxFactory->createScene(_desc);

#ifdef _DEBUG
		scene->setVisualizationParameter(physx::PxVisualizationParameter::eJOINT_LIMITS, 3.f);
		scene->setVisualizationParameter(physx::PxVisualizationParameter::eJOINT_LOCAL_FRAMES, 3.f);
#endif

		return scene;
	}

	physx::PxMaterial* ZnFactoryX::CreateMaterial(float _staticFriction, float _dynamicFriction, float _restitution)
	{
		return pxFactory->createMaterial(_dynamicFriction, _staticFriction, _restitution);
	}

	physx::PxRigidDynamic* ZnFactoryX::CreateRigidBody(void* _key)
	{

	}
}
