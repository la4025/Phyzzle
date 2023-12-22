#pragma once
#include <algorithm>

namespace Eigen
{
	class Vector3f;
	class Quaternionf;
}

namespace ZonaiPhysics
{
	class ZnTransform
	{
	public:
		ZnTransform() noexcept = default;

		ZnTransform(const Eigen::Vector3f& _vec) noexcept : position(_vec), quaternion()
		{}

		ZnTransform(const Eigen::Quaternionf& _qat) noexcept : position(), quaternion(_qat)
		{}

		ZnTransform(const Eigen::Vector3f& _vec, const Eigen::Quaternionf& _qat) noexcept : position(_vec), quaternion(_qat)
		{}

		ZnTransform(const ZnTransform& _tran) noexcept : position(_tran.position), quaternion(_tran.quaternion)
		{}

		ZnTransform& operator=(const ZnTransform& _tran) noexcept
		{
			if (this != & _tran)
			{
				position = _tran.position;
				quaternion = _tran.quaternion;
			}

			return *this;
		}

		ZnTransform& operator=(ZnTransform&& _tran) noexcept
		{
			if (this != &_tran)
			{
				position = std::move(_tran.position);
				quaternion = std::move(_tran.quaternion);
			}

			return *this;
		}

	public:
		Eigen::Vector3f position;
		Eigen::Quaternionf quaternion;
	};
}