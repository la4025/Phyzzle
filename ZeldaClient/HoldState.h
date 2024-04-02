#pragma once
#include "IState.h"

namespace Phyzzle
{
	class HoldState final : public IState
	{
	public:
		~HoldState() override;

	public:
		void Reset() override;
		void Play() override;

		void Button() override;
	};
}