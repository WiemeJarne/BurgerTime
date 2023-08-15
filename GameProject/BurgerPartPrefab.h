#pragma once
#include "GameObject.h"
#include <string>

class BurgerPartPrefab final
{
public:
	BurgerPartPrefab(dae::Scene* pScene, const std::string& textureFilePath, const glm::vec2& cellTopLeftPos);

	~BurgerPartPrefab() = default;
	BurgerPartPrefab(const BurgerPartPrefab&) = delete;
	BurgerPartPrefab(BurgerPartPrefab&&) = delete;
	BurgerPartPrefab& operator=(const BurgerPartPrefab&) = delete;
	BurgerPartPrefab& operator=(BurgerPartPrefab&&) = delete;

	std::unique_ptr<dae::GameObject> GetGameObject() { return std::move(m_go); }

private:
	std::unique_ptr<dae::GameObject> m_go;
};