#pragma once
#include <string>

namespace flt
{
	class Transform;

	enum class RendererFlag
	{
		SOFTWARE = 0x0000'0001,
		ACCELERATED = 0x0000'0002
	};

	struct Renderable
	{
		Renderable(Transform& transform) : transform(transform), name() {}
		// 1. 그릴때 필요한것들.
		// 2. 게임 엔진상에서 데이터를 바꾸고 싶은것들.

		// transform
		// model
		// material, texture
		// shader
		// 

		Transform& transform;
		std::wstring name;
		//bool& isEnable;
	};

	class IRenderer
	{
	public:
		IRenderer() = default;
		virtual ~IRenderer() = default;

		virtual bool Render(float deltaTime) = 0;
		virtual bool RegisterObject(Renderable& renderable) = 0;
		virtual bool DeregisterObject(Renderable& renderable) = 0;

		virtual bool Test() { return false; };
	};
}
