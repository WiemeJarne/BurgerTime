#include "SkipLevelCommand.h"
#include "LevelManager.h"

void commands::SkipLevelCommand::Execute()
{
	LevelManager::GetInstance().LoadNextLevel(true);
}