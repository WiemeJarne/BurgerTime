#include "RenderComponent.h"
#include "GameObject.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include <iostream>

dae::RenderComponent::RenderComponent(dae::GameObject* owner)
	: Component(owner)
{}

dae::RenderComponent::RenderComponent(dae::GameObject* owner, const std::string& textureFilename)
	: Component(owner)
{
	SetTextureComponent(textureFilename);
}

dae::RenderComponent::RenderComponent(dae::GameObject* owner, std::unique_ptr<dae::TextureComponent> textureComponent)
	: Component(owner)
{
	m_pTextureComponent = textureComponent.get();
	owner->AddComponent(std::move(textureComponent));
}

void dae::RenderComponent::Render() const
{
	auto pos = m_pOwner->GetWorldPos();

	if (m_pTextureComponent)
	{
		dae::Renderer::GetInstance().RenderTexture(*m_pTextureComponent, pos.x, pos.y);
	}
}

void dae::RenderComponent::SetTextureComponent(const std::string& filename)
{
	if (m_pTextureComponent)
	{
		m_pTextureComponent->SetSDLTexture(dae::ResourceManager::GetInstance().LoadTexture(filename));
		return;
	}

	auto pTextureComponent{ std::make_unique<dae::TextureComponent>(m_pOwner, filename) };
	m_pTextureComponent = pTextureComponent.get();
	m_pOwner->AddComponent(std::move(pTextureComponent));
}

void dae::RenderComponent::SetSDLTexture(SDL_Texture* pSDLTexture)
{
	if (!m_pTextureComponent)
	{
		auto pTextureComponent{ std::make_unique<dae::TextureComponent>(m_pOwner, pSDLTexture) };
		m_pTextureComponent = pTextureComponent.get();
		m_pOwner->AddComponent(std::move(pTextureComponent));
		return;
	}

	m_pTextureComponent->SetSDLTexture(pSDLTexture);
}