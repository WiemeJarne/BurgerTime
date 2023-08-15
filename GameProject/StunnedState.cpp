#include "StunnedState.h"
#include  "NotStunnedState.h"
#include "Timer.h"
#include <iostream>

StunnedState::StunnedState(DamageComponent* pDamageComponent, HealthComponent* pHealthComponent, EnemyAIComponent* pEnemyAIComponent)
	: m_pDamageComponent{ pDamageComponent }
	, m_pHealthComponent{ pHealthComponent }
	, m_pEnemyAIComponent{ pEnemyAIComponent }
{
	//else make sure he can't do damage
	m_pDamageComponent->SetCanDoDamage(false);

	//make sure he can't move
	m_pEnemyAIComponent->SetCanMove(false);
}

EnemyState* StunnedState::Update()
{
	m_SecSinceStunned += dae::Timer::GetInstance().GetElapsedSec();

	//check if the enemy should stop being stunned
	if (m_SecSinceStunned >= m_pHealthComponent->GetAmountOfSecStunned() || (m_pEnemyAIComponent->GetIsFallingWithBurgerPart() && !m_pEnemyAIComponent->GetOwner()))
	{
		auto newState{ new NotStunnedState(m_pDamageComponent, m_pHealthComponent, m_pEnemyAIComponent) };
		delete this;
		return newState;
	}

	return this;
}