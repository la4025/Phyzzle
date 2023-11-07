#pragma once
#include <string>
#include <iostream>
#include "ResourceMgr.h"

namespace flt
{
	struct IBuilderBase;
	template<typename T> struct IBuilder;


	struct ResourceBase
	{
		friend class ResourceMgr;

		ResourceBase(ResourceMgr* pResourceMgr, const std::wstring& key)	:
			_pResourceMgr(pResourceMgr),
			_key(key),
			_pData(nullptr)
		{

		}
		ResourceBase(const ResourceBase& other) = delete;
		ResourceBase(ResourceBase&& other) noexcept : 
			_pResourceMgr(other._pResourceMgr), 
			_pData(other._pData), 
			_key(other._key)
		{
			other._pData = nullptr;
		}

		ResourceBase& operator=(const ResourceBase& other) = delete;
		ResourceBase& operator=(ResourceBase&& other) noexcept
		{
			_pResourceMgr = other._pResourceMgr;
			_pData = other._pData;
			_key = other._key;
			other._pData = nullptr;
			return *this;
		}

		virtual ~ResourceBase()
		{
		}

	protected:
		std::wstring _key;
		void* _pData;
		ResourceMgr* _pResourceMgr;
	};

	template<typename Derived>
	struct Resource : ResourceBase
	{
	public:
		Resource() : ResourceBase(nullptr, L"") {}
		Resource(ResourceMgr& resourceMgr, const typename IBuilder<Derived>& builder) : ResourceBase(&resourceMgr, builder.key)
		{
			SetData(builder);
		}
		Resource(const Resource& other) = delete;
		Resource(Resource&& other) = default;

		Resource& operator=(const Resource& other) = delete;
		Resource& operator=(Resource&& other) noexcept
		{
			Release();
			return *this;
		}

		virtual ~Resource()
		{
			Release();
		}

		void SetData(const IBuilderBase& builder)
		{
			auto data = _pResourceMgr->GetResource(this, builder);
			Release();
			_pData = data;
		}

		void Release()
		{
			if (_pData)
			{
				if (_pResourceMgr->ReleaseResource(this))
				{
					((Derived*)_pData)->Release();
				}
				_pData = nullptr;
			}
		}

		operator Derived* () const
		{
			return (Derived*)_pData;
		}
	};
}
