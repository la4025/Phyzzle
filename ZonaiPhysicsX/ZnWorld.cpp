#include <PxPhysicsAPI.h>
#include <cassert>

#include "ZnRigidBody.h"
#include "RigidBody.h"
#include "ZnCollider.h"
#include "ZnUtil.h"
#include "ZnRaycastInfo.h"

#include "ZnWorld.h"

#include <ranges>

#include "ZnTransform.h"

namespace ZonaiPhysics
{
	physx::PxScene* ZnWorld::currScene = nullptr;

	void ZnWorld::Run(float _dt)
	{
		assert(currScene != nullptr);

		currScene->simulate(_dt);
		currScene->fetchResults(true);
	}

	void ZnWorld::Release()
	{
		for (auto& [scene, bodyList] : bodies)
		{
			for(auto& body : bodyList)
			{
				body ? delete body : 0;
				body = nullptr;
			}

			bodyList.clear();
			scene->release();
		}

		bodies.clear();

		for (auto& Scene : sceneList | std::views::values)
		{
			Scene ? Scene->release() : 0;
			Scene = nullptr;
		}

		sceneList.clear();
	}

	void ZnWorld::AddScene(void* _key, physx::PxScene* _scene)
	{
		assert(_key != nullptr && _scene != nullptr);

		sceneList.insert({ _key, _scene });
	}

	void ZnWorld::LoadScene(void* _key)
	{
		assert(_key != nullptr && (sceneList.find(_key) == sceneList.end()));

		currScene = sceneList[_key];
	}

	void ZnWorld::UnloadScene(void* _key)
	{
		auto& scene = sceneList[_key];
		assert(scene != nullptr);

		auto& bodylist = bodies[scene];

		for (auto& body : bodylist)
		{
			if (body)
			{
				delete body;
				body = nullptr;
			}
		}

		bodylist.clear();
		PX_RELEASE(scene);
		sceneList.erase(_key);
	}

	void ZnWorld::SetGravity(const Vector3f& _gravity, void* _scene)
	{
		assert(currScene != nullptr);

		_scene ? 
			currScene->setGravity(EigenToPhysx(_gravity)) :
			static_cast<physx::PxScene*>(_scene)->setGravity(EigenToPhysx(_gravity));
	}

	bool ZnWorld::Raycast(const Vector3f& _from, const Vector3f& _to, float _distance, ZnRaycastInfo& _out)
	{
		physx::PxRaycastBuffer temp;

		if (bool hit = currScene->raycast(EigenToPhysx(_from), EigenToPhysx(_to), _distance, temp))
		{
			_out.bodyData = static_cast<ZnRigidBody*>(temp.block.actor->userData)->GetUserData();
			_out.colliderData = static_cast<ZnCollider*>(temp.block.shape->userData)->GetUserData();
			_out.position = PhysxToEigen(temp.block.position);
			_out.distance = temp.block.distance;

			return true;
		}

		return false;
	}

	/// 구현 필요
	bool ZnWorld::Boxcast(float _x, float _y, float _z, const ZnTransform& trans)
	{
		physx::PxOverlapBuffer temp;

		const physx::PxTransform transform(EigenToPhysx(trans.position), EigenToPhysx(trans.quaternion));

		if (bool hit = currScene->overlap(physx::PxBoxGeometry(_x, _y, _z), transform, temp))
		{
			// temp.

			return true;
		}

		return false;
	}

	void ZnWorld::AddBody(void* _znBody, void* _scene)
	{
		assert(currScene != nullptr);
		assert(_scene && _znBody);

		const auto body = static_cast<RigidBody*>(_znBody);
		const auto pxbody = static_cast<physx::PxRigidDynamic*>(body->pxBody);
		physx::PxScene* scene = _scene ? static_cast<physx::PxScene*>(_scene) : currScene;
		scene->addActor(*pxbody);

		bodies[scene].push_back(body);
	}

	RigidBody* ZnWorld::GetBody(void* _znBody, void* _userScene)
	{
		const physx::PxScene* scene = nullptr;
		scene = _userScene ? sceneList[_userScene] : currScene;

		scene;
	}

	/// 수정 필요
	void ZnWorld::AddMaterial(void* _material)
	{
		assert(_material != nullptr);

		const auto material = static_cast<physx::PxMaterial*>(_material);
		materials.insert(material);
	}
}
