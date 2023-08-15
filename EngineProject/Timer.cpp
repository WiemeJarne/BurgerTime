#include "Timer.h"

void dae::Timer::Start()
{
	m_BeginTime = std::chrono::high_resolution_clock::now();
	m_PreviousTime = m_BeginTime;
}

void dae::Timer::Update()
{
	const auto currentTime = std::chrono::high_resolution_clock::now();
	m_ElapsedSec = std::chrono::duration<float>(currentTime - m_PreviousTime).count();
	m_PreviousTime = currentTime;
	
	m_TotalSec += m_ElapsedSec;
}