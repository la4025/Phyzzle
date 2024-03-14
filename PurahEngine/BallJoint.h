#pragma once
#include "Joint.h"
#include "ZnSphericalJoint.h"

namespace PurahEngine
{
	class BallJoint : public Joint<ZonaiPhysics::ZnSphericalJoint>
	{
	public:
		BallJoint() = default;
		~BallJoint() override;

	public:
		void PreSerialize(json& jsonData) const override;
		void PreDeserialize(const json& jsonData) override;
		void PostSerialize(json& jsonData) const override;
		void PostDeserialize(const json& jsonData) override;
	};
}
