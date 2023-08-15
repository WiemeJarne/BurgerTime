#pragma once
#include <vector>
#include <string>
#include <memory>
#include "Singleton.h"
#include "EventListener.h"
#include "ControllerKeyCommandAddedEvent.h"
#include "ControllerAxisCommandAddedEvent.h"

namespace dae
{
	class Scene;
	class SceneManager final : public Singleton<SceneManager>, public EventListener<ControllerKeyCommandAddedEvent>, public EventListener<ControllerAxisCommandAddedEvent>
	{
	public:
		Scene* CreateScene(const std::string& name, bool setAsActiveScene);

		void Update();
		void Render() const;
		void RenderImGui();
		void RemoveScene(Scene* scene);
		//this is only called in the Engine.cpp file calling this anywhere else will not have any effect ont the size of the scenes
		void SetScenesSize(float width, float height);
		float GetScenesWidth() const { return m_ScenesWidth; }
		float GetScenesHeight() const { return m_ScenesHeight; }

		Scene* GetActiveScene() const { return m_ActiveScene; }
		Scene* GetSceneByIndex(int index) const;
		Scene* GetSceneByName(const std::string& name) const;
		void SetActiveSceneByIndex(int index);
		void SetActiveSceneByName(const std::string& name);
		void RemoveSceneByIndex(int index);
		void RemoveSceneByName(const std::string& name);
		void OnNotify(const ControllerKeyCommandAddedEvent* event);
		void OnNotify(const ControllerAxisCommandAddedEvent* event);

	private:
		friend class Singleton<SceneManager>;
		SceneManager();
		std::vector<std::shared_ptr<Scene>> m_scenes;
		Scene* m_ActiveScene;
		float m_ScenesWidth{};
		float m_ScenesHeight{};
	};
}
