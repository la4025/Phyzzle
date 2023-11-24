#include "FBXLoader.h"

#include <filesystem>

namespace FBXLoader
{
	Model* FBXLoader::CreateModelFromFBX(const std::wstring& filePath)
	{
		Model* model = nullptr;

		std::filesystem::path absolutePath = std::filesystem::absolute(filePath);
		std::wstring fileDirectory = absolutePath.parent_path().wstring();

		Assimp::Importer importer;

		std::string multibyteFilePath;
		multibyteFilePath.assign(filePath.begin(), filePath.end());

		const unsigned int DEFAULT_LOAD_FLAG// = aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace | aiProcess_ConvertToLeftHanded | aiProcess_FlipUVs;
			//*
			= aiProcess_Triangulate |
			aiProcess_ConvertToLeftHanded | aiProcess_JoinIdenticalVertices | aiProcess_GenBoundingBoxes |
			aiProcess_CalcTangentSpace | aiProcess_PopulateArmatureData |
			aiProcess_FlipWindingOrder | aiProcess_GenSmoothNormals | aiProcess_SplitLargeMeshes |
			aiProcess_SortByPType | aiProcess_LimitBoneWeights;
		//*/

		// aiProcess_PreTransformVertices

		const aiScene* scene = importer.ReadFile(multibyteFilePath, DEFAULT_LOAD_FLAG);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			std::string errorMessage = importer.GetErrorString();
			std::wstring wErrorMessage;
			wErrorMessage.assign(errorMessage.begin(), errorMessage.end());

			return model;
		}

		model = new Model();

		for (int i = 0; i < scene->mNumAnimations; i++)
		{

		}

		if (scene->HasMaterials())
		{
			for (int i = 0; i < scene->mNumMaterials; i++)
			{
				aiMaterial* assimpMaterial = scene->mMaterials[i];

				Material* materialData = new Material();

				if (assimpMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0)
				{
					materialData->useDiffuseMap = true;

					aiString texturePath;
					if (assimpMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &texturePath) == AI_SUCCESS)
					{
						std::string str = texturePath.C_Str();
						std::wstring wstr;
						wstr.assign(str.begin(), str.end());
						wstr = fileDirectory + L"\\" + wstr;
						materialData->diffuseMap = wstr;
					}
				}
				else
				{
					materialData->useDiffuseMap = false;
				}

				model->materialList.push_back(materialData);
			}
		}

		std::map<std::wstring, Bone*> boneMap;

		model->root = new Bone();
		CopyNodeData(model->root, scene->mRootNode, boneMap);

		if (scene->HasMeshes())
		{
			for (int i = 0; i < scene->mNumMeshes; i++)
			{
				aiMesh* assimpMesh = scene->mMeshes[i];

				Mesh* meshData = new Mesh();

				//for (int j = 0; j < assimpMesh->mNumBones; j++)
				//{
				//	std::string str = assimpMesh->mBones[j]->mName.C_Str();
				//	std::wstring boneName;
				//	boneName.assign(str.begin(), str.end());

				//	for (int k = 0; k < assimpMesh->mBones[j]->mNumWeights; k++)
				//	{
				//		unsigned int vid = assimpMesh->mBones[j]->mWeights[k].mVertexId;
				//		float weight = assimpMesh->mBones[j]->mWeights[k].mWeight;
				//	}
				//}

				for (int j = 0; j < assimpMesh->mNumVertices; j++)
				{
					Vertex vtx;
					if (assimpMesh->HasPositions())
					{
						vtx.position.x = assimpMesh->mVertices[j].x;
						vtx.position.y = assimpMesh->mVertices[j].y;
						vtx.position.z = assimpMesh->mVertices[j].z;
					}

					if (assimpMesh->HasTextureCoords(0))
					{
						vtx.textureCoordinate.u = assimpMesh->mTextureCoords[0][j].x;
						vtx.textureCoordinate.v = assimpMesh->mTextureCoords[0][j].y;
					}

					if (assimpMesh->HasNormals())
					{
						vtx.normal.x = assimpMesh->mNormals[j].x;
						vtx.normal.y = assimpMesh->mNormals[j].y;
						vtx.normal.z = assimpMesh->mNormals[j].z;
					}

					meshData->vertices.push_back(vtx);
				}

				for (int j = 0; j < assimpMesh->mNumFaces; j++)
				{
					aiFace face = assimpMesh->mFaces[j];
					for (int k = 0; k < face.mNumIndices; k++)
					{
						meshData->indices.push_back(face.mIndices[k]);
					}
				}

				meshData->materialIndex = assimpMesh->mMaterialIndex;
				model->meshList.push_back(meshData);
			}
		}

		



		importer.FreeScene();

		return model;
	}

	void FBXLoader::ReleaseModel(Model* model)
	{

	}

	void FBXLoader::CopyNodeData(Bone* bone, aiNode* ainode, std::map<std::wstring, Bone*>& boneMap)
	{
		std::string str = ainode->mName.C_Str();
		std::wstring wstr;
		wstr.assign(str.begin(), str.end());

		bone->name = wstr;

		for (int i = 0; i < ainode->mNumMeshes; i++)
		{
			bone->meshes.push_back(ainode->mMeshes[i]);
		}

		bone->transformMatrix <<
			ainode->mTransformation.a1, ainode->mTransformation.a2, ainode->mTransformation.a3, ainode->mTransformation.a4,
			ainode->mTransformation.b1, ainode->mTransformation.b2, ainode->mTransformation.b3, ainode->mTransformation.b4,
			ainode->mTransformation.c1, ainode->mTransformation.c2, ainode->mTransformation.c3, ainode->mTransformation.c4,
			ainode->mTransformation.d1, ainode->mTransformation.d2, ainode->mTransformation.d3, ainode->mTransformation.d4;

		assert(boneMap.count(wstr) == 0);

		boneMap.insert({ wstr, bone });

		for (int i = 0; i < ainode->mNumChildren; i++)
		{
			aiNode* aichild = ainode->mChildren[i];
			Bone* childBone = new Bone();
			CopyNodeData(childBone, aichild, boneMap);
		}
	}
}