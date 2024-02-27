#pragma once
#include "../../FloaterMath/include/Matrix4f.h"
#include "../../FloaterMath/include/Common.h"
#include "Transform.h"

namespace flt
{
	struct Camera
	{
	public:
		Camera(Transform* pTransform) : Camera(pTransform, DegToRad(60.f), 0.01f, 1000.f, 3.2f, 1.8f, false) {}
		Camera(Transform* pTransform, float fieldOfView, float nearZ, float farZ, float viewWidth, float viewHeight, bool isOrtho) noexcept :
			_pTransform(pTransform),
			_fieldOfView(fieldOfView),
			_near(nearZ),
			_far(farZ),
			_viewWidth(viewWidth),
			_viewHeight(viewHeight),
			_isOrtho(isOrtho) {}
		Matrix4f GetViewMatrix() const noexcept;
		Matrix4f GetProjectionMatrix() const noexcept;


	private:
		Transform* _pTransform;

		float _fieldOfView;
		float _near;
		float _far;
		float _viewWidth;
		float _viewHeight;

		bool _isOrtho;
	};
}
