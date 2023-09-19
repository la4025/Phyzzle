#pragma once

namespace ZonaiMath
{
	struct Vector2D;

	class Matrix1x2
	{
	public:
		/// <summary>
	/// Æ¯¼ö ¸â¹ö ÇÔ¼ö
	/// </summary>
		constexpr Matrix1x2() noexcept :
			e{}
		{}
		constexpr Matrix1x2(
			float _e00, float _e01) noexcept :
			e{ _e00, _e01 }
		{}
		constexpr Matrix1x2(const Matrix1x2&) noexcept = default;
		constexpr Matrix1x2(Matrix1x2&&) noexcept = default;
		~Matrix1x2() noexcept = default;

		constexpr Matrix1x2& operator= (const Matrix1x2&) noexcept = default;
		constexpr Matrix1x2& operator= (Matrix1x2&&) noexcept = default;

		explicit operator Vector2D() noexcept;

		/// <summary>
		/// ¸â¹ö º¯¼ö
		/// </summary>
		union
		{
			struct
			{
				float e00, e01;
			};
			float e[2];
		};

		Matrix1x2& operator += (const Matrix1x2&) noexcept;
		Matrix1x2& operator -= (const Matrix1x2&) noexcept;

		Matrix1x2 operator + (const Matrix1x2&) const noexcept;
		Matrix1x2 operator - (const Matrix1x2&) const noexcept;
		Matrix1x2 operator - () const noexcept;

		Matrix1x2& operator *= (const float) noexcept;
		Matrix1x2& operator /= (const float) noexcept;

		Matrix1x2 operator * (const float) const noexcept;
		Matrix1x2 operator / (const float) const noexcept;
		bool operator == (const Matrix1x2&) const noexcept;
	};
}

