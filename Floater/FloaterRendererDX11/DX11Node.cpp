#include "DX11Node.h"

flt::DX11Node::DX11Node(const Transform& transform, const bool& isDraw) : 
	transform(transform),
	isDraw(isDraw),
	mesh(nullptr),
	camera(nullptr),
	boneIndex(-1),
	children()
{

}
