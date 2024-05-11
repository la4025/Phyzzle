#include "CustomCollider.h"


namespace PurahEngine
{
	CustomCollider::~CustomCollider()
	{
	}

	void CustomCollider::PreDeserialize(const json& jsonData)
	{
		Collider::PreDeserialize(jsonData);

		PREDESERIALIZE_WSTRING(modelName);
	}
}