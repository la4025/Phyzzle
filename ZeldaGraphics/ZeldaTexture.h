#pragma once

#include <d3d11.h>
#include <string>

#include "DirectXTex.h"

class ZeldaTexture
{
public:
	~ZeldaTexture();

	ID3D11ShaderResourceView* GetTexture();

private:
	ZeldaTexture(ID3D11Device* device, const std::wstring& filePath);
	ZeldaTexture(const ZeldaTexture& zeldaTexture);

	ID3D11ShaderResourceView* textureView;

	friend class TextureLoader; // 삭제 예정
	friend class ResourceManager;
};

