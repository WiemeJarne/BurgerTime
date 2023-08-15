#pragma once
#include <memory>
#include <string>
#include "Component.h"
#include "TextureComponent.h"

namespace dae
{
	class RenderComponent final : public Component
	{
	public:
		RenderComponent(dae::GameObject* owner);
		RenderComponent(dae::GameObject* owner, const std::string& textureFilename);
		RenderComponent(dae::GameObject* owner, std::unique_ptr<dae::TextureComponent> textureComponent);

		~RenderComponent() = default;
		RenderComponent(const RenderComponent& other) = delete;
		RenderComponent(RenderComponent&& other) = delete;
		RenderComponent& operator=(const RenderComponent& other) = delete;
		RenderComponent& operator=(RenderComponent&& other) = delete;

		void Update() override {}
		void Render() const override;
		void RenderImGui() override {}
		dae::TextureComponent* GetTextureComponent() const { return m_pTextureComponent; }

		void SetTextureComponent(const std::string& filename);
		void SetSDLTexture(SDL_Texture* pSDLTexture);

	private:
		dae::TextureComponent* m_pTextureComponent;
	};
}