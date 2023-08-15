#pragma once
#include "GameObject.h"
#include "HealthComponent.h"
#include "Component.h"
#include "EnemyAIComponent.h"
#include <vector>

class EnemyManagerComponent final : public dae::Component
{
public:
	EnemyManagerComponent(dae::GameObject* pOwner, dae::Scene* pScene, const std::vector<glm::vec2>& spawnLocations, int amountOfMrHotDogs = 0, int amountOfMrEggs = 0, int amountOfMrPickles = 0, float secBetweenSpawns = 2.f);

	void Update() override;
	void Render() const override {}
	void RenderImGui() override {}

private:
	float m_SecSinceLastSpawn{};
	const float m_SecBetweenSpawns{};
	std::vector<std::tuple<dae::GameObject*, HealthComponent*, EnemyAIComponent*>> m_Enemies{};
	std::vector<glm::vec2> m_SpawnLocations{};
};