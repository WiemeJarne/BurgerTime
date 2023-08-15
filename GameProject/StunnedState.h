#pragma once
#include "EnemyState.h"
#include "DamageComponent.h"
#include "HealthComponent.h"
#include "EnemyAIComponent.h"

class StunnedState final : public EnemyState
{
public:
	StunnedState(DamageComponent* pDamageComponent, HealthComponent* pHealthComponent, EnemyAIComponent* pEnemyAIComponent);

	~StunnedState() = default;
	StunnedState(const StunnedState&) = delete;
	StunnedState(StunnedState&&) = delete;
	StunnedState& operator=(const StunnedState&) = delete;
	StunnedState& operator=(StunnedState&&) = delete;

	EnemyState* Update() override;

private:
	DamageComponent* m_pDamageComponent{};
	HealthComponent* m_pHealthComponent{};
	EnemyAIComponent* m_pEnemyAIComponent{};
	float m_SecSinceStunned{};
};