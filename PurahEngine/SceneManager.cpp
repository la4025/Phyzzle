#include "SceneManager.h"

PurahEngine::SceneManager::SceneManager()
{

}

PurahEngine::SceneManager::~SceneManager()
{

}

PurahEngine::SceneManager& PurahEngine::SceneManager::GetInstance()
{
	static SceneManager instance;
	return instance;
}
