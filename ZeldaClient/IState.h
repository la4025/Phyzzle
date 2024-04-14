#pragma once

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
			player->pad.Lstick.Size = player->pad.gamePad->GetStickRatio(PurahEngine::ePadStick::ePAD_STICK_L, player->pad.Lstick.X, player->pad.Lstick.Y);
			player->pad.Rstick.Size = player->pad.gamePad->GetStickRatio(PurahEngine::ePadStick::ePAD_STICK_R, player->pad.Rstick.X, player->pad.Rstick.Y);

			player->pad.LTrigger = player->pad.gamePad->GetTriggerRatio(PurahEngine::ePadTrigger::ePAD_TRIGGER_L);
			player->pad.RTrigger = player->pad.gamePad->GetTriggerRatio(PurahEngine::ePadTrigger::ePAD_TRIGGER_R);

			player->pad.buttonA = player->pad.gamePad->IsKeyValue(PurahEngine::ePad::ePAD_A);
			player->pad.buttonB = player->pad.gamePad->IsKeyValue(PurahEngine::ePad::ePAD_B);
			player->pad.buttonX = player->pad.gamePad->IsKeyValue(PurahEngine::ePad::ePAD_X);
			player->pad.buttonY = player->pad.gamePad->IsKeyValue(PurahEngine::ePad::ePAD_Y);

			player->pad.buttonUP = player->pad.gamePad->IsKeyValue(PurahEngine::ePad::ePAD_UP);
			player->pad.buttonDOWN = player->pad.gamePad->IsKeyValue(PurahEngine::ePad::ePAD_DOWN);
			player->pad.buttonLEFT = player->pad.gamePad->IsKeyValue(PurahEngine::ePad::ePAD_LEFT);
			player->pad.buttonRIGHT = player->pad.gamePad->IsKeyValue(PurahEngine::ePad::ePAD_RIGHT);

			player->pad.buttonLB = player->pad.gamePad->IsKeyValue(PurahEngine::ePad::ePAD_SHOULDER_L);
			player->pad.buttonRB = player->pad.gamePad->IsKeyValue(PurahEngine::ePad::ePAD_SHOULDER_R);
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
