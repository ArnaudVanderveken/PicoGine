#pragma once

#include "DirectXMath.h"

struct GameSettings
{
	enum class RenderAPI
	{
		DirectX11,
		DirectX12
	};

	inline static unsigned short windowTop{ 100 };
	inline static unsigned short windowLeft{ 100 };
	inline static unsigned short windowWidth{ 1280 };
	inline static unsigned short windowHeight{ 720 };
	inline static float aspectRatio{ 16.f / 9.f };
	inline static float fov{ DirectX::XM_PIDIV4 };
	inline static float nearPlane{ .1f };
	inline static float farPlane{ 3000.f };
	inline static bool useVSync{ true };
	inline static RenderAPI renderAPI{ RenderAPI::DirectX11 };
};