#include "Transform.h"

#define M_PI       3.14159265358979323846

PurahEngine::Transform::Transform() :
	position(Eigen::Vector3f::Zero()),
	rotation(Eigen::Quaternionf::Identity()),
	scale(1, 1, 1),
	parentTransform(nullptr)
{

}

PurahEngine::Transform::~Transform()
{

}

void PurahEngine::Transform::Rotate(Eigen::Vector3f axis, float angle)
{
	// axis는 UnitX(), UnitY(), UnitZ()로 할것
	rotation = Eigen::AngleAxisf(angle * (M_PI / 180.f), axis) * rotation;
}

Eigen::Vector3f PurahEngine::Transform::GetLocalPosition() const
{
	return position;
}

Eigen::Quaternionf PurahEngine::Transform::GetLocalRotation() const
{
	return rotation;
}

Eigen::Vector3f PurahEngine::Transform::GetLocalScale() const
{
	return scale;
}

Eigen::Vector3f PurahEngine::Transform::GetWorldPosition() const
{
	Eigen::Vector3f worldPosition = GetWorldMatrix().block<3, 1>(0, 3);
	return worldPosition;
}

Eigen::Quaternionf PurahEngine::Transform::GetWorldRotation() const
{
	if (parentTransform != nullptr)
	{
		Eigen::Quaternionf parentRotation = parentTransform->GetWorldRotation();
		return parentRotation * rotation;
	}
	else
	{
		return rotation;
	}
}

Eigen::Vector3f PurahEngine::Transform::GetWorldScale() const
{
	if (parentTransform != nullptr)
	{
		Eigen::Vector3f parentScale = parentTransform->GetWorldScale();

		//return parentScale.transpose() * scale;
		return parentScale.cwiseProduct(scale);
	}
	else
	{
		return scale;
	}
}

Eigen::Matrix4f PurahEngine::Transform::GetLocalMatrix() const
{
	// Translation	
	Eigen::Affine3f localTransform = Eigen::Affine3f::Identity();
	localTransform.translation() = position;
	// rotation
	localTransform.linear() = rotation.toRotationMatrix();
	// scale
	localTransform.scale(scale);

	return localTransform.matrix();
}

Eigen::Matrix4f PurahEngine::Transform::GetWorldMatrix() const
{
	if (parentTransform != nullptr)
	{
		Eigen::Affine3f worldTransform = Eigen::Affine3f::Identity();
		worldTransform.translation() = position;
		worldTransform.linear() = rotation.toRotationMatrix();
		worldTransform.scale(scale);

		return parentTransform->GetWorldMatrix() * GetLocalMatrix();
	}
	else
	{
		return GetLocalMatrix();
	}
}

void PurahEngine::Transform::SetLocalPosition(Eigen::Vector3f setPosition)
{
	position = setPosition;
}

void PurahEngine::Transform::SetLocalRotation(Eigen::Quaternionf setRotation)
{
	rotation = setRotation;
}

void PurahEngine::Transform::SetLocalScale(Eigen::Vector3f setScale)
{
	scale = setScale;
}
