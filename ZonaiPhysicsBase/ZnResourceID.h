#pragma once
#include <Rpc.h>

namespace ZonaiPhysics
{
	enum class ResourceType
	{
		eNone,
		eMaterial,
		eMesh,
	};

	template <ResourceType _Ty>
	struct ZnResourceID
	{
		ZnResourceID() : 
			data1(), data2()
		{}
		ZnResourceID(size_t _data1, size_t _data2) : 
			data1(_data1), data2(_data2) 
		{}
		ZnResourceID(const ZnResourceID& _id) :
			data1(_id.data1), data2(_id.data2)
		{}
		ZnResourceID(ZnResourceID&& _id) noexcept :
			data1(_id.data1), data2(_id.data2)
		{}
		ZnResourceID& operator=(const ZnResourceID& _id)
		{
			if (this != &_id)
			{
				data1 = _id.data1;
				data2 = _id.data2;
			}

			return *this;
		}
		ZnResourceID& operator=(ZnResourceID&& _id) noexcept
		{
			if (this != &_id)
			{
				data1 = std::move(_id.data1);
				data2 = std::move(_id.data2);
			}

			return *this;
		}

		ResourceType type = _Ty;

		union 
		{
			GUID id;

			struct
			{
				size_t data1;
				size_t data2;
			};
		};

		static const ZnResourceID None;

		bool operator==(const ZnResourceID& _id) const
		{
			if (type != _id.type)
			{
				return type == _id.type;
			}
			else
			{
				return data1 == _id.data1 && data2 == _id.data2;
			}
		}
		bool operator!=(const ZnResourceID& _id) const
		{
			return !(*this == _id);
		}
		bool operator<(const ZnResourceID& _id) const
		{
			if (data1 < _id.data1)
			{
				return true;
			}
			else if (data1 == _id.data1)
			{
				if (data2 < _id.data2)
					return true;
			}

			return false;
		}
	};

	template <ResourceType _Ty>
	const ZnResourceID<_Ty> ZnResourceID<_Ty>::None{};

	template <ResourceType _Ty>
	void CreateID(ZnResourceID<_Ty>& _id)
	{
		UuidCreate(&_id.id);
	}

	using ZnMaterialID = ZnResourceID<ResourceType::eMaterial>;
	using ZnMeshID = ZnResourceID<ResourceType::eMesh>;
}


namespace std
{
	template<>
	struct hash<ZonaiPhysics::ZnMaterialID>
	{
		size_t operator()(const ZonaiPhysics::ZnMaterialID& r) const noexcept
		{
			std::size_t h1 = std::hash<std::size_t>{}(r.data1);
			std::size_t h2 = std::hash<std::size_t>{}(r.data2);
			return h1 ^ (h2 << 1);
		}
	};

	template<>
	struct hash<ZonaiPhysics::ZnMeshID>
	{
		size_t operator()(const ZonaiPhysics::ZnMeshID& r) const noexcept
		{
			std::size_t h1 = std::hash<std::size_t>{}(r.data1);
			std::size_t h2 = std::hash<std::size_t>{}(r.data2);
			return h1 ^ (h2 << 1);
		}
	};
}