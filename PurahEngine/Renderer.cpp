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
<<<<<<< HEAD
	if (GetGameObject()->IsEnable())
	{

		Eigen::Matrix4f position = GetGameObject()->GetComponent<PurahEngine::Transform>()->GetWorldMatrix();

		renderer->DrawCube(position, texture, false, 0.1, 0.6, 0.6, 1);

	}
	if (light != LightID::ID_NULL)
	{
		renderer->DrawLight(light);
	}
=======
	const Eigen::Matrix4f position = GetGameObject()->GetComponent<PurahEngine::Transform>()->GetWorldMatrix();
>>>>>>> ZonaiPhysics

}

void PurahEngine::Renderer::AddTexture(TextureID textureID)
{
	texture = textureID;
}

void PurahEngine::Renderer::AddLight(LightID lightID)
{
	light = lightID;
}
