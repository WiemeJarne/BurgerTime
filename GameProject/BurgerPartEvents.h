#pragma once
#include "BaseEvent.h"

class BurgerPartDropped1LevelEvent final : public dae::BaseEvent
{
public:
	BurgerPartDropped1LevelEvent() = default;
	~BurgerPartDropped1LevelEvent() = default;
};

class BurgerPartReachedPlateEvent final : public dae::BaseEvent
{
public:
	BurgerPartReachedPlateEvent() = default;
	~BurgerPartReachedPlateEvent() = default;
};

class BurgerPartDroppedWithEnemiesOnEvent final : public dae::BaseEvent
{
public:
	BurgerPartDroppedWithEnemiesOnEvent(int amountOfEnemies)
		: m_AmountOfEnemies{ amountOfEnemies }
	{}

	~BurgerPartDroppedWithEnemiesOnEvent() = default;

	int GetAmountOfEnemiesDropped() const { return m_AmountOfEnemies; }

private:
	const int m_AmountOfEnemies{};
};