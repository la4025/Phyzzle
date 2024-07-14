#pragma once
#include "Eigen/Dense"

namespace Phyzzle
{
	class Spring
	{
	public:
		virtual ~Spring() = default;
	};

	class PositionSpring : public Spring
	{
	public:
		void UpdateVelocity(
			const Eigen::Vector3f& _s, Eigen::Vector3f& _v, const Eigen::Vector3f& _e,
			float _zeta, float _omega, float _dt)
		{
			const float f = 1.f + 2.f * _dt * _zeta * _omega;
			const float oo = _omega * _omega;
			const float hoo = _dt * oo;
			const float hhoo = _dt * hoo;
			const float detInv = 1.f / (f + hhoo);
			const Eigen::Vector3f detV = _v + hoo * (_e - _s);

			_v = detV * detInv;
		}
	};

	class QuaternionSpring : public Spring
	{
	private:
		static constexpr float EPSILON = 1e-5f;

		float halflife_to_damping(float halflife, float eps = EPSILON) const
		{
			return (4.0f * 0.69314718056f) / (halflife + eps);
		}

		Eigen::Quaternionf quat_exp(const Eigen::Vector3f& v, float eps = EPSILON) const
		{
			const float halfangle = v.norm();
			if (halfangle < eps)
			{
				return Eigen::Quaternionf(1.0f, v.x(), v.y(), v.z()).normalized();
			}
			else
			{
				const float c = std::cosf(halfangle);
				const float s = std::sinf(halfangle) / halfangle;
				return Eigen::Quaternionf(c, s * v.x(), s * v.y(), s * v.z());
			}
		}

		Eigen::Vector3f quat_log(const Eigen::Quaternionf& q, float eps = EPSILON) const
		{
			const float length = q.norm();
			if (length < eps)
			{
				return Eigen::Vector3f(q.x(), q.y(), q.z());
			}
			else
			{
				const float halfangle = acosf(std::clamp(q.w(), -1.0f, 1.0f));
				return halfangle * (Eigen::Vector3f(q.x(), q.y(), q.z()) / length);
			}
		}

		Eigen::Vector3f quat_log_approx(const Eigen::Quaternionf& q) const
		{
			return Eigen::Vector3f(q.x(), q.y(), q.z());
		}

		Eigen::Quaternionf quat_from_scaled_angle_axis(const Eigen::Vector3f& v, float eps = EPSILON) const
		{
			return quat_exp(v / 2.0f, eps);
		}

		Eigen::Vector3f quat_to_scaled_angle_axis(const Eigen::Quaternionf& q, float eps = EPSILON) const
		{
			return 2.0f * quat_log(q, eps);
		}

		float fast_negexp(float x) const
		{
			return 1.0f / (1.0f + x + 0.48f * x * x + 0.235f * x * x * x);
		}

	public:
		void UpdateVelocity(
			const Eigen::Quaternionf& x,
			Eigen::Vector3f& v,
			const Eigen::Quaternionf& x_goal,
			float halflife,
			float dt)
		{
			Eigen::Quaternionf goal = x_goal;
			Eigen::Quaternionf minusGoal = { -x_goal.w(), -x_goal.x() , -x_goal.y() , -x_goal.z() };

			float xDot = x.dot(x_goal);
			float minusDot = x.dot(minusGoal);

			if (xDot < minusDot)
				goal = minusGoal;

			const float y = halflife_to_damping(halflife) / 2.0f;

			const Eigen::Vector3f j0 = quat_to_scaled_angle_axis(x * goal.inverse());
			const Eigen::Vector3f j1 = v + j0 * y;

			const float eydt = fast_negexp(y * dt);

			v = eydt * (v - j1 * y * dt);
		}
	};
}
