#include "DX11Mesh.h"
#include "../FloaterUtil/include/FloaterMacro.h"
#include <directxtk/DDSTextureLoader.h>

flt::DX11Mesh* flt::DX11MeshBuilder::build() const 
{
	DX11Mesh* pMesh = new DX11Mesh(*pResourceMgr, vsBuilder, psBuilder);
	return pMesh;
}

void flt::DX11Mesh::Release()
{
	vertexBuffer->Release();
	indexBuffer->Release();
	texture->Release();
	sampler->Release();
}

flt::DX11Mesh* flt::DX11CubeBuilder::build() const
{
	ASSERT(pDevice, "디바이스 세팅 안함.");
	ASSERT(pImmediateContext, "디바이스 컨텍스트 세팅 안함.");
	ASSERT(pResourceMgr, "리소스 메니져 세팅 안함.");

	const std::vector<D3D11_INPUT_ELEMENT_DESC> Basic32 =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	struct VertexUV
	{
		DirectX::XMFLOAT3 Pos;
		DirectX::XMFLOAT2 Tex;
	};

	std::vector<VertexUV> vertcies;
	vertcies.reserve(24);

	float w = 0.5f;
	float h = 0.5f;
	float d = 0.5f;

	vertcies.insert(vertcies.begin(),
		{
			VertexUV{DirectX::XMFLOAT3{-w, -h, -d}, DirectX::XMFLOAT2{0.0f, 1.0f}},
			VertexUV{DirectX::XMFLOAT3{-w, +h, -d}, DirectX::XMFLOAT2{0.0f, 0.0f}},
			VertexUV{DirectX::XMFLOAT3{+w, +h, -d}, DirectX::XMFLOAT2{1.0f, 0.0f}},
			VertexUV{DirectX::XMFLOAT3{+w, -h, -d}, DirectX::XMFLOAT2{1.0f, 1.0f}},

			VertexUV{DirectX::XMFLOAT3{-w, -h, +d}, DirectX::XMFLOAT2{1.0f, 1.0f}},
			VertexUV{DirectX::XMFLOAT3{+w, -h, +d}, DirectX::XMFLOAT2{0.0f, 1.0f}},
			VertexUV{DirectX::XMFLOAT3{+w, +h, +d}, DirectX::XMFLOAT2{0.0f, 0.0f}},
			VertexUV{DirectX::XMFLOAT3{-w, +h, +d}, DirectX::XMFLOAT2{1.0f, 0.0f}},

			VertexUV{DirectX::XMFLOAT3{-w, +h, -d}, DirectX::XMFLOAT2{0.0f, 1.0f}},
			VertexUV{DirectX::XMFLOAT3{-w, +h, +d}, DirectX::XMFLOAT2{0.0f, 0.0f}},
			VertexUV{DirectX::XMFLOAT3{+w, +h, +d}, DirectX::XMFLOAT2{1.0f, 0.0f}},
			VertexUV{DirectX::XMFLOAT3{+w, +h, -d}, DirectX::XMFLOAT2{1.0f, 1.0f}},

			VertexUV{DirectX::XMFLOAT3{-w, -h, -d}, DirectX::XMFLOAT2{1.0f, 1.0f}},
			VertexUV{DirectX::XMFLOAT3{+w, -h, -d}, DirectX::XMFLOAT2{0.0f, 1.0f}},
			VertexUV{DirectX::XMFLOAT3{+w, -h, +d}, DirectX::XMFLOAT2{0.0f, 0.0f}},
			VertexUV{DirectX::XMFLOAT3{-w, -h, +d}, DirectX::XMFLOAT2{1.0f, 0.0f}},

			VertexUV{DirectX::XMFLOAT3{-w, -h, +d}, DirectX::XMFLOAT2{0.0f, 1.0f}},
			VertexUV{DirectX::XMFLOAT3{-w, +h, +d}, DirectX::XMFLOAT2{0.0f, 0.0f}},
			VertexUV{DirectX::XMFLOAT3{-w, +h, -d}, DirectX::XMFLOAT2{1.0f, 0.0f}},
			VertexUV{DirectX::XMFLOAT3{-w, -h, -d}, DirectX::XMFLOAT2{1.0f, 1.0f}},

			VertexUV{DirectX::XMFLOAT3{+w, -h, -d}, DirectX::XMFLOAT2{0.0f, 1.0f}},
			VertexUV{DirectX::XMFLOAT3{+w, +h, -d}, DirectX::XMFLOAT2{0.0f, 0.0f}},
			VertexUV{DirectX::XMFLOAT3{+w, +h, +d}, DirectX::XMFLOAT2{1.0f, 0.0f}},
			VertexUV{DirectX::XMFLOAT3{+w, -h, +d}, DirectX::XMFLOAT2{1.0f, 1.0f}}
		});

	std::vector<int> indices;
	indices.reserve(36);
	indices.insert(indices.begin(),
		{
			0, 1, 2,
			0, 2, 3,

			4, 5, 6,
			4, 6, 7,

			8, 9, 10,
			8, 10, 11,

			12, 13, 14,
			12, 14, 15,

			16, 17, 18,
			16, 18, 19,

			20, 21, 22,
			20, 22, 23
		});


	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	vertexBufferDesc.ByteWidth = (UINT)(sizeof(VertexUV) * vertcies.size());
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = &(vertcies[0]);
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	ID3D11Buffer* vertexBuffer;
	HRESULT hResult = pDevice->CreateBuffer(&vertexBufferDesc, &vertexData, &vertexBuffer);

	if (hResult != S_OK)
	{
		return nullptr;
	}

	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	indexBufferDesc.ByteWidth = (UINT)(sizeof(int) * indices.size());
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = &(indices[0]);
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	ID3D11Buffer* indexBuffer;
	hResult = pDevice->CreateBuffer(&indexBufferDesc, &indexData, &indexBuffer);
	if (hResult != S_OK)
	{
		return nullptr;
	}

	D3D11_BUFFER_DESC cbDesc;
	cbDesc.ByteWidth = sizeof(DirectX::XMMATRIX);
	cbDesc.Usage = D3D11_USAGE_DYNAMIC;
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbDesc.MiscFlags = 0;
	cbDesc.StructureByteStride = 0;

	ID3D11Buffer* constantBuffer;
	hResult = pDevice->CreateBuffer(&cbDesc, nullptr, &constantBuffer);
	if (hResult != S_OK)
	{
		return nullptr;
	}

	ID3D11Resource* texture;
	ID3D11ShaderResourceView* textureView;
	hResult = DirectX::CreateDDSTextureFromFile(pDevice, pImmediateContext, L"../FloaterRendererDX11/testResource/WoodCrate01.dds", &texture, &textureView);
	if (hResult != S_OK)
	{
		return nullptr;
	}

	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC; //D3D11_FILTER_ANISOTROPIC;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = 2; D3D11_FLOAT32_MAX;

	ID3D11SamplerState* samplerState = nullptr;
	pDevice->CreateSamplerState(&samplerDesc, &samplerState);

	DX11VertexShaderBuilder vsBuilder(L"../FloaterRendererDX11/CubeVS.hlsl");
	vsBuilder.pDevice = pDevice;
	vsBuilder.pInputLayoutDesc = &(Basic32[0]);
	vsBuilder.descElementCount = Basic32.size();

	DX11PixelShaderBuilder psBuilder(L"../FloaterRendererDX11/CubePS.hlsl");
	psBuilder.pDevice = pDevice;

	DX11Mesh* pMesh = new DX11Mesh(*pResourceMgr, vsBuilder, psBuilder);
	pMesh->vertexBuffer = vertexBuffer;
	pMesh->singleVertexSize = sizeof(VertexUV);
	pMesh->indexBuffer = indexBuffer;
	pMesh->indexCount = indices.size();
	pMesh->texture = textureView;
	pMesh->sampler = samplerState;



	return pMesh;
}
