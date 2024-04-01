#pragma once

#include "PurahEngine.h"

class LauncherPlatform : public PurahEngine::Component
{
public:


    void Awake();
    void Start();
    void Update();

private:
    void StateUpdate();
    void StateExecute();

private:
    Eigen::Vector3f moveDirection;
    float moveSpeed;

    float pushTime;
    float pullTime;
    float waitTime;

private:
    enum class State
    {
        Push,
        Pull,
        Wait
    };

    State state;

    float elapsedTime;

    PurahEngine::RigidBody* rigid = nullptr;

public:
	// Component을(를) 통해 상속됨
	void PreSerialize(json& jsonData) const override;
	void PreDeserialize(const json& jsonData) override;
	void PostSerialize(json& jsonData) const override;
	void PostDeserialize(const json& jsonData) override;
};

