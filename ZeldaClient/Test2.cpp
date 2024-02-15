#include "Test2.h"
#include "InputManager.h"

PurahEngine::Test2::Test2()
{

}

PurahEngine::Test2::~Test2()
{

}

void PurahEngine::Test2::Awake()
{

}

void PurahEngine::Test2::Start()
{

}

void PurahEngine::Test2::Update()
{
	auto& inputManager = PurahEngine::InputManager::Getinstance();
	const auto trans = GetGameObject()->GetComponent<PurahEngine::Transform>();

	if (inputManager.IsKeyPressed('N') == true)
	{
		if (trans->GetParent() != nullptr)
		{
			if (GetGameObject()->IsRootEnable() == true)
			{
				trans->GetParent()->GetGameObject()->Disable();
			}
		}
		else
		{
			if (GetGameObject()->IsRootEnable() == true)
			{
				GetGameObject()->Disable();
			}
		}

	}

	if (inputManager.IsKeyPressed('M') == true)
	{
		if (trans->GetParent() != nullptr)
		{
			if (GetGameObject()->IsRootEnable() == true)
			{
				trans->GetParent()->GetGameObject()->Enable();
			}
		}
		else
		{
			if (GetGameObject()->IsRootEnable() == true)
			{
				GetGameObject()->Enable();
			}
		}
	}
}
