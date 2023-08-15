#pragma once
#include "Command.h"

namespace commands
{
	class SkipLevelCommand final : public dae::Command
	{
	public:
		SkipLevelCommand() = default;

		virtual ~SkipLevelCommand() = default;
		SkipLevelCommand(const SkipLevelCommand&) = delete;
		SkipLevelCommand(SkipLevelCommand&&) = delete;
		SkipLevelCommand& operator=(const SkipLevelCommand&) = delete;
		SkipLevelCommand& operator=(SkipLevelCommand&&) = delete;

		void Execute() override;
	};
}