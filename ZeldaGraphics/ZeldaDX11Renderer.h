#pragma once

#include "IZeldaRenderer.h"

#include "ConstantBuffer.h"

#include <d3d11.h>
#include <DirectXMath.h>

#include <SpriteBatch.h>
#include <CommonStates.h>

#include <unordered_map>

#define DEFFERED_BUFFER_COUNT 3

class ZeldaCamera;
class ZeldaShader;
class ZeldaModel;
class ZeldaLight;
class ZeldaTexture;
class ZeldaMaterial;
class IRenderable;

struct MeshInstancingInfo
{
	DirectX::XMMATRIX worldMatrix;
	bool wireFrame;
	DirectX::XMFLOAT4 color;
};

struct ModelInstancingInfo
{
	DirectX::XMMATRIX worldMatrix;
	std::wstring animationName;
	float animationTime;
	bool wireFrame;
};

struct SpriteInstancingInfo
{
	DirectX::XMFLOAT2 position;
};

struct MeshRenderInfo
{
	std::vector<MeshInstancingInfo> instancingInfo;
	MeshID meshId;
	TextureID textureID;
};

struct ModelRenderInfo
{
	std::vector<ModelInstancingInfo> instancingInfo;
	ModelID modelID;
};

struct SpriteRenderInfo
{
	std::vector<SpriteInstancingInfo> instancingInfo;
	TextureID textureID;
};

class ZeldaDX11Renderer : public IZeldaRenderer
{
public:
	virtual bool Initialize(unsigned int screenWidth, unsigned int screenHeight, bool vsync, HWND hwnd, bool fullScreen, float screenDepth, float screenNear) override;
	virtual void Finalize() override;

	virtual void BeginDraw(float deltaTime) override;
	virtual void EndDraw() override;

	virtual void DrawCube(const Eigen::Matrix4f& worldMatrix, TextureID texture, bool wireFrame, float r, float g, float b, float a) override;
	virtual void DrawModel(const Eigen::Matrix4f& worldMatrix, ModelID model, bool wireFrame) override;
	virtual void DrawAnimation(const Eigen::Matrix4f& worldMatrix, ModelID model, std::wstring animationName, float animationTime, bool wireFrame) override;

	virtual void DrawSprite(const Eigen::Vector2f& position, TextureID texture) override;

	virtual void CreateBasicResources() override;
	virtual void ReleaseBasicResources() override;

	virtual TextureID CreateTexture(const std::wstring& texturePath) override;
	virtual bool ReleaseTexture(TextureID textureID) override;

	virtual ModelID CreateModel(const std::wstring& modelingFilePath) override;
	virtual bool ReleaseModel(ModelID modelID) override;
	virtual std::vector<std::wstring> GetAnimationListByModel(ModelID modelID) override;
	virtual std::vector<float> GetAnimationPlayTime(ModelID modelID) override;

	virtual CameraID CreateCamera() override;
	virtual bool ReleaseCamera(CameraID cameraID) override;

	virtual bool SetMainCamera(CameraID cameraID) override;
	virtual bool UpdateCamera(CameraID cameraID, const Eigen::Matrix4f& worldMatrix, float fieldOfView, float cameraNear, float cameraFar) override;

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

	// Deffered Rendering
	ID3D11RenderTargetView* defferedRenderTargets[DEFFERED_BUFFER_COUNT];
	ID3D11ShaderResourceView* defferedShaderResources[DEFFERED_BUFFER_COUNT];

	ZeldaShader* defferedObjectShader;

	ZeldaLight* light;

	DirectX::SpriteBatch* spriteBatch;
	DirectX::DX11::CommonStates* commonStates;

	HWND hWnd;
	unsigned int screenWidth;
	unsigned int screenHeight;

	// hwnd
	// fullScreenMode

	// Constant Buffer
	ConstantBuffer<MatrixBufferType, ShaderType::VertexShader>* matrixConstBuffer;
	ConstantBuffer<BoneBufferType, ShaderType::VertexShader>* boneConstBuffer;

	ConstantBuffer<LightBufferType, ShaderType::PixelShader>* lightConstBuffer;
	ConstantBuffer<MaterialBufferType, ShaderType::PixelShader>* materialConstBuffer;


	// Draw함수가 호출되면 채워진다. BeginDraw에서 ClearRenderInfo를 통해 초기화된다.
	std::unordered_map<std::pair<MeshID, TextureID>, MeshRenderInfo> organizedMeshRenderInfo;
	std::unordered_map<ModelID, ModelRenderInfo> organizedModelRenderInfo;
	std::unordered_map<TextureID, SpriteRenderInfo> organizedSpriteRenderInfo;

	void DrawMeshRenderInfo(MeshRenderInfo renderInfo);
	void DrawModelRenderInfo(ModelRenderInfo renderInfo);
	void DrawSpriteRenderInfo(SpriteRenderInfo renderInfo);

	void ClearRenderInfo();
};