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

	virtual void BeginDraw() abstract;
	virtual void EndDraw() abstract;

	virtual void DrawCube(const Eigen::Matrix4f& worldMatrix, ResourceID texture, bool wireFrame) abstract;
	 
	virtual void CreateResources() abstract;
	virtual ResourceID CreateTexture(const std::wstring& texturePath) abstract;

	/// <summary>
	/// 카메라를 생성하고 ID를 리턴받는다.
	/// </summary>
	/// <returns>
	/// <para>Success : return 16bytes of non-zero data</para>
	/// <para>Failed : return ID_NULL</para>
	/// </returns>
	virtual ResourceID CreateCamera() abstract;
	virtual bool ReleaseCamera(ResourceID cameraID) abstract;

	virtual bool SetMainCamera(ResourceID cameraID) abstract;
	virtual bool UpdateCamera(ResourceID cameraID, const Eigen::Matrix4f& worldMatrix, float fieldOfView, float cameraNear, float cameraFar) abstract;
};