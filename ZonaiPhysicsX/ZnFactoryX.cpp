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
		physx::PxMeshScale scale(EigenToPhysx(_scale), EigenToPhysx(_rotation));
		physx::PxTriangleMeshGeometry geom(_mesh, scale);
		return pxFactory->createShape(geom, *_material);
	}

	physx::PxShape* ZnFactoryX::CreateConvexMeshShape(physx::PxConvexMesh* _mesh, const Eigen::Vector3f& _scale,
		const Eigen::Quaternionf& _rotation, const physx::PxMaterial* _material)
	{
		physx::PxMeshScale scale(EigenToPhysx(_scale), EigenToPhysx(_rotation));
		physx::PxConvexMeshGeometry geom(_mesh, scale);
		return pxFactory->createShape(geom, *_material);
	}

	physx::PxTriangleMesh* ZnFactoryX::CookTriagleMesh(FBXLoader::Model* _model)
	{
		std::vector<Eigen::Vector3f> verties;
		auto& pos = _model->meshList[0]->vertices;
		for (size_t i = 0; i < pos.size(); i++)
		{
			verties.emplace_back(pos[i].position.x, pos[i].position.y, pos[i].position.z);
		}

		std::vector<unsigned int> indies;
		auto& index = _model->meshList[0]->indices;
		for (size_t i = 0; i < index.size(); i++)
		{
			indies.emplace_back(index[i]);
		}

		// 정점 정보
		physx::PxTriangleMeshDesc meshDesc;
		meshDesc.points.count = verties.size();
		meshDesc.points.stride = sizeof(Eigen::Vector3f);
		meshDesc.points.data = &verties[0];

		meshDesc.triangles.count = indies.size();
		meshDesc.triangles.stride = sizeof(unsigned int);
		meshDesc.triangles.data = &indies[0];

		meshDesc.sdfDesc;
		meshDesc.flags = physx::PxMeshFlag::e16_BIT_INDICES;

		physx::PxCookingParams params(pxFactory->getTolerancesScale());

		physx::PxDefaultMemoryOutputStream writeBuffer;
		physx::PxTriangleMeshCookingResult::Enum result;
		const bool status = PxCookTriangleMesh(params, meshDesc, writeBuffer, &result);
		if (!status)
			return NULL;

		physx::PxDefaultMemoryInputData readBuffer(writeBuffer.getData(), writeBuffer.getSize());
		physx::PxTriangleMesh* triangleMesh = pxFactory->createTriangleMesh(readBuffer);

		return triangleMesh;
	}

	physx::PxConvexMesh* ZnFactoryX::CookConvexMesh(FBXLoader::Model* _model)
	{
		std::vector<Eigen::Vector3f> verties;
		auto& pos = _model->meshList[0]->vertices;

		for (size_t i = 0; i < _model->meshList.size(); i++)
		{
			auto& mesh = _model->meshList[i];
			auto& points = mesh->vertices;

			for (size_t i = 0; i < points.size(); i++)
			{
				verties.emplace_back(points[i].position.x, points[i].position.y, points[i].position.z);
			}
		}

		// 정점 정보
		physx::PxConvexMeshDesc convexDesc;
		convexDesc.points.count = verties.size();
		convexDesc.points.stride = sizeof(Eigen::Vector3f);
		convexDesc.points.data = &verties[0];
		convexDesc.flags = physx::PxConvexFlag::eCOMPUTE_CONVEX;

		const physx::PxCookingParams params(pxFactory->getTolerancesScale());

		// 정점 메모리 버퍼
		physx::PxDefaultMemoryOutputStream buf;
		physx::PxConvexMeshCookingResult::Enum result;
		const bool status = PxCookConvexMesh(params, convexDesc, buf, &result);
		if (!status)
			return nullptr;

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
