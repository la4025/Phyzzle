#include "ButtonGroup.h"

namespace PurahEngine
{
	void ButtonGroup::PreSerialize(json& jsonData) const
	{
	}
	void ButtonGroup::PreDeserialize(const json& jsonData)
	{
		PREDESERIALIZE_BASE();
	}
	void ButtonGroup::PostSerialize(json& jsonData) const
	{
	}
	void ButtonGroup::PostDeserialize(const json& jsonData)
	{
	}
}