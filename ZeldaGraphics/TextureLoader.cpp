#include "TextureLoader.h"
#include "ZeldaTexture.h"

using namespace DirectX;

void TextureLoader::Initialize(ID3D11Device* device)
{
	this->device = device;
}

ZeldaTexture* TextureLoader::CreateTextureFromImageFile(const std::wstring& filePath)
{
	ZeldaTexture* texture = new ZeldaTexture(device, filePath);

	return texture;
}

ZeldaTexture* TextureLoader::CreateTextureFrom3DModelFile(const std::wstring& filePath)
{
	return nullptr;
}

TextureLoader& TextureLoader::GetInstance()
{
	static TextureLoader instance;
	return instance;
}

TextureLoader::TextureLoader() :
	device(nullptr)
{
}

TextureLoader::~TextureLoader()
{
}
