#pragma once
#include <string>
#include <glm/vec2.hpp>
#include "Scene.h"

namespace dae
{
	class GameObject;
}

class TexturedGameObjectPrefab final
{
public:
	TexturedGameObjectPrefab(dae::Scene* pScene, const std::string& textureFilePath, glm::vec2 pos = { 0.f , 0.f });

	~TexturedGameObjectPrefab() = default;
	TexturedGameObjectPrefab(const TexturedGameObjectPrefab&) = delete;
	TexturedGameObjectPrefab(TexturedGameObjectPrefab&&) = delete;
	TexturedGameObjectPrefab& operator=(const TexturedGameObjectPrefab&) = delete;
	TexturedGameObjectPrefab& operator=(TexturedGameObjectPrefab&&) = delete;

	std::unique_ptr<dae::GameObject> GetGameObject() { return std::move(m_go); }

private:
	std::unique_ptr<dae::GameObject> m_go;
};