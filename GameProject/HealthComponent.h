#pragma once
#include "Component.h"
#include "EventListener.h"
#include <glm/vec2.hpp>
#include <string>
#include <vector>
#include "CollisionEvent.h"

class HealthComponent : public dae::Component, public dae::EventListener<dae::CollisionEvent>
{
public:
	HealthComponent(dae::GameObject* owner, int amountOfLives, bool isPlayer = true);
	HealthComponent(dae::GameObject* owner, int amountOfLives, const glm::vec2& visualizationPos, const std::string& textureFilePath, bool isPlayer = true);
	~HealthComponent();
	HealthComponent(const HealthComponent& other) = delete;
	HealthComponent(HealthComponent&& other) = delete;
	HealthComponent& operator=(const HealthComponent& other) = delete;
	HealthComponent& operator=(HealthComponent&& other) = delete;

	void Update() override;
	void Render() const override {}
	void RenderImGui() override {}
	void OnNotify(const dae::CollisionEvent* pEvent) override;
	void Damage(int amount, bool shouldBreadCastDieEvent = true);
	void Reset();
	void VisualizeHealth(const glm::vec2& pos, const std::string& textureFilePath);
	void Die();
	int GetHealth() const { return m_AmountOfLives; }
	float GetSecSinceDeath() const { return m_SecSinceDeath; }
	bool GetIsCollidingWithPepper() const { return m_IsCollidingWithPepper; }
	void SetIsCollidingWithPepper(bool isCollidingWithPepper) { m_IsCollidingWithPepper = isCollidingWithPepper; }
	float GetAmountOfSecStunned() const { return m_AmountOfSecStunnned; }
	void SetAmountOfSecStunned(float amountOfSecStunned) { m_AmountOfSecStunnned = amountOfSecStunned; }

private:
	int m_StartAmountOfLives{};
	int m_AmountOfLives{};
	float m_SecSinceDeath{};
	const bool m_IsOwnerPlayer{};
	std::vector<dae::GameObject*> m_pVisualizeGameObjects;
	float m_SecSinceLiveLost{};
	bool m_IsCollidingWithPepper{};
	float m_AmountOfSecStunnned{};
};