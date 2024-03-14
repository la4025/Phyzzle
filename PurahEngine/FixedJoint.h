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
		void PreSerialize(json& jsonData) const override;
		void PreDeserialize(const json& jsonData) override;
		void PostSerialize(json& jsonData) const override;
		void PostDeserialize(const json& jsonData) override;
	};
}

