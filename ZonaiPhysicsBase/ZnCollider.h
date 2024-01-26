#pragma once
#include "ZnObject.h"
#include <Eigen/Dense>

namespace ZonaiPhysics
{
	class ZnCollider : public ZnObject
	{
	public:
							ZnCollider() noexcept = default;
		virtual				~ZnCollider() noexcept = default;

	public:
		// 트리거 설정
		virtual void		SetTrigger(bool) noexcept = 0;

		// 레이어 설정
		virtual void		SetLayerData(const uint32_t&) noexcept = 0;

		// 콜라이더 위치 오프셋
		virtual Eigen::Vector3f GetLocalPosition() const noexcept = 0;
		virtual void SetLocalPosition(const Eigen::Vector3f&) noexcept = 0;

		// 콜라이더 회전 오프셋
		virtual Eigen::Quaternionf GetLocalQuaternion() const noexcept = 0;
		virtual void SetLocalQuaternion(const Eigen::Quaternionf&) noexcept = 0;
	};
}
