#include "./include/IZeldaRendererAdapter.h"

bool IZeldaRendererAdapter::Initialize(unsigned int screenWidth, unsigned int screenHeight, bool vsync, HWND hwnd, bool fullScreen, float screenDepth, float cameraNear)
{
	_screenWidth = screenWidth;
	_screenHeight = screenHeight;
	_vsync = vsync;
	_hwnd = hwnd;
	_fullScreen = fullScreen;
	_screenDepth = screenDepth;
	_cameraNear = cameraNear;

	_renderer = flt::CreateRendererDX11(hwnd);

	if (_renderer == nullptr)
	{
		return false;
	}

	return true;
}

void IZeldaRendererAdapter::Finalize()
{
	flt::DestroyRendererDX11(_renderer);
}

void IZeldaRendererAdapter::BeginDraw(float deltaTime)
{
	_renderer->Render(deltaTime);
}

void IZeldaRendererAdapter::EndDraw()
{
	return;
}

void IZeldaRendererAdapter::DrawCube(const Eigen::Matrix4f& worldMatrix, TextureID texture, bool wireFrame, float r, float g, float b, float a)
{

}

void IZeldaRendererAdapter::DrawModel(const Eigen::Matrix4f& worldMatrix, ModelID model, bool wireFrame)
{

}

void IZeldaRendererAdapter::CreateBasicResources()
{

}

void IZeldaRendererAdapter::ReleaseBasicResources()
{

}

TextureID IZeldaRendererAdapter::CreateTexture(const std::wstring& texturePath)
{
	return TextureID{};
}

bool IZeldaRendererAdapter::ReleaseTexture(TextureID textureID)
{
	return false;
}

ModelID IZeldaRendererAdapter::CreateModel(const std::wstring& modelingFilePath)
{
	return ModelID{};
}

bool IZeldaRendererAdapter::ReleaseModel(ModelID modelID)
{
	return false;
}

CameraID IZeldaRendererAdapter::CreateCamera()
{
	return CameraID{};
}

bool IZeldaRendererAdapter::ReleaseCamera(CameraID cameraID)
{
	return false;
}

bool IZeldaRendererAdapter::SetMainCamera(CameraID cameraID)
{
	return false;
}

bool IZeldaRendererAdapter::UpdateCamera(CameraID cameraID, const Eigen::Matrix4f& worldMatrix, float fieldOfView, float cameraNear, float cameraFar)
{
	return false;
}
