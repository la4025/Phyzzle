#pragma once

#include <d3d11.h>
#include <DirectXMath.h>

class ZeldaMesh
{
public:
	ZeldaMesh(const ZeldaMesh& zeldaMesh) = delete;
	~ZeldaMesh();

	void Render(ID3D11DeviceContext* deviceContext);
	int GetIndexCount();

private:
	ZeldaMesh(ID3D11Buffer* vertexBuffer, ID3D11Buffer* indexBuffer, int vertexCount, int indexCount);

private:
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
	int vertexCount, indexCount;

	friend class ResourceManager;
};

