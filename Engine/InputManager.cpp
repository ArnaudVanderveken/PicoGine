#include "InputManager.h"
#include "CleanedWindows.h"

InputManager::Keyboard::Event::Event() noexcept
	: m_KeyCode{ 0u }
	, m_Type{ Type::Invalid }
{
}

InputManager::Keyboard::Event::Event(unsigned char keyCode, Type type) noexcept
	: m_KeyCode{ keyCode }
	, m_Type{ type }
{
}

bool InputManager::Keyboard::Event::IsPressed() const noexcept
{
	return m_Type == Type::Pressed;
}

bool InputManager::Keyboard::Event::IsReleased() const noexcept
{
	return m_Type == Type::Released;
}

bool InputManager::Keyboard::Event::IsValid() const noexcept
{
	return m_Type != Type::Invalid;
}

unsigned char InputManager::Keyboard::Event::GetKeyCode() const noexcept
{
	return m_KeyCode;
}

bool InputManager::Keyboard::IsKeyPressed(unsigned char keyCode) const noexcept
{
	return m_KeyStates[keyCode];
}

InputManager::Keyboard::Event InputManager::Keyboard::PollKeyEvent() noexcept
{
	if (IsEventQueueEmpty())
		return {};

	const Event e = m_KeyboardEvents.front();
	m_KeyboardEvents.pop();
	return e;
}

bool InputManager::Keyboard::IsEventQueueEmpty() const noexcept
{
	return m_KeyboardEvents.empty();
}

void InputManager::Keyboard::FlushEventQueue() noexcept
{
	m_KeyboardEvents = std::queue<Event>{};
}

char InputManager::Keyboard::ReadChar() noexcept
{
	if (IsEventQueueEmpty())
		return 0;

	const char c = m_CharQueue.front();
	m_CharQueue.pop();
	return c;
}

bool InputManager::Keyboard::IsCharQueueEmpty() const noexcept
{
	return m_CharQueue.empty();
}

void InputManager::Keyboard::FlushCharQueue() noexcept
{
	m_CharQueue = std::queue<char>{};
}

void InputManager::Keyboard::FlushAll() noexcept
{
	FlushEventQueue();
	FlushCharQueue();
}

void InputManager::Keyboard::SetAutorepeat(bool isEnabled)
{
	m_AutorepeatEnabled = isEnabled;
}

bool InputManager::Keyboard::IsAutorepeatEnabled() const noexcept
{
	return m_AutorepeatEnabled;
}

void InputManager::Keyboard::OnKeyDown(unsigned char keyCode) noexcept
{
	m_KeyStates[keyCode] = true;
	m_KeyboardEvents.push(Event{ keyCode, Event::Type::Pressed });
	TrimKeybordEventBuffer();
}

void InputManager::Keyboard::OnKeyUp(unsigned char keyCode) noexcept
{
	m_KeyStates[keyCode] = true;
	m_KeyboardEvents.push(Event{ keyCode, Event::Type::Released });
	TrimKeybordEventBuffer();
}

void InputManager::Keyboard::OnChar(char character) noexcept
{
	m_CharQueue.push(character);
	TrimCharBuffer();
}

void InputManager::Keyboard::TrimKeybordEventBuffer() noexcept
{
	while(m_KeyboardEvents.size() > BUFFER_SIZE)
		m_KeyboardEvents.pop();
}

void InputManager::Keyboard::TrimCharBuffer() noexcept
{
	while (m_CharQueue.size() > BUFFER_SIZE)
		m_CharQueue.pop();
}

void InputManager::Keyboard::ClearState() noexcept
{
	m_KeyStates.reset();
}

InputManager::Mouse::Event::Event() noexcept
	: m_Type{ Type::Invalid }
	, m_X{}
	, m_Y{}
{
}

InputManager::Mouse::Event::Event(Type type, int x, int y) noexcept
	: m_Type{ type }
	, m_X{ x }
	, m_Y{ y }
{
}

bool InputManager::Mouse::Event::IsValid() const noexcept
{
	return m_Type != Type::Invalid;
}

InputManager::Mouse::Event::Type InputManager::Mouse::Event::GetType() const noexcept
{
	return m_Type;
}

std::pair<int, int> InputManager::Mouse::Event::GetPos() const noexcept
{
	return { m_X, m_Y };
}

int InputManager::Mouse::Event::GetPosX() const noexcept
{
	return m_X;
}

int InputManager::Mouse::Event::GetPosY() const noexcept
{
	return m_Y;
}

InputManager::Mouse::Mouse() noexcept
	: m_X{}
	, m_Y{}
	, m_IsLeftPressed{}
	, m_IsRightPressed{}
	, m_IsMiddlePressed{}
	, m_IsInWindow{}
	, m_WheelDeltaCarry{}
{
}

std::pair<int, int> InputManager::Mouse::GetPos() const noexcept
{
	return { m_X, m_Y };
}

int InputManager::Mouse::GetPosX() const noexcept
{
	return m_X;
}

int InputManager::Mouse::GetPosY() const noexcept
{
	return m_Y;
}

bool InputManager::Mouse::IsLeftPressed() const noexcept
{
	return m_IsLeftPressed;
}

bool InputManager::Mouse::IsRightPressed() const noexcept
{
	return m_IsRightPressed;
}

bool InputManager::Mouse::IsMiddlePressed() const noexcept
{
	return m_IsMiddlePressed;
}

bool InputManager::Mouse::IsInWindow() const noexcept
{
	return m_IsInWindow;
}

InputManager::Mouse::Event InputManager::Mouse::Read() noexcept
{
	if (IsBufferEmpty())
		return {};

	const Event e = m_MouseEvents.front();
	m_MouseEvents.pop();
	return e;
}

bool InputManager::Mouse::IsBufferEmpty() const noexcept
{
	return m_MouseEvents.empty();
}

void InputManager::Mouse::Flush() noexcept
{
	m_MouseEvents = std::queue<Event>{};
}

void InputManager::Mouse::OnLeftPresed(int x, int y) noexcept
{
	m_IsLeftPressed = true;
	m_MouseEvents.push(Event{ Event::Type::LPress, x, y });
	TrimBuffer();
}

void InputManager::Mouse::OnLeftReleased(int x, int y) noexcept
{
	m_IsLeftPressed = false;
	m_MouseEvents.push(Event{ Event::Type::LRelease, x, y });
	TrimBuffer();
}

void InputManager::Mouse::OnRightPressed(int x, int y) noexcept
{
	m_IsRightPressed = true;
	m_MouseEvents.push(Event{ Event::Type::RPress, x, y });
	TrimBuffer();
}

void InputManager::Mouse::OnRightReleased(int x, int y) noexcept
{
	m_IsRightPressed = false;
	m_MouseEvents.push(Event{ Event::Type::RRelease, x, y });
	TrimBuffer();
}

void InputManager::Mouse::OnMiddlePressed(int x, int y) noexcept
{
	m_IsMiddlePressed = true;
	m_MouseEvents.push(Event{ Event::Type::MPress, x, y });
	TrimBuffer();
}

void InputManager::Mouse::OnMiddleReleased(int x, int y) noexcept
{
	m_IsMiddlePressed = false;
	m_MouseEvents.push(Event{ Event::Type::MRelease, x, y });
	TrimBuffer();
}

void InputManager::Mouse::OnWheelUp(int x, int y) noexcept
{
	m_MouseEvents.push(Event{ Event::Type::WheelUp, x, y });
	TrimBuffer();
}

void InputManager::Mouse::OnWheelDown(int x, int y) noexcept
{
	m_MouseEvents.push(Event{ Event::Type::WheelDown, x, y });
	TrimBuffer();
}

void InputManager::Mouse::OnWheelDelta(int x, int y, int delta) noexcept
{
	m_WheelDeltaCarry += delta;
	while (m_WheelDeltaCarry >= WHEEL_DELTA)
	{
		m_WheelDeltaCarry -= WHEEL_DELTA;
		OnWheelUp(x, y);
	}
	while (m_WheelDeltaCarry <= -WHEEL_DELTA)
	{
		m_WheelDeltaCarry += WHEEL_DELTA;
		OnWheelDown(x, y);
	}
}

void InputManager::Mouse::OnMouseMove(int x, int y) noexcept
{
	m_X = x;
	m_Y = y;
	m_MouseEvents.push(Event{ Event::Type::Move, x, y });
	TrimBuffer();
}

void InputManager::Mouse::OnMouseEnter() noexcept
{
	m_IsInWindow = true;
	m_MouseEvents.push(Event{ Event::Type::Enter, m_X, m_Y });
	TrimBuffer();
}

void InputManager::Mouse::OnMouseLeave() noexcept
{
	m_IsInWindow = false;
	m_MouseEvents.push(Event{ Event::Type::Leave, m_X, m_Y });
	TrimBuffer();
}

void InputManager::Mouse::TrimBuffer() noexcept
{
	while (m_MouseEvents.size() > BUFFER_SIZE)
		m_MouseEvents.pop();
}

InputManager::Keyboard& InputManager::GetKeyboard() noexcept
{
	return m_Keyboard;
}

InputManager::Mouse& InputManager::GetMouse() noexcept
{
	return m_Mouse;
}
