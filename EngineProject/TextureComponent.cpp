#include <SDL.h>
#include "TextureComponent.h"
#include "ResourceManager.h"
#include <iostream>

dae::TextureComponent::TextureComponent(dae::GameObject* owner, const std::string& filename)
	: Component(owner)
	,  m_texture{ dae::ResourceManager::GetInstance().LoadTexture(filename) }
	, m_Filename{ filename }
{ }

dae::TextureComponent::TextureComponent(dae::GameObject* owner, SDL_Texture* texture)
	: Component(owner)
	, m_Filename{ "" }
{
	m_texture = texture;
}

dae::TextureComponent::~TextureComponent()
{
	if(m_texture)
		SDL_DestroyTexture(m_texture);
}

glm::ivec2 dae::TextureComponent::GetSize() const
{
	SDL_Rect dst;
	SDL_QueryTexture(GetSDLTexture(), nullptr, nullptr, &dst.w, &dst.h);
	return { dst.w,dst.h };
}

SDL_Texture* dae::TextureComponent::GetSDLTexture() const
{
	return m_texture;
}

void dae::TextureComponent::SetSDLTexture(SDL_Texture* pSDLTexture)
{
	if(m_texture)
		SDL_DestroyTexture(m_texture);

	m_texture = pSDLTexture;
}