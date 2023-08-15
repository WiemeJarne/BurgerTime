#pragma once
#include "Observer.h"
#include <vector>

namespace dae
{
	template<typename T>
	class Subject
	{
	public:
		Subject() = default;
		virtual ~Subject() = default;
		Subject(const Subject&) = delete;
		Subject(Subject&&) = delete;
		Subject& operator=(const Subject&) = delete;
		Subject& operator=(Subject&&) = delete;

		void AddObserver(Observer<T>* observer)
		{
			//look if the observer is already in the vector
			for (const auto& otherObserver : m_pObservers)
			{
				if (otherObserver == observer)
					return;
			}

			//if not add it to the vector
			m_pObservers.push_back(observer);
		}

		void RemoveObserver(Observer<T>* observer)
		{
			//look if the observer is in the vector if it is there delete it
			for (auto& otherObserver : m_pObservers)
			{
				if (otherObserver == observer)
				{
					delete otherObserver;
					otherObserver = nullptr;
					break;
				}
			}

			//remove the nullptr form the vector
			m_pObservers.erase(std::remove(m_pObservers.begin(), m_pObservers.end(), nullptr), m_pObservers.end());
		}

	protected:
		void Notify(T* pData, int eventId)
		{
			for (int index{}; index < m_pObservers.size(); ++index)
			{
				m_pObservers[index]->OnNotify(pData, eventId);
			}
		}

	private:
		std::vector<Observer<T>*> m_pObservers{};
	};
}