#pragma once

namespace ZonaiPhysics
{
	class RigidBody;

	class JointBase
	{
	public:
		JointBase() noexcept = delete;
		JointBase(RigidBody*, RigidBody*) noexcept;
		virtual ~JointBase() noexcept;

	public:
		virtual void Set() noexcept;
		virtual void PreStep(float _dT) noexcept;
		virtual void Step(float _dT) noexcept;
	};
}
