#pragma once


namespace ZonaiPhysics
{
	struct ZnPoint
	{
		union
		{
			float x;
			float y;
			float z;
		};
	};
}
