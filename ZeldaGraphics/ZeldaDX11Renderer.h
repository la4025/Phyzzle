#pragma once

#include "IZeldaRenderer.h"

#include <d3d11.h>
#include <DirectXMath.h>

class ZeldaCamera;
class ZeldaShader;
class ZeldaModel;
class ZeldaLight;

class ZeldaDX11Renderer : public IZeldaRenderer
{
public:
	virtual bool Initialize(unsigned int screenWidth, unsigned int screenHeight, bool vsync, HWND hwnd, bool fullScreen, float screenDepth, float screenNear) override;
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
	bool bVsyncEnabled;
	int mVideoCardMemory;
	char mVideoCardDescription[128];
	IDXGISwapChain* mSwapChain;
	ID3D11Device* mDevice;
	ID3D11DeviceContext* mDeviceContext;
	ID3D11RenderTargetView* mRenderTargetView;
	ID3D11Texture2D* mDepthStencilBuffer;
	ID3D11DepthStencilState* mDepthStencilState;
	ID3D11DepthStencilView* mDepthStencilView;
	ID3D11RasterizerState* mRasterState;

	ID3D11RasterizerState* defaultRasterState;
	ID3D11RasterizerState* wireFrameRasterState;

	ResourceID mainCameraID;
	ZeldaLight* light;

	HWND hWnd;
	unsigned int screenWidth;
	unsigned int screenHeight;

	// hwnd
	// fullScreenMode

};