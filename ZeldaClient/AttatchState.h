#pragma once
#include "IState.h"

namespace Phyzzle
{
	class AttatchState final : public IState
	{
	public:
		~AttatchState() override;

	public:
		void Reset() override;
		void Play() override;
		void Button() override;
	};
}

