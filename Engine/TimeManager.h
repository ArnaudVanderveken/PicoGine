#pragma once
#include <chrono>

#include "Singleton.h"

class TimeManager final : public Singleton<TimeManager>
{
public:
	void Init();
	void Update();

	[[nodiscard]] float GetElapsedTime() const;
	[[nodiscard]] float GetTotalTime() const;
	//[[nodiscard]] std::chrono::duration<float> GetTimeToNextFrame();
	[[nodiscard]] float GetFixedTimeStep() const;

private:
	friend class Singleton<TimeManager>;
	TimeManager() noexcept = default;

	const float m_FixedTimeStep{ 0.002f };
	float m_TimePerFrame{ 1.f / 144.f };

	float m_DeltaTime{};
	float m_TotalTime{};

	std::chrono::steady_clock::time_point m_StartTime{};
	std::chrono::steady_clock::time_point m_FrameBeginTime{};
	std::chrono::steady_clock::time_point m_LastFrameBeginTime{};
	std::chrono::steady_clock::time_point m_FrameEndTime{};
	
};

