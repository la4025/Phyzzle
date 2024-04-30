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

	void RegisterRenderInfo(RenderType renderType, RenderOption renderOption, InstancingKey instancingKey, InstancingValue instancingValue);

	const std::unordered_map<InstancingKey, std::vector<RenderInfo*>>& GetDeferredRenderInfo() const;
	const std::unordered_map<InstancingKey, std::vector<RenderInfo*>>& GetForwardRenderInfo() const;
	const std::unordered_map<InstancingKey, std::vector<RenderInfo*>>& GetSpriteRenderInfo() const;
	const std::unordered_map<InstancingKey, RenderInfo*>& GetLightRenderInfo() const;
	const std::vector<RenderInfo*>& GetStringRenderInfo() const;
	const RenderInfo* GetCubeMapRenderInfo();

	const std::unordered_map<InstancingKey, std::vector<RenderInfo*>>& GetShadowRenderInfo() const;

private:
	void SortRenderInfo(RenderInfo* renderInfo, std::unordered_map<InstancingKey, std::vector<RenderInfo*>>& targetContainer);

private:
	// Render Info
	std::vector<RenderInfo> renderInfoList;

	// Sorted RenderInfo
	std::unordered_map<InstancingKey, std::vector<RenderInfo*>> deferredRenderInfo;		// ID 1
	std::unordered_map<InstancingKey, std::vector<RenderInfo*>> forwardRenderInfo;		// ID 2
	std::unordered_map<InstancingKey, std::vector<RenderInfo*>> spriteRenderInfo;		// ID 3
	std::unordered_map<InstancingKey, RenderInfo*> lightRenderInfo;						// ID 4
	std::vector<RenderInfo*> stringRenderInfo;											// ID 5
	RenderInfo* cubeMapRenderInfo;														// ID 6
	
	// Option RenderInfo
	std::unordered_map<InstancingKey, std::vector<RenderInfo*>> shadowRenderInfo;
	std::unordered_map<InstancingKey, std::vector<RenderInfo*>> fastOutLineRenderInfo;
	std::vector<RenderInfo*> outLineRenderInfo;



	//singleton
public:
	static RenderInfoManager& GetInstance();

private:
	RenderInfoManager();
	~RenderInfoManager();
	RenderInfoManager(const RenderInfoManager& ref) = delete;
	RenderInfoManager& operator=(const RenderInfoManager& ref) = delete;
};