#pragma once

namespace dae
{
	template <typename EventType>
	class EventListener
	{
	public:
		EventListener() = default;

		virtual ~EventListener() = default;

		virtual void OnNotify(const EventType* event) = 0;
	};
}