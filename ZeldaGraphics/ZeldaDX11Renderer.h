#pragma once

#include "IZeldaRenderer.h"

#include <d3d11.h>

#include <DirectXMath.h>
#include "ZeldaCamera.h"
#include "ZeldaModel.h"
#include "ZeldaShader.h"

class ZeldaDX11Renderer : public IZeldaRenderer
{
public:
	virtual void CreateResources() override;

	virtual bool Initialize(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullScreen, float screenDepth, float screenNear) override;

	virtual void BeginDraw() override;
	virtual void EndDraw() override;

	virtual void DrawCube() override;

private:
	void Shutdown();

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

	ZeldaCamera* camera;
	ZeldaShader* shader;
	ZeldaModel* model;

	DirectX::XMMATRIX mProjectionMatrix;
	DirectX::XMMATRIX mWorldMatrix;
	DirectX::XMMATRIX mOrthoMatrix;

	// hwnd
	// fullScreenMode

};