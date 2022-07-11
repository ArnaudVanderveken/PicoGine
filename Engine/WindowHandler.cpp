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
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

WindowHandler::WindowHandler()
	: m_HInstance{ GetModuleHandle(nullptr) }
{
	WNDCLASS wndClass{};
	wndClass.lpszClassName = CLASS_NAME;
	wndClass.hInstance = m_HInstance;
	wndClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.lpfnWndProc = WindowProc;

	RegisterClass(&wndClass);

	DWORD style{ WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU };

	RECT rect;
	rect.left = 100;
	rect.top = 100;
	rect.right = rect.left + m_WindowWidth;
	rect.bottom = rect.top + m_WindowHeight;

	AdjustWindowRect(&rect, style, false);

	m_HWnd = CreateWindowEx(
		NULL,
		CLASS_NAME,
		L"PicoGine",
		style,
		rect.left,
		rect.top,
		rect.right - rect.left,
		rect.bottom - rect.top,
		NULL,
		NULL,
		m_HInstance,
		NULL
	);

	ShowWindow(m_HWnd, SW_SHOW);
}

WindowHandler::~WindowHandler()
{
	UnregisterClass(CLASS_NAME, m_HInstance);
}

bool WindowHandler::ProcessMessages()
{
	MSG msg{};

	while (PeekMessage(&msg, nullptr, 0u, 0u, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
			return false;

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return true;
}
