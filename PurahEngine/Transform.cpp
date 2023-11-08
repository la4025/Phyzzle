#include "Transform.h"

#define M_PI       3.14159265358979323846

PurahEngine::Transform::Transform() :
	position(Eigen::Vector3f::Zero()),
	rotation(Eigen::Quaternionf::Identity()),
	scale(1,1,1),
	parentTransform(nullptr)
{

}

PurahEngine::Transform::~Transform()
{

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
	if (parentTransform != nullptr)
	{
		Eigen::Vector3f parentPosition = parentTransform->GetWorldPosition();
		//return parentPosition.transpose() * position;
		/// 이부분은 인재원 가서 수정할 것
		return parentPosition.cwiseProduct(position);
	}
	else
	{
		return position;
	}
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
	Eigen::Matrix4f localTransform = Eigen::Matrix4f::Identity();
	localTransform.block<3, 1>(0, 3) = position;

	// Rotation
	float angle = 45.0 * M_PI / 180.0;

}

Eigen::Matrix4f PurahEngine::Transform::GetWorldMatrix() const
{
	Eigen::Matrix4f worldTransform = Eigen::Matrix4f::Identity();
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
