#define _CRT_SECURE_NO_WARNINGS
#include "./include/convString.h"
#include <atlstr.h>


std::wstring convToWstring(const std::string& str)
{
	CA2W wcstr = CA2W(str.c_str());
	std::wstring wstr = static_cast<std::wstring>(wcstr);
	return wstr;
}
