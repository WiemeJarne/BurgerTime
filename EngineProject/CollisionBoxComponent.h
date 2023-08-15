#pragma once
#include "Component.h"
#include "glm/vec2.hpp"

namespace dae
{
	class CollisionBoxComponent final : public Component
	{
	public:
		CollisionBoxComponent(GameObject* pOwner, float width, float height);
		~CollisionBoxComponent();

		void Update() override {}
		void Render() const override {}
		void RenderImGui() override {}
		const glm::vec2& GetTopLeftPos() const;
		GameObject* GetOwner() const;
		float GetWidth() const { return m_Width; }
		float GetHeight() const { return m_Height; }

	private:
		const float m_Width{};
		const float m_Height{};
	};
}