#include <PxPhysics.h>

#include "FilterCallback.h"

#include "RigidBody.h"

#include "BoxCollider.h"
#include "SphereCollider.h"
#include "CapsuleCollider.h"

#include "FixedJoint.h"
#include "PrismaticJoint.h"
#include "DistanceJoint.h"
#include "SphericalJoint.h"
#include "HingeJoint.h"

#include "ZnUtil.h"

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

	void ZnFactoryX::SetSimulationCallback(ZnSimulationCallback* _instance)
	{
		assert(_instance != nullptr);

		eventCallback.setInstance(_instance);
	}

	void ZnFactoryX::Release()
	{
		PxCloseExtensions();
		PX_RELEASE(dispatcher);
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

	physx::PxScene* ZnFactoryX::CreateScene(void* _userScene, const Eigen::Vector3f& _gravity)
	{
		assert(_userScene != nullptr);

		physx::PxSceneDesc sceneDesc(pxFactory->getTolerancesScale());
		sceneDesc.gravity = EigenToPhysx(_gravity);
		dispatcher = physx::PxDefaultCpuDispatcherCreate(2);
		sceneDesc.cpuDispatcher = dispatcher;
		sceneDesc.simulationEventCallback = &eventCallback;
		sceneDesc.filterShader = FilterShader;

		const auto scene = pxFactory->createScene(sceneDesc);
		scene->userData = _userScene;

#ifdef _DEBUG
		scene->setVisualizationParameter(physx::PxVisualizationParameter::eJOINT_LIMITS, 3.f);
		scene->setVisualizationParameter(physx::PxVisualizationParameter::eJOINT_LOCAL_FRAMES, 3.f);

		physx::PxPvdSceneClient* pvdClient = scene->getScenePvdClient();
		if (pvdClient)
		{
			pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
			pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
			pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
		}
#endif

		return scene;
	}

	physx::PxMaterial* ZnFactoryX::CreateMaterial(float _staticFriction, float _dynamicFriction, float _restitution)
	{
		return pxFactory->createMaterial(_dynamicFriction, _staticFriction, _restitution);
	}

	RigidBody* ZnFactoryX::CreateRigidBody(void* _userData)
	{
		assert(_userData != nullptr);

		const auto pxBody = pxFactory->createRigidDynamic(physx::PxTransform());
		assert(pxBody != nullptr, "ZonaiPhysicsX::ZnFactoryX, RigidBody Initialize Error");

		const auto znBody = new RigidBody;
		znBody->SetUserData(znBody);
		znBody->userData = _userData;

		return znBody;
	}

	BoxCollider* ZnFactoryX::CreateBoxCollider(void* _userData, float _x, float _y, float _z, physx::PxMaterial* _material)
	{
		assert(_userData != nullptr);

		const auto znBody = static_cast<RigidBody*>(_userData);
		const auto pxShape = pxFactory->createShape(physx::PxBoxGeometry(_x, _y, _z), *_material);
		assert(pxShape != nullptr, "ZonaiPhysicsX::ZnFactoryX, BoxCollider Initialize Error");

		const auto znBoxCollider = new BoxCollider(pxShape, znBody);
		return znBoxCollider;
	}

	SphereCollider* ZnFactoryX::CreateSphereCollider(void* _userData, float _radius, physx::PxMaterial* _material)
	{
		assert(_userData != nullptr);

		const auto znBody = static_cast<RigidBody*>(_userData);
		const auto pxShape = pxFactory->createShape(physx::PxSphereGeometry(_radius), *_material);
		assert(pxShape != nullptr, "ZonaiPhysicsX::ZnFactoryX, SphereCollider Initialize Error");

		const auto znSphereCollider = new SphereCollider(pxShape, znBody);
		return znSphereCollider;
	}

	CapsuleCollider* ZnFactoryX::CreateCapsuleCollider(void* _userData, float _radius, float _height, physx::PxMaterial* _material)
	{
		assert(_userData != nullptr);

		const auto znBody = static_cast<RigidBody*>(_userData);
		const auto pxShape = pxFactory->createShape(physx::PxCapsuleGeometry(_radius, _height), *_material);
		assert(pxShape != nullptr, "ZonaiPhysicsX::ZnFactoryX, Capsule Initialize Error");

		const auto znCapsuleCollider = new CapsuleCollider(pxShape, znBody);
		return znCapsuleCollider;
	}

	FixedJoint* ZnFactoryX::CreateFixedJoint(void* _znBody0, const ZnTransform& tm0, void* _znBody1, const ZnTransform& tm1)
	{
		assert(_znBody0 != nullptr && _znBody1 != nullptr);

		const auto pxbody0 = static_cast<RigidBody*>(_znBody0)->pxBody;
		const auto pxbody1 = static_cast<RigidBody*>(_znBody1)->pxBody;
		const physx::PxTransform t0(EigenToPhysx(tm0.position), EigenToPhysx(tm0.quaternion));
		const physx::PxTransform t1(EigenToPhysx(tm1.position), EigenToPhysx(tm1.quaternion));

		auto joint = physx::PxFixedJointCreate(*pxFactory, pxbody0, t0, pxbody1, t1);
		assert(joint != nullptr, "ZonaiPhysicsX :: Distance Joint Initialize Error");

		auto znFixedJoint = new FixedJoint(joint, );
		znFixedJoint->joint = 
	}

	PrismaticJoint* ZnFactoryX::CreatePrismaticJoint(void* _pxBody0, const ZnTransform& tm0, void* _pxBody1, const ZnTransform& tm1)
	{
		assert(_znBody0 != nullptr && _znBody1 != nullptr);

		const auto pxbody0 = static_cast<physx::PxRigidDynamic*>(_pxBody0);
		const auto pxbody1 = static_cast<physx::PxRigidDynamic*>(_pxBody1);
		const physx::PxTransform t0(EigenToPhysx(tm0.position), EigenToPhysx(tm0.quaternion));
		const physx::PxTransform t1(EigenToPhysx(tm1.position), EigenToPhysx(tm1.quaternion));

		auto joint = physx::PxPrismaticJointCreate(*pxFactory, pxbody0, t0, pxbody1, t1);
		assert(joint != nullptr, "ZonaiPhysicsX :: Distance Joint Initialize Error");

	}

	DistanceJoint* ZnFactoryX::CreateDistanceJoint(void* _pxBody0, const ZnTransform& tm0, void* _pxBody1, const ZnTransform& tm1)
	{
		assert(_znBody0 != nullptr && _znBody1 != nullptr);

		const auto pxbody0 = static_cast<physx::PxRigidDynamic*>(_pxBody0);
		const auto pxbody1 = static_cast<physx::PxRigidDynamic*>(_pxBody1);
		const physx::PxTransform t0(EigenToPhysx(tm0.position), EigenToPhysx(tm0.quaternion));
		const physx::PxTransform t1(EigenToPhysx(tm1.position), EigenToPhysx(tm1.quaternion));

		auto joint = physx::PxDistanceJointCreate(*pxFactory, pxbody0, t0, pxbody1, t1);
		assert(joint != nullptr, "ZonaiPhysicsX :: Distance Joint Initialize Error");

	}

	SphericalJoint* ZnFactoryX::CreateSphericalJoint(void* _pxBody0, const ZnTransform& tm0, void* _pxBody1, const ZnTransform& tm1)
	{
		assert(_znBody0 != nullptr && _znBody1 != nullptr);

		const auto pxbody0 = static_cast<physx::PxRigidDynamic*>(_pxBody0);
		const auto pxbody1 = static_cast<physx::PxRigidDynamic*>(_pxBody1);
		const physx::PxTransform t0(EigenToPhysx(tm0.position), EigenToPhysx(tm0.quaternion));
		const physx::PxTransform t1(EigenToPhysx(tm1.position), EigenToPhysx(tm1.quaternion));

		auto joint = physx::PxSphericalJointCreate(*pxFactory, pxbody0, t0, pxbody1, t1);
		assert(joint != nullptr, "ZonaiPhysicsX :: Distance Joint Initialize Error");

	}

	HingeJoint* ZnFactoryX::CreateHingeJoint(void* _pxBody0, const ZnTransform& tm0, void* _pxBody1, const ZnTransform& tm1)
	{
		assert(_znBody0 != nullptr && _znBody1 != nullptr);

		const auto pxbody0 = static_cast<physx::PxRigidDynamic*>(_pxBody0);
		const auto pxbody1 = static_cast<physx::PxRigidDynamic*>(_pxBody1);
		const physx::PxTransform t0(EigenToPhysx(tm0.position), EigenToPhysx(tm0.quaternion));
		const physx::PxTransform t1(EigenToPhysx(tm1.position), EigenToPhysx(tm1.quaternion));

		auto joint = physx::PxRevoluteJointCreate(*pxFactory, pxbody0, t0, pxbody1, t1);
		assert(joint != nullptr, "ZonaiPhysicsX :: Distance Joint Initialize Error");

	}
}
