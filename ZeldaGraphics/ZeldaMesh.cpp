#include "ZeldaMesh.h"

#include "ZeldaGraphicsDefine.h"

ZeldaMesh::ZeldaMesh(ID3D11Buffer* vertexBuffer, ID3D11Buffer* indexBuffer, int vertexCount, int indexCount) :
	vertexBuffer(vertexBuffer), indexBuffer(indexBuffer), vertexCount(vertexCount), indexCount(indexCount)
{
}

ZeldaMesh::~ZeldaMesh()
{
	if (vertexBuffer != nullptr)
	{
		vertexBuffer->Release();
		vertexBuffer = nullptr;
	}

	if (indexBuffer != nullptr)
	{
		indexBuffer->Release();
		indexBuffer = nullptr;
	}
}

void ZeldaMesh::Render(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;

	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType);
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

int ZeldaMesh::GetIndexCount()
{
	return indexCount;
}
