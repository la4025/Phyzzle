#pragma once

#include <d3d11.h>
#include <DirectXMath.h>

#include <vector>
#include <string>

class ZeldaTexture;

class ZeldaModel
{
public:
	struct Node
	{
		
		std::vector<Node*> children;
	};

	ZeldaModel();
	ZeldaModel(const ZeldaModel& zeldaModel) = delete;

	~ZeldaModel();
};

