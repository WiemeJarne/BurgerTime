#pragma once
#include "BaseEvent.h"
#include <memory>
#include "PlayerController.h"
#include "ThumbstickCommand.h"

namespace dae
{
	class ControllerAxisCommandAddedEvent final : public BaseEvent
	{
	public:
		ControllerAxisCommandAddedEvent(std::shared_ptr<ThumbstickCommand> command, PlayerController::ControllerAxis controllerAxis, int controllerIndex)
			: m_Command{ std::move(command) }
			, m_ControllerAxis{ controllerAxis }
			, m_ControllerIndex{ controllerIndex }
		{}

		std::shared_ptr<ThumbstickCommand> GetCommand() const { return m_Command; }
		PlayerController::ControllerAxis GetControllerAxis() const { return m_ControllerAxis; }
		int GetControllerIndex() const { return m_ControllerIndex; }

	private:
		std::shared_ptr<ThumbstickCommand> m_Command{};
		PlayerController::ControllerAxis m_ControllerAxis{};
		int m_ControllerIndex{};
	};
}