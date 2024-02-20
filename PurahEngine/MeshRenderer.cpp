#include "MeshRenderer.h"

#include "GameObject.h"
#include "Transform.h"

namespace PurahEngine
{
	MeshRenderer::MeshRenderer() :
		textureName(L""),
		meshType(MeshType::None),
		wireFrame(false),
		meshColor(White)
	{

	}

	void MeshRenderer::Render(IZeldaRenderer* renderer)
	{
		// 게임오브젝트가 활성화 되어 있는 경우에만 작동한다.
		if (GetGameObject()->IsRootEnable())
		{
			Eigen::Matrix4f worldTM = GetGameObject()->GetTransform()->GetWorldMatrix();
			TextureID textureID = TextureID::ID_NULL;
			if (textureName != L"")
			{
				textureID = GetTextureID(textureName);
			}
			
			switch (meshType)
			{
				case PurahEngine::MeshRenderer::MeshType::None:
				{
					// Please Select MeshType
					assert(0);
					break;
				}
				case PurahEngine::MeshRenderer::MeshType::Cube:
				{
					renderer->DrawCube(worldTM, textureID, wireFrame, meshColor.r, meshColor.g, meshColor.b, meshColor.a);

					break;
				}
				default:
				{
					assert(0);
					break;
				}
			}
		}
	}
	void MeshRenderer::SetTexture(const std::wstring& textureName)
	{
		this->textureName = textureName;
	}

	void MeshRenderer::SetMesh(MeshType type)
	{
		meshType = type;
	}

	void MeshRenderer::SetWireFrame(bool value)
	{
		wireFrame = value;
	}

	void MeshRenderer::SetColor(float r, float g, float b, float a)
	{
		meshColor.r = r;
		meshColor.r = g;
		meshColor.r = b;
		meshColor.r = a;
	}

	void MeshRenderer::SetColor(Color color)
	{
		meshColor = color;
	}
}