#define FLT_WINDOWS
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include "../FloaterPlatform/include/Platform.h"
#include "../FloaterUtil/include/Timer.h"
#include "../FloaterUtil/include/FloaterMacro.h"
#include "../FloaterRendererCommon/include/Transform.h"

#include "../FloaterUtil/include/ConvString.h"

#pragma region testInclude
#include "../FloaterRendererCommon/include/IBuilder.h"
#include "../FloaterRendererCommon/include/Resource.h"
#include "../FloaterRendererCommon/include/ResourceMgr.h"
#include "../FloaterUtil/TesterRBTree.h"

#include "../FloaterUtil/TesterFixedSizeMemoryPool.h"
#include "../FloaterUtil/include/Hash.h"

#include <chrono>
#pragma endregion

void StackOverflow()
{
	int i = 10;
	StackOverflow();
	std::cout << i << std::endl;
}

int main()
{
	setlocale(LC_ALL, ".UTF8");
	std::cout << std::boolalpha;

#pragma region 테스트
	{
		using namespace flt::test;

		flt::Timer timer;

		flt::wyhash hash1;
		flt::wyhash hash2(0);
		flt::wyhash::State state;
		hash2.GetState(&state);
		flt::wyhash hash3(state);

		std::string str1 = "test";
		uint64 i64_1 = 0x90237840;

		timer.Start();
		constexpr int loopCount = 10000000;
		for (int i = 0; i < loopCount; ++i)
		{
			//uint64 hash1_2 = hash2(str1.c_str(), str1.size());
			uint64 hash1_2 = hash2(&i64_1, sizeof(i64_1));
			uint64 cmp = hash3(&i64_1, sizeof(i64_1));

			if(hash1_2 != cmp)
			{
				ASSERT(false, "hash fail");
			}
		}
		auto time = timer.GetLabTimeSeconds();
		std::cout << "loopCount " << loopCount << ", time : " << time << std::endl;

#pragma omp parallel for
		for (int i = 0; i < 1; ++i)
		{
			TesterRBTree tester{ };
			if (!tester.Test())
			{
				ASSERT(false, "test fail");
			}
		}

		TesterFixedSizeMemoryPool testerFixedSizeMemoryPool;
		if (!testerFixedSizeMemoryPool.Test())
		{
			ASSERT(false, "test fail");
		}
	}
#pragma endregion


	flt::Platform platform;
	platform.Initialize(1280, 720, L"title", L".\\path");

	//StackOverflow();
	//int* i = nullptr;
	//*i = 10;

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
