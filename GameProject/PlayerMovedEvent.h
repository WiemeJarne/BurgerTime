#pragma once
#include "BaseEvent.h"
#include "glm/vec2.hpp"

class PlayerMovedEvent final : public dae::BaseEvent
{
public:
	PlayerMovedEvent(const glm::vec2& playerMiddlePos)
		: m_PlayerMiddlePos{ playerMiddlePos }
	{}

	~PlayerMovedEvent() = default;

	const glm::vec2& GetPlayerMiddlePos() const { return m_PlayerMiddlePos; }

private:
	const glm::vec2 m_PlayerMiddlePos{};
};