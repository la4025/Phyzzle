
#include <ctype.h>
#include <windows.h>

#include "Eigen/Dense"
#include "ZnCollider.h"
#include "ZnPhysicsBase.h"
#include "ZnRigidBody.h"


#include "ZnDistanceJoint.h"
#include "ZnFixedJoint.h"
#include "ZnHingeJoint.h"
#include "ZnJoint.h"
#include "ZnPrismaticJoint.h"
#include "ZnSphericalJoint.h"
#include "ZnTransform.h"
#include "../ZonaiMath/ZonaiMath.h"


#include <chrono>
#include <iostream>

#include "ZnCollision.h"
#include "ZnRaycastInfo.h"
#include "ZnSimulationCallback.h"

class Timer {
private:
	std::chrono::steady_clock::time_point lastTime;

public:
	Timer() {
		lastTime = std::chrono::steady_clock::now();
	}

	// 반환값: 초 단위의 델타 타임
	double GetDelta() {
		auto currentTime = std::chrono::steady_clock::now();
		std::chrono::duration<double> delta = currentTime - lastTime;
		lastTime = currentTime;
		return delta.count();
	}
};

class PhysicsEvent : public ZonaiPhysics::ZnSimulationCallback
{
public:
	void OnWake(const ZonaiPhysics::ZnRigidBody*) noexcept override
	{
		std::cout << "OnWake" << std::endl;
	}
	void OnSleep(const ZonaiPhysics::ZnRigidBody*) noexcept override
	{
		std::cout << "OnSleep" << std::endl;
	}
	void OnConstraintBreak(const ZonaiPhysics::ZnJoint*) noexcept override
	{
		std::cout << "OnConstraintBreak" << std::endl;
	}
	void OnTriggerEnter(const ZonaiPhysics::ZnCollider*, const ZonaiPhysics::ZnCollider*) noexcept override
	{
		std::cout << "OnTriggerEnter" << std::endl;
	}
	void OnTriggerStay(const ZonaiPhysics::ZnCollider*, const ZonaiPhysics::ZnCollider*) noexcept override
	{
		std::cout << "OnTriggerStay" << std::endl;
	}
	void OnTriggerExit(const ZonaiPhysics::ZnCollider*, const ZonaiPhysics::ZnCollider*) noexcept override
	{
		std::cout << "OnTriggerExit" << std::endl;
	}
	void OnCollisionEnter(const ZonaiPhysics::ZnCollider*, const ZonaiPhysics::ZnCollider*,
		const ZonaiPhysics::ZnCollision& _collision) noexcept override
	{
		std::cout << "OnCollisionEnter " << _collision.contactCount << std::endl;
	}
	void OnCollisionStay(const ZonaiPhysics::ZnCollider*, const ZonaiPhysics::ZnCollider*,
		const ZonaiPhysics::ZnCollision& _collision) noexcept override
	{
		std::cout << "OnCollisionStay " << _collision.contactCount << std::endl;
	}
	void OnCollisionExit(const ZonaiPhysics::ZnCollider*, const ZonaiPhysics::ZnCollider*,
		const ZonaiPhysics::ZnCollision& _collision) noexcept override
	{
		std::cout << "OnCollisionExit " << _collision.contactCount << std::endl;
	}
};

int snippetMain(int, const char* const*)
{
	std::wstring path = L"../x64/Debug/ZonaiPhysicsX.dll";

	HMODULE physicsDLL = LoadLibraryW(path.c_str());
	if (!physicsDLL)
	{
		// MessageBox(_hwnd, L"해당 경로에 Physics DLL 파일이 존재하지 않습니다.", L"DLL 오류", MB_OK | MB_ICONWARNING);
		return false;
	}

	using ImportFunction = ZonaiPhysics::ZnPhysicsBase* (*) ();
	ImportFunction CreateInstance{ reinterpret_cast<ImportFunction>(GetProcAddress(physicsDLL, "CreatePhysics")) };

	if (CreateInstance == nullptr)
	{
		// MessageBox(_hwnd, L"Physics DLL에서 함수 포인터를 받아오지 못했습니다.", L"DLL 오류", MB_OK | MB_ICONWARNING);
		return false;
	}

	ZonaiPhysics::ZnPhysicsBase* physicsEngine = CreateInstance();

	if (physicsEngine == nullptr)
	{
		// MessageBox(_hwnd, L"Graphics Engine 객체 생성 실패", L"DLL 오류", NULL);
		return false;
	}

	PhysicsEvent event;

	physicsEngine->Initialize(&event);

	auto collider = physicsEngine->CreateBoxCollider(L"rigidBody", 2.f, 0.5f, 6.f);
	const auto rigid = physicsEngine->CreateRigidBody(L"rigidBody");
	rigid->SetMaxLinearVelocity(100.f);
	rigid->SetPosition({0.f, 15.f, 0.f});
	rigid->UseGravity(false);

	auto collider2 = physicsEngine->CreateSphereCollider(L"rigidBody2", 2.f);
	collider2->SetPosition({ 0.f, 3.f, 0.f });
	const auto rigid2 = physicsEngine->CreateRigidBody(L"rigidBody2");
	collider2->SetTrigger(true);

	auto groundCollider = physicsEngine->CreateBoxCollider(L"ground", 1000, 1, 1000);
	const auto ground = physicsEngine->CreateRigidBody(L"ground");
	ground->SetPosition({ 0, 0, 0 });
	ground->SetKinematic(true);

	Timer timer;

	while (true)
	{
		float dt = timer.GetDelta();

		physicsEngine->Simulation(1.f/5000.f);

		//if (GetAsyncKeyState(VK_RIGHT))
		//{
		//	rigid4->AddForce({10.f, 0.f, 0.f});
		//}
		//if (GetAsyncKeyState(VK_LEFT))
		//{
		//	rigid4->AddForce({ -10.f, 0.f, 0.f });
		//}
		//if (GetAsyncKeyState(VK_DOWN))
		//{
		//	rigid4->AddForce({0.f, 0.f, 10.f});
		//}
		//if (GetAsyncKeyState(VK_UP))
		//{
		//	rigid4->AddForce({ 0.f, 10.f, 10.f });
		//}

		bool clickFront = GetAsyncKeyState('I');
		bool clickBack = GetAsyncKeyState('K');
		bool clickLeft = GetAsyncKeyState('J');
		bool clickRight = GetAsyncKeyState('L');
		bool clickUp = GetAsyncKeyState('U');
		bool clickDown = GetAsyncKeyState('O');

		bool KeySpace = GetAsyncKeyState(VK_SPACE);

		// if (KeySpace)
		// {
 		//	ZonaiPhysics::ZnRaycastInfo info;
			//Eigen::Vector3f pos = ground->GetPosition() + Eigen::Vector3f{ 0.f, 1.1f, 0.f };
			//if (physicsEngine->Raycast(pos, Eigen::Vector3f{0.f, 1.f, 0.f}.normalized(), 100.f, info))
			//{
			//	std::cout << "O : " << info.distance << std::endl;
			//}
			//else
			//{
			//	std::cout << "X" << std::endl;
			//}
		// }

		if (clickUp || clickRight || clickLeft || clickDown || clickFront || clickBack)
		{
			Eigen::Vector3f nowpos = rigid->GetPosition();
			Eigen::Vector3f movement{ 0.f, 0.f, 0.f };

			if (clickFront)
			{
				movement += Eigen::Vector3f{ 0.f, 0.f, -0.01f };
			}
			if (clickBack)
			{
				movement += Eigen::Vector3f{ 0.f, 0.f, 0.01f };
			}
			if (clickUp)
			{
				movement += Eigen::Vector3f{ 0.f, 0.01f, 0.f };
			}
			if (clickDown)
			{
				movement += Eigen::Vector3f{ 0.f, -0.01f, 0.f };
			}
			if (clickLeft)
			{
				movement += Eigen::Vector3f{ -0.01f, 0.f, 0.f };
			}
			if (clickRight)
			{
				movement += Eigen::Vector3f{ 0.01f, 0.f, 0.f };
			}

			rigid->SetPosition(nowpos + movement);
		}
	}

	return 0;
}
