#pragma once
#include <Eigen/Dense>

#include "ForceType.h"

namespace ZonaiPhysics
{
	class RigidBodyHelper
	{
	public:
		static void				Detach(void* _body, void* _shape) noexcept;
		static void				Attach(void* _body, void* _shape) noexcept;

	public:
		static void				Release(void*) noexcept;

		static void				WakeUp(void*) noexcept;
		static bool				IsSleeping(void*) noexcept;

		static void				UseGravity(void*, bool) noexcept;

		static void				SetKinematic(void*, bool) noexcept;

		static void				UpdateInertiaTensor(void*) noexcept;

		static void				Disable(void*, bool);

		static uint8_t			GetDynamicLockFlags(void*) noexcept;
		static void				SetDynamicLockFlag(void*, uint8_t flag, bool) noexcept;
		static void				SetDynamicLockFlags(void*, uint8_t flags) noexcept;

		static void*			GetUserData(void*) noexcept;
		static void				SetUserData(void*, void*) noexcept;

	public:
		// Áú·®
		static float			GetMass(void*) noexcept;
		static void				SetMass(void*,float) noexcept;
		static float			GetInvMass(void*) noexcept;

		static Eigen::Vector3f	GetInertiaTensor(void*) noexcept;
		static void				SetInertiaTensor(void*, const Eigen::Vector3f&) noexcept;

		static float			GetLinearDamping(void*) noexcept;
		static void				SetLinearDamping(void*, float) noexcept;

		static float			GetAngularDamping(void*) noexcept;
		static void				SetAngularDamping(void*, float) noexcept;

	public:
		// ¼Óµµ
		static Eigen::Vector3f	GetLinearVelocity(void*) noexcept;
		static void				SetLinearVelocity(void*, const Eigen::Vector3f&) noexcept;

		static Eigen::Vector3f	GetAngularVelocity(void*) noexcept;
		static void				SetAngularVelocity(void*, const Eigen::Vector3f&) noexcept;

		static float			GetMaxLinearVelocity(void*) noexcept;
		static void				SetMaxLinearVelocity(void*, const float&) noexcept;

		static float			GetMaxAngularVelocity(void*) noexcept;
		static void				SetMaxAngularVelocity(void*, const float&) noexcept;

		static Eigen::Vector3f	GetPosition(void*) noexcept;
		static void				SetPosition(void*, const Eigen::Vector3f&) noexcept;

		static Eigen::Quaternionf	GetQuaternion(void*) noexcept;
		static void				SetQuaternion(void*, const Eigen::Quaternionf&) noexcept;

		// Èû
		static void				SetForceAndTorque(void*, const Eigen::Vector3f& _force, const Eigen::Vector3f& _torque, ForceType _type) noexcept;

		static void				AddForce(void*, const Eigen::Vector3f&, ForceType) noexcept;
		static void				ClearForce(void*) noexcept;

		static void				AddTorque(void*, const Eigen::Vector3f&, ForceType) noexcept;
		static void				ClearTorque(void*) noexcept;
	};
}

