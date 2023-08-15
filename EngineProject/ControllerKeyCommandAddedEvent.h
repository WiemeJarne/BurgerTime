#pragma once
#include "BaseEvent.h"
#include <memory>
#include "PlayerController.h"
#include "Command.h"

namespace dae
{
	class ControllerKeyCommandAddedEvent final : public BaseEvent
	{
	public:
		ControllerKeyCommandAddedEvent(std::shared_ptr<Command> command, PlayerController::Control controllerKey, int controllerIndex)
			: m_Command{ std::move(command) }
			, m_ControllerKey{ controllerKey }
			, m_ControllerIndex{ controllerIndex }
		{}

		std::shared_ptr<Command> GetCommand() const { return m_Command; }
		PlayerController::Control GetControllerKey() const { return m_ControllerKey; }
		int GetControllerIndex() const { return m_ControllerIndex; }

	private:
		std::shared_ptr<Command> m_Command{};
		PlayerController::Control m_ControllerKey{};
		int m_ControllerIndex{};
	};
}