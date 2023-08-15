#pragma once
#include "Component.h"
#include "MoveComponent.h"
#include "EventListener.h"
#include "glm/vec2.hpp"
#include "EnemyState.h"
#include "PlayerMovedEvent.h"

class EnemyAIComponent final : public dae::Component, public dae::EventListener<PlayerMovedEvent>
{
public:
	EnemyAIComponent(dae::GameObject* pOwner, float moveSpeed, bool shouldRandomlyClimbLadder = false, float secBetweenRandomClimbLadder = 5.f, bool shouldRandomlyFlipDirection = false, float secBetweenRandomFlipDirection = 5.f);
	~EnemyAIComponent();

	void Update() override;
	void Render() const override {}
	void RenderImGui() override {}

	void OnNotify(const PlayerMovedEvent* pEvent);
	float GetHeight() const { return m_EnemyHeight; }
	void SetCanMove(bool canWalk) { m_CanWalk = canWalk; }
	bool GetCanMove() const { return m_CanWalk; }
	void SetIsFallingWithBurgerPart(bool isFallingWithBurgerPart) { m_IsFallingWithBurgerPart = isFallingWithBurgerPart; }
	bool GetIsFallingWithBurgerPart() const { return m_IsFallingWithBurgerPart; }

private:
	MoveComponent* m_pMoveComponent{};
	glm::vec2 m_PlayerMiddlePos{};
	glm::vec2 m_PreviousDirection{};
	Cell* m_pToNavigateCell{};
	Cell* m_pPreviousClimbedLadder{};
	LevelGrid* m_pActiveGrid{};
	bool m_ShouldRandomlyClimbLadder{};
	const float m_SecBetweenRandomClimbLadder{};
	float m_SecSinceLastRandomClimbedLadder{};
	bool m_ShouldRandomlyFlipDirection{};
	const float m_SecBetweenRandomFlipDirection{};
	float m_SecSinceRandomDirectionFlip{};
	float m_EnemyWidth{};
	float m_EnemyHeight{};
	bool m_IsNavigationToGridOrPlatform{};
	bool m_CanWalk{};
	EnemyState* m_pEnemyState{};
	bool m_IsFallingWithBurgerPart{};
	

	void MoveTowardsGrid(const glm::vec2& ownerMiddlePos);
	void MoveTowardsClosestPlatform(const glm::vec2& ownerMiddlePos);
	void FlipDirection();
	bool RandomlyClimbLadder(Cell* currentCell);

	static glm::vec2 m_sUpDirection;
	static glm::vec2 m_sDownDirection;
	static glm::vec2 m_sLeftDirection;
	static glm::vec2 m_sRightDirection;
};