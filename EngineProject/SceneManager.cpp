#include "SceneManager.h"
#include "Scene.h"
#include "EventQueueManager.h"
#include "NewSceneActivatedEvent.h"

class SetSceneActiveBeforeAddingControllerCommandsException final
{};

dae::SceneManager::SceneManager()
{
	EventQueueManager::GetInstance().AddListener<ControllerKeyCommandAddedEvent>(this);
	EventQueueManager::GetInstance().AddListener<ControllerAxisCommandAddedEvent>(this);
}

void dae::SceneManager::Update()
{
	//update the active scene if there is a scene active
	if(m_ActiveScene)
		m_ActiveScene->Update();
}

void dae::SceneManager::Render() const
{
	//render the active scene if there is a scene active
	if (m_ActiveScene)
		m_ActiveScene->Render();
}

void dae::SceneManager::RenderImGui()
{
	//render ImGui for the active scene if there is a scene active
	if (m_ActiveScene)
		m_ActiveScene->RenderImGui();
}

dae::Scene* dae::SceneManager::CreateScene(const std::string& name, bool setAsActiveScene)
{
	const auto& scene = std::shared_ptr<Scene>(new Scene(name));
	m_scenes.push_back(scene);
	
	if (setAsActiveScene)
	{
		m_ActiveScene = scene.get();
		EventQueueManager::GetInstance().AddEvent<NewSceneActivatedEvent>(std::make_unique<NewSceneActivatedEvent>(m_ActiveScene));
	}

	return scene.get();
}

void dae::SceneManager::RemoveScene(Scene* scene)
{
	//if the scene to remove is the active scene set the active scene to nullptr
	if (scene == m_ActiveScene)
		m_ActiveScene = nullptr;

	m_scenes.erase(std::remove_if(m_scenes.begin(), m_scenes.end(), [&](std::shared_ptr<Scene> pScene) { return scene == pScene.get(); }));
}

void dae::SceneManager::SetScenesSize(float width, float height)
{
	m_ScenesWidth = width;
	m_ScenesHeight = height;
}

dae::Scene* dae::SceneManager::GetSceneByIndex(int index) const
{
	if (static_cast<int>(m_scenes.size()) <= index)
		return nullptr;

	return m_scenes[index].get();
}

dae::Scene* dae::SceneManager::GetSceneByName(const std::string& name) const
{
	for (const auto& scene : m_scenes)
	{
		if (scene->GetName() == name)
		{
			return scene.get();
		}
	}

	return nullptr;
}

void dae::SceneManager::SetActiveSceneByIndex(int index)
{
	//check if the index is valid
	if (static_cast<int>(m_scenes.size()) <= index)
		return;

	m_ActiveScene = GetSceneByIndex(index);
	EventQueueManager::GetInstance().AddEvent<NewSceneActivatedEvent>(std::make_unique<NewSceneActivatedEvent>(m_ActiveScene));
}

void dae::SceneManager::SetActiveSceneByName(const std::string& name)
{
	auto sceneToSetActive{ GetSceneByName(name) };

	//check if the scene exists
	if (sceneToSetActive)
	{
		m_ActiveScene = sceneToSetActive;
		EventQueueManager::GetInstance().AddEvent<NewSceneActivatedEvent>(std::make_unique<NewSceneActivatedEvent>(m_ActiveScene));
	}
}

void dae::SceneManager::RemoveSceneByIndex(int index)
{
	auto sceneToRemove{ GetSceneByIndex(index) };

	if (sceneToRemove)
		RemoveScene(sceneToRemove);
}

void dae::SceneManager::RemoveSceneByName(const std::string& name)
{
	auto sceneToRemove{ GetSceneByName(name) };

	if (sceneToRemove)
		RemoveScene(sceneToRemove);
}

void dae::SceneManager::OnNotify(const ControllerKeyCommandAddedEvent* event)
{
	if (!m_ActiveScene)
		throw(SetSceneActiveBeforeAddingControllerCommandsException());

	m_ActiveScene->AddControllerKeyCommand(event->GetCommand(), event->GetControllerKey(), event->GetControllerIndex());
}

void dae::SceneManager::OnNotify(const ControllerAxisCommandAddedEvent* event)
{
	if (!m_ActiveScene)
		throw(SetSceneActiveBeforeAddingControllerCommandsException());

	m_ActiveScene->AddControllerAxisCommand(event->GetCommand(), event->GetControllerAxis(), event->GetControllerIndex());
}