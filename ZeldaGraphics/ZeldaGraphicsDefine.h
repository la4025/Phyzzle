#pragma once

#include <d3d11.h>
#include <DirectXMath.h>

#define LIGHT_TYPE_DIRECTIONAL 0u
#define LIGHT_TYPE_POINT 1u
#define LIGHT_TYPE_SPOT 2u

#define LIGHT_COUNT_MAX 50u

#define BONE_COUNT_MAX 256u

struct VertexType
{
	DirectX::XMFLOAT4 position;
	DirectX::XMFLOAT3 normal;
	DirectX::XMFLOAT2 texture;
	DirectX::XMUINT4 boneIndices;
	DirectX::XMFLOAT4 weight;

	const static unsigned int size;
	const static D3D11_INPUT_ELEMENT_DESC layout[];
};

#pragma region Constant Buffer

struct MatrixBufferType
{
	DirectX::XMMATRIX world;
	DirectX::XMMATRIX view;
	DirectX::XMMATRIX projection;

	constexpr static unsigned int registerNum = 0;
};

struct BoneBufferType
{
	DirectX::XMMATRIX boneTM[BONE_COUNT_MAX];

	constexpr static unsigned int registerNum = 1;
};

struct LightColor
{
	DirectX::XMFLOAT4 ambient;
	DirectX::XMFLOAT4 diffuse;
	DirectX::XMFLOAT4 specular;
};

struct LightInfo
{
	LightColor color;
	DirectX::XMFLOAT4 position;
	DirectX::XMFLOAT4 direction;
	unsigned int type;
	float range;
	float angle;
	float padding;
};

struct LightBufferType
{
	unsigned int lightCount;
	float padding[3];
	LightInfo lights[LIGHT_COUNT_MAX];

	constexpr static unsigned int registerNum = 2;
};

struct MaterialBufferType
{
	DirectX::XMFLOAT4 baseColor;

	unsigned int useBaseColor;
	unsigned int useSRGB;
	unsigned int useDiffuse;
	unsigned int useNormal;

	unsigned int useARM;
	unsigned int useHeight;
	unsigned int useEmission;
	unsigned int useTemp0;

	unsigned int useTemp1;
	unsigned int useTemp2;
	unsigned int useTemp3;
	unsigned int useTemp4;

	constexpr static unsigned int registerNum = 3;
};

#pragma endregion

enum class ShaderType
{
	VertexShader,
	PixelShader
};