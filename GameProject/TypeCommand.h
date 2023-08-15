#pragma once
#include "Command.h"
#include "TextComponent.h"

namespace commands
{
	class TypeCommand final : public dae::Command
	{
	public:
		TypeCommand(char charToType, dae::TextComponent* textComponentToTypeTo); //use /b for backspace

		void Execute() override;
		
	private:
		char m_CharToType;
		dae::TextComponent* m_pTextComponentToTypeTo;
		dae::GameObject* m_pTextComponentOwner;

		void RecenterAlongXAxis() const;
	};
}