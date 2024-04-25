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
	std::vector<RenderInfo> RenderInfoList;

	std::unordered_map<InstancingKey, std::vector<RenderInfo*>> deferredRenderInfo;
	std::unordered_map<InstancingKey, std::vector<RenderInfo*>> forwardRenderInfo;
	std::unordered_map<InstancingKey, std::vector<RenderInfo*>> spriteRenderInfo;
	
	std::unordered_set<InstancingKey> lightRenderInfo;

	

	//singleton
public:
	static RenderInfoManager& GetInstance();

private:
	RenderInfoManager();
	~RenderInfoManager();
	RenderInfoManager(const RenderInfoManager& ref) = delete;
	RenderInfoManager& operator=(const RenderInfoManager& ref) = delete;
};