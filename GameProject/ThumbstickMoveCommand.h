#pragma once
#include "ThumbstickCommand.h"
#include "MoveComponent.h"

namespace commands
{
	class ThumbstickMoveCommand : public dae::ThumbstickCommand
	{
	public:
		ThumbstickMoveCommand(dae::GameObject* go);

		virtual ~ThumbstickMoveCommand() = default;
		ThumbstickMoveCommand(const ThumbstickMoveCommand&) = delete;
		ThumbstickMoveCommand(ThumbstickMoveCommand&&) = delete;
		ThumbstickMoveCommand& operator=(const ThumbstickMoveCommand&) = delete;
		ThumbstickMoveCommand& operator=(ThumbstickMoveCommand&&) = delete;

		void Execute() override;

	private:
		MoveComponent* m_pMoveComponent;
	};
}