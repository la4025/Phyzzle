#pragma once
#include <cstdint>
#include <cassert>
#include <map>
#include <bitset>
#include <initializer_list>

namespace ZonaiPhysics
{
	class ZnLayer
	{
	public:
		static constexpr uint32_t MIN_LAYER = 0;
		static constexpr uint32_t MAX_LAYER = 31;

		static void Clear()
		{
			layerData.clear();
		}

		static void SetCollisionData(uint32_t _layerID, const std::initializer_list<uint32_t>& _collisionLayers)
		{
			ValidateLayer(_layerID);

			for (const auto layer : _collisionLayers)
			{
				ValidateLayer(layer);

				layerData[_layerID].set(layer);
				layerData[layer].set(_layerID);
			}
		}

		static void SetCollisionLayer(uint32_t _layerID, uint32_t _collisionLayerID, bool _value)
		{
			ValidateLayer(_layerID);
			ValidateLayer(_collisionLayerID);

			_value ? layerData[_layerID].set(_collisionLayerID) : layerData[_layerID].reset(_collisionLayerID);
			_value ? layerData[_collisionLayerID].set(_layerID) : layerData[_collisionLayerID].reset(_layerID);
		}

		static bool CanCollide(uint32_t _layerID0, uint32_t _layerID1)
		{
			ValidateLayer(_layerID0);
			ValidateLayer(_layerID1);

			// 해당 비트를 확인해보고 1이면 true를 반환함
			return layerData[_layerID0].test(_layerID1);
		}

	private:
		static std::map<uint32_t, std::bitset<MAX_LAYER + 1>> layerData;

		static void ValidateLayer(uint32_t _layerID)
		{
			assert(MIN_LAYER <= _layerID && _layerID <= MAX_LAYER && "Invalid layer ID");
		}
	};
}
