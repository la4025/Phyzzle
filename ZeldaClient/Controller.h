#pragma once
#include "PurahEngine.h"

namespace PurahEngine
{
	class Controller : public Component
	{
	public:
		~Controller() override;

		void Awake() override;
		void Start() override;

		void Update() override;
		void FixedUpdate() override;
		void LateUpdate() override;

		void OnCollisionEnter() override;
		void OnCollisionStay() override;
		void OnCollisionExit() override;

		void OnTriggerEnter() override;
		void OnTriggerStay() override;
		void OnTriggerExit() override;

		void OnMouseEnter() override;
		void OnMouseStay() override;
		void OnMouseExit() override;
	};
}

