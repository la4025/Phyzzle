#pragma once

#include <d3d11.h>

class IConstantBuffer
{
public:
	virtual void SetBuffer(ID3D11DeviceContext* deviceContext) abstract;

};