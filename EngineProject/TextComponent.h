#pragma once
#include <string>
#include <memory>
#include <SDL_ttf.h>
#include "Component.h"

namespace dae
{
	class Font;
	class RenderComponent;
	class TextureComponent;

	class TextComponent final : public Component
	{
	public:
		TextComponent(dae::GameObject* owner, const std::string& text, std::shared_ptr<Font> font, SDL_Color color = { 255, 255, 255 });
		~TextComponent() = default;
		TextComponent(const TextComponent& other) = delete;
		TextComponent(TextComponent&& other) = delete;
		TextComponent& operator=(const TextComponent& other) = delete;
		TextComponent& operator=(TextComponent&& other) = delete;

		void Update() override;
		void Render() const override {}
		void RenderImGui() override {}

		std::string GetText() const { return m_text; }
		void SetText(const std::string& text);
		void SetColor(const SDL_Color& color);
		dae::TextureComponent* GetTextureComponent() const;

	private:
		bool m_needsUpdate;
		std::string m_text;
		std::shared_ptr<Font> m_font;
		RenderComponent* m_pRenderComponent;
		SDL_Color m_Color;

		void CreateTextTexture();
	};
}