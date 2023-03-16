#pragma once

struct GameSettings
{
	inline static unsigned short windowTop{ 100 };
	inline static unsigned short windowLeft{ 100 };
	inline static unsigned short windowWidth{ 1280 };
	inline static unsigned short windowHeight{ 720 };
	inline static  float aspectRatio{ 16.f / 9.f };
	inline static bool useVSync{ true };
};