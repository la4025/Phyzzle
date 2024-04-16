#include "Button.h"

#include "Image.h"

namespace PurahEngine
{
	void Button::Start()
	{
		selected = selectedWithStart;
	}

	void Button::Push()
	{

	}

	void Button::Select()
	{
		if (selected == false)
		{
			selected = true;
			if (targetImage != nullptr && selectedImage != L"")
			{
				targetImage->SetTexture(selectedImage);
			}
		}
	}

	void Button::Unselect()
	{
		if (selected == true)
		{
			selected = false;
			if (targetImage != nullptr && unselectedImage != L"")
			{
				targetImage->SetTexture(unselectedImage);
			}
		}
	}

	void Button::PushEvent()
	{

	}

	void Button::PreSerialize(json& jsonData) const
	{
	}

	void Button::PreDeserialize(const json& jsonData)
	{
		PREDESERIALIZE_BASE();
		PREDESERIALIZE_VALUE(selectedWithStart);
		PREDESERIALIZE_WSTRING(selectedImage);
		PREDESERIALIZE_WSTRING(unselectedImage);
		PREDESERIALIZE_WSTRING(pushedImage);
	}

	void Button::PostSerialize(json& jsonData) const
	{
	}

	void Button::PostDeserialize(const json& jsonData)
	{
		POSTDESERIALIZE_PTR(targetImage);
	}
}