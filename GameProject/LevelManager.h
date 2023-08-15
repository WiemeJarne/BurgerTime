#pragma once
#include <string>
#include <tuple>
#include "Singleton.h"
#include "Scene.h"
#include "EventListener.h"
#include "PointsComponent.h"
#include "LevelGrid.h"
#include "Font.h"
#include "HealthComponent.h"
#include "BurgerPartEvents.h"
#include "PlayerDiedEvent.h"

enum class GameMode
{
	singlePlayer,
	coOp,
	versus
};

class LevelManager final : public dae::Singleton<LevelManager>, public dae::EventListener<BurgerPartReachedPlateEvent>, public dae::EventListener<PlayerDiedEvent>
{
public:
	LevelManager();
	~LevelManager();

	void LoadLevel(int levelNr, GameMode gameMode, bool cycleLevels = false);
	void LoadNextLevel(bool cycleLevels = false);
	void OnNotify(const BurgerPartReachedPlateEvent* pEvent) override;
	void OnNotify(const PlayerDiedEvent* pEvent) override;
	LevelGrid* GetActiveLevelGrid() const;

private:
	int m_AmountOfBurgerPartsInCurrentLevel{};
	int m_AmountOfBurgerParsReachedPlate{};
	bool m_HasBeenInitialized{};
	int m_LevelNr{};
	GameMode m_GameMode{};
	PointsComponent* m_pPointsComponent{};
	int m_AmountOfPoints{};
	std::vector<HealthComponent*> m_PlayersHealthComponents{};
	std::vector<int> m_PlayersHealth{};
	std::vector<std::tuple<std::unique_ptr<LevelGrid>, int, int>> m_LevelGrids{};
	const glm::vec2 m_LevelTopLeftPos{ 0.f, 32.f };
	std::shared_ptr<dae::Font> m_Font;
	std::vector<std::pair<std::string, int>> m_HighScoreList;
	int m_AmountOfPlayersDead{};
	std::vector<glm::vec2> m_EnemiesSpawnLocations{};

	bool LoadLevelPlatforms(int levelNr); //return if it succeeded to open the file
	void LoadLevelBurgerParts(int levelNr);
	const glm::vec2 AddLevelElementToCurrentScene(CellKind cellKind, int colNr, int rowNr);
	void ShowPointsScreen();
	void SortHighScoreList();
	void WriteHighScoreListToFile();
};