#pragma once

#include <d3d11.h>
#include <DirectXMath.h>

#include <string>

class ZeldaTexture;

class ZeldaModel
{
private:
	struct VertexType
	{
		DirectX::XMFLOAT4 position;
		DirectX::XMFLOAT2 texture;
		DirectX::XMFLOAT3 normal;

	};

public:
	ZeldaModel();
	ZeldaModel(const ZeldaModel& zeldaModel);
	~ZeldaModel();

	bool Initialize(ID3D11Device* device, const std::wstring& textureFilename);
	void Shutdown();
	void Render(ID3D11DeviceContext* deviceContext);
	int GetIndexCount();
	ID3D11ShaderResourceView* GetTexture();

private:
	bool InitializeBuffers(ID3D11Device* device);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext* deviceContext);

	bool LoadTexture(ID3D11Device* device, const std::wstring& fileName);
	void ReleaseTexture();

private:
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
	int vertexCount, indexCount;
	ZeldaTexture* texture;

	ID3D11RasterizerState* rasterState;
};

