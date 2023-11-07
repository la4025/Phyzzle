#include "ModelingData.h"

ZNode::ZNode() :
	parent(nullptr)
{
}

ZNode::~ZNode()
{
	for (int i = 0; i < children.size(); i++)
	{
		delete children[i];
	}
}

ZMesh::ZMesh() :
	hasPosition(false),
	hasUV(false),
	hasNormal(false),
	material(nullptr)
{
}

ZMaterial::ZMaterial() :
	hasTexture(false)
{
}
