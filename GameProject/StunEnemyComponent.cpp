#include "StunEnemyComponent.h"
#include "CollisionManager.h"
#include "GameObject.h"
#include "DamageComponent.h"
#include "HealthComponent.h"
#include "Timer.h"
#include "EventQueueManager.h"

StunEnemyComponent::StunEnemyComponent(dae::GameObject* pOwner, float amountOfSecStunned, float amountOfSecAlive)
	: Component(pOwner)
	, m_AmountOfSecStunned{ amountOfSecStunned }
	, m_AmountOfSecAlive{ amountOfSecAlive }
{
	dae::EventQueueManager::GetInstance().AddListener<dae::CollisionEvent>(this);
}

StunEnemyComponent::~StunEnemyComponent()
{
	dae::EventQueueManager::GetInstance().RemoveListener<dae::CollisionEvent>(this);
}

void StunEnemyComponent::Update()
{
	m_AmountOfSecSinceSpawned += dae::Timer::GetInstance().GetElapsedSec();

	if(m_AmountOfSecSinceSpawned >= m_AmountOfSecAlive)
	{
		auto pScene{ dae::SceneManager::GetInstance().GetSceneByName("levelScene")};

		if (pScene)
			pScene->QueueForRemove(m_pOwner);
	}
}

void StunEnemyComponent::OnNotify(const dae::CollisionEvent* pEvent)
{
	auto pTriggeredGameObject{ pEvent->GetTriggeredGameObject() };
	auto pOtherGameObject{ pEvent->GetOtherGameObject() };

	//if the triggered object is not this object or the other object doesn't have a damageComponent (so it is not an enemy) return
	if (pTriggeredGameObject != GetOwner() || !pOtherGameObject->HasComponent<DamageComponent>())
		return;

	//get the healthComponent from other object
	const auto pHealthComponent{ pOtherGameObject->GetComponent<HealthComponent>() };

	//check if the other object is colliding with pepper
	if(pHealthComponent && !pHealthComponent->GetIsCollidingWithPepper())
	{
		pHealthComponent->SetIsCollidingWithPepper(true);
		pHealthComponent->SetAmountOfSecStunned(m_AmountOfSecStunned);
	}
}