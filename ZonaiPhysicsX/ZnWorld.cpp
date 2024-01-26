#include <PxPhysicsAPI.h>
#include <cassert>

#include "EventCallback.h"
#include "FilterCallback.h"
#include "ZnRigidBody.h"
#include "ZnCollider.h"
#include "ZnUtil.h"
#include "ZnRaycastInfo.h"

#include "ZnWorld.h"

namespace ZonaiPhysics
{
	void ZnWorld::Run(float _dt)
	{
		currScene->simulate(_dt);
		currScene->fetchResults(true);
	}

	physx::PxScene* ZnWorld::CreateScene(physx::PxPhysics** _physx, physx::PxDefaultCpuDispatcher** _dispatcher, EventCallback* _eventCallback)
	{
		physx::PxSceneDesc sceneDesc((*_physx)->getTolerancesScale());
		sceneDesc.gravity = physx::PxVec3(0.0f, -9.81f, 0.0f);
		*_dispatcher = physx::PxDefaultCpuDispatcherCreate(2);
		sceneDesc.cpuDispatcher = *_dispatcher;

		sceneDesc.simulationEventCallback = _eventCallback;
		sceneDesc.filterShader = FilterShader;

		physx::PxScene* scene = nullptr;
		scene = (*_physx)->createScene(sceneDesc);
		scene->setVisualizationParameter(physx::PxVisualizationParameter::eJOINT_LIMITS, 3.f);
		scene->setVisualizationParameter(physx::PxVisualizationParameter::eJOINT_LOCAL_FRAMES, 3.f);

		physx::PxPvdSceneClient* pvdClient = scene->getScenePvdClient();
		if (pvdClient)
		{
			pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
			pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
			pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
		}
	}

	void ZnWorld::AddScene(physx::PxScene* _scene)
	{
		assert(_scene);

		sceneList.push_back(_scene);
	}

	void ZnWorld::AddBody(void* _scene, void* _body)
	{
		assert(_scene && _body);

		static_cast<physx::PxScene*>(_scene)->addActor(*static_cast<physx::PxRigidDynamic*>(_body));
	}

	void ZnWorld::SetGravity(void* _scene, const Eigen::Vector3f& _gravity)
	{
		assert(_scene);

		static_cast<physx::PxScene*>(_scene)->setGravity(EigenToPhysx(_gravity));
	}

	bool ZnWorld::Raycast(void* _scene, const Eigen::Vector3f& _from, const Eigen::Vector3f& _to, float _distance, ZnRaycastInfo& _out)
	{
		physx::PxRaycastBuffer temp;

		if (bool hit = static_cast<physx::PxScene*>(_scene)->raycast({ _from.x(), _from.y(), _from.z() }, { _to.x(), _to.y(), _to.z() }, _distance, temp))
		{
			_out.bodyData = static_cast<ZnRigidBody*>(temp.block.actor->userData)->GetUserData();
			_out.colliderData = static_cast<ZnCollider*>(temp.block.shape->userData)->GetUserData();
			_out.position = { temp.block.position.x, temp.block.position.y, temp.block.position.z };
			_out.distance = temp.block.distance;

			return true;
		}
		return false;
	}
}
