#pragma once
#include <map>
#include <string>

namespace FBXLoader
{
	struct Model;
}

namespace ZonaiPhysics
{
	class ResourceManager
	{
	private:
		static std::map<std::wstring, FBXLoader::Model*> 

	public:
		static FBXLoader::Model* LoadFBXFile(const std::wstring& _path);
		static void UnloadFBXFile(const std::wstring& _path);
	};
}
