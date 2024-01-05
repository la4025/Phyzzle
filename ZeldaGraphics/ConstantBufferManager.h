#pragma once

#include <vector>

#include "IConstantBuffer.h"

class ConstantBufferManager
{
public:
	void Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	void Finalize();

	void RegisterBuffer(IConstantBuffer* buffer);
	void DeRegisterBuffer(IConstantBuffer* buffer);

	void SetBuffer();

private:
	std::vector<IConstantBuffer*> bufferList;
	ID3D11Device* device;
	ID3D11DeviceContext* deviceContext;


	//singleton
public:
	static ConstantBufferManager& GetInstance();

private:
	ConstantBufferManager();
	~ConstantBufferManager();
	ConstantBufferManager(const ConstantBufferManager& ref) = delete;
	ConstantBufferManager& operator=(const ConstantBufferManager& ref) = delete;
};

