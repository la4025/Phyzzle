#include "Vector3D.h"

#include "Matrix1x3.h"

namespace ZonaiMath
{
	Matrix1x3::operator Vector3D() noexcept
	{
		return Vector3D{ e00, e01, e02 };
	}

	Matrix1x3& Matrix1x3::operator+=(const Matrix1x3& other) noexcept
	{
		this->e00 += other.e00;
		this->e01 += other.e01;
		this->e02 += other.e02;

		return *this;
	}

	Matrix1x3& Matrix1x3::operator-=(const Matrix1x3& other) noexcept
	{
		this->e00 -= other.e00;
		this->e01 -= other.e01;
		this->e02 -= other.e02;

		return *this;
	}

	Matrix1x3 Matrix1x3::operator+(const Matrix1x3& other) const noexcept
	{
		return Matrix1x3
		{
			this->e00 + other.e00,
			this->e01 + other.e01,
			this->e02 + other.e02,
		};
	}

	Matrix1x3 Matrix1x3::operator-(const Matrix1x3& other) const noexcept
	{
		return Matrix1x3
		{
			this->e00 - other.e00,
			this->e01 - other.e01,
			this->e02 - other.e02,
		};
	}

	Matrix1x3 Matrix1x3::operator-() const noexcept
	{
		return Matrix1x3
		{
			-this->e00,
			-this->e01,
			-this->e02,
		};
	}

	Matrix1x3& Matrix1x3::operator*=(const float n) noexcept
	{
		this->e00 *= n;
		this->e01 *= n;
		this->e02 *= n;

		return *this;
	}

	Matrix1x3& Matrix1x3::operator/=(const float n) noexcept
	{
		float invN = 1.f / n;
		this->e00 *= invN;
		this->e01 *= invN;
		this->e02 *= invN;

		return *this;
	}

	Matrix1x3 Matrix1x3::operator*(const float n) const noexcept
	{
		Matrix1x3 temp;
		temp.e00 = this->e00 * n;
		temp.e01 = this->e01 * n;
		temp.e02 = this->e02 * n;

		return temp;
	}

	Matrix1x3 Matrix1x3::operator/(const float n) const noexcept
	{
		float invN = 1.f / n;

		Matrix1x3 temp;
		temp.e00 = this->e00 * invN;
		temp.e01 = this->e01 * invN;
		temp.e02 = this->e02 * invN;

		return temp;
	}

	bool Matrix1x3::operator==(const Matrix1x3& other) const noexcept
	{
		return (this->e00 == other.e00) && (this->e01 == other.e01) && (this->e02 == other.e02);
	}
}