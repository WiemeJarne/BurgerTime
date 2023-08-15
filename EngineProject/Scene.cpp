#include "Scene.h"
#include "GameObject.h"
#include "EventQueueManager.h"
#include "ButtonAddedToActiveSceneEvent.h"
#include "ButtonRemovedFromActiveSceneEvent.h"
#include "KeyboardCommandAddedToActiveSceneEvent.h"
#include <iostream>

using namespace dae;

unsigned int Scene::m_idCounter = 0;

Scene::Scene(const std::string& name) : m_name(name) {}

Scene::~Scene() = default;

void Scene::Add(std::shared_ptr<GameObject> object, bool isButton)
{
	//check if it is a button
	if (isButton)
	{
		//get the buttonComponent
		auto pButtonComponent{ object->GetComponent<ButtonComponent>() };

		//check if it exists
		if (pButtonComponent)
			AddButton(pButtonComponent); //if so add it
	}

	m_objects.emplace_back(std::move(object));
}

void Scene::QueueForAdd(std::shared_ptr<GameObject> object, bool isButton)
{
	//check if it is a button
	if (isButton)
	{
		//get the buttonComponent
		auto pButtonComponent{ object->GetComponent<ButtonComponent>() };

		//check if it exists
		if (pButtonComponent)
			AddButton(pButtonComponent); //if so add it
	}

	m_ObjectsQueuedToAdd.emplace_back(std::move(object));
}

void Scene::Remove(std::shared_ptr<GameObject> object, bool isButton)
{
	//check if it is a button
	if (isButton)
	{
		//get the buttonComponent
		auto pButtonComponent{ object->GetComponent<ButtonComponent>() };

		//check if it exists
		if (pButtonComponent)
			RemoveButton(pButtonComponent); //if so remove it
	}

	m_objects.erase(std::remove(m_objects.begin(), m_objects.end(), object), m_objects.end());
}

void Scene::Remove(GameObject* object, bool isButton)
{
	//check if it is a button
	if (isButton)
	{
		//get the buttonComponent
		auto pButtonComponent{ object->GetComponent<ButtonComponent>() };

		//check if it exists
		if (pButtonComponent)
			RemoveButton(pButtonComponent); //if so remove it
	}

	m_objects.erase(std::remove_if(m_objects.begin(), m_objects.end(), [&object](std::shared_ptr<GameObject> otherObject) { return otherObject.get() == object; }), m_objects.end());
}

void Scene::QueueForRemove(std::shared_ptr<GameObject> object, bool isButton)
{
	//check if it is a button
	if (isButton)
	{
		//get the buttonComponent
		auto pButtonComponent{ object->GetComponent<ButtonComponent>() };

		//check if it exists
		if (pButtonComponent)
			RemoveButton(pButtonComponent); //if so remove it
	}

	m_ObjectsQueuedForRemove.push_back(object.get());
}

void Scene::QueueForRemove(GameObject* object, bool isButton)
{
	//check if it is a button
	if (isButton)
	{
		//get the buttonComponent
		auto pButtonComponent{ object->GetComponent<ButtonComponent>() };

		//check if it exists
		if (pButtonComponent)
			RemoveButton(pButtonComponent); //if so remove it
	}

	m_ObjectsQueuedForRemove.push_back(object);
}

void Scene::RemoveAll()
{
	m_objects.clear();
}

std::shared_ptr<GameObject> Scene::GetSharedPtr(GameObject* pGameObject) const
{
	for (const auto& object : m_objects)
	{
		if (object.get() == pGameObject)
			return object;
	}

	return nullptr;
}

void Scene::AddKeyboardCommand(std::unique_ptr<dae::Command> command, KeyState keyState, InputManager::KeyboardKey keyboardKey)
{
	//create the KeyboardAction
	KeyboardAction keyboardAction{ keyState, InputManager::GetInstance().ConvertKeyboardKeyToInt(keyboardKey) };	

	//add the command and its keyboardAction to the scene
	m_KeyboardCommands[keyboardAction] = std::move(command);

	//if this scene is active send out a KeyboardCommandAddedToActiveSceneEvent
	if (SceneManager::GetInstance().GetActiveScene() == this)
	{
		EventQueueManager::GetInstance().AddEvent<KeyboardCommandAddedToActiveSceneEvent>(std::make_unique<KeyboardCommandAddedToActiveSceneEvent>(m_KeyboardCommands[keyboardAction].get(), keyboardAction));
	}
}

std::map<Scene::KeyboardAction, Command*> Scene::GetKeyboardCommands() const
{
	std::map<KeyboardAction, Command*> keyboardCommands{};

	for (const auto& keyBoardActionAndCommand : m_KeyboardCommands)
	{
		keyboardCommands[keyBoardActionAndCommand.first] = keyBoardActionAndCommand.second.get();
	}

	return keyboardCommands;
}

void Scene::AddControllerKeyCommand(std::shared_ptr<Command> command, Control controllerKey, int controllerIndex)
{
	m_ControllerButtonCommands[controllerIndex][controllerKey] = command;
}

void Scene::AddControllerAxisCommand(std::shared_ptr<ThumbstickCommand> command, PlayerController::ControllerAxis controllerAxis, int controllerIndex)
{
	m_ControllerAxisCommands[controllerIndex][controllerAxis] = command;
}

void Scene::Update()
{
	for(auto& object : m_objects)
	{
		object->Update();
	}

	for (auto& object : m_objects)
	{
		object->EraseComponentsMarkedForDelete();
	}

	//remove all objects queued for remove
	for(auto& object : m_ObjectsQueuedForRemove)
	{
		Remove(object);
	}

	m_ObjectsQueuedForRemove.clear();

	//add objects queued for add
	for (auto& object : m_ObjectsQueuedToAdd)
	{
		Add(object);
	}

	m_ObjectsQueuedToAdd.clear();
}

void Scene::Render() const
{
	for (const auto& object : m_objects)
	{
		object->Render();
	}
}

void Scene::RenderImGui()
{
	for (const auto& object : m_objects)
	{
		object->RenderImGui();
	}
}

void Scene::AddButton(ButtonComponent* pButtonComponent)
{
	//check if the button is already on the scene
	auto pButton = std::find_if(m_pButtons.begin(), m_pButtons.end(), [pButtonComponent](ButtonComponent* pOtherButton) { return pButtonComponent == pOtherButton; });

	//if it was not found add it to the vector and if this scene is the active scene send ButtonAddedToActiveScene out if it was found ignore it
	if (pButton == m_pButtons.end())
	{
		m_pButtons.push_back(pButtonComponent);

		if (SceneManager::GetInstance().GetActiveScene() == this)
		{
			EventQueueManager::GetInstance().AddEvent<ButtonAddedToActiveSceneEvent>(std::make_unique<ButtonAddedToActiveSceneEvent>(pButtonComponent));
		}
	}
}

void Scene::RemoveButton(ButtonComponent* pButtonComponent)
{
	//erase and remove the button component from the vector
	m_pButtons.erase(std::remove(m_pButtons.begin(), m_pButtons.end(), pButtonComponent), m_pButtons.end());

	//remove the owner from the button component from the scene
	Remove(pButtonComponent->GetOwner());

	//check if this scene is the active scene
	if (SceneManager::GetInstance().GetActiveScene() == this)
	{
		//if so send ButttonRemovedFromActiveScene out
		EventQueueManager::GetInstance().AddEvent<ButtonRemovedFromActiveSceneEvent>(std::make_unique<ButtonRemovedFromActiveSceneEvent>(pButtonComponent));
	}
}