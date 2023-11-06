#pragma once
#include <string>
#include "../../FloaterUtil/include/ConvString.h"

namespace flt
{
	//class ResourceMgr;

	struct IBuilderBase
	{
		IBuilderBase() = default;
		IBuilderBase(const std::wstring& key) : key(key) {}
		virtual void* operator()(std::wstring* typeName) const = 0;
		std::wstring key;
	};

	template<typename Derived>
	struct IBuilder : public IBuilderBase
	{
		using type = Derived;

		IBuilder() = default;
		IBuilder(const std::wstring& key) : IBuilderBase(key) {}

		virtual void* operator()(std::wstring* typeName) const final
		{
			*typeName = convToWstring(typeid(Derived).name());

			return build();
		}

		virtual Derived* build() const = 0;
	};
}
