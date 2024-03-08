#pragma once
#include <map>
#include <Xinput.h>
#include <windows.h>

#include "ePad.h"
#include "PurahEngineAPI.h"
#pragma comment(lib, "xinput.lib")

namespace PurahEngine
{
	class GamePadManager;

	constexpr short				SHORT_MAX = 32767;
	constexpr unsigned short	USHORT_MAX = 65535;
	constexpr float				inv32767 = 1.f / 32767.0f;
	constexpr float				inv255 = 1.f / 255.f;

	class PURAHENGINE_API GamePad
	{
	private:
		enum class State
		{
			NONE,
			DOWN,
			PRESSED,
			UP,
		};

		void Initialize(int _id, ePad* _inputArr, int _size);
		void Update();

		void UpdateInputMap(const XINPUT_STATE& _state);

		/// 게임패드
		XINPUT_STATE		GetState();

	public:
		friend GamePadManager;

		/// 키 입력
		bool				IsKeyDown(ePad _input);
		bool				IsKeyPressed(ePad _input);
		bool				IsKeyUp(ePad _input);

		/// 트리거 값
		int					GetTriggerValue(ePadTrigger _index) const;
		float				GetTriggerRatio(ePadTrigger _index) const;

		/// 스틱 값
		void				GetStickValue(ePadStick _index, int&, int&) const;
		void				GetStickRatio(ePadStick _index, float&, float&) const;

		/// 진동
		void				Vibrate(int, int) const;
		void				VibrateRatio(float, float) const;
		void				Vibrate(int, int, float _time) const;
		void				VibrateRatio(float, float, float _time) const;

		/// 데드존 설정
		void				SetDeadZone(int);
		void				SetDeadZoneRatio(float _ratio);

		/// 연결 여부
		bool				IsConnected();

	private:
		int				id;
		HWND			hWnd;
		DWORD			result;
		XINPUT_STATE	state;
		int				deadZone = 1000;
		std::map<ePad, State> inputMap;
	};
}
