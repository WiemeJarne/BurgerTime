#pragma once
#include <string>
#include <glm/vec2.hpp>
#include "Component.h"
#include <memory>

namespace dae
{
	class GameObject;
	class Scene;
}

class PlayerPrefab final
{
public:
	PlayerPrefab(dae::Scene* pScene, const std::string& textureFilePath, const glm::vec2& pos, const glm::vec2& healthVisualizationPos, const std::string& healthVisualizationTextureFilepath, int amountOfLives = 3, bool useKeyboard = true, float moveSpeed = 100);

	~PlayerPrefab() = default;
	PlayerPrefab(const PlayerPrefab&) = delete;
	PlayerPrefab(PlayerPrefab&&) = delete;
	PlayerPrefab& operator=(const PlayerPrefab&) = delete;
	PlayerPrefab& operator=(PlayerPrefab&&) = delete;

	std::unique_ptr<dae::GameObject> GetGameObject() { return std::move(m_go); }

private:
	std::unique_ptr<dae::GameObject> m_go;
};