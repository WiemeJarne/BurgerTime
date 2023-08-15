#pragma once
#include "Component.h"
#include "glm/vec2.hpp"
#include <functional>

namespace dae
{
	class ButtonComponent final : public Component
	{
	public:
		ButtonComponent(dae::GameObject* pOwner, const glm::vec2& topLeftPos, float width, float height, std::function<void()> onClickFunction);

		void Update() override {}
		void Render() const override {}
		void RenderImGui() override {}
		void OnMouseClick(const glm::vec2& mousePos) const;

	private:
		glm::vec2 m_TopLeftPos;
		float width;
		float height;
		std::function<void()> m_OnClickFunction;
	};
}