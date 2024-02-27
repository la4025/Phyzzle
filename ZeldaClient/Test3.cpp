#include "Test3.h"

PurahEngine::Test3::Test3()
{

}

PurahEngine::Test3::~Test3()
{

}

void PurahEngine::Test3::Awake()
{

}

void PurahEngine::Test3::Start()
{

}

void PurahEngine::Test3::Update()
{
	auto& inputManager = PurahEngine::InputManager::Getinstance();
	const auto trans = GetGameObject()->GetComponent<PurahEngine::Transform>();
	const auto name = GetGameObject()->GetName();
	bool isRootable = GetGameObject()->IsRootEnable();

	if (inputManager.IsKeyPressed('B') == true)
	{
		if (name == L"1")
		{
			GetGameObject()->SetEnable(true);
		}
	}
}
