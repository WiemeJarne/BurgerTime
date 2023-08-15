#include "PointsComponent.h"
#include "ResourceManager.h"
#include "TextureComponent.h"
#include "GameObject.h"
#include "RenderComponent.h"
#include "EventQueueManager.h"

PointsComponent::PointsComponent(dae::GameObject* owner, const glm::vec2& middlePos)
	: Component(owner)
	, m_MiddlePos{ middlePos }
{
	auto textComponent{ owner->GetComponent<dae::TextComponent>() };

	if (textComponent)
	{
		m_pTextComponent = textComponent;
	}
	else
	{
		auto font{ dae::ResourceManager::GetInstance().LoadFont("PressStart2P-vaV7.ttf", 12) };
		auto newTextComponent{ std::make_unique<dae::TextComponent>(owner, "0", font) };
		m_pTextComponent = newTextComponent.get();
		owner->AddComponent(std::move(newTextComponent));
		Recenter();
	}

	dae::EventQueueManager::GetInstance().AddListener<BurgerPartDropped1LevelEvent>(this);
	dae::EventQueueManager::GetInstance().AddListener<BurgerPartReachedPlateEvent>(this);
	dae::EventQueueManager::GetInstance().AddListener<BurgerPartDroppedWithEnemiesOnEvent>(this);
	dae::EventQueueManager::GetInstance().AddListener<EnemyDiedEvent>(this);
}

PointsComponent::~PointsComponent()
{
	dae::EventQueueManager::GetInstance().RemoveListener<BurgerPartDropped1LevelEvent>(this);
	dae::EventQueueManager::GetInstance().RemoveListener<BurgerPartReachedPlateEvent>(this);
	dae::EventQueueManager::GetInstance().RemoveListener<BurgerPartDroppedWithEnemiesOnEvent>(this);
	dae::EventQueueManager::GetInstance().RemoveListener<EnemyDiedEvent>(this);
}

void PointsComponent::OnNotify(const BurgerPartDropped1LevelEvent* )
{
	AddPoints(50);
}

void PointsComponent::OnNotify(const BurgerPartReachedPlateEvent* )
{
	AddPoints(50);
}

void PointsComponent::OnNotify(const BurgerPartDroppedWithEnemiesOnEvent* pEvent)
{
	const int amountOfEnemiesDropped{ pEvent->GetAmountOfEnemiesDropped() };

	int score{ 500 }; //when 1 enemy was dropped the score is 500 for every aditional enemy that is dropped to score is doubled so for 2 enemies it would be 1000, for 3 2000 ...
	for (int index{}; index < amountOfEnemiesDropped; ++index)
	{
		if (index > 0)
		{
			score *= 2;
		}
	}

	AddPoints(score);
	m_pTextComponent->SetText(std::to_string(m_AmountOfPoints));
	Recenter();
}

void PointsComponent::OnNotify(const EnemyDiedEvent* pEvent)
{
	//get the texture file name
	const std::string& textureFileName{ pEvent->GetEnemy()->GetComponent<dae::RenderComponent>()->GetTextureComponent()->GetFileName()};

	if (textureFileName == "MrHotDog.png")
		m_AmountOfPoints += 100;
	else if (textureFileName == "MrPickle.png")
		m_AmountOfPoints += 200;
	else if (textureFileName == "MrEgg.png")
		m_AmountOfPoints += 300;

	Recenter();
}

void PointsComponent::AddPoints(int amount)
{
	m_AmountOfPoints += amount;
	m_pTextComponent->SetText(std::to_string(m_AmountOfPoints));
	Recenter();
}

void PointsComponent::Recenter()
{
	const auto scale{ m_pTextComponent->GetTextureComponent()->GetSize() };

	m_pOwner->SetLocalPosition(m_MiddlePos.x - scale.x / 2.f, m_MiddlePos.y - scale.y / 2.f);
}