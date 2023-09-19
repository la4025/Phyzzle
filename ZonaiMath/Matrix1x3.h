#pragma once

namespace ZonaiMath
{
	struct Vector3D;

	class Matrix1x3
	{
	public:
		/// <summary>
	/// Æ¯¼ö ¸â¹ö ÇÔ¼ö
	/// </summary>
		constexpr Matrix1x3() noexcept :
			e{}
		{}
		constexpr Matrix1x3(
			float _e00, float _e01, float _e02) noexcept :
			e{ _e00, _e01, _e02 }
		{}
		constexpr Matrix1x3(const Matrix1x3&) noexcept = default;
		constexpr Matrix1x3(Matrix1x3&&) noexcept = default;
		~Matrix1x3() noexcept = default;

		constexpr Matrix1x3& operator= (const Matrix1x3&) noexcept = default;
		constexpr Matrix1x3& operator= (Matrix1x3&&) noexcept = default;

		explicit operator Vector3D() noexcept;

		/// <summary>
		/// ¸â¹ö º¯¼ö
		/// </summary>
		union
		{
			struct
			{
				float e00, e01, e02;
			};
			float e[3];
		};

		Matrix1x3& operator += (const Matrix1x3&) noexcept;
		Matrix1x3& operator -= (const Matrix1x3&) noexcept;

		Matrix1x3 operator + (const Matrix1x3&) const noexcept;
		Matrix1x3 operator - (const Matrix1x3&) const noexcept;
		Matrix1x3 operator - () const noexcept;

		Matrix1x3& operator *= (const float) noexcept;
		Matrix1x3& operator /= (const float) noexcept;

		Matrix1x3 operator * (const float) const noexcept;
		Matrix1x3 operator / (const float) const noexcept;
		bool operator == (const Matrix1x3&) const noexcept;
	};
}

