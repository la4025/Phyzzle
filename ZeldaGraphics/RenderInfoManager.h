#pragma once

#include "RenderInfo.h"

#include <unordered_set>
#include <unordered_map>
#include <map>

class RenderInfoManager final
{
public:
	void ClearRenderInfo();

	// RenderInfo를 정렬한다. (DrawID가 정해진다.)
	void SortRenderInfo();

	void RegisterRenderInfo(RenderType renderType, ResourceType resourceType, RenderOption renderOption, InstancingKey instancingKey, InstancingValue instancingValue);

private:
	void SortRenderInfo(RenderInfo* renderInfo, std::unordered_map<InstancingKey, std::vector<RenderInfo*>>& targetContainer);

private:
	// Render Info
	std::vector<RenderInfo> renderInfoList;

	// Sorted RenderInfo
	std::unordered_map<InstancingKey, std::vector<RenderInfo*>> deferredRenderInfo;
	std::unordered_map<InstancingKey, std::vector<RenderInfo*>> forwardRenderInfo;
	std::unordered_map<InstancingKey, std::vector<RenderInfo*>> spriteRenderInfo;

	std::unordered_set<InstancingKey> lightRenderInfo;
	std::vector<RenderInfo*> stringRenderInfo;
	

	//singleton
public:
	static RenderInfoManager& GetInstance();

private:
	RenderInfoManager();
	~RenderInfoManager();
	RenderInfoManager(const RenderInfoManager& ref) = delete;
	RenderInfoManager& operator=(const RenderInfoManager& ref) = delete;
};