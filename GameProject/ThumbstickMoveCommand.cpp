#include "ThumbstickMoveCommand.h"
#include "GameObject.h"

commands::ThumbstickMoveCommand::ThumbstickMoveCommand(dae::GameObject* go)
{
	m_pMoveComponent = go->GetComponent<MoveComponent>();
}

void commands::ThumbstickMoveCommand::Execute()
{
	if (!m_pMoveComponent)
		return;

	glm::vec2 moveDirection{ m_ThumbStickDirection };

	if (abs(moveDirection.x) > abs(moveDirection.y))
		moveDirection.y = 0.f;
	else 
		moveDirection.x = 0.f;

	m_pMoveComponent->Move(moveDirection);
}