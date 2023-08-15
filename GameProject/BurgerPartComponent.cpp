#include "BurgerPartComponent.h"
#include "LevelManager.h"
#include "Timer.h"
#include "RenderComponent.h"
#include "EventQueue.h"
#include "CollisionManager.h"
#include "DamageComponent.h"
#include "EnemyAIComponent.h"
#include "EventQueueManager.h"
#include "BurgerPartEvents.h"

BurgerPartComponent::BurgerPartComponent(dae::GameObject* owner, float fallSpeed)
	: Component(owner)
	, m_FallSpeed{ fallSpeed }
{
	//get the RenderComponent from the owner
	auto renderComponent{ owner->GetComponent<dae::RenderComponent>() };

	//if there is a renderComponent get use the textureComponent to set m_Width and m_Height
	if (renderComponent)
	{
		//get the texture component
		auto textureComponent{ renderComponent->GetTextureComponent() };

		//get the size of the texture
		auto textureSize{ textureComponent->GetSize() };

		//set the member variables m_Width and m_Height
		m_Width = static_cast<float>(textureSize.x);
		m_Height = static_cast<float>(textureSize.y);
	}

	dae::EventQueueManager::GetInstance().AddListener<dae::CollisionEvent>(this);
}

BurgerPartComponent::~BurgerPartComponent()
{
	dae::EventQueueManager::GetInstance().RemoveListener<dae::CollisionEvent>(this);
}

void BurgerPartComponent::Update()
{
	if (m_HasReachedPlate)
		return;

	//when all the parts have been walked over or m_StartFalling is true because another burgerpart hit this one the burgerPart has to fall
	if ((m_FirstQuarterWalkedOver && m_SecondWalkedOver && m_ThirdWalkedOver && m_FourthWalkedOver || m_IsFalling))
	{
		m_IsFalling = true;

		if (m_UseDelay)
		{
			m_SecSinceDelayStart += dae::Timer::GetInstance().GetElapsedSec();

			if (m_SecSinceDelayStart <= 0.25f)
				return;
		}

		//get the pos of the owner of this component
		auto ownerPos{ m_pOwner->GetLocalPos() };

		//calculate the middle of the owner of this component on x-axis only (this is to find the correct cell the owner of the gameObject is in
		auto ownerXMiddlePos{ ownerPos };
		ownerXMiddlePos.x += m_Width / 2.f;

		//get the cell the owner of this component is in
		m_pCell = LevelManager::GetInstance().GetActiveLevelGrid()->GetCell(ownerXMiddlePos);

		const int childCount{ static_cast<int>(m_pOwner->GetChildCount()) };

		//check if the owner is now in another cell
		if (m_pCell && m_pPreviousCell && (m_pCell != m_pPreviousCell) && !m_ShouldFallUntilPlatform)
		{
			
			//if so check if it is a cell with a long platform
			switch (m_pCell->cellKind)
			{
			case CellKind::plate:
				m_HasReachedPlate = true;
					dae::EventQueueManager::GetInstance().AddEvent<BurgerPartReachedPlateEvent>(std::make_unique<BurgerPartReachedPlateEvent>());
			case CellKind::longFloor:
			case CellKind::longGoDown:
			case CellKind::longGoUp:
			case CellKind::longGoUpAndDown:
				++m_AmountOfLevelsDropped;
				m_SecSinceDelayStart = 0.f;
				m_ShouldFallUntilPlatform = true;
				m_ToGoYValue = m_pCell->middlePos.y;
				if (!m_HasReachedPlate)
					dae::EventQueueManager::GetInstance().AddEvent<BurgerPartDropped1LevelEvent>(std::make_unique<BurgerPartDropped1LevelEvent>());

				if (childCount != 0)
					HandleChildren(childCount);

				break;
			}
		}

		m_pPreviousCell = m_pCell;

		if (m_ShouldFallUntilPlatform && m_pCell)
		{
			//check if the platform has been reached
			if (abs(ownerPos.y - m_pCell->middlePos.y) < 0.3f) //the 0.3f is because of the speed it is possible that ownerPos.y will never be close enough to m_ToGoYValue
			{
				m_ShouldFallUntilPlatform = false;
				m_IsFalling = false;
				m_FirstQuarterWalkedOver = false;
				m_SecondWalkedOver = false;
				m_ThirdWalkedOver = false;
				m_FourthWalkedOver = false;
				m_IsFalling = false;
				m_UseDelay = false;

				if (m_AmountOfLevelsDropped != (childCount + 1))
				{
					m_IsFalling = true;
					m_UseDelay = true;
				}
				else
				{
					HandleChildren(childCount);
					m_AmountOfLevelsDropped = 0;
				}
				return;
			}
		}

		//calculate new pos
		auto newPos{ m_pOwner->GetLocalPos() };
		newPos.y += m_FallSpeed * dae::Timer::GetInstance().GetElapsedSec();

		m_pOwner->SetLocalPosition(newPos.x, newPos.y);
	}
}

void BurgerPartComponent::OnNotify(const dae::CollisionEvent* event)
{
	auto pTriggeredGameObject{ event->GetTriggeredGameObject() };
	auto pOtherGameObject{ event->GetOtherGameObject() };

	//check if the triggered object is this component parent if not return
	if (pTriggeredGameObject != m_pOwner)
		return;

	//check if the other object is a player this is done by checking if the gameObject does not have a DamageComponent
	if (!pOtherGameObject->HasComponent<BurgerPartComponent>())
	{
		if (!pOtherGameObject->HasComponent<DamageComponent>())
			CalculateWalkedOver(pOtherGameObject);
	}
	//if pData is not a player check if it is another burger part by checking if the gameObject has a BurgerPartComponent
	else CollidedWithOtherBurgerPart(pOtherGameObject);
}

void BurgerPartComponent::CalculateWalkedOver(dae::GameObject* pGameObject)
{
	//get the pos of the owner of this component
	auto ownerPos{ m_pOwner->GetLocalPos() };

	//calculate the middle of the owner of this component on x-axis only (this is to find the correct cell the owner of the gameObject is in
	auto ownerXMiddlePos{ ownerPos };
	ownerXMiddlePos.x += m_Width / 2.f;

	//get the pos of the given game object
	auto goPos{ pGameObject->GetLocalPos() };

	//calculate the middle of the given GameObject
	auto pTextureComponent{ pGameObject->GetComponent<dae::TextureComponent>() };
	if (pTextureComponent)
	{
		const auto size{ pTextureComponent->GetSize() };
		goPos.x += size.x / 2.f;
		goPos.y += size.y / 2.f;
	}

	//get the active grid
	auto pActiveGrid{ LevelManager::GetInstance().GetActiveLevelGrid() };

	//get the cell the owner of this component is in
	auto pOwnerCell{ pActiveGrid->GetCell(ownerXMiddlePos) };

	//get the cell the given game object is is
	auto pGoCell{ pActiveGrid->GetCell(goPos) };

	if (!pOwnerCell || !pGoCell)
		return;

	//if they are not in the same row they don't thouch each other so return
	if (pOwnerCell->rowNr != pGoCell->rowNr)
		return;

	//if they are in the same row check how far the given gameObject is in the first quarter of this gameObject
	if (goPos.x > ownerPos.x && goPos.x < ownerPos.x + m_Width / 4.f)
	{
		m_FirstQuarterWalkedOver = true;
		return;
	}

	//check if the given gameObject is in the second quarter of this gameObject
	if (goPos.x > ownerPos.x + m_Width / 4.f && goPos.x < ownerPos.x + m_Width / 2.f)
	{
		m_SecondWalkedOver = true;
		return;
	}

	//check if the given gameObject is in the third quarter of this gameObject
	if (goPos.x > ownerPos.x + m_Width / 2.f && goPos.x < ownerPos.x + 3 * m_Width / 4.f)
	{
		m_ThirdWalkedOver = true;
		return;
	}

	//check if the given gameObject is in the fourth quarter of this gameObject
	if (goPos.x > ownerPos.x + 3 * m_Width / 4.f && goPos.x < ownerPos.x + m_Width)
	{
		m_FourthWalkedOver = true;
		return;
	}
}

void BurgerPartComponent::CollidedWithOtherBurgerPart(dae::GameObject* pGameObject)
{
	//get the burgerPartComponent of the colliding gameObject
	auto pOtherBurgerPartComponent{ pGameObject->GetComponent<BurgerPartComponent>() };

	const int childCount{ static_cast<int>(m_pOwner->GetChildCount()) };

	//check if this burgerPart is above the other burgerPart
	if (m_pOwner->GetLocalPos().y < pGameObject->GetLocalPos().y)
	{
		//check if the other bugerPart has reached a plate
		if (pOtherBurgerPartComponent->GetHasReachedPlate() && m_HasReachedPlate != true)
		{
			m_HasReachedPlate = true; //if so then is object also reached a plate
			dae::EventQueueManager::GetInstance().AddEvent<BurgerPartReachedPlateEvent>(std::make_unique<BurgerPartReachedPlateEvent>());

			if (childCount != 0)
				HandleChildren(childCount);
		}
	}
	else if (!m_HasReachedPlate) //if this burgerPart is not above the other burgerPart (so it is below) then start with falling unless this burgerPart already is on a plate
	{
		m_IsFalling = true;
	}
}

void BurgerPartComponent::HandleChildren(int childCount)
{
	if (m_AmountOfLevelsDropped == (childCount + 1) || m_HasReachedPlate)
	{
		if(childCount > 0)
			dae::EventQueueManager::GetInstance().AddEvent<BurgerPartDroppedWithEnemiesOnEvent>(std::make_unique<BurgerPartDroppedWithEnemiesOnEvent>(childCount));

		//loop over all the children, set there parent to nullptr (so they get added to the scene)
		for (int index{ childCount - 1 }; index >= 0; --index)
		{
			m_pOwner->GetChildAt(index)->SetParent(nullptr, true);

			auto pEnemyAIComponent{ m_pOwner->GetComponent<EnemyAIComponent>() };
			if(pEnemyAIComponent)
				pEnemyAIComponent->SetIsFallingWithBurgerPart(false);
		}
	}
}