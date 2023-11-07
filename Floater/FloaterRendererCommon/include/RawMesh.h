#pragma once
#include "../../FloaterMath/include/Vector2f.h"
#include "../../FloaterMath/include/Vector3f.h"
#include "../../FloaterMath/include/Vector4f.h"
#include <vector>

namespace flt
{
	struct Vertex
	{
		using type = struct
		{
			Vector3f Pos;
			Vector2f Tex;
			Vector3f Normal;
		};
	};

	struct RawMesh
	{
		std::vector<Vertex::type> vertices;
		std::vector<int> indices;
	};

}
