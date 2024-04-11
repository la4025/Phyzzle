#pragma once
#include <string>
#include <Eigen/Dense>

namespace ZonaiPhysics
{
	struct ZnRaycastInfo
	{
		void* bodyData = nullptr;					// 오브젝트 포인터
		void* colliderData = nullptr;				// 콜라이더 포인터
		Eigen::Vector3f position{ 0.f, 0.f, 0.f };	// 충돌 위치
		float distance = FLT_MIN;				// 충돌 거리
	};
}
