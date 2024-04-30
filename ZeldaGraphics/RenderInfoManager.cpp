#include "RenderInfoManager.h"

void RenderInfoManager::ClearRenderInfo()
{
	renderInfoList.clear();
}

void RenderInfoManager::SortRenderInfo()
{
	// 정렬된 RenderInfo를 저장하는 컨테이너들 초기화
	deferredRenderInfo.clear();
	forwardRenderInfo.clear();
	spriteRenderInfo.clear();
	lightRenderInfo.clear();
	stringRenderInfo.clear();
	cubeMapRenderInfo = nullptr;

	shadowRenderInfo.clear();
	fastOutLineRenderInfo.clear();
	outLineRenderInfo.clear();

	// renderInfoList를 순회하며 정렬한다.
	for (int i = 0; i < renderInfoList.size(); i++)
	{
		RenderInfo& renderInfo = renderInfoList[i];

		RenderType renderType = renderInfo.renderType;
		RenderOption renderOption = renderInfo.renderOption;

		switch (renderType)
		{
			case RenderType::Deferred_Mesh:
			case RenderType::Deferred_Model:
			case RenderType::Deferred_BlendingAnimation:
			{
				SortRenderInfo(&renderInfo, deferredRenderInfo);

				// Option
				if ((renderOption & RenderInfoOption::OutLine) > 0u)
				{
					outLineRenderInfo.push_back(&renderInfo);
				}
				// FastOutLine은 OutLine을 그린다면 진행하지 않는다.
				else if ((renderOption & RenderInfoOption::FastOutLine) > 0u)
				{
					SortRenderInfo(&renderInfo, fastOutLineRenderInfo);
				}

				if ((renderOption & RenderInfoOption::Shadow) > 0u)
				{
					SortRenderInfo(&renderInfo, shadowRenderInfo);
				}

				break;
			}
			case RenderType::Forward_Mesh:
			case RenderType::Forward_Model:
			case RenderType::Forward_BlendingAnimation:
			{
				SortRenderInfo(&renderInfo, forwardRenderInfo);
				break;
			}
			case RenderType::Sprite:
			{
				SortRenderInfo(&renderInfo, spriteRenderInfo);
				break;
			}
			case RenderType::String:
			{
				stringRenderInfo.push_back(&renderInfo);
				break;
			}
			case RenderType::Light:
			{
				lightRenderInfo.insert({ renderInfo.instancingKey, &renderInfo });
				break;
			}
			case RenderType::CubeMap:
			{
				cubeMapRenderInfo = &renderInfo;
				break;
			}
			default:
			{
				assert(0);
				break;
			}
		}
	}


	// ID를 부여한다.
	int drawIDCounter = 1;

	for (auto& [key, value] : deferredRenderInfo)
	{
		for (auto& renderInfo : value)
		{
			renderInfo->drawID = drawIDCounter;
			drawIDCounter += 1;
		}
	}

	for (auto& [key, value] : forwardRenderInfo)
	{
		for (auto& renderInfo : value)
		{
			renderInfo->drawID = drawIDCounter;
			drawIDCounter += 1;
		}
	}

	for (auto& [key, value] : spriteRenderInfo)
	{
		for (auto& renderInfo : value)
		{
			renderInfo->drawID = drawIDCounter;
			drawIDCounter += 1;
		}
	}

	for (auto& [key, value] : lightRenderInfo)
	{
		value->drawID = drawIDCounter;
		drawIDCounter += 1;
	}

	for (auto& iter : stringRenderInfo)
	{
		iter->drawID = drawIDCounter;
		drawIDCounter += 1;
	}

	if (cubeMapRenderInfo != nullptr)
	{
		cubeMapRenderInfo->drawID = drawIDCounter;
		drawIDCounter += 1;
	}
}

void RenderInfoManager::RegisterRenderInfo(RenderType renderType, RenderOption renderOption, InstancingKey instancingKey, InstancingValue instancingValue)
{
	RenderInfo renderInfo;
	renderInfo.renderType = renderType;
	renderInfo.renderOption = renderOption;
	renderInfo.instancingKey = instancingKey;
	renderInfo.instancingValue = instancingValue;

	renderInfoList.push_back(renderInfo);
}

const std::unordered_map<InstancingKey, std::vector<RenderInfo*>>& RenderInfoManager::GetDeferredRenderInfo() const
{
	return deferredRenderInfo;
}

const std::unordered_map<InstancingKey, std::vector<RenderInfo*>>& RenderInfoManager::GetForwardRenderInfo() const
{
	return forwardRenderInfo;
}

const std::unordered_map<InstancingKey, std::vector<RenderInfo*>>& RenderInfoManager::GetSpriteRenderInfo() const
{
	return spriteRenderInfo;
}

const std::unordered_map<InstancingKey, RenderInfo*>& RenderInfoManager::GetLightRenderInfo() const
{
	return lightRenderInfo;
}

const std::vector<RenderInfo*>& RenderInfoManager::GetStringRenderInfo() const
{
	return stringRenderInfo;
}

const RenderInfo* RenderInfoManager::GetCubeMapRenderInfo()
{
	return cubeMapRenderInfo;
}

const std::unordered_map<InstancingKey, std::vector<RenderInfo*>>& RenderInfoManager::GetShadowRenderInfo() const
{
	return shadowRenderInfo;
}

void RenderInfoManager::SortRenderInfo(RenderInfo* renderInfo, std::unordered_map<InstancingKey, std::vector<RenderInfo*>>& targetContainer)
{
	auto iter = targetContainer.find(renderInfo->instancingKey);

	// 동일한 instancingKey로 그린적이 있음
	if (iter != targetContainer.end())
	{
		auto& infoVector = iter->second;
		infoVector.push_back(renderInfo);
	}
	// 동일한 instancingKey로 그린적이 없음
	else
	{
		targetContainer.insert({ renderInfo->instancingKey, std::vector<RenderInfo*>(1, renderInfo) });
	}
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
