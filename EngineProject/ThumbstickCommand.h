#pragma once
#include "Command.h"
#include <glm/vec2.hpp>

namespace dae
{
	class ThumbstickCommand : public Command
	{
	public:
		ThumbstickCommand() = default;
		virtual ~ThumbstickCommand() = default;

		virtual void Execute() override = 0;
		void SetThumbstickDirection(const glm::vec2& thumbStickDirection) { m_ThumbStickDirection = thumbStickDirection; }

	protected:
		glm::vec2 m_ThumbStickDirection{};
	};
}