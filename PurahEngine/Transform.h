#pragma once
#include "Component.h"

namespace PurahEngine
{
	class Transform : public Component
	{
	public:
		float position[3];
		float rotation[4];       // 오일러? 쿼터니언?
		float scale[3];


	private:

	};
}
