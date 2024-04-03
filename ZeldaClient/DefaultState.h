#pragma once
#include "IState.h"

namespace Phyzzle
{
	class DefaultState final : public IState
	{
	public:
		~DefaultState() override;

	public:
		void Reset() override;
		void Play() override;
		void Button() override;
	};
}
