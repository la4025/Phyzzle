#pragma once

#include "ZeldaGraphicsAPI.h"
#include "GraphicsResourceID.h"

#include <Windows.h>
#include <string>
#include <Eigen\Dense>

class IZeldaRenderer
{
public:
	virtual bool Initialize(unsigned int screenWidth, unsigned int screenHeight, bool vsync, HWND hwnd, bool fullScreen, float screenDepth, float cameraNear) abstract;
	virtual void Finalize() abstract;

	// deltaTime의 단위는 second
	virtual void BeginDraw(float deltaTime) abstract;
	virtual void EndDraw() abstract;

	virtual void DrawCube(const Eigen::Matrix4f& worldMatrix, TextureID texture, bool wireFrame, float r, float g, float b, float a) abstract;
	virtual void DrawModel(const Eigen::Matrix4f& worldMatrix, ModelID model, bool wireFrame) abstract;

	virtual void CreateBasicResources() abstract;
	virtual void ReleaseBasicResources() abstract;

	virtual TextureID CreateTexture(const std::wstring& texturePath) abstract;
	virtual bool ReleaseTexture(TextureID textureID) abstract;

	virtual ModelID CreateModel(const std::wstring& modelingFilePath) abstract;
	virtual bool ReleaseModel(ModelID modelID) abstract;

	virtual CameraID CreateCamera() abstract;
	virtual bool ReleaseCamera(CameraID cameraID) abstract;

	virtual bool SetMainCamera(CameraID cameraID) abstract;
	virtual bool UpdateCamera(CameraID cameraID, const Eigen::Matrix4f& worldMatrix, float fieldOfView, float cameraNear, float cameraFar) abstract;
};