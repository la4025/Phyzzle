#pragma once

#include <string>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "FBXData.h"

namespace FBXLoader
{
	class FBXLoader
	{
	public:
		Model* CreateModelFromFBX(const std::wstring& filePath);
		void ReleaseModel(Model* model);

	private:
		void CopyNodeData(Bone* bone, aiNode* ainode, std::map<std::wstring, Bone*>& boneMap);

	};
}