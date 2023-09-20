#pragma once
#include "ZonaiMath.h"

namespace ZonaiEngine
{
	class Transform
	{
	private:
		Transform* parent_;
		ZonaiMath::Vector3D position_;
		ZonaiMath::Quaternion rotation_;
		ZonaiMath::Vector3D scale_;
	
	public:
#pragma region Property
		__declspec(property(put = SetParent, get = GetParent)) Transform* parent;

		/// <summary>
		/// 월드 스페이스 트랜스폼 정보
		/// </summary>
		__declspec(property(put = SetWorldPosition, get = GetWorldPosition)) ZonaiMath::Vector3D position;
		__declspec(property(put = SetWorldRotation, get = GetWorldRotation)) ZonaiMath::Quaternion rotation;
		__declspec(property(put = SetWorldEulerAngles, get = GetWorldEulerAngles)) ZonaiMath::Vector3D eulerAngles;
		__declspec(property(put = SetWorldScale, get = GetWorldScale)) ZonaiMath::Vector3D scale;

		/// <summary>
		/// 로컬 스페이스 트랜스폼 정보
		/// </summary>
		__declspec(property(put = SetLocalPosition, get = GetLocalPosition)) ZonaiMath::Vector3D localPosition;
		__declspec(property(put = SetLocalRotation, get = GetLocalRotation)) ZonaiMath::Quaternion localRotation;
		__declspec(property(put = SetLocalEulerAngles, get = GetLocalEulerAngles)) ZonaiMath::Vector3D localEulerAngles;
		__declspec(property(put = SetLocalScale, get = GetLocalScale)) ZonaiMath::Vector3D localScale;

		/// <summary>
		/// 월드 스페이스 정보
		/// </summary>
		__declspec(property(put = SetRight, get = GetRight)) ZonaiMath::Vector3D right;			// x
		__declspec(property(put = SetUp, get = GetUp)) ZonaiMath::Vector3D up;					// y
		__declspec(property(put = SetForward, get = GetForward)) ZonaiMath::Vector3D forward;	// z

		__declspec(property(get = GetLocalTransform)) ZonaiMath::Matrix4x4 localTransform;
		__declspec(property(get = GetWorldTransform)) ZonaiMath::Matrix4x4 worldTransform;
#pragma endregion Property

#pragma region Get
		Transform* GetParent() noexcept;

		const ZonaiMath::Vector3D& GetLocalPosition() noexcept;
		const ZonaiMath::Quaternion& GetLocalRotation() noexcept;
		const ZonaiMath::Vector3D GetLocalEulerAngles() noexcept;
		const ZonaiMath::Vector3D& GetLocalScale() noexcept;

		const ZonaiMath::Vector3D GetWorldPosition() noexcept;
		const ZonaiMath::Quaternion GetWorldRotation() noexcept;
		const ZonaiMath::Vector3D GetWorldEulerAngles() noexcept;
		const ZonaiMath::Vector3D GetWorldScale() noexcept;

		const ZonaiMath::Vector3D GetRight() noexcept;
		const ZonaiMath::Vector3D GetUp() noexcept;
		const ZonaiMath::Vector3D GetForward() noexcept;

		const ZonaiMath::Matrix4x4 GetLocalTransform() noexcept;
		const ZonaiMath::Matrix4x4 GetWorldTransform() noexcept;
#pragma endregion Get

#pragma region Set
		void SetParent(Transform*) noexcept;

		void SetLocalPosition(const ZonaiMath::Vector3D&) noexcept;
		void SetLocalRotation(const ZonaiMath::Quaternion&) noexcept;
		void SetLocalEulerAngles(const ZonaiMath::Vector3D&) noexcept;
		void SetLocalScale(const ZonaiMath::Vector3D&) noexcept;

		void SetWorldPosition(const ZonaiMath::Vector3D&) noexcept;
		void SetWorldRotation(const ZonaiMath::Quaternion&) noexcept;
		void SetWorldEulerAngles(const ZonaiMath::Vector3D&) noexcept;
		void SetWorldScale(const ZonaiMath::Vector3D&) noexcept;

		void SetRight(const ZonaiMath::Vector3D&) noexcept;
		void SetUp(const ZonaiMath::Vector3D&) noexcept;
		void SetForward(const ZonaiMath::Vector3D&) noexcept;
#pragma endregion Set
	};
}

