#include <SDL.h>
#include "InputManager.h"
#include "imgui_impl_sdl2.h"
#include "EventQueueManager.h"
#include "NewSceneActivatedEvent.h"
#include <Windows.h>

dae::InputManager::InputManager()
{
	EventQueueManager::GetInstance().AddListener<NewSceneActivatedEvent>(this);
	EventQueueManager::GetInstance().AddListener<ButtonAddedToActiveSceneEvent>(this);
	EventQueueManager::GetInstance().AddListener<ButtonRemovedFromActiveSceneEvent>(this);
	EventQueueManager::GetInstance().AddListener<KeyboardCommandAddedToActiveSceneEvent>(this);
}

bool dae::InputManager::ProcessInput()
{
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) {
			return false;
		}
		if (e.type == SDL_KEYDOWN) {
			
		}
		if (e.type == SDL_MOUSEBUTTONUP) {

		}
		if (e.type == SDL_MOUSEBUTTONDOWN) {
			if (e.button.button == SDL_BUTTON_LEFT)
			{
				for (auto& button : m_pButtons)
				{
					button->OnMouseClick(glm::vec2(static_cast<float>(e.button.x), static_cast<float>(e.button.y)));
				}
			}
		}
		// etc...
		ImGui_ImplSDL2_ProcessEvent(&e);
	}

	for (auto& controller : m_Controllers)
	{
		controller->Update();
	}

	HandleKeyboardInput();

	return true;
}

void dae::InputManager::AddController(std::unique_ptr<PlayerController> playerController)
{
	m_Controllers.push_back(std::move(playerController));
}

void dae::InputManager::AddButton(ButtonComponent* pButtonComponent)
{
	//check if the button was already added
	auto pButton = std::find_if(m_pButtons.begin(), m_pButtons.end(), [pButtonComponent](ButtonComponent* pOtherButton) { return pButtonComponent == pOtherButton; });

	//if it was not found add it to the vector if it was found ignore it
	if(pButton == m_pButtons.end())
		m_pButtons.push_back(pButtonComponent);
}

void dae::InputManager::RemoveButton(ButtonComponent* pButtonComponent)
{
	m_pButtons.erase(std::remove(m_pButtons.begin(), m_pButtons.end(), pButtonComponent), m_pButtons.end());
}

void dae::InputManager::HandleKeyboardInput()
{
	memcpy(m_PreviousKeyboardKeysState, m_CurrentKeyboardKeysState, sizeof(m_PreviousKeyboardKeysState));
	GetKeyboardState(m_CurrentKeyboardKeysState);

	for (const auto& command : m_KeyboardCommands)
	{
		switch (command.first.first) //check what state the button should be for the command to be executed
		{
		case KeyState::down:
			//check if the button is down in the current frame if so execute the command
			if (IsDownThisFrame(command.first.second))
			{
				command.second->Execute();
			}
			break;

		case KeyState::up:
			//check if the button is up in the current frame if so execute the command
			if (IsUpThisFrame(command.first.second))
			{
				command.second->Execute();
			}
			break;

		case KeyState::pressed:
			//check if the button is pressed if so execute the command
			if (IsPressed(command.first.second))
			{
				command.second->Execute();
			}
			break;
		}

		if(m_CommandsWhereRemoved)
		{
			m_CommandsWhereRemoved = false;
			break;
		}
	}
}

bool dae::InputManager::IsDownThisFrame(int button) const
{
	return (m_CurrentKeyboardKeysState[button] & KEY_DOWN_MASK) && !(m_PreviousKeyboardKeysState[button] & KEY_DOWN_MASK);
}

bool dae::InputManager::IsUpThisFrame(int button) const
{
	return !(m_CurrentKeyboardKeysState[button] & KEY_DOWN_MASK) && (m_PreviousKeyboardKeysState[button] & KEY_DOWN_MASK);
}

bool dae::InputManager::IsPressed(int button) const
{
	return (m_CurrentKeyboardKeysState[button] & KEY_DOWN_MASK);
}

void dae::InputManager::OnNotify(const NewSceneActivatedEvent* pEvent)
{
	auto pScene{ pEvent->GetNewlyActivatedScene() };

	m_pButtons.clear();
	m_pButtons = pScene->GetButtons();
	m_KeyboardCommands = pScene->GetKeyboardCommands();

	for (auto& playerController : m_Controllers)
	{
		playerController->RemoveAllCommands();
	}

	//get the ControllerKeyCommands of the new active scene
	auto controllerKeyCommands{ pScene->GetControllerKeyCommands() };
	for (auto& playerController : controllerKeyCommands)
	{
		for (auto& controllerKeyCommand : playerController.second)
		{
			m_Controllers[playerController.first]->AddCommand(controllerKeyCommand.second, controllerKeyCommand.first);
		}
	}

	//get the controllerAxisCommands of the new active scene
	auto controllerAxisCommands{ pScene->GetControllerAxisCommmands() };
	for (auto& playerController : controllerAxisCommands)
	{
		for (auto& controllerAxisCommand : playerController.second)
		{
			m_Controllers[playerController.first]->AddCommand(controllerAxisCommand.second, controllerAxisCommand.first);
		}
	}
}

void dae::InputManager::OnNotify(const ButtonAddedToActiveSceneEvent* pEvent)
{
	AddButton(pEvent->GetButtonComponent());
}

void dae::InputManager::OnNotify(const ButtonRemovedFromActiveSceneEvent* pEvent)
{
	RemoveButton(pEvent->GetButtonComponent());
}

void dae::InputManager::OnNotify(const KeyboardCommandAddedToActiveSceneEvent* pEvent)
{
	m_KeyboardCommands.insert(std::pair<KeyboardAction, dae::Command*>(pEvent->GetKeyboardAction(), pEvent->GetCommand()));
}

int dae::InputManager::ConvertKeyboardKeyToInt(KeyboardKey keyboardKey) const
{
	switch (keyboardKey)
	{
	case KeyboardKey::A: return 'A';

	case KeyboardKey::B: return 'B';

	case KeyboardKey::C: return 'C';

	case KeyboardKey::D: return 'D';

	case KeyboardKey::E: return 'E';

	case KeyboardKey::F: return 'F';

	case KeyboardKey::G: return 'G';

	case KeyboardKey::H: return 'H';

	case KeyboardKey::I: return 'I';

	case KeyboardKey::J: return 'J';

	case KeyboardKey::K: return 'K';

	case KeyboardKey::L: return 'L';

	case KeyboardKey::M: return 'M';

	case KeyboardKey::N: return 'N';

	case KeyboardKey::O: return 'O';

	case KeyboardKey::P: return 'P';

	case KeyboardKey::Q: return 'Q';

	case KeyboardKey::R: return 'R';

	case KeyboardKey::S: return 'S';

	case KeyboardKey::T: return 'T';

	case KeyboardKey::U: return 'U';

	case KeyboardKey::V: return 'V';

	case KeyboardKey::W: return 'W';

	case KeyboardKey::X: return 'X';

	case KeyboardKey::Y: return 'Y';

	case KeyboardKey::Z: return 'Z';

	case KeyboardKey::F1: return VK_F1;

	case KeyboardKey::F2: return VK_F2;

	case KeyboardKey::F3: return VK_F3;

	case KeyboardKey::F4: return VK_F4;

	case KeyboardKey::F5: return VK_F5;

	case KeyboardKey::F6: return VK_F6;

	case KeyboardKey::F7: return VK_F7;

	case KeyboardKey::F8: return VK_F8;

	case KeyboardKey::F9: return VK_F9;

	case KeyboardKey::F10: return VK_F10;

	case KeyboardKey::F11: return VK_F11;

	case KeyboardKey::F12: return VK_F12;

	case KeyboardKey::BackSpace: return VK_BACK;

	case KeyboardKey::Minus: return VK_OEM_MINUS;

	case KeyboardKey::Space: return VK_SPACE;

	case KeyboardKey::esc: return VK_ESCAPE;
	}
	
	return -1;
}