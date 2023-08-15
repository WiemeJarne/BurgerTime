#include "EnemyAIComponent.h"
#include "GameObject.h"
#include "MoveComponent.h"
#include "LevelManager.h"
#include "TextureComponent.h"
#include "Timer.h"
#include "NotStunnedState.h"
#include <glm/glm.hpp>
#include "EventQueueManager.h"

glm::vec2 EnemyAIComponent::m_sUpDirection;
glm::vec2 EnemyAIComponent::m_sDownDirection;
glm::vec2 EnemyAIComponent::m_sLeftDirection;
glm::vec2 EnemyAIComponent::m_sRightDirection;

EnemyAIComponent::EnemyAIComponent(dae::GameObject* pOwner, float moveSpeed, bool shouldRandomlyClimbLadder, float secBetweenRandomClimbLadder, bool shouldRandomlyFlipDirection, float secBetweenRandomFlipDirection)
	: Component(pOwner)
	, m_ShouldRandomlyClimbLadder{ shouldRandomlyClimbLadder }
	, m_SecBetweenRandomClimbLadder{ secBetweenRandomClimbLadder }
	, m_ShouldRandomlyFlipDirection{ shouldRandomlyFlipDirection }
	, m_SecBetweenRandomFlipDirection{ secBetweenRandomFlipDirection }
{
	auto moveComponent{ std::make_unique<MoveComponent>(pOwner, moveSpeed, false, true) };
	m_pMoveComponent = moveComponent.get();
	pOwner->AddComponent(std::move(moveComponent));

	//if the abs(y) of the m_sUpDirection is close to zero initialize all the directions
	if (abs(m_sUpDirection.y) <= 0.01f)
	{
		m_sUpDirection = { 0.f, -1.f };
		m_sDownDirection = { 0.f, 1.f };
		m_sLeftDirection = { -1.f, 0.f };
		m_sRightDirection = { 1.f, 0.f };
	}
	
	dae::EventQueueManager::GetInstance().AddListener<PlayerMovedEvent>(this);
}

EnemyAIComponent::~EnemyAIComponent()
{
	delete m_pEnemyState;
	dae::EventQueueManager::GetInstance().RemoveListener<PlayerMovedEvent>(this);
}

void EnemyAIComponent::Update()
{
	if(!m_pEnemyState)
		m_pEnemyState = new NotStunnedState(m_pOwner->GetComponent<DamageComponent>(), m_pOwner->GetComponent<HealthComponent>(), this);

	m_pEnemyState = m_pEnemyState->Update();

	//check if the owner has a parent if so then don't move
	if (!m_CanWalk)
		return;

	//calculate the owner middlePos
	auto ownerMiddlePos{ m_pOwner->GetLocalPos() };

	//if the width of the enmy is close to zero get the width and height from the TextureCompontent of the owner of this component
	if (abs(m_EnemyWidth) < 0.01f)
	{
		auto pTextureComponent{ m_pOwner->GetComponent<dae::TextureComponent>() };
		if (pTextureComponent)
		{
			auto size{ pTextureComponent->GetSize() };
			m_EnemyWidth = static_cast<float>(size.x);
			m_EnemyHeight = static_cast<float>(size.y);
		}
	}

	ownerMiddlePos.x += m_EnemyWidth / 2.f;
	ownerMiddlePos.y += m_EnemyHeight / 2.f;

	//get the active grid
	if(!m_pActiveGrid)
		m_pActiveGrid = LevelManager::GetInstance().GetActiveLevelGrid();

	if (!m_pActiveGrid)
		return;

	//get the cell where this enemy is in
	auto pEnemyCell{ m_pActiveGrid->GetCell(ownerMiddlePos) };

	//if the enemy is not in a cell move towards the grid
	if (!pEnemyCell)
	{
		MoveTowardsGrid(ownerMiddlePos);
		return;
	}

	if (m_IsNavigationToGridOrPlatform && pEnemyCell->cellKind != CellKind::shortEmpty && pEnemyCell->cellKind != CellKind::longEmpty)
	{
		m_IsNavigationToGridOrPlatform = false;
		m_pToNavigateCell = nullptr;
	}

	//if the enemy is in an empty cell move towards the closest longFloor
	if (pEnemyCell->cellKind == CellKind::shortEmpty || pEnemyCell->cellKind == CellKind::longEmpty)
		MoveTowardsClosestPlatform(ownerMiddlePos);

	if (m_ShouldRandomlyClimbLadder && !m_IsNavigationToGridOrPlatform)
	{
		m_SecSinceLastRandomClimbedLadder += dae::Timer::GetInstance().GetElapsedSec();
		if (m_SecSinceLastRandomClimbedLadder >= m_SecBetweenRandomClimbLadder)
		{
			if(RandomlyClimbLadder(pEnemyCell))
			return;
		}
	}

	if (m_ShouldRandomlyFlipDirection && !m_IsNavigationToGridOrPlatform)
	{
		m_SecSinceRandomDirectionFlip += dae::Timer::GetInstance().GetElapsedSec();
		if (m_SecSinceRandomDirectionFlip >= m_SecBetweenRandomFlipDirection)
		{
			FlipDirection();
			return;
		}
	}

	//get the cell the player is in
	auto pPlayerCell{ m_pActiveGrid->GetCell(m_PlayerMiddlePos) };

	if (!pPlayerCell)
		return;

	const float distanceToPlayer{ glm::length(m_PlayerMiddlePos - ownerMiddlePos) };

	//check if this enemy is on a ladder
	if (pEnemyCell->cellKind == CellKind::ladder && m_pToNavigateCell != m_pPreviousClimbedLadder)
	{
		m_pToNavigateCell = nullptr;
		m_pPreviousClimbedLadder = pEnemyCell;

		//if the previous direction was going up try going up
		if (m_PreviousDirection == m_sUpDirection && m_pMoveComponent->Move(m_sUpDirection))
			return;

		//if the previous direction was going down try going down
		if (m_PreviousDirection == m_sDownDirection && m_pMoveComponent->Move(m_sDownDirection))
			return;

		//if the previous direction was not up or down check if the player is above the enemy if so try to move up
		if (ownerMiddlePos.y > m_PlayerMiddlePos.y && m_pMoveComponent->Move(m_sUpDirection))
		{
			m_PreviousDirection = m_sUpDirection;
			return;
		}
		else if (m_pMoveComponent->Move(m_sDownDirection))
		{
			m_PreviousDirection = m_sDownDirection;
			return;
		}
	}

	//get the closest ladder cell if this enemy is not already walking to one
	if(!m_pToNavigateCell)
		m_pToNavigateCell = m_pActiveGrid->GetNearestCellOfKind(ownerMiddlePos, CellKind::ladder);

	//calculate the distance to the ladder
	const float distanceToToNavigateCell{ glm::length(m_pToNavigateCell->middlePos - ownerMiddlePos) };

	//calculate the distance from the ladder to the player
	const float distanceFromLadderToPlayer{ glm::length(m_PlayerMiddlePos - m_pToNavigateCell->middlePos) };

	glm::vec2 toMovePos{};

	//check if the ladder is closer then the player and if it brings this enemy closer to the player
	if ((distanceToToNavigateCell < distanceToPlayer && distanceFromLadderToPlayer < distanceToPlayer) || m_pToNavigateCell->cellKind == CellKind::longFloor)
		toMovePos = m_pToNavigateCell->middlePos; //if so move to the ladder
	else toMovePos = m_PlayerMiddlePos; //else move to the player

	//if this enemy is not in the same row then the player
	if (pEnemyCell->rowNr != pPlayerCell->rowNr)
	{
		//if the previous direction was going to the left try going to the left
		if (m_PreviousDirection == m_sUpDirection && m_pMoveComponent->Move(m_sUpDirection))
			return;

		//if the previous direction was going to the right try going to the right
		if (m_PreviousDirection == m_sDownDirection && m_pMoveComponent->Move(m_sDownDirection))
			return;

		//check if the ladder is to the left of this enemy if so try to move to the left
		if (toMovePos.y < ownerMiddlePos.y && m_pMoveComponent->Move(m_sUpDirection))
		{
			m_PreviousDirection = m_sUpDirection;
			return;
		}
		else if (m_pMoveComponent->Move(m_sDownDirection)) //otherwise try to move to the right
		{
			m_PreviousDirection = m_sDownDirection;
			return;
		}
	}

	//if the previous direction was going to the left try going to the left
	if (m_PreviousDirection == m_sLeftDirection && m_pMoveComponent->Move(m_sLeftDirection))
		return;

	//if the previous direction was going to the right try going to the right
	if (m_PreviousDirection == m_sRightDirection && m_pMoveComponent->Move(m_sRightDirection))
		return;

	//check if the ladder is to the left of this enemy if so try to move to the left
	if (toMovePos.x < ownerMiddlePos.x && m_pMoveComponent->Move(m_sLeftDirection))
	{
		m_PreviousDirection = m_sLeftDirection;
		return;
	}
	else if (m_pMoveComponent->Move(m_sRightDirection)) //otherwise try to move to the right
	{
		m_PreviousDirection = m_sRightDirection;
		return;
	}
}

void EnemyAIComponent::OnNotify(const PlayerMovedEvent* pEvent)
{
	m_PlayerMiddlePos = pEvent->GetPlayerMiddlePos();
}

void EnemyAIComponent::MoveTowardsGrid(const glm::vec2& ownerMiddlePos)
{
	m_IsNavigationToGridOrPlatform = true;

	//if the enemy is on the left of the grid try to move to the right
	if (ownerMiddlePos.x < 0.f && m_pMoveComponent->Move(m_sRightDirection))
	{
		m_PreviousDirection = m_sRightDirection;
		return;
	}
	
	const auto cellSideLenght{ m_pActiveGrid->GetCellSideLenght() };
	
	//if the enemy is on the right of the grid try to move to the left
	if (ownerMiddlePos.x > m_pActiveGrid->GetMaxAmountOfCollumns() * cellSideLenght && m_pMoveComponent->Move(m_sLeftDirection))
	{
		m_PreviousDirection = m_sLeftDirection;
		return;
	}
	
	//if the enemy is below the grid try to move up
	if (ownerMiddlePos.y > m_pActiveGrid->GetAmountOfRows() * cellSideLenght && m_pMoveComponent->Move(m_sUpDirection))
	{
		m_PreviousDirection = m_sUpDirection;
		return;
	}
	
	//if the enemy is above the grid try to move down
	if (ownerMiddlePos.y < 0.f && m_pMoveComponent->Move(m_sDownDirection))
	{
		m_PreviousDirection = m_sDownDirection;
	}
}

void EnemyAIComponent::MoveTowardsClosestPlatform(const glm::vec2& ownerMiddlePos)
{
	m_IsNavigationToGridOrPlatform = true;

	if (!m_pToNavigateCell || m_pToNavigateCell->cellKind != CellKind::longFloor)
	{
		m_pToNavigateCell = m_pActiveGrid->GetNearestCellOfKind(ownerMiddlePos, CellKind::longFloor);
	}
}

void EnemyAIComponent::FlipDirection()
{
	if (m_PreviousDirection == m_sUpDirection)
		m_PreviousDirection = m_sDownDirection;

	else if (m_PreviousDirection == m_sDownDirection)
		m_PreviousDirection = m_sUpDirection;

	else if (m_PreviousDirection == m_sLeftDirection)
		m_PreviousDirection = m_sRightDirection;

	else if (m_PreviousDirection == m_sRightDirection)
		m_PreviousDirection = m_sLeftDirection;

	m_pMoveComponent->Move(m_PreviousDirection);

	m_SecSinceRandomDirectionFlip = 0.f;
}

bool EnemyAIComponent::RandomlyClimbLadder(Cell* currentCell)
{
	if (currentCell->cellKind == CellKind::ladder)
		return false;

	//generate a random int
	int randomInt{ rand() % 2 };

	//if the randomInt is 0 check if there is a ladder above the current cell if so move up
	if (randomInt == 0 && currentCell->pTopNeighbor && currentCell->pTopNeighbor->cellKind == CellKind::ladder && m_pMoveComponent->Move(m_sUpDirection))
	{
		m_PreviousDirection = m_sUpDirection;
		m_SecSinceLastRandomClimbedLadder = 0.f;
		return true;
	}

	//if the randomInt is 1 check if there is a ladder below the current cell if so move down
	else if (randomInt == 1 && currentCell->pBottomNeighbor && currentCell->pBottomNeighbor->cellKind == CellKind::ladder && m_pMoveComponent->Move(m_sDownDirection))
	{
		m_PreviousDirection = m_sDownDirection;
		m_SecSinceLastRandomClimbedLadder = 0.f;
		return true;
	}

	return false;
}