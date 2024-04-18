#pragma once
#include "PurahEngine.h"
#include <vector>


namespace Phyzzle
{
	class Attachable : public PurahEngine::Component
	{
	public:
		~Attachable() override;

		void Selected();

	private:
		bool isKinematic = false;
		bool hasGravity = false;
		float originMass = -1.f;
	};
}

