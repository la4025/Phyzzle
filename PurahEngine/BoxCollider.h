#pragma once
#include "PurahEngineAPI.h"
#include "Collider.h"

namespace PurahEngine
{
	class PURAHENGINE_API BoxCollider final : public Collider
	{
	public:
		BoxCollider() = default;
		~BoxCollider() override = default;

		void Awake() override;

	public:
		void SetSize(const Eigen::Vector3f& _size);

	public:
		Eigen::Vector3f size{ 1.f, 1.f, 1.f };
	};
}
