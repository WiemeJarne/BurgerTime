#pragma once
#include "BaseEvent.h"
#include "Command.h"
#include "InputManager.h"
#include <iostream>

namespace dae
{
	class KeyboardCommandAddedToActiveSceneEvent final : public BaseEvent
	{
	public:
		KeyboardCommandAddedToActiveSceneEvent(Command* command, std::pair<KeyState, int> keyboardAction)
			: m_Command{ command }
			, m_KeyboardAction{ keyboardAction }
		{}

		Command* GetCommand() const { return m_Command; }
		std::pair<KeyState, int> GetKeyboardAction() const { return m_KeyboardAction; }

	private:
		Command* m_Command;
		std::pair<KeyState, int> m_KeyboardAction;
	};
}