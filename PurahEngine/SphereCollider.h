#pragma once
#include "PurahEngineAPI.h"
#include "Collider.h"

namespace PurahEngine
{
	class PURAHENGINE_API SphereCollider : public Collider
	{
	public:
		SphereCollider() = default;
		~SphereCollider() override = default;

	public:
		void Awake() override;

	public:
		void SetSize(float _radius);

	public:
		virtual void PreSerialize(json& jsonData) const override;
		virtual void PreDeserialize(const json& jsonData) override;
		virtual void PostSerialize(json& jsonData) const override;
		virtual void PostDeserialize(const json& jsonData) override;

	public:
		float radius;
	};
}
