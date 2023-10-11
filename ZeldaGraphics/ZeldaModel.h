#pragma once

#include <d3d11.h>
#include <DirectXMath.h>

class ZeldaModel
{
private:
	struct VertexType
	{
		DirectX::XMFLOAT4 position;
		DirectX::XMFLOAT4 color;
	};

public:
	ZeldaModel();
	ZeldaModel(const ZeldaModel& zeldaModel);
	~ZeldaModel();

	bool Initialize(ID3D11Device* device);
	void Shutdown();
	void Render(ID3D11DeviceContext* deviceContext);
	int GetIndexCount();

private:
	bool InitializeBuffers(ID3D11Device* device);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext* deviceContext);

private:
	ID3D11Buffer* mVertexBuffer;
	ID3D11Buffer* mIndexBuffer;
	int mVertexCount, mIndexCount;
};

