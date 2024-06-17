#include "SpriteRenderer.h"

#include "GameObject.h"
#include "Transform.h"

namespace PurahEngine
{
	void SpriteRenderer::Render(IZeldaRenderer* renderer)
	{
		if (IsRootEnable())
		{
			TextureID textureID = TextureID::ID_NULL;
			if (textureName != L"")
			{
				textureID = GetTextureID(textureName);
				renderer->DrawSprite(GetGameObject()->GetTransform()->GetWorldMatrix(), textureID, false);
			}
		}
	}

	void SpriteRenderer::SetTexture(const std::wstring& fileName)
	{
		textureName = fileName;
	}

	void SpriteRenderer::PreSerialize(json& jsonData) const
	{
	}
	void SpriteRenderer::PreDeserialize(const json& jsonData)
	{
		PREDESERIALIZE_BASE();
		PREDESERIALIZE_WSTRING(textureName);
	}
	void SpriteRenderer::PostSerialize(json& jsonData) const
	{
	}
	void SpriteRenderer::PostDeserialize(const json& jsonData)
	{
	}
}