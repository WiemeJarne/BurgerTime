#pragma once
#include "EnemyState.h"
#include "DamageComponent.h"
#include "HealthComponent.h"
#include "EnemyAIComponent.h"

class NotStunnedState final : public EnemyState
{
public:
	NotStunnedState(DamageComponent* pDamageComponent, HealthComponent* pHealthComponent, EnemyAIComponent* pEnemyAIComponent);

	~NotStunnedState() = default;
	NotStunnedState(const NotStunnedState&) = delete;
	NotStunnedState(NotStunnedState&&) = delete;
	NotStunnedState& operator=(const NotStunnedState&) = delete;
	NotStunnedState& operator=(NotStunnedState&&) = delete;

	EnemyState* Update() override;

private:
	DamageComponent* m_pDamageComponent{};
	HealthComponent* m_pHealthComponent{};
	EnemyAIComponent* m_pEnemyAIComponent{};
};