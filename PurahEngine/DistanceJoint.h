#pragma once
#include "Joint.h"
#include "ZnDistanceJoint.h"

namespace PurahEngine
{
	class DistanceJoint : public Joint<ZonaiPhysics::ZnDistanceJoint>
	{
	public:
		DistanceJoint() = default;
		~DistanceJoint() override;

	public:
		void PreSerialize(json& jsonData) const override;
		void PreDeserialize(const json& jsonData) override;
		void PostSerialize(json& jsonData) const override;
		void PostDeserialize(const json& jsonData) override;
	};
}

