#include "GroundCheck.h"

bool Phyzzle::GroundCheck::IsGrounded()
{
	return isGrounded;
}

void Phyzzle::GroundCheck::OnTriggerEnter(const PurahEngine::Collider* _other)
{
	isGrounded = true;
	count++;
}

void Phyzzle::GroundCheck::OnTriggerExit(const PurahEngine::Collider* _other)
{
	count--;

	if (count == 0)
		isGrounded = false;
}

void Phyzzle::GroundCheck::PreSerialize(json& jsonData) const
{
}

void Phyzzle::GroundCheck::PreDeserialize(const json& jsonData)
{
}

void Phyzzle::GroundCheck::PostSerialize(json& jsonData) const
{
}

void Phyzzle::GroundCheck::PostDeserialize(const json& jsonData)
{
}
