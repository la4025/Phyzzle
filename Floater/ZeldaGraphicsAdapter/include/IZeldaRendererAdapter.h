#pragma once
#include "../FloaterRendererCommon/include/IRenderer.h"
#include "../FloaterRendererDX11/include/CreateRenderer.h"
#include "IZeldaRenderer.h"

class IZeldaRendererAdapter : public IZeldaRenderer
{
public:
	virtual bool Initialize(unsigned int screenWidth, unsigned int screenHeight, bool vsync, HWND hwnd, bool fullScreen, float screenDepth, float cameraNear) override;
	virtual void Finalize() override;

	virtual void BeginDraw() override;
	virtual void EndDraw() override;

	virtual void DrawCube(const Eigen::Matrix4f& worldMatrix, ResourceID texture, bool wireFrame) override;

	virtual void CreateResources() override;
	virtual ResourceID CreateTexture(const std::wstring& texturePath) override;

	virtual ResourceID CreateCamera() override;
	virtual bool ReleaseCamera(ResourceID cameraID) override;

	virtual bool SetMainCamera(ResourceID cameraID) override;
	virtual bool UpdateCamera(ResourceID cameraID, const Eigen::Matrix4f& worldMatrix, float fieldOfView, float cameraNear, float cameraFar) override;

private:
	unsigned int _screenWidth;
	unsigned int _screenHeight;
	bool _vsync;
	HWND _hwnd;
	bool _fullScreen;
	float _screenDepth;
	float _cameraNear;
	flt::IRenderer* _renderer;

	std::map<ResourceID, std::wstring> _cameraMap;
};

