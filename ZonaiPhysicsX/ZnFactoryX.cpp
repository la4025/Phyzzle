#include "FilterCallback.h"

#include "RigidBodyHelper.h"
#include "RigidBody.h"

#include "FixedJoint.h"
#include "PrismaticJoint.h"
#include "DistanceJoint.h"
#include "SphericalJoint.h"
#include "HingeJoint.h"

#include "ZnUtil.h"

#include "ZnTransform.h"

#include "ZnFactoryX.h"

#include "../ZeldaFBXLoader/FBXLoader.h"

namespace ZonaiPhysics
{
	EventCallback ZnFactoryX::eventCallback{};
	physx::PxDefaultAllocator ZnFactoryX::allocator{};
	physx::PxDefaultErrorCallback ZnFactoryX::errorCallback{};
	physx::PxDefaultCpuDispatcher* ZnFactoryX::dispatcher = nullptr;
	physx::PxFoundation* ZnFactoryX::foundation = nullptr;
	physx::PxPhysics* ZnFactoryX::pxFactory = nullptr;
	physx::PxPvd* ZnFactoryX::pxPvd = nullptr;
	physx::PxOmniPvd* ZnFactoryX::omniPvd = nullptr;
	// physx::PxCookingParams* ZnFactoryX::pxCooking = nullptr;

	void ZnFactoryX::CreatePhysxFactory()
	{
		/// SDK 생성
		{
			foundation = PxCreateFoundation(PX_PHYSICS_VERSION, allocator, errorCallback);
#if PX_SUPPORT_PVD
			pxPvd = PxCreatePvd(*foundation);
			physx::PxPvdTransport* transport = physx::PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
			pxPvd->connect(*transport, physx::PxPvdInstrumentationFlag::eALL);

			pxFactory = PxCreatePhysics(PX_PHYSICS_VERSION, *foundation, physx::PxTolerancesScale(), true, pxPvd);

			PxInitExtensions(*pxFactory, pxPvd);
#endif

#if PX_SUPPORT_OMNI_PVD
			omniPvd = PxCreateOmniPvd(*foundation);
			if (!omniPvd)
			{
				printf("Error : could not create PxOmniPvd!");
				return;
			}
			OmniPvdWriter* omniWriter = omniPvd->getWriter();
			if (!omniWriter)
			{
				printf("Error : could not get an instance of PxOmniPvdWriter!");
				return;
			}
			OmniPvdFileWriteStream* fStream = omniPvd->getFileWriteStream();
			if (!fStream)
			{
				printf("Error : could not get an instance of PxOmniPvdFileWriteStream!");
				return;
			}
			fStream->setFileName("gOmniPvdName");
			omniWriter->setWriteStream(static_cast<OmniPvdWriteStream&>(*fStream));

			pxFactory = PxCreatePhysics(PX_PHYSICS_VERSION, *foundation, physx::PxTolerancesScale(), true, NULL, omniPvd);
			if (!pxFactory)
			{
				printf("Error : could not create a PhysX instance!");
				return;
			}

			if (pxFactory->getOmniPvd())
			{
				pxFactory->getOmniPvd()->startSampling();
			}
			else
			{
				printf("Error : could not start OmniPvd sampling!");
				return;
			}
#endif

			// pxCooking = PxCreate
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

#ifdef PX_SUPPORT_PVD
		scene->setVisualizationParameter(physx::PxVisualizationParameter::eJOINT_LIMITS, 10.f);
		scene->setVisualizationParameter(physx::PxVisualizationParameter::eJOINT_LOCAL_FRAMES, 10.f);

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

	physx::PxMaterial* ZnFactoryX::CreateMaterial(const MaterialDesc& _desc)
	{
		physx::PxMaterial* newMaterial = pxFactory->createMaterial(
			_desc.dynamicFriction, _desc.staticFriction, _desc.restitution);

		newMaterial->setFrictionCombineMode(static_cast<physx::PxCombineMode::Enum>(_desc.eFriction));
		newMaterial->setRestitutionCombineMode(static_cast<physx::PxCombineMode::Enum>(_desc.eRestitution));

		return newMaterial;
	}

	RigidBody* ZnFactoryX::CreateDynamicRigidBody(void* _userData)
	{
		assert(_userData != nullptr);

		const auto pxBody = pxFactory->createRigidDynamic(physx::PxTransform(physx::PxVec3{0.f, 0.f, 0.f}));
		assert(pxBody != nullptr);

		const auto znBody = new RigidBody(pxBody, _userData);
		return znBody;
	}

	RigidBody* ZnFactoryX::CreateStaticRigidBody(void* _userData)
	{
		assert(_userData != nullptr);

		const auto pxBody = pxFactory->createRigidStatic(physx::PxTransform(physx::PxVec3{ 0.f, 0.f, 0.f }));
		assert(pxBody != nullptr);

		// const auto znBody = new RigidBody(pxBody, _userData);
		// return znBody;
		return nullptr;
	}

	physx::PxShape* ZnFactoryX::CreateBoxShape(const Eigen::Vector3f& _extend, const physx::PxMaterial* _material)
	{
		return pxFactory->createShape(physx::PxBoxGeometry(_extend.x(), _extend.y(), _extend.z()), *_material);
	}

	physx::PxShape* ZnFactoryX::CreateSphereShape(float _radius, const physx::PxMaterial* _material)
	{
		return pxFactory->createShape(physx::PxSphereGeometry(_radius), *_material);
	}

	physx::PxShape* ZnFactoryX::CreateCapsuleShape(float _radius, float _height, const physx::PxMaterial* _material)
	{
		physx::PxShape* shape = pxFactory->createShape(physx::PxCapsuleGeometry(_radius, _height), *_material);

		const physx::PxQuat rotation(physx::PxPi / 2.f, physx::PxVec3(0.f, 0.f, 1.f));
		shape->setLocalPose(physx::PxTransform(rotation));

		return shape;
	}

	physx::PxShape* ZnFactoryX::CreateTriagleMeshShape(physx::PxTriangleMesh* _mesh, const Eigen::Vector3f& _scale,
		const Eigen::Quaternionf& _rotation, const physx::PxMaterial* _material)
	{
		return pxFactory->createShape(physx::PxTriangleMeshGeometry(_mesh), *_material);
	}

	physx::PxShape* ZnFactoryX::CreateConvexMeshShape(physx::PxConvexMesh* _mesh, const Eigen::Vector3f& _scale,
		const Eigen::Quaternionf& _rotation, const physx::PxMaterial* _material)
	{
		return pxFactory->createShape(physx::PxConvexMeshGeometry(_mesh), *_material);
	}

	physx::PxTriangleMesh* ZnFactoryX::CookTriagleMesh(const physx::PxMaterial* _material)
	{
		const physx::PxVec3 meshVerts[] = {
			physx::PxVec3(0,1,0),
			physx::PxVec3(1,0,0),
			physx::PxVec3(-1,0,0),
			physx::PxVec3(0,0,1),
			physx::PxVec3(0,0,-1)
		};

		const physx::PxU32 meshIndies[] = {
			0, 1, 2,
			0, 2, 3
		};

		physx::PxCookingParams params(pxFactory->getTolerancesScale());

		// 메시 정리 비활성화 - 개발 구성에서 메시 유효성 검사 수행
		params.meshPreprocessParams |= physx::PxMeshPreprocessingFlag::eDISABLE_CLEAN_MESH;

		// 엣지 사전 계산 비활성화, 각 삼각형에 대해 엣지가 설정됩니다. 이는 접촉 생성을 느리게 합니다.
		params.meshPreprocessParams |= physx::PxMeshPreprocessingFlag::eDISABLE_ACTIVE_EDGES_PRECOMPUTE;

		// 내부 메시의 계층 구조를 낮춥니다.
		// params.meshCookingHint = physx::PxMeshCookingHint::eCOOKING_PERFORMANCE;
		// params.meshPreprocessParams |= physx::PxMeshPreprocessingFlag::eWELD_VERTICES;
		// params.meshPreprocessParams ^= physx::PxMeshPreprocessingFlag::eDISABLE_CLEAN_MESH;

		//physx::PxSDFDesc sdfDesc;
		//sdfDesc.spacing = sdfSpacing;
		//sdfDesc.subgridSize = sdfSubgridSize;
		//sdfDesc.bitsPerSubgridPixel = bitsPerSdfSubgridPixel;
		//sdfDesc.numThreadsForSdfConstruction = 8;
		// 
		//if (enableGpuAcceleratedCooking)
		//{
		//	//If sdfBuilder is NULL, the sdf will get cooked on the CPU using the number of threads specified above
		//	sdfDesc.sdfBuilder = physx::PxGetPhysicsGpu()->createSDFBuilder(cudaContextManager);
		//}

		// 정점 정보
		physx::PxTriangleMeshDesc meshDesc;
		meshDesc.points.count = sizeof(meshVerts) / sizeof(physx::PxVec3);
		meshDesc.points.stride = sizeof(physx::PxVec3);
		meshDesc.points.data = meshVerts;

		meshDesc.triangles.count = sizeof(meshIndies) / sizeof(physx::PxU32);
		meshDesc.triangles.stride = sizeof(physx::PxU32);
		meshDesc.triangles.data = meshIndies;

		meshDesc.sdfDesc;
		meshDesc.flags = physx::PxMeshFlag::e16_BIT_INDICES;


//#ifdef _DEBUG
//// 메시가 정리되지 않은 상태로 쿠킹하기 전에 메시를 유효성 검사해야 합니다.
//		bool res = PxValidateTriangleMesh(params, meshDesc);
//		PX_ASSERT(res);
//#endif
//
//		physx::PxTriangleMesh* aTriangleMesh = PxCreateTriangleMesh(params, meshDesc,
//			pxFactory->getPhysicsInsertionCallback());

		physx::PxDefaultMemoryOutputStream writeBuffer;
		physx::PxTriangleMeshCookingResult::Enum result;
		const bool status = PxCookTriangleMesh(params, meshDesc, writeBuffer, &result);
		if (!status)
			return NULL;

		physx::PxDefaultMemoryInputData readBuffer(writeBuffer.getData(), writeBuffer.getSize());
		physx::PxTriangleMesh* triangleMesh = pxFactory->createTriangleMesh(readBuffer);

		return triangleMesh;
	}

	physx::PxConvexMesh* ZnFactoryX::CookConvexMesh(const physx::PxMaterial* _material)
	{
		FBXLoader::FBXLoader loader;

		// 이부분은 바뀔 예정
		// 정점 갯수, 정점 데이터, 정점 크기를 받아서 Convex를 만들어야함.
		const physx::PxVec3 convexVerts[] = {
			physx::PxVec3(0,1,0),
			physx::PxVec3(1,0,0),
			physx::PxVec3(-1,0,0),
			physx::PxVec3(0,0,1),
			physx::PxVec3(0,0,-1)
		};

		// 정점 정보
		physx::PxConvexMeshDesc convexDesc;
		convexDesc.points.count = sizeof(convexVerts) / sizeof(physx::PxVec3);
		convexDesc.points.stride = sizeof(physx::PxVec3);
		convexDesc.points.data = convexVerts;
		convexDesc.flags = physx::PxConvexFlag::eCOMPUTE_CONVEX;

		const physx::PxCookingParams params(pxFactory->getTolerancesScale());

		// 정점 메모리 버퍼
		physx::PxDefaultMemoryOutputStream buf;
		physx::PxConvexMeshCookingResult::Enum result;
		if (!PxCookConvexMesh(params, convexDesc, buf, &result))
			return NULL;

		// 버퍼를 바탕으로 Mesh 생성
		physx::PxDefaultMemoryInputData input(buf.getData(), buf.getSize());
		physx::PxConvexMesh* convexMesh = pxFactory->createConvexMesh(input);

		return convexMesh;
	}

	FixedJoint* ZnFactoryX::CreateFixedJoint(RigidBody* _znBody0, const ZnTransform& tm0, RigidBody* _znBody1, const ZnTransform& tm1)
	{
		assert(_znBody0 != nullptr);

		const auto pxbody0 = static_cast<physx::PxRigidDynamic*>(_znBody0->pxBody);
		const physx::PxTransform t0(EigenToPhysx(tm0.position), EigenToPhysx(tm0.quaternion));

		physx::PxRigidDynamic* pxbody1 = nullptr;
		if (_znBody1)
		{
			pxbody1 = static_cast<physx::PxRigidDynamic*>(_znBody1->pxBody);
		}
		const physx::PxTransform t1(EigenToPhysx(tm1.position), EigenToPhysx(tm1.quaternion));

		const auto joint = physx::PxFixedJointCreate(*pxFactory, pxbody0, t0, pxbody1, t1);
		assert(joint != nullptr);

#ifdef _DEBUG
		joint->setConstraintFlag(physx::PxConstraintFlag::eVISUALIZATION, true);
#endif

		const auto znFixedJoint = new FixedJoint(joint, _znBody0, _znBody1);
		return znFixedJoint;
	}

	PrismaticJoint* ZnFactoryX::CreatePrismaticJoint(RigidBody* _znBody0, const ZnTransform& tm0, RigidBody* _znBody1, const ZnTransform& tm1)
	{
		assert(_znBody0 != nullptr);

		const auto pxbody0 = static_cast<physx::PxRigidDynamic*>(_znBody0->pxBody);
		const physx::PxTransform t0(EigenToPhysx(tm0.position), EigenToPhysx(tm0.quaternion));

		physx::PxRigidDynamic* pxbody1 = nullptr;
		if (_znBody1)
		{
			pxbody1 = static_cast<physx::PxRigidDynamic*>(_znBody1->pxBody);
		}
		const physx::PxTransform t1(EigenToPhysx(tm1.position), EigenToPhysx(tm1.quaternion));

		const auto joint = physx::PxPrismaticJointCreate(*pxFactory, pxbody0, t0, pxbody1, t1);
		assert(joint != nullptr);

#ifdef _DEBUG
		joint->setConstraintFlag(physx::PxConstraintFlag::eVISUALIZATION, true);
#endif

		const auto znPrismaticJoint = new PrismaticJoint(joint, _znBody0, _znBody1, &pxFactory->getTolerancesScale());
		return znPrismaticJoint;
	}

	DistanceJoint* ZnFactoryX::CreateDistanceJoint(RigidBody* _znBody0, const ZnTransform& tm0, RigidBody* _znBody1, const ZnTransform& tm1)
	{
		assert(_znBody0 != nullptr);

		const auto pxbody0 = static_cast<physx::PxRigidDynamic*>(_znBody0->pxBody);
		const physx::PxTransform t0(EigenToPhysx(tm0.position), EigenToPhysx(tm0.quaternion));

		physx::PxRigidDynamic* pxbody1 = nullptr;
		if (_znBody1)
		{
			pxbody1 = static_cast<physx::PxRigidDynamic*>(_znBody1->pxBody);
		}
		const physx::PxTransform t1(EigenToPhysx(tm1.position), EigenToPhysx(tm1.quaternion));

		const auto joint = physx::PxDistanceJointCreate(*pxFactory, pxbody0, t0, pxbody1, t1);
		assert(joint != nullptr);

#ifdef _DEBUG
		joint->setConstraintFlag(physx::PxConstraintFlag::eVISUALIZATION, true);
#endif

		const auto znDistanceJoint = new DistanceJoint(joint, _znBody0, _znBody1);
		return znDistanceJoint;
	}

	SphericalJoint* ZnFactoryX::CreateSphericalJoint(RigidBody* _znBody0, const ZnTransform& tm0, RigidBody* _znBody1, const ZnTransform& tm1)
	{
		assert(_znBody0 != nullptr);

		const auto pxbody0 = static_cast<physx::PxRigidDynamic*>(_znBody0->pxBody);
		const physx::PxTransform t0(EigenToPhysx(tm0.position), EigenToPhysx(tm0.quaternion));

		physx::PxRigidDynamic* pxbody1 = nullptr;
		if (_znBody1)
		{
			pxbody1 = static_cast<physx::PxRigidDynamic*>(_znBody1->pxBody);
		}
		const physx::PxTransform t1(EigenToPhysx(tm1.position), EigenToPhysx(tm1.quaternion));

		const auto joint = physx::PxSphericalJointCreate(*pxFactory, pxbody0, t0, pxbody1, t1);
		assert(joint != nullptr);

#ifdef _DEBUG
		joint->setConstraintFlag(physx::PxConstraintFlag::eVISUALIZATION, true);
#endif

		auto znSphericalJoint = new SphericalJoint(joint, _znBody0, _znBody1);
		return znSphericalJoint;
	}

	HingeJoint* ZnFactoryX::CreateHingeJoint(RigidBody* _znBody0, const ZnTransform& tm0, RigidBody* _znBody1, const ZnTransform& tm1)
	{
		assert(_znBody0 != nullptr);

		const auto pxbody0 = static_cast<physx::PxRigidDynamic*>(_znBody0->pxBody);
		const physx::PxTransform t0(EigenToPhysx(tm0.position), EigenToPhysx(tm0.quaternion));

		physx::PxRigidDynamic* pxbody1 = nullptr;
		if (_znBody1)
		{
			pxbody1 = static_cast<physx::PxRigidDynamic*>(_znBody1->pxBody);
		}
		const physx::PxTransform t1(EigenToPhysx(tm1.position), EigenToPhysx(tm1.quaternion));

		const auto joint = physx::PxRevoluteJointCreate(*pxFactory, pxbody0, t0, pxbody1, t1);
		assert(joint != nullptr);

#ifdef _DEBUG
		joint->setConstraintFlag(physx::PxConstraintFlag::eVISUALIZATION, true);
#endif

		const auto znHingeJoint = new HingeJoint(joint, _znBody0, _znBody1);
		return znHingeJoint;
	}
}
