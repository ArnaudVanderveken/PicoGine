#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "Singleton.h"

class WindowHandler final : public Singleton<WindowHandler>
{
public:
	WindowHandler();
	~WindowHandler() override;
	WindowHandler(const WindowHandler&) = delete;
	WindowHandler& operator=(const WindowHandler&) = delete;
	WindowHandler(WindowHandler&&) noexcept = delete;
	WindowHandler& operator=(WindowHandler&&) noexcept = delete;

	bool ProcessMessages();

private:
	HINSTANCE m_hInstance;
	HWND m_hWnd;
};

