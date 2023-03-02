#include "TimeManager.h"

using namespace std::chrono;

void TimeManager::Init()
{
	m_StartTime = high_resolution_clock::now();
	m_FrameBeginTime = high_resolution_clock::now();
}

void TimeManager::Update()
{
	m_LastFrameBeginTime = m_FrameBeginTime;
	m_FrameBeginTime = high_resolution_clock::now();
	m_DeltaTime = duration<float>(m_FrameBeginTime - m_LastFrameBeginTime).count();
	m_TotalTime = duration<float>(m_FrameBeginTime - m_StartTime).count();
}

float TimeManager::GetElapsedTime() const
{
	return m_DeltaTime;
}

float TimeManager::GetTotalTime() const
{
	return m_TotalTime;
}

std::chrono::duration<float> TimeManager::GetTimeToNextFrame()
{
	m_CurrentTime = high_resolution_clock::now();
	return duration<float>(m_TimePerFrame - duration<float>(m_CurrentTime - m_FrameBeginTime).count());
}

float TimeManager::GetFixedTimeStep() const
{
	return m_FixedTimeStep;
}
