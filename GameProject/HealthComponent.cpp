#include "HealthComponent.h"
#include "Timer.h"
#include "GameObject.h"
#include "BurgerPartComponent.h"
#include "DamageComponent.h"
#include "TexturedGameObjectPrefab.h"
#include "Scene.h"
#include "TextureComponent.h"
#include "EnemyAIComponent.h"
#include "SoundServiceLocator.h"
#include "PlayerGotDamagedEvent.h"
#include "PlayerDiedEvent.h"
#include "EnemyDiedEvent.h"
#include "EventQueueManager.h"

HealthComponent::HealthComponent(dae::GameObject* owner, int amountOfLives, bool isPlayer)
	: Component(owner)
	, m_StartAmountOfLives{ amountOfLives }
	, m_AmountOfLives{ amountOfLives }
	, m_IsOwnerPlayer{ isPlayer }
{
	dae::EventQueueManager::GetInstance().AddListener<dae::CollisionEvent>(this);
}

HealthComponent::HealthComponent(dae::GameObject* owner, int amountOfLives, const glm::vec2& visualizationPos, const std::string& textureFilePath, bool isPlayer)
	: HealthComponent(owner, amountOfLives, isPlayer)
{
	VisualizeHealth(visualizationPos, textureFilePath);
}

HealthComponent::~HealthComponent()
{
	dae::EventQueueManager::GetInstance().RemoveListener<dae::CollisionEvent>(this);
}

void HealthComponent::Update()
{
	if (m_AmountOfLives <= 0)
	{
		m_SecSinceDeath += dae::Timer::GetInstance().GetElapsedSec();
	}
	else m_SecSinceLiveLost += dae::Timer::GetInstance().GetElapsedSec();
}

void HealthComponent::OnNotify(const dae::CollisionEvent* pEvent)
{
	if (m_AmountOfLives == 0)
		return;

	auto pTriggeredGameObject{ pEvent->GetTriggeredGameObject() };
	auto pOtherGameObject{ pEvent->GetOtherGameObject() };

	if (pTriggeredGameObject != m_pOwner)
		return;

	auto pBurgerPartComponent{ pOtherGameObject->GetComponent<BurgerPartComponent>() };

	if (!m_IsOwnerPlayer) //if this component's owner is not a player
	{
		//check if the pOther has a burgerPartComponent and check if the burgerPart is falling
		if (pBurgerPartComponent && pBurgerPartComponent->GetIsFalling())
		{
			//if the enemy is below it damage the enemy
			if (pBurgerPartComponent->GetTopLeftPos().y < m_pOwner->GetLocalPos().y)
				Damage(m_AmountOfLives);
			//if the enemy is not below the burgerPart set the owner of the enemy equal to the owner of the burgerPart
			else if (!pBurgerPartComponent->GetHasReachedPlate())
			{
				//get the EnemyAIComponent to get the height of the enemy and the burgerPart and check if the feet of the enemy are below the middle of the burgerPart
				if(pBurgerPartComponent->GetOwner()->GetLocalPos().y + pBurgerPartComponent->GetHeight() <= m_pOwner->GetLocalPos().y + m_pOwner->GetComponent<EnemyAIComponent>()->GetHeight() + 5.f)
				{
					m_pOwner->SetParent(pBurgerPartComponent->GetOwner(), true);
					m_pOwner->GetComponent<EnemyAIComponent>()->SetIsFallingWithBurgerPart(true);
				}
			}
		}
	}
	else //if this component's owner is a player
	{
		//check if the otherComponent has a damage component if so the player takes damage
		auto pDamageComponent{ pOtherGameObject->GetComponent<DamageComponent>() };

		if (pDamageComponent)
		{
			Damage(pDamageComponent->GetDamageAmount());
		}
	}
}

void HealthComponent::Damage(int amount, bool shouldBreadCastDieEvent)
{
	if(amount <= 0)
		return;

	constexpr float invincibilitySec{ 2.f };

	if (m_AmountOfLives > 0 && m_SecSinceLiveLost >= invincibilitySec)
	{
		m_SecSinceLiveLost = 0.f;

		m_AmountOfLives -= amount;

		if(m_IsOwnerPlayer)
		{
			if (shouldBreadCastDieEvent && m_AmountOfLives <= 0)
				dae::EventQueueManager::GetInstance().AddEvent<PlayerDiedEvent>(std::make_unique<PlayerDiedEvent>());

			dae::EventQueueManager::GetInstance().AddEvent<PlayerGotDamagedEvent>(std::make_unique<PlayerGotDamagedEvent>(1));
			dae::ServiceLocator::GetSoundSystem().Play("sound/LoseLife.wav", 30);
		}

		if (!m_pVisualizeGameObjects.empty())
		{
			auto pScene{ dae::SceneManager::GetInstance().GetSceneByName("levelScene")};

			if (pScene)
				pScene->Remove(m_pVisualizeGameObjects[m_AmountOfLives]);
		}

		if (m_AmountOfLives == 0)
		{
			m_SecSinceDeath = 0.f;

			if (shouldBreadCastDieEvent)
			{
				if (m_IsOwnerPlayer)
					dae::EventQueueManager::GetInstance().AddEvent<PlayerDiedEvent>(std::make_unique<PlayerDiedEvent>());
				else dae::EventQueueManager::GetInstance().AddEvent<EnemyDiedEvent>(std::make_unique<EnemyDiedEvent>(m_pOwner));
			}
		}
	}
}

void HealthComponent::Reset()
{
	m_AmountOfLives = m_StartAmountOfLives;
	m_SecSinceDeath = 0.f;
}

void HealthComponent::VisualizeHealth(const glm::vec2& pos, const std::string& textureFilePath)
{
	if (!m_pVisualizeGameObjects.empty())
		return;

	dae::Scene* pScene{ dae::SceneManager::GetInstance().GetSceneByName("levelScene") };

	if (!pScene)
		return;

	constexpr float spaceBetweenLives{ 20 };

	for (int index{}; index < m_AmountOfLives; ++index)
	{
		auto gameObject{ std::make_unique<TexturedGameObjectPrefab>(pScene, textureFilePath, glm::vec2(pos.x + index * spaceBetweenLives, pos.y))->GetGameObject() };
		m_pVisualizeGameObjects.push_back(gameObject.get());
		pScene->Add(std::move(gameObject));
	}
}

void HealthComponent::Die()
{
	m_AmountOfLives = 0;
}