#pragma once

#include "CleanedWindows.h"
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
	void SetWindowTitle(const std::wstring& title) const;
	[[nodiscard]] HWND GetHandle() const noexcept;
	[[nodiscard]] HINSTANCE GetInstance() const noexcept;
	[[nodiscard]] int GetExitCode() const noexcept;

private:
	/* DATAMEMBERS */
	HINSTANCE m_HInstance;
	HWND m_HWnd;

	int m_ExitCode{};

	const wchar_t* m_ClassName{ L"PicoGineClass" };
	const unsigned short m_WindowTop;
	const unsigned short m_WindowLeft;
	const unsigned short m_WindowWidth;
	const unsigned short m_WindowHeight;

	/* PRIVATE METHODS */
	static LRESULT CALLBACK MsgHandlerSetup(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) noexcept;
	static LRESULT CALLBACK MsgHandlerAdapter(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) noexcept;
	LRESULT MsgHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) noexcept;
};

