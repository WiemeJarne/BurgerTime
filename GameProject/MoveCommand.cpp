#include "MoveCommand.h"
#include "GameObject.h"

using namespace commands;

MoveCommand::MoveCommand(dae::GameObject* go, glm::vec2 moveDirection)
	: m_MoveDirection{ moveDirection }
{
	m_pMoveComponent = go->GetComponent<MoveComponent>();
}

void MoveCommand::Execute()
{
	if (!m_pMoveComponent)
		return;

	m_pMoveComponent->Move(m_MoveDirection);
}