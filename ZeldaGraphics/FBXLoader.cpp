#include "FBXLoader.h"

#include "ZeldaModel.h"

const unsigned int FBXLoader::DEFAULT_LOAD_FLAG = aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace;

ZeldaModel* FBXLoader::LoadFBX(const std::wstring& filePath)
{
	Assimp::Importer importer;

	std::string multibyteFilePath;
	multibyteFilePath.assign(filePath.begin(), filePath.end());

	const aiScene* scene = importer.ReadFile(multibyteFilePath, DEFAULT_LOAD_FLAG);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::string errorMessage = importer.GetErrorString();
		std::wstring wErrorMessage;
		wErrorMessage.assign(errorMessage.begin(), errorMessage.end());

		MessageBox(0, wErrorMessage.c_str(), L"FBXLoader Error", MB_OK);

		return nullptr;
	}


	ZeldaModel* model;


	importer.FreeScene();

	return nullptr;
}