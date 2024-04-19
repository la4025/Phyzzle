#include "Image.h"

#include "GameObject.h"
#include "UI.h"

#include "UnifiedInputManager.h"
#include "GraphicsManager.h"

namespace PurahEngine
{
	void Image::Awake()
	{
		ui = GetGameObject()->GetComponent<UI>();
	}

	void Image::Render(IZeldaRenderer* renderer)
	{
		Eigen::Vector2f position = ui->GetScreenPosition();
		Eigen::Vector2f size = ui->GetSize();

		TextureID textureID = TextureID::ID_NULL;
		if (textureName != L"")
		{
			textureID = GetTextureID(textureName);
			renderer->DrawSprite(position, size, textureID, layer);
		}
	}

	void Image::SetTexture(const std::wstring& fileName)
	{
		textureName = fileName;
	}

	void Image::PreSerialize(json& jsonData) const
	{
	}

	void Image::PreDeserialize(const json& jsonData)
	{
		PREDESERIALIZE_BASE();
		PREDESERIALIZE_WSTRING(textureName);
		PREDESERIALIZE_VALUE(layer);
	}

	void Image::PostSerialize(json& jsonData) const
	{
	}

	void Image::PostDeserialize(const json& jsonData)
	{
	}
}
