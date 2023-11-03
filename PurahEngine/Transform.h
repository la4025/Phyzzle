#pragma once
#include "Component.h"
#include "../ZonaiMath/ZonaiMath.h"
#pragma comment(lib, "../x64/Debug/ZonaiMath.lib")

#include <Eigen\Dense>
#include <Eigen\Core>

#include <vector>

namespace PurahEngine
{
	class Transform : public Component
	{
	public:
		Transform();
		virtual ~Transform();

		// 회전 함수
		void Rotate();

		/// get
		// Local
		// 오브젝트의 포지션을 가져온다.
		ZonaiMath::Vector3D GetLocalPosition() const;
		// 오브젝트의 로테이션을 가져온다.
		ZonaiMath::Quaternion GetLocalRotation() const;
		// 오브젝트의 스케일을 가져온다.
		ZonaiMath::Vector3D GetLocalScale() const;

		// World
		// 오브젝트의 월드 포지션을 가져온다.
		ZonaiMath::Vector3D GetWorldPosition() const;
		// 오브젝트의 월드 로테이션을 가져온다.
		ZonaiMath::Quaternion GetWorldRotation() const;
		// 오브젝트의 월드 스케일을 가져온다.
		ZonaiMath::Vector3D GetWorldScale() const;

		/// set
		// 포지션 세팅
		void SetLocalPosition(ZonaiMath::Vector3D setPosition);
		// 로테이션 세팅
		void SetLocalRotation(ZonaiMath::Quaternion setRotation);
		// 스케일 세팅
		void SetLocalScale(ZonaiMath::Vector3D setScale);
	private:
		// 위치
		ZonaiMath::Vector3D position;
		// 로테이션
		ZonaiMath::Quaternion rotation;
		// 스케일
		ZonaiMath::Vector3D scale;

		// 부모 Transform
		Transform* parentTransform;
		std::vector<Transform*> children;
	};
}