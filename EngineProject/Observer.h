#pragma once

namespace dae
{
	template<typename T>
	class Observer
	{
	public:
		Observer() = default;
		virtual ~Observer() = default;
		Observer(const Observer&) = delete;
		Observer(Observer&&) = delete;
		Observer& operator=(const Observer&) = delete;
		Observer& operator=(Observer&&) = delete;

		virtual void OnNotify(T* pData, int eventId) = 0;
	};
}