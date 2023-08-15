#define IMGUI_DEFINE_MATH_OPERATORS
#pragma once
#include <SDL.h>
#include "Singleton.h"
#include "TextureComponent.h"

class TrashTheCache;

namespace dae
{
	/**
	 * Simple RAII wrapper for the SDL renderer
	 */
	class Renderer final : public Singleton<Renderer>
	{
		SDL_Renderer* m_renderer{};
		SDL_Window* m_window{};
		SDL_Color m_clearColor{};	
	public:
		void Init(SDL_Window* window);
		void Render();
		void Destroy();

		void RenderTexture(const TextureComponent& texture, float x, float y) const;
		void RenderTexture(const TextureComponent& texture, float x, float y, float width, float height) const;

		SDL_Renderer* GetSDLRenderer() const;

		const SDL_Color& GetBackgroundColor() const { return m_clearColor; }
		void SetBackgroundColor(const SDL_Color& color) { m_clearColor = color; }

	private:
		bool m_showDemo{ false };
	};
}

