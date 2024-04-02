#pragma once
#include "IState.h"

namespace Phyzzle
{
	class RewindState final : public IState
	{
	public:
		~RewindState() override;

		void Reset() override;
		void Play() override;
		void Button() override;
	};
}