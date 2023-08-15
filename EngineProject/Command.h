#pragma once

namespace dae
{
	class Command
	{
	public:
		Command() = default;

		virtual ~Command() = default;
		Command(const Command&) = delete;
		Command(Command&&) = default;
		Command& operator=(const Command&) = delete;
		Command& operator=(Command&&) = delete;

		virtual void Execute() = 0;
		virtual void Undo() {}
	};
}