#pragma once
#include <chrono>
#include "Singleton.h"

namespace dae
{
	class Timer final : public dae::Singleton<Timer>
	{
	public:
		void Start();
		void Update();

		float GetTotalSec() const { return m_TotalSec; }
		float GetElapsedSec() const { return m_ElapsedSec; }
		std::chrono::high_resolution_clock::time_point GetPreviousTime() const { return m_PreviousTime; }

	private:
		float m_TotalSec{ 0.f };
		float m_ElapsedSec{ 0.f };
		std::chrono::high_resolution_clock::time_point m_PreviousTime;
		std::chrono::high_resolution_clock::time_point m_BeginTime;
	};
}