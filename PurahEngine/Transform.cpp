#include "Transform.h"

PurahEngine::Transform::Transform() :
	position(ZonaiMath::Vector3D::Zero),
	rotation(ZonaiMath::Quaternion::Identity),
	scale(ZonaiMath::Vector3D(1.0f, 1.0f, 1.0f))
{

}

PurahEngine::Transform::~Transform()
{

}

ZonaiMath::Vector3D PurahEngine::Transform::GetLocalPosition() const
{
	return position;
}

ZonaiMath::Quaternion PurahEngine::Transform::GetLocalRotation() const
{
	return rotation;
}

ZonaiMath::Vector3D PurahEngine::Transform::GetLocalScale() const
{
	return scale;
}

void PurahEngine::Transform::SetLocalPosition(ZonaiMath::Vector3D setPosition)
{
	position = setPosition;
}

void PurahEngine::Transform::SetLocalRotation(ZonaiMath::Quaternion serQuaternion)
{

}

void PurahEngine::Transform::SetLocalScale(ZonaiMath::Vector3D setScale)
{
	scale = setScale;
}
