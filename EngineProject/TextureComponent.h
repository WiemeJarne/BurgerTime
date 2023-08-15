#pragma once
#include <glm/vec2.hpp>
#include "Component.h"
#include <string>

struct SDL_Texture;

namespace dae
{
	/**
 * Simple RAII wrapper for an SDL_Texture
 */
	class TextureComponent final : public Component
	{
	public:
		SDL_Texture* GetSDLTexture() const;
		TextureComponent(dae::GameObject* owner, const std::string& filename);
		TextureComponent(dae::GameObject* owner, SDL_Texture* texture);
		~TextureComponent();
		TextureComponent(const TextureComponent&) = delete;
		TextureComponent(TextureComponent&&) = delete;
		TextureComponent& operator= (const TextureComponent&) = delete;
		TextureComponent& operator= (const TextureComponent&&) = delete;

		virtual void Update() override {}
		virtual void Render() const override {}
		virtual void RenderImGui() override {}
		glm::ivec2 GetSize() const;
		const std::string& GetFileName() const { return m_Filename; }
		void SetSDLTexture(SDL_Texture* pSDLTexture);

	private:
		SDL_Texture* m_texture;
		const std::string m_Filename;
	};
}