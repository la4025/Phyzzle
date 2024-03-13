#pragma once
#include "ZnFixedJoint.h"
#include "Joint.h"

namespace PurahEngine
{
	class FixedJoint : public Joint<ZonaiPhysics::ZnFixedJoint>
	{
	public:
		FixedJoint() = default;
		~FixedJoint() override;

	public:
		virtual void PreSerialize(json& jsonData) const override;
		virtual void PreDeserialize(const json& jsonData) override;
		virtual void PostSerialize(json& jsonData) const override;
		virtual void PostDeserialize(const json& jsonData) override;
	};
}

