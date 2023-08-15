#pragma once
#include "Singleton.h"
#include "EventQueue.h"
#include "BaseEvent.h"
#include <unordered_map>
#include <typeindex>
#include <iostream>

namespace dae
{
	class EventQueueManager final : public Singleton<EventQueueManager>
	{
	public:
		~EventQueueManager()
		{
			m_pvEventQueues.clear();
			m_pvNewEventQueues.clear();
		}

		template <typename EventType>
		void AddEvent(std::unique_ptr<EventType> pEvent)
		{
			GetEventQueue<EventType>()->AddEvent(std::move(pEvent));
			m_EventWasAdded = true;
		}
	
		template <typename EventType>
		void AddListener(EventListener<EventType>* pEventListener)
		{
			GetEventQueue<EventType>()->AddListener(pEventListener);
		}
	
		template <typename EventType>
		void RemoveListener(EventListener<EventType>* pEventListener)
		{
			auto queue = GetEventQueue<EventType>(false);

			if (queue)
			{
				queue->RemoveListener(pEventListener);
				m_EventWasAdded = true;
			}
		}
	
		template <typename EventType>
		EventQueue<EventType>* GetEventQueue(bool makeNewQueue = true) //when a queue is not found a new queue is made if this is set to false then nullptr is returned
		{
			static_assert(std::is_base_of<BaseEvent, EventType>::value, "EventType should be derived from dae::BaseEvent");

			if (m_pvEventQueues.empty() && m_pvNewEventQueues.empty() && !makeNewQueue)
				return nullptr;

			//search for the queue using std::type_index of the queues in the unordered_map
			auto queue = m_pvEventQueues.find(typeid(EventQueue<EventType>));

			//check if the queue was found
			if (queue == m_pvEventQueues.end())
			{
				//queue was not found in the existing queues so check if it is waiting to be added
				queue = m_pvNewEventQueues.find(typeid(EventQueue<EventType>));

				//check if the queue was found
				if (queue == m_pvNewEventQueues.end())
				{
					//queue was not found so create a new one and return it
					auto newEventQueue = std::make_unique<EventQueue<EventType>>();
					m_pvNewEventQueues.emplace(typeid(EventQueue<EventType>), reinterpret_cast<EventQueue<BaseEvent>*>(newEventQueue.release()));
					return reinterpret_cast<EventQueue<EventType>*>(m_pvNewEventQueues[typeid(EventQueue<EventType>)].get());
				}

				//queue was found in the queues waiting to be added to the rest of the queues so return it
				return reinterpret_cast<EventQueue<EventType>*>(queue->second.get());
			}

			//queue was found so return it
			return reinterpret_cast<EventQueue<EventType>*>(queue->second.get());
		}
	
		void HandleEventQueues();
	
	private:
		std::unordered_map<std::type_index, std::unique_ptr<EventQueue<BaseEvent>>> m_pvEventQueues;
		std::unordered_map<std::type_index, std::unique_ptr<EventQueue<BaseEvent>>> m_pvNewEventQueues;
		bool m_EventWasAdded{};
	};
}