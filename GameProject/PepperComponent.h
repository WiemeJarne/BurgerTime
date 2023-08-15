#pragma once
#include "Component.h"
#include "gameObject.h"
#include <vector>

class PepperComponent final : public dae::Component
{
public:
	PepperComponent(dae::GameObject* pOwner, int amount, const glm::vec2& visializePos);
	~PepperComponent() = default;

	void Update() override {}
	void Render() const override {}
	void RenderImGui() override {}

	void Use();

private:
	float m_SecSinceActivation{};
	bool m_IsActive{};
	int m_Amount{};
	std::vector<dae::GameObject*> m_pVisualizeGameObjects{};
};