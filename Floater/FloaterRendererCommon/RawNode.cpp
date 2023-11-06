#include "./include/RawNode.h"

flt::RawNode::RawNode(const std::wstring& name) :
	name(name),
	transform(),
	mesh(nullptr),
	camera(nullptr),
	boneIndex(-1),
	children()
{

}

