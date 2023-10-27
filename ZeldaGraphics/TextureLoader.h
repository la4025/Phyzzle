#pragma once

#include <d3d11.h>
#include <string>
#include "DirectXTex.h"

class ZeldaTexture;

class TextureLoader
{
public:
	void Initialize(ID3D11Device* device);

	ZeldaTexture* CreateTextureFromImageFile(const std::wstring& filePath);
	ZeldaTexture* CreateTextureFrom3DModelFile(const std::wstring& filePath);

private:
	ID3D11Device* device;


	//singleton
public:
	static TextureLoader& GetInstance();

private:
	TextureLoader();
	~TextureLoader();
	TextureLoader(const TextureLoader& ref) = delete;
	TextureLoader& operator=(const TextureLoader& ref) = delete;
};

