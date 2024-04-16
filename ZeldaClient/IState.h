#pragma once
#include "ePad.h"
#include "Player.h"

namespace Phyzzle
{
	class Player;

	class IState
	{
	public:
		IState() = delete;
		IState(Player* _player) : player(_player) {}
		virtual ~IState() = default;

	protected:
		Player* player;

	public:
		virtual void Input()
		{
			player->prevInput = player->currInput;

			player->currInput.Lstick.Size = player->gamePad->GetStickRatio(PurahEngine::ePadStick::ePAD_STICK_L, player->currInput.Lstick.X, player->currInput.Lstick.Y);
			player->currInput.Rstick.Size = player->gamePad->GetStickRatio(PurahEngine::ePadStick::ePAD_STICK_R, player->currInput.Rstick.X, player->currInput.Rstick.Y);

			player->currInput.LTrigger = player->gamePad->GetTriggerRatio(PurahEngine::ePadTrigger::ePAD_TRIGGER_L);
			player->currInput.RTrigger = player->gamePad->GetTriggerRatio(PurahEngine::ePadTrigger::ePAD_TRIGGER_R);

			player->currInput.buttonA = player->gamePad->IsKeyValue(PurahEngine::ePad::ePAD_A);
			player->currInput.buttonB = player->gamePad->IsKeyValue(PurahEngine::ePad::ePAD_B);
			player->currInput.buttonX = player->gamePad->IsKeyValue(PurahEngine::ePad::ePAD_X);
			player->currInput.buttonY = player->gamePad->IsKeyValue(PurahEngine::ePad::ePAD_Y);

			player->currInput.buttonUP = player->gamePad->IsKeyValue(PurahEngine::ePad::ePAD_UP);
			player->currInput.buttonDOWN = player->gamePad->IsKeyValue(PurahEngine::ePad::ePAD_DOWN);
			player->currInput.buttonLEFT = player->gamePad->IsKeyValue(PurahEngine::ePad::ePAD_LEFT);
			player->currInput.buttonRIGHT = player->gamePad->IsKeyValue(PurahEngine::ePad::ePAD_RIGHT);

			player->currInput.buttonLB = player->gamePad->IsKeyValue(PurahEngine::ePad::ePAD_SHOULDER_L);
			player->currInput.buttonRB = player->gamePad->IsKeyValue(PurahEngine::ePad::ePAD_SHOULDER_R);
		}

		virtual void operator()() {}
		virtual void StateEnter() {}
		virtual void StateExit() {}

	public:
		virtual void Stick_L() {}
		virtual void Stick_R() {}

		virtual void Trigger_L() {}
		virtual void Trigger_R() {}

		virtual void Click_DUp() {}
		virtual void Click_DDown() {}
		virtual void Click_DLeft() {}
		virtual void Click_DRight() {}

		virtual void Click_A() {}
		virtual void Click_B() {}
		virtual void Click_X() {}
		virtual void Click_Y() {}

		virtual void Click_LB() {}
		virtual void Click_RB() {}

	};
}
