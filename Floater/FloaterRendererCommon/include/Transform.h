#pragma once

#include "../../FloaterMath/include/Matrix4f.h"
#include "../../FloaterMath/include/Vector3f.h"
#include "../../FloaterMath/include/Vector4f.h"
#include "../../FloaterMath/include/Quaternion.h"
#include <vector>


namespace flt
{
	class Transform
	{
	public:
		Transform() : _position(), _scale(1.0f, 1.0f, 1.0f, 0.0f), _rotation(),
		_pParent(nullptr), _children() {}

		void SetMatrix(const Matrix4f& worldMatrix);

		void SetPosition(float x, float y, float z);
		void SetPosition(const Vector4f& position);
		Vector4f GetPosition() const { return _position; }

		void SetRotation(float degreeX, float degreeY, float degreeZ);
		void SetRotation(const Vector3f& axis, float radian);
		void SetRotation(const Quaternion& q);
		Quaternion GetRotation() const { return _rotation; }

		void SetScale(float x, float y, float z);
		void SetScale(const Vector4f& scale);
		Vector4f GetScale() const { return _scale; }

		void AddPosition(float x, float y, float z);
		void AddPosition(const Vector4f& position);
		void AddRotation(const Vector3f& axis, float radian);
		void AddRotation(const Quaternion& q);
		void AddScale(float x, float y, float z);

		Matrix4f GetTransformMatrix4f() const;
		Matrix4f GetRotationMatrix4f() const;
		Matrix4f GetScaleMatrix4f() const;
		Matrix4f GetMatrix4f() const;

		void LookAt(Vector4f target);

		Transform* GetParent() const { return _pParent; }
		Transform* GetChild(size_t index) const { return _children[index]; }
		const std::vector<Transform*>& GetChildren() const { return _children; }

		bool SetParent(Transform* pParent);

	private:
		Vector4f _position;
		Vector4f _scale;
		Quaternion _rotation;

		Transform* _pParent;
		std::vector<Transform*> _children;
	};
}
