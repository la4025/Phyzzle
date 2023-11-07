#include "../../FloaterUtil/include/FloaterMacro.h"
#include "Vector3f.h"

flt::Vector3f flt::Vector3f::Lerp(const Vector3f& v0, const Vector3f& v1, float t) noexcept
{
	ASSERT(t >= 0.0f && t <= 1.0f, "");
	return v0 + (v1 - v0) * t;
}
