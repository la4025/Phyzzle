#pragma once
#include "Joint.h"
#include "ZnHingeJoint.h"

namespace PurahEngine
{
	class HingeJoint : public Joint<ZonaiPhysics::ZnHingeJoint>
	{
	public:
		HingeJoint() = default;
		~HingeJoint() override;

	public:
		void PreSerialize(json& jsonData) const override;
		void PreDeserialize(const json& jsonData) override;
		void PostSerialize(json& jsonData) const override;
		void PostDeserialize(const json& jsonData) override;
	};
}
