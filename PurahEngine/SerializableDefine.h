#pragma once
#include "Serializable.h"
#include "FileManager.h"

#define PREDESERIALIZE_BASE() \
{ \
	PurahEngine::FileManager::GetInstance().SetAddress(jsonData["__Base__instanceID"], this);\
}

#define PREDESERIALIZE_VALUE(value)\
{\
	value = jsonData[#value];\
}

#define POSTDESERIALIZE_PTR(value)\
{\
	value = static_cast<decltype(value)>(PurahEngine::FileManager::GetInstance().GetAddress(jsonData[std::string("__ID__") + #value]));\
}

#define PREDESERIALIZE_VECTOR3F(value)\
{\
	value = Eigen::Vector3f(jsonData[#value]["x"], jsonData[#value]["y"], jsonData[#value]["z"]); \
}

#define PREDESERIALIZE_WSTRING(value)\
{\
	std::string text = jsonData[#value];\
	value = std::wstring(text.begin(), text.end());\
}

#define PREDESERIALIZE_QUATERNIONF(value)\
{\
	value = Eigen::Quaternionf(jsonData[#value]["w"], jsonData[#value]["x"], jsonData[#value]["y"], jsonData[#value]["z"]); \
}