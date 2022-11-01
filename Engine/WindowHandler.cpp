#include "WindowHandler.h"

#include "InputManager.h"
#include "WindowsException.h"


WindowHandler::WindowHandler()
	: m_HInstance{ GetModuleHandle(nullptr) }
	, m_HWnd{}
{
	//Register window class
	WNDCLASSEX wndClass{};
	wndClass.cbSize = sizeof(wndClass);
	wndClass.style = CS_OWNDC;
	wndClass.lpfnWndProc = MsgHandlerSetup;
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

	//Create Window Style
	constexpr DWORD style{ WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU };

	//Adjust Window Dimensions
	RECT wr{};
	wr.left = m_WindowLeft;
	wr.right = m_WindowLeft + m_WindowWidth;
	wr.top = m_WindowTop;
	wr.bottom = m_WindowTop + m_WindowHeight;
	if (!AdjustWindowRect(&wr, style, FALSE)) // Returns a BOOL, not an HRESULT
		throw PGWND_LAST_EXCEPTION();

	//Create Window
	m_HWnd = CreateWindowEx(
		NULL,
		m_ClassName,
		L"PicoGine",
		style,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		wr.right - wr.left,
		wr.bottom - wr.top,
		nullptr,
		nullptr,
		m_HInstance,
		this
	);
	if (!m_HWnd)
		throw PGWND_LAST_EXCEPTION();

	ShowWindow(m_HWnd, SW_SHOW);
}

WindowHandler::~WindowHandler()
{
	DestroyWindow(m_HWnd);
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

void WindowHandler::SetWindowTitle(const std::wstring& title) const
{
	if (SetWindowText(m_HWnd, title.c_str()) == 0)
		throw PGWND_LAST_EXCEPTION();
}

HWND WindowHandler::GetHandle() const noexcept
{
	return m_HWnd;
}

HINSTANCE WindowHandler::GetInstance() const noexcept
{
	return m_HInstance;
}

int WindowHandler::GetExitCode() const noexcept
{
	return m_ExitCode;
}

LRESULT CALLBACK WindowHandler::MsgHandlerSetup(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) noexcept
{
	//Executed once upon window creation
	if (uMsg == WM_NCCREATE)
	{
		//Extract pointer to WindowHandler class instance
		const CREATESTRUCTW* const pCreateStruct = reinterpret_cast<CREATESTRUCTW*>(lParam);
		auto* const pWnd = static_cast<WindowHandler*>(pCreateStruct->lpCreateParams);

		//Set WinAPI-managed user data to store that WindowHandler pointer
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));

		//Set WndProc to normal MessageHandler
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&WindowHandler::MsgHandlerAdapter));

		//Forward message to class MsgHandler
		return pWnd->MsgHandler(hWnd, uMsg, wParam, lParam);
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK WindowHandler::MsgHandlerAdapter(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) noexcept
{
	//Retrieve ptr to WindowHandler inst
	auto* const pWnd = reinterpret_cast<WindowHandler*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

	//Forward message to class MsgHandler
	return pWnd->MsgHandler(hWnd, uMsg, wParam, lParam);
}

LRESULT WindowHandler::MsgHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) noexcept
{
	POINTS ePos{};
	int delta;

	switch (uMsg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;

	case WM_KILLFOCUS:
		InputManager::Get().GetKeyboard().ClearState();
		break;

#pragma region KeyboardMessages
	case WM_SYSKEYDOWN:
	case WM_KEYDOWN:
		if (!(lParam & 0x40000000) || InputManager::Get().GetKeyboard().IsAutorepeatEnabled()) //Filter AutoRepeat with lParam bit 30
			InputManager::Get().GetKeyboard().OnKeyDown(static_cast<unsigned char>(wParam));
		break;

	case WM_SYSKEYUP:
	case WM_KEYUP:
		InputManager::Get().GetKeyboard().OnKeyUp(static_cast<unsigned char>(wParam));
		break;

	case WM_CHAR:
		InputManager::Get().GetKeyboard().OnChar(static_cast<char>(wParam));
		break;
#pragma endregion KeyboardMessages
#pragma region MouseMessages
	case WM_MOUSEMOVE:
		ePos = MAKEPOINTS(lParam);
		//Mouse in window
		if (ePos.x >= 0 && ePos.x < m_WindowWidth && ePos.y >= 0 && ePos.y < m_WindowHeight)
		{
			InputManager::Get().GetMouse().OnMouseMove(ePos.x, ePos.y);

			//If entered the window, start capture and log enter event
			if (!InputManager::Get().GetMouse().IsInWindow())
			{
				SetCapture(m_HWnd);
				InputManager::Get().GetMouse().OnMouseEnter();
			}
		}
		//Mouse not in window
		else
		{
			//If mouse button pressed, keep capture
			if (InputManager::Get().GetMouse().IsLeftPressed() || InputManager::Get().GetMouse().IsRightPressed())
				InputManager::Get().GetMouse().OnMouseMove(ePos.x, ePos.y);

			//Else release capture & log leave event
			else
			{
				ReleaseCapture();
				InputManager::Get().GetMouse().OnMouseLeave();
			}
		}
		break;

	case WM_LBUTTONDOWN:
		ePos = MAKEPOINTS(lParam);
		InputManager::Get().GetMouse().OnLeftPresed(ePos.x, ePos.y);
		break;

	case WM_LBUTTONUP:
		ePos = MAKEPOINTS(lParam);
		InputManager::Get().GetMouse().OnLeftReleased(ePos.x, ePos.y);
		break;

	case WM_RBUTTONDOWN:
		ePos = MAKEPOINTS(lParam);
		InputManager::Get().GetMouse().OnRightPressed(ePos.x, ePos.y);
		break;

	case WM_RBUTTONUP:
		ePos = MAKEPOINTS(lParam);
		InputManager::Get().GetMouse().OnRightReleased(ePos.x, ePos.y);
		break;

	case WM_MBUTTONDOWN:
		ePos = MAKEPOINTS(lParam);
		InputManager::Get().GetMouse().OnMiddlePressed(ePos.x, ePos.y);
		break;

	case WM_MBUTTONUP:
		ePos = MAKEPOINTS(lParam);
		InputManager::Get().GetMouse().OnMiddleReleased(ePos.x, ePos.y);
		break;

	case WM_MOUSEWHEEL:
		ePos = MAKEPOINTS(lParam);
		delta = GET_WHEEL_DELTA_WPARAM(wParam);
		InputManager::Get().GetMouse().OnWheelDelta(ePos.x, ePos.y, delta);
		break;

#pragma endregion MouseMessages

	default:
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
