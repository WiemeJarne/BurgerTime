#include "BurgerPartPrefab.h"
#include "TexturedGameObjectPrefab.h"
#include "BurgerPartComponent.h"
#include "RenderComponent.h"
#include "LevelManager.h"
#include "CollisionBoxComponent.h"

BurgerPartPrefab::BurgerPartPrefab(dae::Scene* pScene, const std::string& textureFilePath, const glm::vec2& cellTopLeftPos)
{
	glm::vec2 pos{ cellTopLeftPos };
	auto texturedGoPrefab{ std::make_unique<TexturedGameObjectPrefab>(pScene, textureFilePath, pos) };
	m_go = std::move(texturedGoPrefab->GetGameObject());

	//get the TextureComponent from the RenderComponent
	auto renderComponent{ m_go->GetComponent<dae::RenderComponent>() };
	if (renderComponent)
	{
		pos.y += LevelManager::GetInstance().GetActiveLevelGrid()->GetCellSideLenght() - renderComponent->GetTextureComponent()->GetSize().y;
		m_go->SetLocalPosition(cellTopLeftPos.x, cellTopLeftPos.y);

		auto textureSize{ renderComponent->GetTextureComponent()->GetSize() };
		auto collisionBoxComponent{ std::make_unique<dae::CollisionBoxComponent>(m_go.get(), static_cast<float>(textureSize.x), static_cast<float>(textureSize.y)) };
		m_go->AddComponent(std::move(collisionBoxComponent));
	}

	m_go->SetLocalPosition(pos.x, pos.y);

	auto burgerPartComponent{ std::make_unique<BurgerPartComponent>(m_go.get(), 75.f) };

	m_go->AddComponent(std::move(burgerPartComponent));
}