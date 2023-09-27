#pragma once

#include "IZeldaRenderer.h"

#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")

class ZeldaDX11Renderer : public IZeldaRenderer
{
public:
	virtual bool Initialize(HWND hwnd) override;

	virtual void BeginDraw() override;
	virtual void EndDraw() override;

private:
	ID3D11Device* md3dDevice;
	D3D_FEATURE_LEVEL featureLevel;
	ID3D11DeviceContext* md3dImmediateContext;

	IDXGISwapChain* swapChain;

	ID3D11RenderTargetView* renderTargetView;

	ID3D11Texture2D* depthStencilBuffer;
	ID3D11DepthStencilView* depthStencilView;

	unsigned int clientWidth = 1920u;
	unsigned int clientHeight = 1080u;

	bool enable4xMsaa = false;
	bool fullScreenMode = false;

	HWND hWnd;
};

