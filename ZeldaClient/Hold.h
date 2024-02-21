#pragma once
#include "Component.h"
#include "IAbility.h"

class Hold final : public IAbility, public PurahEngine::Component
{
public:
	~Hold() override;

public:
	void Awake() override;
	void Start() override;
	void FixedUpdate() override;
	void Update() override;
	void OnCollisionEnter() override;
	void OnCollisionStay() override;
	void OnCollisionExit() override;
	void OnTriggerEnter() override;
	void OnTriggerStay() override;
	void OnTriggerExit() override;
};

