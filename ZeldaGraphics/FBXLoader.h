#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <string>

class ZeldaModel;

class FBXLoader
{
public:
	static ZeldaModel* LoadFBX(const std::wstring& filePath);

private:
	const static unsigned int DEFAULT_LOAD_FLAG;

};