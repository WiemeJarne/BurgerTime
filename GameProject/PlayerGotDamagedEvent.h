#pragma once
#include "BaseEvent.h"

class PlayerGotDamagedEvent final : public dae::BaseEvent
{
public:
	PlayerGotDamagedEvent(int damageAmount)
		: m_DamageAmount{ damageAmount }
	{}

	~PlayerGotDamagedEvent() = default;

private:
	const int m_DamageAmount{};
};