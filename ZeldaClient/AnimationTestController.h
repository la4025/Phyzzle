#pragma once

#include "PurahEngine.h"

using namespace PurahEngine;

class AnimationTestController : public Component
{
public:
	void Awake() override;
	void Update() override;

private:
	float timer;
	Animator* animator;
};

