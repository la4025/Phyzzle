#include "Button.h"

#include "Door.h"

namespace Phyzzle
{
	void Button::Awake()
	{		
		originPos = pushableObject->GetTransform()->GetLocalPosition();

		powerElapsed = 0.0f;
		pushElapsed = 0.0f;

		isLastButtonDown = false;
		isPowerOn = false;
		isButtonDown = false;

		pushableRigidbody = pushableObject->GetComponent<PurahEngine::RigidBody>();
	}

	void Button::Update()
	{
		float deltaTime = PurahEngine::TimeController::GetInstance().GetDeltaTime();

		// Update isButtonDown
		if ((originPos.y() - pushableObject->GetTransform()->GetLocalPosition().y()) >= activationDepth)
		{
			isButtonDown = true;
		}
		else if ((originPos.y() - pushableObject->GetTransform()->GetLocalPosition().y()) <= deactivationDepth)
		{
			isButtonDown = false;
		}

		switch (buttonType)
		{
			case Phyzzle::Button::ButtonType::Toggle:
			{
				if (isButtonDown)
				{
					pushElapsed += deltaTime;
					if (pushElapsed >= executeDelay)
					{
						TargetPowerOn();
						FastenButton();
					}
				}
				else
				{
					pushElapsed = 0.0f;
				}
				break;
			}
			case Phyzzle::Button::ButtonType::Clickable:
			{
				if (isButtonDown)
				{
					pushElapsed += deltaTime;
					if (pushElapsed >= executeDelay)
					{
						TargetPowerOn();
					}
				}
				else
				{
					TargetPowerOff();
					pushElapsed = 0.0f;
				}
				break;
			}
			case Phyzzle::Button::ButtonType::Timer:
			{
				TimerButtonUpdate();

				break;
			}
			default:
			{
				assert(0);
				break;
			}
		}
	}

	void Button::FixedUpdate()
	{
		// Pushable Object의 속도 고정
		pushableRigidbody->SetLinearVelocity({ 0.0f, releasedSpeed, 0.0f });
	}

	void Button::TargetPowerOn()
	{
		if (isPowerOn == true)
		{
			return;
		}

		isPowerOn = true;
		powerElapsed = 0.0f;

		for (int i = 0; i < targetDeviceList.size(); i++)
		{
			targetDeviceList[i]->PowerOn();
		}
	}

	void Button::TargetPowerOff()
	{
		if (isPowerOn == false)
		{
			return;
		}

		isPowerOn = false;

		for (int i = 0; i < targetDeviceList.size(); i++)
		{
			targetDeviceList[i]->PowerOff();
		}
	}

	void Button::FastenButton()
	{
		pushableRigidbody->SetKinematic(true);
	}

	void Button::TimerButtonUpdate()
	{
		float deltaTime = PurahEngine::TimeController::GetInstance().GetDeltaTime();

		switch (timerType)
		{
			case Phyzzle::Button::TimerType::Hold:
			{
				if (isButtonDown)
				{
					pushElapsed += deltaTime;
					if (pushElapsed >= time + executeDelay)
					{
						TargetPowerOn();
					}
				}
				else
				{
					pushElapsed = 0.0f;
				}

				break;
			}
			case Phyzzle::Button::TimerType::Push:
			{
				if (isPowerOn)
				{
					powerElapsed += deltaTime;

					if (powerElapsed >= time)
					{
						TargetPowerOff();
						pushElapsed = 0.0f;
						powerElapsed = 0.0f;
					}
				}
				
				if (isButtonDown)
				{
					pushElapsed += deltaTime;
					if (pushElapsed >= executeDelay)
					{
						TargetPowerOn();
						powerElapsed = 0.0f;
					}
				}
				else
				{
					pushElapsed = 0.0f;
				}

				break;
			}
			case Phyzzle::Button::TimerType::Release:
			{
				if (isPowerOn)
				{
					powerElapsed += deltaTime;

					if (powerElapsed >= time)
					{
						TargetPowerOff();
						pushElapsed = 0.0f;
						powerElapsed = 0.0f;
					}
				}

				if (isButtonDown)
				{
					pushElapsed += deltaTime;
				}
				else
				{
					if (pushElapsed >= executeDelay)
					{
						TargetPowerOn();
						powerElapsed = 0.0f;
					}
					pushElapsed = 0.0f;
				}

				break;
			}
			default:
			{
				assert(0);
				break;
			}
		}
	}
	
	void Button::PreSerialize(json& jsonData) const
	{
	}
	void Button::PreDeserialize(const json& jsonData)
	{
		PREDESERIALIZE_BASE();
		int buttonTypeInt = jsonData["buttonType"];
		buttonType = static_cast<ButtonType>(buttonTypeInt);
		int timerTypeInt = jsonData["timerType"];
		timerType = static_cast<TimerType>(timerTypeInt);

		PREDESERIALIZE_VALUE(time);
		PREDESERIALIZE_VALUE(releasedSpeed);
		PREDESERIALIZE_VALUE(activationDepth);
		PREDESERIALIZE_VALUE(deactivationDepth);
		PREDESERIALIZE_VALUE(executeDelay);
	}
	void Button::PostSerialize(json& jsonData) const
	{
	}
	void Button::PostDeserialize(const json& jsonData)
	{
		POSTDESERIALIZE_PTR(pushableObject);
		POSTDESERIALIZE_VECTOR_PTR(targetDeviceList);
	}
}