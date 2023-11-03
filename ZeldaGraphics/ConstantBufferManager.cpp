#include "ConstantBufferManager.h"

#include "ConstantBuffer.h"

void ConstantBufferManager::RegisterVSBuffer(IConstantBuffer* buffer)
{
	vsBufferList.push_back(buffer);
}

void ConstantBufferManager::RegisterPSBuffer(IConstantBuffer* buffer)
{
	psBufferList.push_back(buffer);
}

void ConstantBufferManager::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	this->device = device;
	this->deviceContext = deviceContext;
}

void ConstantBufferManager::Finalize()
{
}

void ConstantBufferManager::SetBuffer()
{
	for (unsigned int i = 0; i < vsBufferList.size(); i++)
	{
		vsBufferList[i]->SetBuffer(deviceContext, i);
	}

	for (unsigned int i = 0; i < psBufferList.size(); i++)
	{
		psBufferList[i]->SetBuffer(deviceContext, i);
	}
}

ConstantBufferManager& ConstantBufferManager::GetInstance()
{
	static ConstantBufferManager instance;
	return instance;
}

ConstantBufferManager::ConstantBufferManager()
{
}

ConstantBufferManager::~ConstantBufferManager()
{
}
