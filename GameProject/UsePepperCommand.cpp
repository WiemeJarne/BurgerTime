#include "UsePepperCommand.h"

commands::UsePepperCommand::UsePepperCommand(PepperComponent* pPepperComponent)
	: m_pPepperComponent{ pPepperComponent }
{}

void commands::UsePepperCommand::Execute()
{
	if(m_pPepperComponent)
		m_pPepperComponent->Use();
}