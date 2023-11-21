#pragma once

namespace ZonaiMath
{
	class Vector3D;
	class Quaternion;
}


namespace ZonaiPhysics
{
	using namespace ZonaiMath;
	
	class ZnObject;

	class ZnJoint
	{
	public:
							ZnJoint() noexcept = default;
		virtual				~ZnJoint() noexcept = 0;

	public:
		/**
		오브젝트를 조인트로 연결함.
		둘 중 하나는 NULL일 수 있음.
		*/
		virtual void		SetObject(ZnObject*, ZnObject*) noexcept = 0;
		virtual void		GetObject(ZnObject*&, ZnObject*&) const noexcept = 0;
		
		/**
		오브젝트의 포지션
		*/
		virtual void		SetLocalPosition(const Vector3D&) noexcept = 0;
		virtual Vector3D	GetLocalPosition() const noexcept = 0;

		/**
		오브젝트의 로테이션
		*/
		virtual void		SetLocalQuaternion(const Quaternion&) noexcept = 0;
		virtual Quaternion	GetLocalQuaternion() const noexcept = 0;

		/**
		오브젝트0을 기준으로 오브젝트1의 상대 속도를 반환함.
		*/
		virtual Vector3D	GetRelativeLinearVelocity() const noexcept = 0;
		virtual Vector3D	GetRelativeAngularVelocity() const noexcept = 0;

		/**
		조인트 파괴 힘을 설정함
		*/
		virtual void		SetBreakForce(float _force, float _torque) noexcept = 0;
		virtual void		GetBreakForce(float& _force, float& _torque) const noexcept = 0;
	};
}