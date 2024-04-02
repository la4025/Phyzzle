#pragma once
#include "IState.h"

namespace Phyzzle
{
	class Hold final : public IState
	{
	public:
		~Hold() override;

	public:
		void Reset() override;
		void Play() override;
		void Button() override;
	};
}