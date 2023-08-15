#pragma once
#include "Command.h"
#include <string>

namespace commands
{
	class SetActiveSceneCommand final : public dae::Command
	{
	public:
		SetActiveSceneCommand(const std::string& sceneName);

		void Execute() override;

	private:
		const std::string m_SceneName{};
	};
}