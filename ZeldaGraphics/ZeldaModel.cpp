#include "ZeldaModel.h"

#include "MathConverter.h"
#include "ZeldaMesh.h"
#include "ZeldaMaterial.h"
#include "ZeldaCamera.h"
#include "GraphicsResourceID.h"
#include "ZeldaShader.h"
#include "ZeldaLight.h"
#include "ZeldaMaterial.h"
#include "ZeldaTexture.h"
#include "ResourceManager.h"
#include "ConstantBufferManager.h"


#include <fstream>

using namespace DirectX;

ZeldaModel::~ZeldaModel()
{
	if (animationResourceView)
	{
		animationResourceView->Release();
		animationResourceView = nullptr;
	}

	// root 부터 트리타고 전부 제거
	std::queue<Node*> q;
	if (root != nullptr)
	{
		q.push(root);
	}

	while (!q.empty())
	{
		Node* current = q.front();
		q.pop();

		for (int i = 0; i < current->children.size(); i++)
		{
			q.push(current->children[i]);
		}

		delete current;
	}

	root = nullptr;

	// meshes 제거 (ZeldaMesh)
	for (int i = 0; i < meshes.size(); i++)
	{
		delete meshes[i];
	}
	meshes.clear();

	// material 제거 (ZeldaMaterial)
	for (int i = 0; i < materials.size(); i++)
	{
		delete materials[i];
	}
	materials.clear();

	// Animation 제거
	for (auto iter = animationTable.begin(); iter != animationTable.end(); iter++)
	{
		Animation* animation = iter->second;
		delete animation;
	}
	animationTable.clear();

	if (animationResourceView != nullptr)
	{
		animationResourceView->Release();
		animationResourceView = nullptr;
	}
}

void ZeldaModel::Render(
	ID3D11DeviceContext* deviceContext,
	ConstantBuffer<MatrixBufferType, ShaderType::VertexShader>* matrixConstBuffer,
	ConstantBuffer<BoneBufferType, ShaderType::VertexShader>* boneConstBuffer,
	ConstantBuffer<MaterialBufferType, ShaderType::PixelShader>* materialConstBuffer,
	DirectX::XMMATRIX worldMatrix,
	ZeldaShader* shader,
	const std::wstring& animationName,
	float animationTime)
{
	// 정상적인 애니메이션 정보가 들어온 경우 RenderAnimation을 호출한다.
	if (animationTime > 0.0f && animationTable.count(animationName) > 0)
	{
		RenderAnimation(deviceContext, matrixConstBuffer, boneConstBuffer, materialConstBuffer, worldMatrix, shader, animationName, animationTime);
		return;
	}

	// 애니메이션 정보가 완전하지 않은 경우, 정적인 모델을 그린다.

	// 첫 번째 Render에 한하여 월드행렬을 계산하고 그 후에는 저장된 것을 사용한다.
	if (updatedWorldMatrix == false)
	{
		updatedWorldMatrix = true;

		std::queue<std::pair<Node*, DirectX::XMMATRIX>> nodeQueue;

		nodeQueue.push({ root, root->transformMatrix });

		while (!nodeQueue.empty())
		{
			auto current = nodeQueue.front();
			nodeQueue.pop();
			Node* currentNode = current.first;
			DirectX::XMMATRIX currentMatrix = current.second;

			currentNode->worldMatrix = currentNode->offsetMatrix * currentMatrix;

			for (int i = 0; i < currentNode->children.size(); i++)
			{
				Node* nextNode = currentNode->children[i];
				DirectX::XMMATRIX nextMatrix = nextNode->transformMatrix * currentMatrix;

				nodeQueue.push({ nextNode, nextMatrix });
			}
		}
	}

	BoneBufferType* boneBuffer = new BoneBufferType();
	for (int i = 0; i < bones.size(); i++)
	{
		if (bones[i] == nullptr) continue;
		// 셰이더에 넘기는 행렬을 전치를 한 후 넘겨야 한다.
		boneBuffer->boneTM[i] = XMMatrixTranspose(bones[i]->worldMatrix);
	}

	ZeldaCamera* currentcamera = ResourceManager::GetInstance().GetCamera(ZeldaCamera::GetMainCamera());

	// 셰이더에 넘기는 행렬을 전치를 한 후 넘겨야 한다.
	matrixConstBuffer->SetData({ XMMatrixTranspose(worldMatrix), XMMatrixTranspose(currentcamera->GetViewMatrix()), XMMatrixTranspose(currentcamera->GetProjMatrix()) });
	boneConstBuffer->SetData(*boneBuffer);

	delete boneBuffer;

	// 모든 메쉬 그리기
	for (int i = 0; i < meshes.size(); i++)
	{
		ZeldaMesh* currentMesh = meshes[i];
		currentMesh->Render(deviceContext);
		int indexCount = currentMesh->GetIndexCount();

		materialConstBuffer->SetData({
			materials[materialIndex[i]]->baseColor,
			!materials[materialIndex[i]]->useDiffuseMap,
			materials[materialIndex[i]]->UseSRGB(ZeldaMaterial::DIFFUSE_MAP) ,
			materials[materialIndex[i]]->useDiffuseMap
			});

		ConstantBufferManager::GetInstance().SetBuffer();

		materials[materialIndex[i]]->SetShaderResource(deviceContext);

		shader->Render(deviceContext, indexCount);
	}
}

std::vector<std::wstring> ZeldaModel::GetAnimationList()
{
	std::vector<std::wstring> result;

	for (auto iter = animationTable.begin(); iter != animationTable.end(); iter++)
	{
		std::wstring animationName = iter->first;
		result.push_back(animationName);
	}

	sort(result.begin(), result.end());

	return result;
}

std::vector<float> ZeldaModel::GetAnimationPlayTime()
{
	std::vector<std::pair<std::wstring, float>> result;

	for (auto iter = animationTable.begin(); iter != animationTable.end(); iter++)
	{
		std::wstring animationName = iter->first;
		float duration = iter->second->duration / iter->second->tickPerSecond;

		result.push_back({ animationName, duration });
	}

	sort(result.begin(), result.end());

	std::vector<float> durationResult;

	for (auto iter = result.begin(); iter != result.end(); iter++)
	{
		durationResult.push_back(iter->second);
	}

	return durationResult;
}

ZeldaModel::ZeldaModel(ID3D11Device* device, FBXLoader::Model* fbxModel) :
	root(nullptr),
	updatedWorldMatrix(false),
	animationResourceView(nullptr)
{
	root = new Node();

	std::map<std::wstring, Node*> nodeTable;
	CopyNode(root, fbxModel->root, nodeTable);

	bones.assign(256, nullptr);
	for (int i = 0; i < fbxModel->boneList.size(); i++)
	{
		if (fbxModel->boneList[i] != nullptr)
		{
			bones[i] = nodeTable[fbxModel->boneList[i]->name];
		}
	}

	for (int i = 0; i < fbxModel->meshList.size(); i++)
	{
		FBXLoader::Mesh* fbxMesh = fbxModel->meshList[i];

		std::vector<VertexType> vertices;
		std::vector<unsigned int> indices;

		for (int j = 0; j < fbxMesh->vertices.size(); j++)
		{
			VertexType vertexType;
			vertexType.position.x = fbxMesh->vertices[j].position.x;
			vertexType.position.y = fbxMesh->vertices[j].position.y;
			vertexType.position.z = fbxMesh->vertices[j].position.z;

			vertexType.normal.x = fbxMesh->vertices[j].normal.x;
			vertexType.normal.y = fbxMesh->vertices[j].normal.y;
			vertexType.normal.z = fbxMesh->vertices[j].normal.z;

			vertexType.texture.x = fbxMesh->vertices[j].textureCoordinate.u;
			vertexType.texture.y = fbxMesh->vertices[j].textureCoordinate.v;

			vertexType.boneIndices.x = fbxMesh->vertices[j].boneIndices[0];
			vertexType.boneIndices.y = fbxMesh->vertices[j].boneIndices[1];
			vertexType.boneIndices.z = fbxMesh->vertices[j].boneIndices[2];
			vertexType.boneIndices.w = fbxMesh->vertices[j].boneIndices[3];

			vertexType.weight.x = fbxMesh->vertices[j].weight[0];
			vertexType.weight.y = fbxMesh->vertices[j].weight[1];
			vertexType.weight.z = fbxMesh->vertices[j].weight[2];
			vertexType.weight.w = fbxMesh->vertices[j].weight[3];

			vertices.push_back(vertexType);
		}

		for (int j = 0; j < fbxMesh->indices.size(); j++)
		{
			indices.push_back(fbxMesh->indices[j]);
		}

		meshes.push_back(new ZeldaMesh(device, vertices, indices));
		materialIndex.push_back(fbxMesh->materialIndex);
	}

	for (int i = 0; i < fbxModel->materialList.size(); i++)
	{
		FBXLoader::Material* fbxMaterial = fbxModel->materialList[i];
		ZeldaMaterial* material = new ZeldaMaterial(device, DirectX::XMFLOAT4(fbxMaterial->baseColor.r, fbxMaterial->baseColor.g, fbxMaterial->baseColor.b, fbxMaterial->baseColor.a), fbxMaterial->diffuseMap);
		materials.push_back(material);
	}

	for (int i = 0; i < fbxModel->animationList.size(); i++)
	{
		FBXLoader::Animation* fbxAnimation = fbxModel->animationList[i];

		Animation* animation = new Animation();
		animation->duration = fbxAnimation->duration;
		animation->tickPerSecond = fbxAnimation->tickPerSecond;
		
		for (auto iter = fbxAnimation->animationKey.begin(); iter != fbxAnimation->animationKey.end(); iter++)
		{
			std::wstring nodeName = iter->first;

			std::map<double, AnimationKeyInfo> keys;

			for (auto iter2 = iter->second.begin(); iter2 != iter->second.end(); iter2++)
			{
				double time = iter2->first;
				DirectX::XMVECTOR scale;
				scale.m128_f32[0] = iter2->second.scaleKey.x;
				scale.m128_f32[1] = iter2->second.scaleKey.y;
				scale.m128_f32[2] = iter2->second.scaleKey.z;
				scale.m128_f32[3] = 1.0f;

				DirectX::XMVECTOR rotation;
				rotation.m128_f32[0] = iter2->second.rotationKey.x;
				rotation.m128_f32[1] = iter2->second.rotationKey.y;
				rotation.m128_f32[2] = iter2->second.rotationKey.z;
				rotation.m128_f32[3] = iter2->second.rotationKey.w;

				DirectX::XMVECTOR position;
				position.m128_f32[0] = iter2->second.positionKey.x;
				position.m128_f32[1] = iter2->second.positionKey.y;
				position.m128_f32[2] = iter2->second.positionKey.z;
				position.m128_f32[3] = 1.0f;

				keys[time] = AnimationKeyInfo(scale, rotation, position);
			}
			animation->animationKey[nodeName] = keys;
		}

		animationTable[fbxAnimation->name] = animation;
	}

	CreateAnimationResourceView(device);
}

void ZeldaModel::CreateAnimationResourceView(ID3D11Device* device)
{
	// 애니메이션 데이터 미리 계산하기

	// float4가 아닌 float이기 때문에 추가로 4배의 크기로 생성
	float** textureData = new float*[static_cast<unsigned long long>(animationTable.size())];
	for (int i = 0; i < static_cast<unsigned long long>(animationTable.size()); i++)
	{
		textureData[i] = new float[4 * 4 * BONE_COUNT_MAX * ANIMATION_FRAME_MAX];
	}

	int animationCount = 0;
	for (auto animIter = animationTable.begin(); animIter != animationTable.end(); animIter++)
	{
		Animation* currentAnimation = animIter->second;

		double currentDuration = currentAnimation->duration;
		double currentTickPerSecond = currentAnimation->tickPerSecond;
		auto& currentAnimationKey = currentAnimation->animationKey;

		int intDuration = static_cast<int>(currentDuration);

		// assert에 걸린다면 단순히 static_cast가 아니라 반올림 후 캐스팅하는것으로 수정할 것
		assert(abs(currentDuration - intDuration) < 0.1);

		// 모든 애니메이션의 각 프레임의 행렬을 미리 계산한다.
		for (int currentFrame = 0; currentFrame < intDuration; currentFrame++)
		{
			double frameTime = static_cast<double>(currentFrame);

			// 각 노드의 finalTM에 frameTime의 행렬을 계산하여 넣는다.
			std::queue<std::pair<Node*, DirectX::XMMATRIX>> nodeQueue;

			nodeQueue.push({ root, root->transformMatrix });

			while (!nodeQueue.empty())
			{
				auto current = nodeQueue.front();
				nodeQueue.pop();
				Node* currentNode = current.first;
				DirectX::XMMATRIX currentMatrix = current.second;

				currentNode->finalTM = currentNode->offsetMatrix * currentMatrix;

				for (int i = 0; i < currentNode->children.size(); i++)
				{
					Node* nextNode = currentNode->children[i];
					DirectX::XMMATRIX nextMatrix = nextNode->transformMatrix * currentMatrix;

					if (currentAnimation->animationKey.count(nextNode->name) != 0)
					{
						auto rightIter = currentAnimation->animationKey[nextNode->name].lower_bound(frameTime);
						auto endIter = currentAnimation->animationKey[nextNode->name].end();

						if (rightIter == endIter)
						{
							rightIter--;
						}

						XMMATRIX animationMatrix;

						AnimationKeyInfo keyinfo = rightIter->second;

						DirectX::XMMATRIX scaleMatrix = XMMatrixScalingFromVector(keyinfo.scale);
						DirectX::XMMATRIX rotationMatrix = XMMatrixRotationQuaternion(keyinfo.rotation);
						DirectX::XMMATRIX translationMatrix = XMMatrixTranslationFromVector(keyinfo.position);
						animationMatrix = scaleMatrix * rotationMatrix * translationMatrix;

						nextMatrix = animationMatrix * currentMatrix;
					}

					nodeQueue.push({ nextNode, nextMatrix });
				}
			}

			// 계산된 finalTM을 Texture에 넣는다.
			for (int i = 0; i < bones.size(); i++)
			{
				if (bones[i] == nullptr) continue;

				textureData[animationCount][(4 * 4 * BONE_COUNT_MAX) * currentFrame + 16 * i + 0x0] = bones[i]->finalTM.r[0].m128_f32[0];
				textureData[animationCount][(4 * 4 * BONE_COUNT_MAX) * currentFrame + 16 * i + 0x1] = bones[i]->finalTM.r[0].m128_f32[1];
				textureData[animationCount][(4 * 4 * BONE_COUNT_MAX) * currentFrame + 16 * i + 0x2] = bones[i]->finalTM.r[0].m128_f32[2];
				textureData[animationCount][(4 * 4 * BONE_COUNT_MAX) * currentFrame + 16 * i + 0x3] = bones[i]->finalTM.r[0].m128_f32[3];
				textureData[animationCount][(4 * 4 * BONE_COUNT_MAX) * currentFrame + 16 * i + 0x4] = bones[i]->finalTM.r[1].m128_f32[0];
				textureData[animationCount][(4 * 4 * BONE_COUNT_MAX) * currentFrame + 16 * i + 0x5] = bones[i]->finalTM.r[1].m128_f32[1];
				textureData[animationCount][(4 * 4 * BONE_COUNT_MAX) * currentFrame + 16 * i + 0x6] = bones[i]->finalTM.r[1].m128_f32[2];
				textureData[animationCount][(4 * 4 * BONE_COUNT_MAX) * currentFrame + 16 * i + 0x7] = bones[i]->finalTM.r[1].m128_f32[3];
				textureData[animationCount][(4 * 4 * BONE_COUNT_MAX) * currentFrame + 16 * i + 0x8] = bones[i]->finalTM.r[2].m128_f32[0];
				textureData[animationCount][(4 * 4 * BONE_COUNT_MAX) * currentFrame + 16 * i + 0x9] = bones[i]->finalTM.r[2].m128_f32[1];
				textureData[animationCount][(4 * 4 * BONE_COUNT_MAX) * currentFrame + 16 * i + 0xa] = bones[i]->finalTM.r[2].m128_f32[2];
				textureData[animationCount][(4 * 4 * BONE_COUNT_MAX) * currentFrame + 16 * i + 0xb] = bones[i]->finalTM.r[2].m128_f32[3];
				textureData[animationCount][(4 * 4 * BONE_COUNT_MAX) * currentFrame + 16 * i + 0xc] = bones[i]->finalTM.r[3].m128_f32[0];
				textureData[animationCount][(4 * 4 * BONE_COUNT_MAX) * currentFrame + 16 * i + 0xd] = bones[i]->finalTM.r[3].m128_f32[1];
				textureData[animationCount][(4 * 4 * BONE_COUNT_MAX) * currentFrame + 16 * i + 0xe] = bones[i]->finalTM.r[3].m128_f32[2];
				textureData[animationCount][(4 * 4 * BONE_COUNT_MAX) * currentFrame + 16 * i + 0xf] = bones[i]->finalTM.r[3].m128_f32[3];
			}
		}

		animationCount += 1;
	}


	// 계산된 애니메이션 데이터로 텍스쳐 생성하여 셰이더 리소스 뷰 생성

	ID3D11Texture2D** tex2d = new ID3D11Texture2D*[static_cast<unsigned long long>(animationTable.size())];

	// 텍스처 생성
	D3D11_TEXTURE2D_DESC textureDesc = {};
	textureDesc.Width = 4 * BONE_COUNT_MAX;		// 텍스처 가로 크기 (Matrix를 이루는 Vector의 수) x (본 최대치)
	textureDesc.Height = ANIMATION_FRAME_MAX;	// 텍스처 세로 크기 (프레임 최대치)
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = static_cast<unsigned long long>(animationTable.size());	// ArraySize 애니메이션 종류
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	//textureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	std::vector<D3D11_SUBRESOURCE_DATA> initData(static_cast<unsigned long long>(animationTable.size()));
	for (int i = 0; i < static_cast<unsigned long long>(animationTable.size()); i++)
	{
		initData[i].pSysMem = textureData[i];
		initData[i].SysMemPitch = 16 * 4 * BONE_COUNT_MAX; // 바이트 단위인거 같다...
		initData[i].SysMemSlicePitch = 16 * 4 * BONE_COUNT_MAX * ANIMATION_FRAME_MAX;
	}

	HRESULT hr = device->CreateTexture2D(&textureDesc, initData.data(), tex2d);
	if (FAILED(hr))
	{
		MessageBox(0, L"Failed to Create Animation Resource View", L"Model Error", MB_OK);
		assert(0);
	}

	// 셰이더 리소스 뷰 생성
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	srvDesc.Texture2DArray.MipLevels = 1;
	srvDesc.Texture2DArray.ArraySize = static_cast<unsigned long long>(animationTable.size());

	hr = device->CreateShaderResourceView(*tex2d, &srvDesc, &animationResourceView);
	if (FAILED(hr))
	{
		MessageBox(0, L"Failed to Create Animation Resource View", L"Model Error", MB_OK);
		assert(0);
	}


	for (int i = 0; i < static_cast<unsigned long long>(animationTable.size()); i++)
	{
		tex2d[i]->Release();
		delete[] textureData[i];
	}

	delete[] tex2d;
	delete[] textureData;
}

void ZeldaModel::CopyNode(Node* node, FBXLoader::Bone* bone, std::map<std::wstring, Node*>& nodeTable)
{
	node->name = bone->name;
	node->meshes = bone->meshes;

	node->transformMatrix = MathConverter::EigenMatrixToXMMatrix(bone->transformMatrix);
	node->offsetMatrix = MathConverter::EigenMatrixToXMMatrix(bone->offsetMatrix);

	nodeTable[node->name] = node;

	for (int i = 0; i < bone->children.size(); i++)
	{
		Node* child = new Node();
		child->parent = node;
		node->children.push_back(child);
		CopyNode(child, bone->children[i], nodeTable);
	}
}

void ZeldaModel::RenderAnimation(
	ID3D11DeviceContext* deviceContext,
	ConstantBuffer<MatrixBufferType, ShaderType::VertexShader>* matrixConstBuffer,
	ConstantBuffer<BoneBufferType, ShaderType::VertexShader>* boneConstBuffer,
	ConstantBuffer<MaterialBufferType, ShaderType::PixelShader>* materialConstBuffer,
	DirectX::XMMATRIX worldMatrix,
	ZeldaShader* shader,
	const std::wstring& animationName,
	float animationTime)
{
	assert(animationTable.count(animationName) != 0);
	
	float frameTime = animationTime * animationTable[animationName]->tickPerSecond;

	// 애니메이션 시간에 따라 Bone정보 변경
	std::queue<std::pair<Node*, DirectX::XMMATRIX>> nodeQueue;

	nodeQueue.push({ root, root->transformMatrix });

	while (!nodeQueue.empty())
	{
		auto current = nodeQueue.front();
		nodeQueue.pop();
		Node* currentNode = current.first;
		DirectX::XMMATRIX currentMatrix = current.second;

		currentNode->finalTM = currentNode->offsetMatrix * currentMatrix;

		for (int i = 0; i < currentNode->children.size(); i++)
		{
			Node* nextNode = currentNode->children[i];
			DirectX::XMMATRIX nextMatrix = nextNode->transformMatrix * currentMatrix;

			if (animationTable[animationName]->animationKey.count(nextNode->name) != 0)
			{
				auto lowerIter = animationTable[animationName]->animationKey[nextNode->name].lower_bound(static_cast<double>(frameTime));

				auto upperIter = animationTable[animationName]->animationKey[nextNode->name].upper_bound(static_cast<double>(frameTime));

				if (lowerIter != animationTable[animationName]->animationKey[nextNode->name].begin())
				{
					lowerIter--;
				}

				if (upperIter == animationTable[animationName]->animationKey[nextNode->name].end())
				{
					upperIter--;
				}

				XMMATRIX interpolatedMatrix;
				if (lowerIter != upperIter)
				{
					float beginTime = static_cast<float>(lowerIter->first);
					float endTime = static_cast<float>(upperIter->first);
					float currentTime = static_cast<float>(static_cast<double>(frameTime));

					// 시간에 대한 보간 계수 계산
					float t = (currentTime - beginTime) / (endTime - beginTime);

					// 선형 보간을 사용하여 행렬 보간
					AnimationKeyInfo lowerKey = lowerIter->second;
					AnimationKeyInfo upperKey = upperIter->second;

					XMVECTOR scale = XMVectorLerp(lowerKey.scale, upperKey.scale, t);
					XMVECTOR rotation = XMQuaternionSlerp(lowerKey.rotation, upperKey.rotation, t);
					XMVECTOR position = XMVectorLerp(lowerKey.position, upperKey.position, t);

					// 보간된 행렬 구성
					XMMATRIX scaleMatrix = XMMatrixScalingFromVector(scale);
					XMMATRIX rotationMatrix = XMMatrixRotationQuaternion(rotation);
					XMMATRIX translationMatrix = XMMatrixTranslationFromVector(position);

					interpolatedMatrix = scaleMatrix * rotationMatrix * translationMatrix;
				}
				else
				{
					AnimationKeyInfo keyinfo = lowerIter->second;
					DirectX::XMMATRIX scaleMatrix = XMMatrixScalingFromVector(keyinfo.scale);
					DirectX::XMMATRIX rotationMatrix = XMMatrixRotationQuaternion(keyinfo.rotation);
					DirectX::XMMATRIX translationMatrix = XMMatrixTranslationFromVector(keyinfo.position);
					interpolatedMatrix = scaleMatrix * rotationMatrix * translationMatrix;
				}

				nextMatrix = interpolatedMatrix * currentMatrix;
			}

			nodeQueue.push({ nextNode, nextMatrix });
		}
	}

	BoneBufferType* boneBuffer = new BoneBufferType();
	for (int i = 0; i < bones.size(); i++)
	{
		if (bones[i] == nullptr) continue;
		// 셰이더에 넘기는 행렬을 전치를 한 후 넘겨야 한다.
		boneBuffer->boneTM[i] = XMMatrixTranspose(bones[i]->finalTM);
	}

	ZeldaCamera* currentcamera = ResourceManager::GetInstance().GetCamera(ZeldaCamera::GetMainCamera());

	// 셰이더에 넘기는 행렬을 전치를 한 후 넘겨야 한다.
	matrixConstBuffer->SetData({ XMMatrixTranspose(worldMatrix), XMMatrixTranspose(currentcamera->GetViewMatrix()), XMMatrixTranspose(currentcamera->GetProjMatrix()) });
	boneConstBuffer->SetData(*boneBuffer);

	delete boneBuffer;

	// 모든 메쉬 그리기
	for (int i = 0; i < meshes.size(); i++)
	{
		ZeldaMesh* currentMesh = meshes[i];
		currentMesh->Render(deviceContext);
		int indexCount = currentMesh->GetIndexCount();

		materialConstBuffer->SetData({
			materials[materialIndex[i]]->baseColor,
			!materials[materialIndex[i]]->useDiffuseMap,
			materials[materialIndex[i]]->UseSRGB(ZeldaMaterial::DIFFUSE_MAP),
			materials[materialIndex[i]]->useDiffuseMap
			});

		ConstantBufferManager::GetInstance().SetBuffer();

		materials[materialIndex[i]]->SetShaderResource(deviceContext);

		shader->Render(deviceContext, indexCount);
	}
}