#pragma once
#include "BaseEvent.h"

class PlayerDiedEvent final : public dae::BaseEvent
{
public:
	PlayerDiedEvent() = default;
	~PlayerDiedEvent() = default;
};