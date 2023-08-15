#pragma once
#include <memory>
#include "Command.h"
#include "ThumbstickCommand.h"
#include <map>

namespace dae
{
	enum class KeyState;

	class PlayerController final
	{
	public:
		PlayerController();
		PlayerController(int controllerIndex);

		~PlayerController();
		PlayerController(const PlayerController& other) = delete;
		PlayerController(PlayerController&& other) = default;
		PlayerController& operator=(const PlayerController& other) = delete;
		PlayerController& operator=(PlayerController&& other) = delete;

		enum class ControllerKey : unsigned int
		{
			dPadUp = 0x0001,
			dPadDown = 0x0002,
			dPadLeft = 0x0004,
			dPadRight = 0x0008,
			start = 0x0010,
			back = 0x0020,
			shoulderLeft = 0x0100,
			shoulderRight = 0x0200,
			A = 0x1000,
			B = 0x2000,
			X = 0x4000,
			Y = 0x8000
		};

		enum class ControllerAxis : unsigned int
		{
			tumbStickLeft = 0x0040,
			tumbStickRight = 0x0080
		};

		void Update();

		using Control = std::pair<dae::KeyState, ControllerKey>;
		void AddCommand(std::shared_ptr<Command> command, Control controllerKey);
		void AddCommand(std::shared_ptr<ThumbstickCommand> command, ControllerAxis controllerAxis);
		void InvertThumbstickLeftYAxis();
		void RemoveAllCommands(); //this does not delete the commands the commands are still on the scene where they where added on

	private:
		class PlayerControllerImpl;
		std::unique_ptr<PlayerControllerImpl> m_PlayerControllerImpl;
	};
}