#include "CameraEventDevice.h"

#include "Player.h"

namespace Phyzzle
{
	void CameraEventDevice::Awake()
	{
		running = false;
		powerCounter = 0;
		originMainCamera = nullptr;
		workOnce = false;
		pauseLevel = 0;
	}

	void CameraEventDevice::Update()
	{
		float deltaTime = PurahEngine::TimeController::GetInstance().GetDeltaTime(pauseLevel);
		eventElapsed += deltaTime;

		bool loop = true;

		if (running)
		{
			while (loop)
			{
				float moveElapsed = moveSpeed * eventElapsed;

				if (eventLevel == 0)
				{
					if (startDelay < eventElapsed)
					{
						eventElapsed -= startDelay;
						eventLevel += 1;
					}
					else
					{
						loop = false;
					}
				}
				else if (eventLevel < cameraPath.size())
				{
					PurahEngine::Transform* trs1 = cameraPath[eventLevel - 1];
					PurahEngine::Transform* trs2 = cameraPath[eventLevel];
					Eigen::Matrix4f m1 = trs1->GetWorldMatrix();
					Eigen::Matrix4f m2 = trs2->GetWorldMatrix();

					float currentLength = PurahEngine::PurahMath::GetInstance().CalculateTransformPathLength(m1, m2, 30);
					
					if (currentLength < moveElapsed)
					{
						eventElapsed = (moveElapsed - currentLength) / moveSpeed;
						eventLevel += 1;
					}
					else
					{
						float t = moveElapsed / currentLength;
						t = std::clamp(t, 0.f, 1.f);

						Eigen::Matrix4f interpolatedmatrix = PurahEngine::PurahMath::GetInstance().InterpolateTransform(m1, m2, t);

						targetCamera->GetGameObject()->GetTransform()->SetWorldMatrix(interpolatedmatrix);
						loop = false;
					}
				}
				else if (eventLevel == cameraPath.size())
				{
					if (powerDelay < eventElapsed)
					{
						for (int i = 0; i < targetDeviceList.size(); i++)
						{
							targetDeviceList[i]->PowerOn();
						}

						eventElapsed -= powerDelay;
						eventLevel += 1;
						PurahEngine::TimeController::GetInstance().ResumeAll();
						pauseLevel = 0;
						player->SetStopUpdate(false);
					}
					else
					{
						loop = false;
					}
				}
				else if (eventLevel == cameraPath.size() + 1)
				{
					if (delay < eventElapsed)
					{
						originMainCamera->SetMainCamera();
						running = false;
						eventLevel += 1;

						if (powerCounter < 1)
						{
							for (int i = 0; i < targetDeviceList.size(); i++)
							{
								targetDeviceList[i]->PowerOff();
							}
						}
					}
					else
					{
						loop = false;
					}
				}
				else
				{
					loop = false;
				}
			}
		}
	}

	void CameraEventDevice::PowerOn()
	{
		powerCounter += 1;

		if (powerCounter >= 1)
		{
			Run();
		}
	}
	void CameraEventDevice::PowerOff()
	{
		powerCounter -= 1;

		if (powerCounter < 1 && running == false)
		{
			for (int i = 0; i < targetDeviceList.size(); i++)
			{
				targetDeviceList[i]->PowerOff();
			}
		}
	}

	void CameraEventDevice::Run()
	{
		if (running)
		{
			return;
		}

		if ((worksOnlyOnce && workOnce) == false)
		{
			running = true;
			pauseLevel = PurahEngine::TimeController::GetInstance().PauseAll();
			player->SetStopUpdate(true);
			eventElapsed = 0.0f;
			eventLevel = 0;
			workOnce = true;

			if (cameraPath.size() != 0)
			{
				targetCamera->GetGameObject()->GetTransform()->SetWorldMatrix(cameraPath[0]->GetWorldMatrix());
			}

			// 메인 카메라 교체
			originMainCamera = PurahEngine::SceneManager::GetInstance().GetMainCamera();
			targetCamera->SetMainCamera();
		}
		else
		{
			for (int i = 0; i < targetDeviceList.size(); i++)
			{
				targetDeviceList[i]->PowerOn();
			}
		}
	}

	void CameraEventDevice::PreSerialize(json& jsonData) const
	{
	}

	void CameraEventDevice::PreDeserialize(const json& jsonData)
	{
		PREDESERIALIZE_BASE();
		PREDESERIALIZE_VALUE(worksOnlyOnce);
		PREDESERIALIZE_VALUE(moveSpeed);
		PREDESERIALIZE_VALUE(startDelay);
		PREDESERIALIZE_VALUE(powerDelay);
		PREDESERIALIZE_VALUE(delay);
	}

	void CameraEventDevice::PostSerialize(json& jsonData) const
	{
	}

	void CameraEventDevice::PostDeserialize(const json& jsonData)
	{
		POSTDESERIALIZE_PTR(targetCamera);
		POSTDESERIALIZE_VECTOR_PTR(cameraPath);
		POSTDESERIALIZE_VECTOR_PTR(targetDeviceList);
		POSTDESERIALIZE_PTR(player);
	}
}