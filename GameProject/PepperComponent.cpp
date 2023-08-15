#include "PepperComponent.h"
#include "TexturedGameObjectPrefab.h"
#include "SceneManager.h"
#include "StunEnemyComponent.h"
#include "MoveComponent.h"
#include "LevelManager.h"
#include "TextureComponent.h"
#include "CollisionBoxComponent.h"

PepperComponent::PepperComponent(dae::GameObject* pOwner, int amount, const glm::vec2& visializePos)
	: Component(pOwner)
	, m_Amount{ amount }
{
	auto pScene{ dae::SceneManager::GetInstance().GetSceneByName("levelScene")};

	if (!pScene)
		return;

	constexpr float spaceBetweenPepperMills{ 20.f };

	for(int index{}; index < amount; ++index)
	{
		auto gameObject{ std::make_unique<TexturedGameObjectPrefab>(pScene, "PepperMill.png", glm::vec2(visializePos.x + index * spaceBetweenPepperMills, visializePos.y))->GetGameObject() };
		m_pVisualizeGameObjects.push_back(gameObject.get());
		pScene->Add(std::move(gameObject));
	}
}

void PepperComponent::Use()
{
	if (m_Amount <= 0)
		return;

	--m_Amount;
	m_IsActive = true;

	auto pScene{ dae::SceneManager::GetInstance().GetSceneByName("levelScene") };

	if (!pScene)
		return;

	pScene->Remove(m_pVisualizeGameObjects[m_Amount]);

	glm::vec2 spawnPos{ m_pOwner->GetLocalPos() };

	auto moveComponent{ m_pOwner->GetComponent<MoveComponent>() };

	//if the owner of this object has a moveComponent then use the previous direction to determine the spawnPos of the pepper
	if(moveComponent)
	{
		//get the grid
		auto pGrid{ LevelManager::GetInstance().GetActiveLevelGrid() };

		if(pGrid)
		{
			//get the last direction the player moved in
			glm::vec2 direction{ moveComponent->GetPreviousDirection() };

			//if the y of the direction is smaller then zero spawn towards the right of the player
			if(direction.y < 0.f)
			{
				direction.y = 0.f;
				direction.x = 1.f;

				spawnPos = m_pOwner->GetLocalPos() + direction * pGrid->GetCellSideLenght();
			}
			else if(direction.y > 0.f) //if the y of the direction is bigger then zero spawn to the left
			{
				direction.y = 0.f;
				direction.x = -1.f;

				spawnPos = m_pOwner->GetLocalPos() + direction * pGrid->GetCellSideLenght();
			}

			spawnPos = m_pOwner->GetLocalPos() + direction * pGrid->GetCellSideLenght();
		}
	}

	//spawn the pepper
	auto gameObject{ std::make_unique<TexturedGameObjectPrefab>(pScene, "Pepper.png", spawnPos)->GetGameObject() };
	gameObject->AddComponent(std::make_unique<StunEnemyComponent>(gameObject.get(), 2.f, 2.f));
	const auto size{ gameObject->GetComponent<dae::TextureComponent>()->GetSize() };
	gameObject->AddComponent(std::make_unique<dae::CollisionBoxComponent>(gameObject.get(), static_cast<float>(size.x), static_cast<float>(size.y)));
	pScene->Add(std::move(gameObject));
}