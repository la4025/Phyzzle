#pragma once
#include "ZonaiMath.h"

namespace ZonaiPhysics
{
	class IRigidBody;
}

namespace ZonaiEngine
{
	class RigidBody
	{
	public:
		RigidBody() noexcept = default;
		virtual ~RigidBody() noexcept = default;

	private:
		ZonaiPhysics::IRigidBody* rigidbody;

	public:
		_declspec(property(put = SetMass, get = GetMass)) float mass;							// Áú·®
		_declspec(property(put = SetRestitution, get = GetRestitution)) float restitution;		// 
		_declspec(property(put = SetLinerDrag)) float linerDrag;								// ¼±Çü °¨¼è
		_declspec(property(put = SetAngularDrag)) float angularDrag;							// È¸Àü °¨¼è
		_declspec(property(put = SetKinematic, get = isKinematic)) bool kinematic;				// 
		// _declspec(property(put = )) bool freezeX;
		// _declspec(property(put = )) bool freezeY;
		// _declspec(property(put = )) bool freezeZ;

		_declspec(property(put = SetVelocity, get = GetVelocity)) ZonaiMath::Vector3D velocity;
		_declspec(property(put = SetAngularVelocity, get = GetAngularVelocity)) ZonaiMath::Vector3D angularVelocity;

#pragma region Get
		bool isKinematic() noexcept;
		const float GetMass() noexcept;
		const float GetRestitution() noexcept;
		const ZonaiMath::Vector3D GetVelocity() noexcept;
		const ZonaiMath::Vector3D GetAngularVelocity() noexcept;
#pragma endregion Get

#pragma region Set
		void SetKinematic(bool) noexcept;
		void SetMass(const float) noexcept;
		void SetRestitution(const float) noexcept;
		void SetLinerDrag(const float) noexcept;
		void SetAngularDrag(const float) noexcept;
		void SetVelocity(const ZonaiMath::Vector3D&) noexcept;
		void SetAngularVelocity(const ZonaiMath::Vector3D&) noexcept;
#pragma endregion Set

		bool isAwake() noexcept;
		void AddForce(const ZonaiMath::Vector3D&) noexcept;
		void AddTorque(const ZonaiMath::Vector3D&) noexcept;
		void AddImpulse(const ZonaiMath::Vector3D&, const ZonaiMath::Vector3D&) noexcept;
	};
}

