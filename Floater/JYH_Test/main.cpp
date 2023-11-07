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
#pragma endregion


class TestResource
{
public:
	void Release()
	{
		std::cout << *data << " Release" << std::endl;
	}

	int* data;
};

template struct flt::Resource<TestResource>;

struct TestBuilder : public flt::IBuilder<TestResource>
{
	TestBuilder(const std::wstring& key) : flt::IBuilder<TestResource>(key) {}

	virtual TestResource* build() const override;
	int num = 0;
};

TestResource* TestBuilder::build() const
{
	auto resource = new TestResource();
	resource->data = new int(num);
	std::cout << *resource->data << " Build" << std::endl;
	return resource;
}

int main()
{
	setlocale(LC_ALL, ".UTF8");
	std::cout << std::boolalpha;



#pragma region 테스트
	{
		flt::ResourceMgr mgr;


		TestBuilder builder{ L"ONE" };
		builder.num = 1;

		TestBuilder builder2{ L"ONE" };
		builder2.num = 2;

		flt::Resource<TestResource> resource1{ mgr, builder };
		flt::Resource<TestResource>* pResource1 = new flt::Resource<TestResource>{ mgr, builder };
		flt::Resource<TestResource>* pResource2 = new flt::Resource<TestResource>{ mgr, builder };
		flt::Resource<TestResource>* pResource3 = new flt::Resource<TestResource>{ mgr, builder };
		flt::Resource<TestResource>* pResource4 = new flt::Resource<TestResource>{ mgr, builder };
		flt::Resource<TestResource> resource2{ mgr, builder2 };
		flt::Resource<TestResource> resource3{ mgr, builder };
		flt::Resource<TestResource> resource4{ mgr, builder };
	}
#pragma endregion


	flt::Platform platform;
	platform.Initialize(1280, 720, L"title", L".\\path");
	auto renderer = platform.CreateRenderer(flt::RendererType::DX11);


	flt::Transform transform;
	transform.SetPosition(0.0f, 0.0f, 0.7f);
	transform.SetScale(0.5f, 0.5f, 0.5f);
	flt::Renderable renderable(transform);
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
