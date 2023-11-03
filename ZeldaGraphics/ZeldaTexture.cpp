#include "ZeldaTexture.h"

using namespace DirectX;

ZeldaTexture::ZeldaTexture(ID3D11Device* device, const std::wstring& filePath) :
	textureView(nullptr)
{
	HRESULT result = S_FALSE;

	ScratchImage image;

	std::wstring fileType = filePath.substr(filePath.size() - 4, 4);

	if (fileType == L".tga")
	{
		result = LoadFromTGAFile(filePath.c_str(), nullptr, image);
	}
	else if (fileType == L".dds")
	{
		result = LoadFromDDSFile(filePath.c_str(), DDS_FLAGS_NONE, nullptr, image);
	}
	else if (fileType == L".jpg" || fileType == L".png")
	{
		result = LoadFromWICFile(filePath.c_str(), WIC_FLAGS_NONE, nullptr, image);
	}

	if (FAILED(result))
	{
		assert(0);
	}

	result = CreateShaderResourceView(device, image.GetImages(), image.GetImageCount(), image.GetMetadata(), &textureView);

	if (FAILED(result))
	{
		assert(0);
	}
}

ZeldaTexture::ZeldaTexture(const ZeldaTexture& zeldaTexture) :
	textureView(nullptr)
{
	
}

ZeldaTexture::~ZeldaTexture()
{
	if (textureView)
	{
		textureView->Release();
		textureView = 0;
	}
}

ID3D11ShaderResourceView* ZeldaTexture::GetTexture()
{
	return textureView;
}
