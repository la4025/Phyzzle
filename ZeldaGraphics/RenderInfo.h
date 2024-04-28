#pragma once

#include "GraphicsResourceID.h"
#include "ZeldaGraphicsDefine.h"

#include "Color.h"

enum class RenderType
{
	Deferred_Mesh,
	Deferred_Model,
	Deferred_BlendingAnimation,
	Forward_Mesh,
	Forward_Model,
	Forward_BlendingAnimation,
	Sprite,
	String,
	Light
};

enum class RenderOption : unsigned int
{
	None = 0x00000000u,
	OutLine = 0x00000001u,
	FastOutLine = 0x00000002u,
	Shadow = 0x00000004u
};

struct InstancingKey
{
	MeshID meshID;
	TextureID textureID;
	ModelID modelID;
	LightID lightID;
	unsigned int animationID; // 기존의 animationName을 Model class를 통해 id로 변환해서 저장하도록 하자
	Color color;

	InstancingKey() :
		meshID(MeshID::ID_NULL),
		textureID(TextureID::ID_NULL),
		modelID(ModelID::ID_NULL),
		lightID(LightID::ID_NULL),
		animationID(0xffffffff),
		color({ 0.0f, 0.0f, 0.0f, 0.0f })
	{}

	bool operator==(const InstancingKey& right) const
	{
		return
			meshID == right.meshID &&
			textureID == right.textureID &&
			modelID == right.modelID &&
			lightID == right.lightID &&
			animationID == right.animationID &&
			color == right.color;
	}
};

struct InstancingValue
{
	DirectX::XMMATRIX worldMatrix;
	DirectX::XMFLOAT2 position;
	DirectX::XMFLOAT2 size;
	float animationTime;
	Color color; // 기존에는 InstancingKey였으나 그냥 1024종류의 색 넘겨주면서 인스턴싱 해보자
	int layer;
	float fontSize;
	std::wstring str;

	InstancingValue() :
		worldMatrix(DirectX::XMMatrixIdentity()),
		position({ 0.0f, 0.0f }),
		size({ 0.0f, 0.0f }),
		animationTime(0.0f),
		color({ 1.0f, 1.0f, 1.0f, 1.0f }),
		layer(0),
		fontSize(30.0f),
		str(L"")
	{}
};

struct RenderInfo
{
	RenderType renderType;
	RenderOption renderOption;

	InstancingKey instancingKey;
	InstancingValue instancingValue;

	unsigned int drawID;
};

namespace std
{
	template <>
	struct hash<InstancingKey>
	{
		size_t operator()(const InstancingKey& obj) const
		{
			return
				hash<MeshID>{}(obj.meshID) ^
				hash<TextureID>{}(obj.textureID) ^
				hash<ModelID>{}(obj.modelID) ^
				hash<LightID>{}(obj.lightID) ^
				hash<unsigned int>{}(obj.animationID) ^
				hash<Color>{}(obj.color);
		};
	};
}