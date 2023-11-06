#include "DX11Node.h"

flt::DX11Node::DX11Node(Transform& transform) : 
	transform(transform),
	mesh(nullptr),
	camera(nullptr),
	boneIndex(-1),
	children()
{

}
