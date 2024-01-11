#pragma once
#include <Eigen/Dense>

#include "ZnBase.h"


namespace ZonaiPhysics
{
	class ZnObject : public ZnBase
	{
	public:
							ZnObject() noexcept : userData()
		{}
		virtual				~ZnObject() noexcept = default;

	public:
		/**
		위치
		*/
		virtual Eigen::Vector3f	GetPosition() const noexcept = 0;
		virtual void		SetPosition(const Eigen::Vector3f& _position) noexcept = 0;

		/**
		회전
		*/
		virtual Eigen::Quaternionf	GetQuaternion() const noexcept = 0;
		virtual void		 SetQuaternion(const Eigen::Quaternionf& _quaternion) noexcept = 0;

		/**
		유저 데이터
		*/
		virtual void*		GetUserData() const noexcept = 0;
		virtual void		SetUserData(void* _userData) noexcept = 0;

	protected:
		void* userData;
	};
}
