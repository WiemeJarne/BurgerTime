#pragma once
#include "Command.h"
#include "MoveComponent.h"

namespace commands
{
	class MoveCommand final : public dae::Command
	{
	public:
		MoveCommand(dae::GameObject* go, glm::vec2 moveDirection);

		virtual ~MoveCommand() = default;
		MoveCommand(const MoveCommand&) = delete;
		MoveCommand(MoveCommand&&) = delete;
		MoveCommand& operator=(const MoveCommand&) = delete;
		MoveCommand& operator=(MoveCommand&&) = delete;

		void Execute() override;

	private:
		MoveComponent* m_pMoveComponent;
		glm::vec2 m_MoveDirection{};
	};
}