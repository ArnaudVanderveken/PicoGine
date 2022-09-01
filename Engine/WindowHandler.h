#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "Singleton.h"

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

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
	[[nodiscard]] HWND GetHandle() const;
	[[nodiscard]] HINSTANCE GetInstance() const;

private:
	/* DATAMEMBERS */
	HINSTANCE m_HInstance;
	HWND m_HWnd;

	const wchar_t* CLASS_NAME{ L"PicoGineClass" };
	const unsigned short m_WindowWidth{ 1280 };
	const unsigned short m_WindowHeight{ 720 };

	/* PRIVATE METHODS */
};

