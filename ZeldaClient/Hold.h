#pragma once
#include "Component.h"
#include "IAbility.h"

namespace Phyzzle
{
	class Hold final : public IAbility, public PurahEngine::Component
	{
	public:
		~Hold() override;

	public:
		void Awake() override;
		void Start() override;
		void FixedUpdate() override;
		void Update() override;
	};
}