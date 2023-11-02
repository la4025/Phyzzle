#pragma once

#include <DirectXMath.h>

struct VertexType
{
	DirectX::XMFLOAT4 position;
	DirectX::XMFLOAT4 color;
	DirectX::XMFLOAT2 texture;
	DirectX::XMFLOAT3 normal;

	const static int size = 4;
};

struct MatrixBufferType
{
	DirectX::XMMATRIX world;
	DirectX::XMMATRIX view;
	DirectX::XMMATRIX projection;
};

struct LightBufferType
{
	DirectX::XMFLOAT4 ambient;
	DirectX::XMFLOAT4 diffuse;
	DirectX::XMFLOAT4 specular;
	DirectX::XMFLOAT3 lightDirection;
	float padding; // CreateBuffer함수가 성공하려면 16바이트의 배수로 만들어야 한다.(업데이트 및 전송이 빈번하기 때문에 효율적인 메모리 정렬이 되어있어야 한다고 함)
};

struct useBufferType
{
	unsigned int useTexture;
	unsigned int useTemp0;
	unsigned int useTemp1;
	unsigned int useTemp2;
};

enum class ShaderType
{
	VertexShader,
	PixelShader
};