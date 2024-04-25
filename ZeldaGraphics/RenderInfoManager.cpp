#include "RenderInfoManager.h"

void RenderInfoManager::ClearRenderInfo()
{
}

RenderInfoManager& RenderInfoManager::GetInstance()
{
	static RenderInfoManager instance;
	return instance;
}

RenderInfoManager::RenderInfoManager()
{
}

RenderInfoManager::~RenderInfoManager()
{
}
