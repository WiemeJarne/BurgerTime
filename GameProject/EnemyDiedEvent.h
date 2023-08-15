#pragma once
#include "BaseEvent.h"
#include "GameObject.h"

class EnemyDiedEvent final : public dae::BaseEvent
{
public:
	EnemyDiedEvent(dae::GameObject* pEnemy)
		: m_pEnemy{ pEnemy }
	{}

	~EnemyDiedEvent() = default;

	dae::GameObject* GetEnemy() const { return m_pEnemy; }

private:
	dae::GameObject* m_pEnemy{};
};