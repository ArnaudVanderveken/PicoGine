#include "WindowHandler.h"

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	default:
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

WindowHandler::WindowHandler()
	: m_HInstance{ GetModuleHandle(nullptr) }
{
	//Register window class
	WNDCLASSEX wndClass{};
	wndClass.cbSize = sizeof(wndClass);
	wndClass.style = CS_OWNDC;
	wndClass.lpfnWndProc = WindowProc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = m_HInstance;
	wndClass.hIcon = nullptr;
	wndClass.hCursor = nullptr;
	wndClass.hbrBackground = nullptr;
	wndClass.lpszMenuName = nullptr;
	wndClass.lpszClassName = m_ClassName;
	wndClass.hIconSm = nullptr;

	RegisterClassEx(&wndClass);

	//Create Window
	constexpr DWORD style{ WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU };

	m_HWnd = CreateWindowEx(
		NULL,
		m_ClassName,
		L"PicoGine",
		style,
		m_WindowTop,
		m_WindowLeft,
		m_WindowWidth,
		m_WindowHeight,
		nullptr,
		nullptr,
		m_HInstance,
		nullptr
	);

	ShowWindow(m_HWnd, SW_SHOW);
}

WindowHandler::~WindowHandler()
{
	UnregisterClass(m_ClassName, m_HInstance);
}

bool WindowHandler::ProcessMessages()
{
	MSG msg{};
	BOOL msgResult;
	
	while ((msgResult = PeekMessage(&msg, nullptr, 0u, 0u, PM_REMOVE)) > 0)
	{
		if (msg.message == WM_QUIT)
		{
			m_ExitCode = int(msg.wParam);
			return false;
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	if (msgResult == -1)
	{
		m_ExitCode = -1;
		return false;
	}

	return true;
}

HWND WindowHandler::GetHandle() const
{
	return m_HWnd;
}

HINSTANCE WindowHandler::GetInstance() const
{
	return m_HInstance;
}

int WindowHandler::GetExitCode() const
{
	return m_ExitCode;
}
