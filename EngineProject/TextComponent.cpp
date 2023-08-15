#include <stdexcept>
#include "TextComponent.h"
#include "Renderer.h"
#include "Font.h"
#include "RenderComponent.h"
#include "GameObject.h"
#include <memory>

dae::TextComponent::TextComponent(dae::GameObject* owner, const std::string& text, std::shared_ptr<Font> font, SDL_Color color)
	: Component{ owner }
	, m_needsUpdate{ true }
	, m_text{ text }
	, m_font{ std::move(font) }
	, m_Color{ color }
{ 
	if (const auto renderComponent = owner->GetComponent<RenderComponent>())
	{
		m_pRenderComponent = renderComponent;
	}
	else
	{
		auto newRenderComponent{ std::make_unique<RenderComponent>(owner) };
		m_pRenderComponent = newRenderComponent.get();
		owner->AddComponent(std::move(newRenderComponent));
	}

	CreateTextTexture();
}

void dae::TextComponent::Update()
{
	if (m_needsUpdate)
	{
		CreateTextTexture();
		m_needsUpdate = false;
	}
}

// This implementation uses the "dirty flag" pattern
void dae::TextComponent::SetText(const std::string& text)
{
	m_text = text;
	m_needsUpdate = true;
}

void dae::TextComponent::SetColor(const SDL_Color& color)
{
	m_Color = color;
}

void dae::TextComponent::CreateTextTexture()
{
	const auto surf = TTF_RenderText_Blended(m_font->GetFont(), m_text.c_str(), m_Color);
	if (surf == nullptr)
	{
		throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
	}
	auto texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surf);
	if (texture == nullptr)
	{
		throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
	}
	SDL_FreeSurface(surf);

	if (m_pRenderComponent)
	{
		m_pRenderComponent->SetSDLTexture(texture);
	}
}

dae::TextureComponent* dae::TextComponent::GetTextureComponent() const
{
	return m_pRenderComponent->GetTextureComponent();
}