#include "GamePad.h"

#include <algorithm>
#include <cmath>


namespace PurahEngine
{
	void GamePad::Initialize(int _id,ePad* _inputArr, int _size)
	{
		id = _id;

		ZeroMemory(&state, sizeof(XINPUT_STATE));

		// 입력 맵 초기화
		for (auto i = 0; i < _size; i++)
		{
			inputMap.insert(std::make_pair(_inputArr[i], State::NONE));
		}
	}

	void GamePad::Update()
	{
		const auto newState = GetState();

		UpdateInputMap(newState);

	}

	void GamePad::UpdateInputMap(const XINPUT_STATE& _state)
	{
		for (auto& itr : inputMap)
		{
			const auto& input = itr.first;
			auto& currentState = itr.second;

			const WORD button = static_cast<WORD>(input);

			// 현재 상태 업데이트
			if (_state.Gamepad.wButtons & button && currentState == State::NONE)
			{
				currentState = State::DOWN;
			}
			else if (_state.Gamepad.wButtons & button && currentState == State::DOWN)
			{
				currentState = State::PRESSED;
			}
			else if (!(_state.Gamepad.wButtons & button) && currentState == State::PRESSED)
			{
				currentState = State::UP;
			}
			else if (!(_state.Gamepad.wButtons & button) && currentState == State::UP)
			{
				currentState = State::NONE;
			}
		}
	}

	XINPUT_STATE GamePad::GetState()
	{
		XInputGetState(id, &state);
		return state;
	}

	bool GamePad::IsKeyDown(ePad _input)
	{
		return inputMap[_input] == State::DOWN;
	}

	bool GamePad::IsKeyPressed(ePad _input)
	{
		return  inputMap[_input] == State::PRESSED;
	}

	bool GamePad::IsKeyUp(ePad _input)
	{
		return inputMap[_input] == State::UP;
	}

	int GamePad::GetTriggerValue(ePadTrigger _index) const
	{
		if (_index == ePadTrigger::ePAD_TRIGGER_L)
		{
			return state.Gamepad.bLeftTrigger;
		}
		else
		{
			return state.Gamepad.bRightTrigger;
		}
	}

	float GamePad::GetTriggerRatio(ePadTrigger _index) const
	{
		return inv255 * static_cast<float>(GetTriggerValue(_index));
	}

	void GamePad::GetStickValue(ePadStick _index, int& _outX, int& _outY) const
	{
		int xValue = 0, yValue = 0;

		// 좌측 스틱의 경우
		if (_index == ePadStick::ePAD_STICK_L)
		{
			xValue = state.Gamepad.sThumbLX;
			yValue = state.Gamepad.sThumbLY;
		}
		// 우측 스틱의 경우
		else if (_index == ePadStick::ePAD_STICK_R)
		{
			xValue = state.Gamepad.sThumbRX;
			yValue = state.Gamepad.sThumbRY;
		}

		// 데드존 체크 및 값 할당
		_outX = std::abs(xValue) < deadZone ? 0 : xValue;
		_outY = std::abs(yValue) < deadZone ? 0 : yValue;
	}

	void GamePad::GetStickRatio(ePadStick _index, float& _outX, float& _outY) const
	{
		// X, Y 값을 각각 변수로 설정
		int xValue = 0, yValue = 0;

		GetStickValue(_index, xValue, yValue);

		if (xValue != 0)
		{
			_outX = static_cast<float>(xValue) * inv32767;
		}
		if (yValue != 0)
		{
			_outY = static_cast<float>(yValue) * inv32767;
		}
	}

	void GamePad::Vibrate(int _left, int _right) const
	{
		XINPUT_VIBRATION vibration;
		ZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));
		vibration.wLeftMotorSpeed = static_cast<unsigned short>(_left);
		vibration.wRightMotorSpeed = static_cast<unsigned short>(_right);
		XInputSetState(id, &vibration);
	}

	void GamePad::VibrateRatio(float _left, float _right) const
	{
		_left = std::clamp(_left, 0.f, 1.f);
		_right = std::clamp(_right, 0.f, 1.f);

		const auto left = static_cast<int>(_left * static_cast<float>(USHORT_MAX));
		const auto right = static_cast<int>(_right * static_cast<float>(USHORT_MAX));

		Vibrate(left, right);
	}

	void GamePad::Vibrate(int _left, int _right, float _time) const
	{

		// Timer를 만들게 된다면 여기에.
		Vibrate(_left, _right);
	}

	void GamePad::VibrateRatio(float _left, float _right, float _time) const
	{

		// Timer를 만들게 된다면 여기에.
		VibrateRatio(_left, _right);
	}

	void GamePad::SetDeadZone(int _value)
	{
		deadZone = _value; // 데드존 값을 설정합니다.
	}

	void GamePad::SetDeadZoneRatio(float _ratio)
	{
		_ratio = std::clamp(_ratio, 0.f, 1.f);

		// 스틱 값의 범위: -32768 ~ 32767
		// 데드존 값을 비율로 정의합니다.
		// 예를 들어, 0.2의 경우 -32768 * 0.2 = -6553.6 이므로,
		// -6553.6에서 6553.6 사이의 값을 데드존으로 설정할 수 있습니다.
		deadZone = static_cast<int>(32767.0f * _ratio);
	}

	bool GamePad::IsConnected()
	{
		return XInputGetState(id, &state) == ERROR_SUCCESS;
	}
}
