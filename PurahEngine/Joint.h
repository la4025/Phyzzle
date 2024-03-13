#pragma once
#include "Component.h"
#include "ZnJoint.h"

namespace PurahEngine
{
	template <typename Base>
	concept ZonaiJoint = std::is_base_of_v<ZonaiPhysics::ZnJoint, Base>;

	template <typename ZonaiJoint>
	class Joint : public Component
	{
	public:
		Joint();
		~Joint() override;

	protected:
		ZonaiJoint* joint;

	public:
		/**
		오브젝트를 조인트로 연결함.
		둘 중 하나는 NULL일 수 있음.
		*/
		//virtual void		SetObject(ZnObject*, ZnObject*) noexcept = 0;
		//virtual void		GetObject(ZnObject*&, ZnObject*&) const noexcept = 0;

		/**
		오브젝트의 포지션
		*/
		virtual void		SetLocalPosition(int _index, const Eigen::Vector3f& _localPos) noexcept
		{
			assert(joint != nullptr);

			joint->SetLocalPosition((ZonaiPhysics::ZnJoint::eOBJECT)_index, _localPos);
		}

		virtual Eigen::Vector3f	GetLocalPosition(int _index) const noexcept
		{
			assert(joint != nullptr);

			return joint->GetLocalPosition((ZonaiPhysics::ZnJoint::eOBJECT)_index);
		}

		/**
		오브젝트의 로테이션
		*/
		virtual void		SetLocalQuaternion(int _index, const Eigen::Quaternionf& _localQuat) noexcept
		{
			assert(joint != nullptr);

			joint->SetLocalQuaternion((ZonaiPhysics::ZnJoint::eOBJECT)_index, _localQuat);
		}

		virtual Eigen::Quaternionf	GetLocalQuaternion(int _index) const noexcept
		{
			assert(joint != nullptr);

			return joint->GetLocalQuaternion((ZonaiPhysics::ZnJoint::eOBJECT)_index);
		}

		/**
		오브젝트0을 기준으로 오브젝트1의 상대 속도를 반환함.
		*/
		virtual Eigen::Vector3f	GetRelativeLinearVelocity() const noexcept
		{
			assert(joint != nullptr);

			return joint->GetRelativeLinearVelocity();
		}
		
		virtual Eigen::Vector3f	GetRelativeAngularVelocity() const noexcept
		{
			assert(joint != nullptr);

			return joint->GetRelativeAngularVelocity();
		}

		/**
		조인트 파괴 힘을 설정함
		*/
		virtual void		SetBreakForce(float _force, float _torque) noexcept
		{
			assert(joint != nullptr);

			joint->SetBreakForce(_force, _torque);
		}

		virtual void		GetBreakForce(float& _force, float& _torque) const noexcept
		{
			assert(joint != nullptr);

			joint->GetBreakForce(_force, _torque);
		}
	};
}

