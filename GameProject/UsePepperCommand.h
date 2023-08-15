#pragma once
#include "Command.h"
#include "GameObject.h"
#include "PepperComponent.h"

namespace commands
{
	class UsePepperCommand final : public dae::Command
	{
	public:
		UsePepperCommand(PepperComponent* pPepperComponent);

		void Execute() override;

	private:
		PepperComponent* m_pPepperComponent{};
	};
}