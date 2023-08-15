#include "NotStunnedState.h"
#include "StunnedState.h"

NotStunnedState::NotStunnedState(DamageComponent* pDamageComponent, HealthComponent* pHealthComponent, EnemyAIComponent* pEnemyAIComponent)
	: m_pDamageComponent{ pDamageComponent }
	, m_pHealthComponent{ pHealthComponent }
	, m_pEnemyAIComponent{ pEnemyAIComponent }
{
	//make sure he can do damage again
	m_pDamageComponent->SetCanDoDamage(true);

	//make sure he can move again
	m_pEnemyAIComponent->SetCanMove(true);

	//make sure to set that he isn't colliding with pepper
	m_pHealthComponent->SetIsCollidingWithPepper(false);
}

EnemyState* NotStunnedState::Update()
{
	//check if the enemy should be stunned
	if (m_pHealthComponent->GetIsCollidingWithPepper() || m_pHealthComponent->GetHealth() == 0 || m_pEnemyAIComponent->GetIsFallingWithBurgerPart())
	{
		auto newState{ new StunnedState(m_pDamageComponent, m_pHealthComponent, m_pEnemyAIComponent) };
		delete this;
		return newState;
	}

	return this;
}