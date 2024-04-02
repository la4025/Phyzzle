#pragma once

namespace Phyzzle
{
	class IState
	{
	public:
		virtual ~IState() = default;

	public:
		virtual void Reset() = 0;
		virtual void Play() = 0;

		virtual void Button() = 0;
	};
}
