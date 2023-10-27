#include "ZeldaModel.h"

#include "ZeldaTexture.h"
#include "TextureLoader.h"

using namespace DirectX;

ZeldaModel::ZeldaModel() :
	vertexBuffer(nullptr),
	indexBuffer(nullptr),
	vertexCount(0),
	indexCount(0),
	texture(nullptr),
	rasterState(nullptr)
{

}

ZeldaModel::ZeldaModel(const ZeldaModel& zeldaModel) :
	vertexBuffer(zeldaModel.vertexBuffer),
	indexBuffer(zeldaModel.indexBuffer),
	vertexCount(zeldaModel.vertexCount),
	indexCount(zeldaModel.indexCount),
	texture(zeldaModel.texture),
	rasterState(zeldaModel.rasterState)
{

}

ZeldaModel::~ZeldaModel()
{
}

bool ZeldaModel::Initialize(ID3D11Device* device, const std::wstring& textureFilename)
{
	bool result;

	// Initialize the vertex and index buffers.
	result = InitializeBuffers(device);
	if (!result) return false;

	// Load the texture for this model.
	result = LoadTexture(device, textureFilename);
	if (!result) return false;

	return true;
}

void ZeldaModel::Shutdown()
{
	// Shutdown the vertex and index buffers.
	ShutdownBuffers();

	return;
}

void ZeldaModel::Render(ID3D11DeviceContext* deviceContext)
{
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	RenderBuffers(deviceContext);

	return;
}

int ZeldaModel::GetIndexCount()
{
	return indexCount;
}

ID3D11ShaderResourceView* ZeldaModel::GetTexture()
{
	return texture->GetTexture();
}

bool ZeldaModel::InitializeBuffers(ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	// Set the number of vertices in the vertex array.
	vertexCount = 24;
	// Set the number of indices in the index array.
	indexCount = 36;
	// Create the vertex array.
	vertices = new VertexType[vertexCount];
	if (!vertices) return false;
	// Create the index array.
	indices = new unsigned long[indexCount];
	if (!indices) return false;

	XMVECTOR worldUp = { 0.0f, 1.0f, 0.0f, 0.0f };
	XMVECTOR worldBack = { 0.0f, 0.0f, 1.0f, 0.0f };

	// 순서 바꾸면 안됨(아래의 Cross계산 때문에)
	XMFLOAT3 dir[6] = {
		{0.0f, 0.0f, -1.0f},
		{0.0f, 0.0f, 1.0f},
		{-1.0f, 0.0f, 0.0f},
		{1.0f, 0.0f, 0.0f},
		{0.0f, -1.0f, 0.0f},
		{0.0f, 1.0f, 0.0f}
	};

	for (int faceNum = 0; faceNum < 6; faceNum++)
	{
		XMVECTOR up = worldUp;
		if (faceNum == 4 || faceNum == 5)
		{
			up = worldBack;
		}

		XMVECTOR down = -up;
		XMVECTOR front = XMLoadFloat3(&dir[faceNum]);
		XMVECTOR left = XMVector3Cross(up, XMLoadFloat3(&dir[faceNum]));
		XMVECTOR right = -left;

		// 왼쪽 아래
		XMStoreFloat4(&vertices[faceNum * 4 + 0].position, 0.5f * (front + left + down));
		vertices[faceNum * 4 + 0].position.w = 1.0f;
		vertices[faceNum * 4 + 0].texture = XMFLOAT2(0.0f, 1.0f);
		vertices[faceNum * 4 + 0].normal = dir[faceNum];

		// 왼쪽 위
		XMStoreFloat4(&vertices[faceNum * 4 + 1].position, 0.5f * (front + left + up));
		vertices[faceNum * 4 + 0].position.w = 1.0f;
		vertices[faceNum * 4 + 1].texture = XMFLOAT2(0.0f, 0.0f);
		vertices[faceNum * 4 + 1].normal = dir[faceNum];

		// 오른쪽 위
		XMStoreFloat4(&vertices[faceNum * 4 + 2].position, 0.5f * (front + right + up));
		vertices[faceNum * 4 + 0].position.w = 1.0f;
		vertices[faceNum * 4 + 2].texture = XMFLOAT2(1.0f, 0.0f);
		vertices[faceNum * 4 + 2].normal = dir[faceNum];

		// 오른쪽 아래
		XMStoreFloat4(&vertices[faceNum * 4 + 3].position, 0.5f * (front + right + down));
		vertices[faceNum * 4 + 0].position.w = 1.0f;
		vertices[faceNum * 4 + 3].texture = XMFLOAT2(1.0f, 1.0f);
		vertices[faceNum * 4 + 3].normal = dir[faceNum];
	}

	// Load the index array with data.
	for (int faceNum = 0; faceNum < 6; faceNum++)
	{
		indices[6 * faceNum + 0] = 4 * faceNum + 0;
		indices[6 * faceNum + 1] = 4 * faceNum + 1;
		indices[6 * faceNum + 2] = 4 * faceNum + 2;
		indices[6 * faceNum + 3] = 4 * faceNum + 2;
		indices[6 * faceNum + 4] = 4 * faceNum + 3;
		indices[6 * faceNum + 5] = 4 * faceNum + 0;
	}

	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;
	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;
	// Now create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &vertexBuffer);
	if (FAILED(result)) return false;

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;
	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;
	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &indexBuffer);
	if (FAILED(result)) return false;

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete[] vertices;
	vertices = 0;
	delete[] indices;
	indices = 0;





	D3D11_RASTERIZER_DESC rasterDesc;

	// Setup the raster description which will determine how and what polygons will be drawn.
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;
	// Create the rasterizer state from the description we just filled out.
	result = device->CreateRasterizerState(&rasterDesc, &rasterState);
	if (FAILED(result)) return false;

	return true;
}

void ZeldaModel::ShutdownBuffers()
{
	// Release the index buffer.
	if (indexBuffer)
	{
		indexBuffer->Release();
		indexBuffer = 0;
	}
	// Release the vertex buffer.
	if (vertexBuffer)
	{
		vertexBuffer->Release();
		vertexBuffer = 0;
	}

	return;
}

void ZeldaModel::RenderBuffers(ID3D11DeviceContext* deviceContext)
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

	return;
}

bool ZeldaModel::LoadTexture(ID3D11Device* device, const std::wstring& fileName)
{
	bool result;

	// Create the texture object.
	texture = TextureLoader::GetInstance().CreateTextureFromImageFile(fileName);
	if (!texture) return false;

	return true;
}

void ZeldaModel::ReleaseTexture()
{
	// Release the texture object.
	if (texture)
	{
		delete texture;
		texture = nullptr;
	}

	return;
}
