#ifndef Scene_h
#define Scene_h
#include "InputManager.h"
#include "SceneManager.h"
#include "GameObject.h"
#include "ButtonComponent.h"
#include "PlayerController.h"

namespace dae
{
	class Scene final
	{
		friend Scene* SceneManager::CreateScene(const std::string& name, bool setAsActiveScene = true);
	public:
		void Add(std::shared_ptr<GameObject> object, bool isButton = false);
		void QueueForAdd(std::shared_ptr<GameObject> object, bool isButton = false);
		void Remove(std::shared_ptr<GameObject> object, bool isButton = false);
		void Remove(GameObject* object, bool isButton = false);
		void QueueForRemove(std::shared_ptr<GameObject> object, bool isButton = false);
		void QueueForRemove(GameObject* object, bool isButton = false);
		void RemoveAll();
		std::shared_ptr<GameObject> GetSharedPtr(GameObject* pGameObject) const;
		const std::string& GetName() const { return m_name; }
		std::vector<ButtonComponent*> GetButtons() const { return m_pButtons; }
		void AddKeyboardCommand(std::unique_ptr<Command> command, KeyState keyState, InputManager::KeyboardKey keyboardKey);
		using KeyboardAction = std::pair<KeyState, int>;
		std::map<KeyboardAction, Command*> GetKeyboardCommands() const;
		using Control = std::pair<dae::KeyState, PlayerController::ControllerKey>;
		void AddControllerKeyCommand(std::shared_ptr<Command> command, Control controllerKey, int controllerIndex);
		using ControllerButtonCommandsMap = std::map<Control, std::shared_ptr<dae::Command>>;
		std::map<int, ControllerButtonCommandsMap> GetControllerKeyCommands() const { return m_ControllerButtonCommands; }
		void AddControllerAxisCommand(std::shared_ptr<ThumbstickCommand> command, PlayerController::ControllerAxis controllerAxis, int controllerIndex);
		using ControllerAxisCommandsMap = std::map<PlayerController::ControllerAxis, std::shared_ptr<ThumbstickCommand>>;
		std::map<int, ControllerAxisCommandsMap> GetControllerAxisCommmands() const { return m_ControllerAxisCommands; }

		void Update();
		void Render() const;
		void RenderImGui();

		~Scene();
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

	private: 
		explicit Scene(const std::string& name);
		void AddButton(ButtonComponent* pButtonComponent);
		void RemoveButton(ButtonComponent* pButtonComponent);

		std::string m_name;
		std::vector < std::shared_ptr<GameObject>> m_objects{};
		std::vector <std::shared_ptr<GameObject>> m_ObjectsQueuedToAdd{};
		std::vector<GameObject*> m_ObjectsQueuedForRemove{};
		std::vector<ButtonComponent*> m_pButtons{};

		using KeyboardCommandsMap = std::map<KeyboardAction, std::unique_ptr<Command>>;
		KeyboardCommandsMap m_KeyboardCommands;

		std::map<int, ControllerButtonCommandsMap> m_ControllerButtonCommands{}; //the int is for the index of the playerController
		std::map<int, ControllerAxisCommandsMap> m_ControllerAxisCommands{}; //the int is for the index of the playerController

		static unsigned int m_idCounter; 
	};
}
#endif // !Scene_h