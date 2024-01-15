#include "Transform.h"
#include "RigidBody.h"
#include "GameObject.h"

#define M_PI       3.14159265358979323846

PurahEngine::Transform::Transform() :
	position(Eigen::Vector3f::Zero()),
	rotation(Eigen::Quaternionf::Identity()),
	scale(1, 1, 1),
	parentTransform(nullptr),
	rigidbody(nullptr)
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
	
	if (rigidbody != nullptr)
	{
		rigidbody->SetPosition(setPosition);
	}
}

void PurahEngine::Transform::SetLocalRotation(Eigen::Quaternionf setRotation)
{
	rotation = setRotation;
}

void PurahEngine::Transform::SetLocalScale(Eigen::Vector3f setScale)
{
	scale = setScale;
}

void PurahEngine::Transform::SetWorldPosition(Eigen::Vector3f setPosition)
{
	/// 이론 : WorldMatrix 에서 Position 부분만 교체한다.
	// Eigen::Matrix4f worldMatrix = GetWorldMatrix();
	// worldMatrix.block<3, 1>(0, 3) = setPosition;
	

	if (parentTransform != nullptr)
	{
		position = -(parentTransform->GetWorldPosition()) + setPosition;
	}
	else
	{
		position = setPosition;
	}

	if (rigidbody != nullptr)
	{
		rigidbody->SetPosition(setPosition);
	}
}

void PurahEngine::Transform::SetWorldRotation(Eigen::Quaternionf setRotation)
{

	if (parentTransform != nullptr)
	{
		rotation = parentTransform->GetWorldRotation().conjugate() * setRotation;
	}
	else
	{
		rotation = setRotation;
	}
}

void PurahEngine::Transform::SetParent(PurahEngine::Transform* parentObject)
{
	if (parentTransform == nullptr)
	{
		parentTransform = parentObject;
		parentObject->children.push_back(this);
	}
	else
	{
		// erase, remove 각각 따로 쓰게 될 경우 단점이 있다.
		// erase 는 값을 삭제하면 size가 줄어든다. 하지만 return 값이 의미가 없다.
		// remove 는 값을 삭제해도 size가 줄어들지 않는다. return 값이 남은 값들 중 마지막 값의 바로 뒤를 가리킨다.
		// 고로 erase(remove())를 하게 되면 remove의 return으로 정렬을 하고, erase를 하게되면 size까지 줄어들게 할 수 있다.
		parentTransform->children.erase(remove(parentTransform->children.begin(), parentTransform->children.end(), this), parentTransform->children.end());

		// 위의 과정에서 parentTransform을 nullptr로 만들어줬다.
		parentTransform = parentObject;
		parentObject->children.push_back(this);
	}
}

void PurahEngine::Transform::SetRigidBody(RigidBody* rigid)
{
	assert(rigidbody == nullptr || (rigid == nullptr && rigidbody != nullptr));
	
	rigidbody = rigid;

}
