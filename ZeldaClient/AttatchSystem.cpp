#include "AttatchSystem.h"

#include <numbers>

#include "RigidBody.h"


namespace Phyzzle
{
	AttatchSystem::~AttatchSystem()
	= default;

	void AttatchSystem::Update()
	{

	}

	bool AttatchSystem::HasSelectedBody() const
	{
		return selectBody != nullptr;
	}

	void AttatchSystem::SelectBody(PurahEngine::RigidBody* _body)
	{
		selectBody = _body;

		gravity = selectBody->HasGravity();
		mass = selectBody->GetMass();

		selectBody->UseGravity(false);
		selectBody->SetMass(0.001f);
	}

	void AttatchSystem::Cancel()
	{
		selectBody->UseGravity(gravity);
		selectBody->SetMass(mass);

		selectBody = nullptr;
	}

	void AttatchSystem::XRotate(float _degree)
	{
		const float radian = (_degree / 180.f) * std::numbers::pi_v<float>;
		 
		const auto R = selectBody->GetRotation();
		// Eigen::Vector3f right = Eigen::Vector3f{ 1.f, 0.f, 0.f } * R.toRotationMatrix();
	}

	void AttatchSystem::YRotate(float _degree)
	{
		const float radian = (_degree / 180.f) * std::numbers::pi_v<float>;
		
		const auto R = selectBody->GetRotation();
		// Eigen::Vector3f up = Eigen::Vector3f{ 0.f, 1.f, 0.f } * R.toRotationMatrix();
	}

	void AttatchSystem::Attatch(PurahEngine::RigidBody* _base, PurahEngine::RigidBody* _target)
	{

	}
}
