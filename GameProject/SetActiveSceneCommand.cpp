#include "SetActiveSceneCommand.h"
#include "SceneManager.h"

commands::SetActiveSceneCommand::SetActiveSceneCommand(const std::string& sceneName)
	: m_SceneName{ sceneName }
{}


void commands::SetActiveSceneCommand::Execute()
{
	dae::SceneManager::GetInstance().SetActiveSceneByName(m_SceneName);
}