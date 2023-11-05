#include "ZeldaModel.h"

#include "ZeldaTexture.h"

const unsigned int ZeldaModel::DEFAULT_LOAD_FLAG = aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace;

ZeldaModel::ZeldaModel() :
	root(nullptr)
{

}

ZeldaModel::~ZeldaModel()
{
}

bool ZeldaModel::LoadFBX(const std::wstring& filePath)
{
	Clear();

	Assimp::Importer importer;

	std::string multibyteFilePath;
	multibyteFilePath.assign(filePath.begin(), filePath.end());

	const aiScene* scene = importer.ReadFile(multibyteFilePath, DEFAULT_LOAD_FLAG);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::string errorMessage = importer.GetErrorString();
		std::wstring wErrorMessage;
		wErrorMessage.assign(errorMessage.begin(), errorMessage.end());

		MessageBox(0, wErrorMessage.c_str(), L"ZeldaModel Error", MB_OK);

		return false;
	}

	if (scene->HasMeshes())
	{
		for (int i = 0; i < scene->mNumMeshes; i++)
		{
			aiMesh* assimpMesh = scene->mMeshes[i];
			
			Mesh* meshData = new Mesh();
			meshData->hasPosition = assimpMesh->HasPositions();
			meshData->hasUV = assimpMesh->HasTextureCoords(0);
			meshData->hasNormal = assimpMesh->HasNormals();



			meshes.push_back(meshData);
		}
	}

	if (scene->HasMaterials())
	{
		for (int i = 0; i < scene->mNumMaterials; i++)
		{
			aiMaterial* assimpMaterial = scene->mMaterials[i];

			Material* materialData = new Material();

			if (assimpMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0)
			{
				materialData->hasTexture = true;

				aiString texturePath;
				if (assimpMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &texturePath) == AI_SUCCESS)
				{
					std::string str = texturePath.C_Str();
					std::wstring wstr;
					wstr.assign(str.begin(), str.end());
					materialData->textureFilePath = wstr;
				}
			}
			else
			{
				materialData->hasTexture = false;
			}

			materials.push_back(materialData);
		}
	}

	root = new Node();
	CopyNodeData(root, scene->mRootNode, scene);

	importer.FreeScene();

	return true;
}

void ZeldaModel::CopyNodeData(Node* node, const aiNode* assimpNode, const aiScene* assimpScene)
{
	assert(node == root);
	CopyNodeData_Recursive(node, assimpNode, assimpScene);
}

void ZeldaModel::CopyNodeData_Recursive(Node* node, const aiNode* assimpNode, const aiScene* assimpScene)
{
	assert(node->children.size() == 0);

	for (int i = 0; i < assimpNode->mNumMeshes; i++)
	{
		assert(meshes.size() > assimpNode->mMeshes[i]);
		node->meshes.push_back(meshes[assimpNode->mMeshes[i]]);
	}

	for (int i = 0; i < assimpNode->mNumChildren; i++)
	{
		Node* child = new Node();
		aiNode* childAssimpNode = assimpNode->mChildren[i];

		child->parent = node;
		node->children.push_back(child);

		CopyNodeData_Recursive(child, childAssimpNode, assimpScene);
	}
}

void ZeldaModel::Clear()
{
	for (int i = 0; i < meshes.size(); i++)
	{
		delete meshes[i];
	}
	meshes.clear();

	for (int i = 0; i < materials.size(); i++)
	{
		delete materials[i];
	}
	materials.clear();

	if (root != nullptr)
	{
		delete root;
	}
	root = nullptr;
}

ZeldaModel::Node::Node() :
	parent(nullptr)
{
}

ZeldaModel::Node::~Node()
{
	for (int i = 0; i < children.size(); i++)
	{
		delete children[i];
	}
}

ZeldaModel::Mesh::Mesh() :
	hasPosition(false),
	hasUV(false),
	hasNormal(false),
	material(nullptr)
{
}

ZeldaModel::Material::Material() :
	hasTexture(false)
{
}
