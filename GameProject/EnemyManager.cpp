#include "EnemyManager.h"
#include "RenderComponent.h"
#include "EnemyAIComponent.h"
#include "CollisionBoxComponent.h"
#include "DamageComponent.h"
#include "Timer.h"

EnemyManagerComponent::EnemyManagerComponent(dae::GameObject* pOwner, dae::Scene* pScene, const std::vector<glm::vec2>& spawnLocations, int amountOfMrHotDogs, int amountOfMrEggs, int amountOfMrPickles, float secBetweenSpawns)
	: Component(pOwner)
	, m_SpawnLocations{ spawnLocations }
	, m_SecBetweenSpawns{ secBetweenSpawns }
{
	const int maxRandomInt{ static_cast<int>(spawnLocations.size()) };
	constexpr float moveSpeed{ 50.f };

	//create the MrHotDogs
	for (int index{}; index < amountOfMrHotDogs; ++index)
	{
		auto enemy{ std::make_shared<dae::GameObject>(pScene) };
		const int randomInt{ rand() % maxRandomInt };
		enemy->SetLocalPosition(spawnLocations[randomInt].x, spawnLocations[randomInt].y);
		enemy->AddComponent(std::make_unique<dae::RenderComponent>(enemy.get(), "MrHotDog.png"));
		auto aiComponent{ std::make_unique<EnemyAIComponent>(enemy.get(), moveSpeed, true) };
		auto enemySize{ enemy->GetComponent<dae::RenderComponent>()->GetTextureComponent()->GetSize() };
		enemy->AddComponent(std::make_unique<dae::CollisionBoxComponent>(enemy.get(), static_cast<float>(enemySize.x), static_cast<float>(enemySize.y)));
		auto healthComponent{ std::make_unique<HealthComponent>(enemy.get(), 1, false) };
		healthComponent->Die();
		enemy->AddComponent(std::make_unique<DamageComponent>(enemy.get(), 1));
		m_Enemies.push_back(std::make_tuple(enemy.get(), healthComponent.get(), aiComponent.get()));
		enemy->AddComponent(std::move(aiComponent));
		enemy->AddComponent(std::move(healthComponent));
		pScene->Add(enemy);
	}

	//create the MrEggs
	for (int index{}; index < amountOfMrEggs; ++index)
	{
		auto enemy{ std::make_shared<dae::GameObject>(pScene) };
		const int randomInt{ rand() % maxRandomInt };
		enemy->SetLocalPosition(spawnLocations[randomInt].x, spawnLocations[randomInt].y);
		enemy->AddComponent(std::make_unique<dae::RenderComponent>(enemy.get(), "MrEgg.png"));
		auto aiComponent{ std::make_unique<EnemyAIComponent>(enemy.get(), moveSpeed, true) };
		auto enemySize{ enemy->GetComponent<dae::RenderComponent>()->GetTextureComponent()->GetSize() };
		enemy->AddComponent(std::make_unique<dae::CollisionBoxComponent>(enemy.get(), static_cast<float>(enemySize.x), static_cast<float>(enemySize.y)));
		auto healthComponent{ std::make_unique<HealthComponent>(enemy.get(), 1, false) };
		healthComponent->Die();
		enemy->AddComponent(std::make_unique<DamageComponent>(enemy.get(), 1));
		m_Enemies.push_back(std::make_tuple(enemy.get(), healthComponent.get(), aiComponent.get()));
		enemy->AddComponent(std::move(aiComponent));
		enemy->AddComponent(std::move(healthComponent));
		pScene->Add(enemy);
	}

	//create the MrPickles
	for (int index{}; index < amountOfMrPickles; ++index)
	{
		auto enemy{ std::make_shared<dae::GameObject>(pScene) };
		const int randomInt{ rand() % maxRandomInt };
		enemy->SetLocalPosition(spawnLocations[randomInt].x, spawnLocations[randomInt].y);
		enemy->AddComponent(std::make_unique<dae::RenderComponent>(enemy.get(), "MrPickle.png"));
		auto aiComponent{ std::make_unique<EnemyAIComponent>(enemy.get(), moveSpeed, false, 0.f, true) };
		auto enemySize{ enemy->GetComponent<dae::RenderComponent>()->GetTextureComponent()->GetSize() };
		enemy->AddComponent(std::make_unique<dae::CollisionBoxComponent>(enemy.get(), static_cast<float>(enemySize.x), static_cast<float>(enemySize.y)));
		auto healthComponent{ std::make_unique<HealthComponent>(enemy.get(), 1, false) };
		healthComponent->Die();
		enemy->AddComponent(std::make_unique<DamageComponent>(enemy.get(), 1));
		m_Enemies.push_back(std::make_tuple(enemy.get(), healthComponent.get(), aiComponent.get()));
		enemy->AddComponent(std::move(aiComponent));
		enemy->AddComponent(std::move(healthComponent));
		pScene->Add(enemy);
	}
}

void EnemyManagerComponent::Update()
{
	m_SecSinceLastSpawn += dae::Timer::GetInstance().GetElapsedSec();

	//loop over the enemies
	for (auto& enemy : m_Enemies)
	{
		//check if the enemy has zero health
		if (std::get<1>(enemy)->GetHealth() == 0)
		{
			//if so check if he can walk
			if (std::get<2>(enemy)->GetCanMove())
			{
				//if so make sure the any can not move and set the location from where he will restart walking again when needed
				std::get<2>(enemy)->SetCanMove(false);
				const int randomInt{ rand() % static_cast<int>(m_SpawnLocations.size()) };
				std::get<0>(enemy)->SetLocalPosition(m_SpawnLocations[randomInt].x, m_SpawnLocations[randomInt].y);
			}
			//if not set that he can't walk unless it is time to active an enemy
			else if (m_SecSinceLastSpawn >= m_SecBetweenSpawns)
			{
				//active this enemy
				m_SecSinceLastSpawn = 0.f;
				std::get<1>(enemy)->Reset();
				std::get<2>(enemy)->SetCanMove(true);
			}
		}
		
	}
}