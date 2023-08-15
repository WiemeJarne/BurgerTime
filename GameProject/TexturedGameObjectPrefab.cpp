#include "TexturedGameObjectPrefab.h"
#include "GameObject.h"
#include "RenderComponent.h"

TexturedGameObjectPrefab::TexturedGameObjectPrefab(dae::Scene* pScene, const std::string& textureFilePath, glm::vec2 pos)
{
	m_go = std::make_unique<dae::GameObject>(pScene);

	m_go->SetLocalPosition(pos.x, pos.y);

	auto renderComponent{ std::make_unique<dae::RenderComponent>(m_go.get(), textureFilePath) };
	m_go->AddComponent(std::move(renderComponent));
}