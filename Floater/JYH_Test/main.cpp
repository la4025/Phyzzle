#define FLT_WINDOWS

#include <map>
#include <iostream>
#include "../FloaterPlatform/include/Platform.h"
#include "../FloaterUtil/include/Timer.h"
#include "../FloaterUtil/include/FloaterMacro.h"
#include "../FloaterRendererCommon/include/Transform.h"
//#include <windows.h>


#pragma region testInclude
#include "../FloaterRendererCommon/include/IBuilder.h"
#include "../FloaterRendererCommon/include/Resource.h"
#include "../FloaterRendererCommon/include/ResourceMgr.h"
#include "../FloaterUtil/TesterRBTree.h"
#pragma endregion


int main()
{
	setlocale(LC_ALL, ".UTF8");
	std::cout << std::boolalpha;



#pragma region 테스트
	{
		using namespace flt::test;
		TesterRBTree tester;
		if (!tester.Test())
		{
			ASSERT(false, "test fail");
		}
	}
#pragma endregion


	flt::Platform platform;
	platform.Initialize(1280, 720, L"title", L".\\path");
	auto renderer = platform.CreateRenderer(flt::RendererType::DX11);


	flt::Transform transform;
	bool isDraw = true;
	transform.SetPosition(0.0f, 0.0f, 0.7f);
	transform.SetScale(0.5f, 0.5f, 0.5f);
	flt::Renderable renderable(transform, isDraw);
	renderable.name = L"test";
	renderer->RegisterObject(renderable);

	//ASSERT(renderer, "렌더러 생성 실패");

	while (true)
	{
		if (!platform.Update())
		{
			break;
		}

		renderer->Render(1.0f);
		transform.AddRotation({ 1.0f, 0.0f, 0.0f }, 0.01f);
		transform.AddRotation({ 0.0f, 1.0f, 0.0f }, 0.01f);
		transform.AddRotation({ 0.0f, 0.0f, 1.0f }, 0.01f);

		//if (!renderer->Render(deltaTime))
		//{
		//	break;
		//}

		{
			auto keyData = platform.GetKey(flt::KeyCode::z);
			if (keyData)
			{
				std::cout << "z" << keyData.keyTime << std::endl;
			}
			keyData = platform.GetKey(flt::KeyCode::x);
			if (keyData)
			{
				std::cout << "x" << keyData.keyTime << std::endl;
			}
			keyData = platform.GetKey(flt::KeyCode::mouseLButton);
			if (keyData)
			{
				std::cout << "LL " << keyData.keyTime << " " << keyData.x << " " << keyData.y << std::endl;
			}
			keyData = platform.GetKey(flt::KeyCode::mouseRButton);
			if (keyData)
			{
				std::cout << "RR " << keyData.keyTime << std::endl;
			}

			keyData = platform.GetKey(flt::KeyCode::mouseRelativePos);
			if (keyData)
			{
				std::cout << "diff Pos " << keyData.x << " " << keyData.y << std::endl;
			}

			keyData = platform.GetKey(flt::KeyCode::mouseWheelUp);
			if (keyData)
			{
				std::cout << "wheel up " << keyData.x << std::endl;
			}

			keyData = platform.GetKey(flt::KeyCode::mouseWheelDown);
			if (keyData)
			{
				std::cout << "wheel down " << keyData.x << std::endl;
			}

			keyData = platform.GetKey(flt::KeyCode::mouseButton3);
			if (keyData)
			{
				std::cout << "button3 " << keyData.keyTime << std::endl;
			}

			keyData = platform.GetKey(flt::KeyCode::mouseButton4);
			if (keyData)
			{
				std::cout << "button4 " << keyData.keyTime << std::endl;
			}

			keyData = platform.GetKey(flt::KeyCode::mouseMButton);
			if (keyData)
			{
				std::cout << "WheelDown " << keyData.keyTime << std::endl;
			}

			keyData = platform.GetKey(flt::KeyCode::mouseAbsolutePos);
			if (keyData)
			{
				std::cout << "abs Pos " << keyData.x << " " << keyData.y << std::endl;
			}
		}


		//Sleep(10);
	}

	renderer->DeregisterObject(renderable);
	platform.Finalize();
}
