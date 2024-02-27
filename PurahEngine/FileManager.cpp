#include "FileManager.h"

PurahEngine::FileManager::FileManager()
	: isRunning(false)
{

}

PurahEngine::FileManager::~FileManager()
{

}

PurahEngine::FileManager& PurahEngine::FileManager::GetInstance()
{
	static FileManager instance;
	return instance;
}
