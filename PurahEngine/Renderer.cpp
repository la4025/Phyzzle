#include "Renderer.h"
#include "GameObject.h"
#include "Transform.h"
#include "GraphicsSystem.h"

PurahEngine::Renderer::Renderer() : texture()
{
	GraphicsSystem::GetInstance().AddRenderer(this);
}

PurahEngine::Renderer::~Renderer()
= default;

void PurahEngine::Renderer::Render(IZeldaRenderer* renderer)
{
	const Eigen::Matrix4f position = GetGameObject()->GetComponent<PurahEngine::Transform>()->GetWorldMatrix();

	renderer->DrawCube(position, texture, false, 0, 1, 0, 1);
}

void PurahEngine::Renderer::AddTexture(TextureID textureID)
{
	texture = textureID;
}
