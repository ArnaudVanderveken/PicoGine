#pragma once
#include "Singleton.h"

#include <bitset>
#include <queue>

class InputManager final : public Singleton<InputManager>
{
public:
	class Keyboard
	{
		friend class WindowHandler;
	public:
		class Event
		{
		public:
			enum class Type
			{
				Pressed,
				Released,
				Invalid
			};

			Event() noexcept;
			Event(unsigned char keyCode, Type type) noexcept;
			~Event() = default;

			Event(const Event& other) = default;
			Event& operator=(const Event& other) noexcept = default;
			Event(Event&& other) = default;
			Event& operator=(Event&& other) noexcept = default;

			[[nodiscard]] bool IsPressed() const noexcept;
			[[nodiscard]] bool IsReleased() const noexcept;
			[[nodiscard]] bool IsValid() const noexcept;
			[[nodiscard]] unsigned char GetKeyCode() const noexcept;

		private:
			/* DATA MEMBERS */ 
			unsigned char m_KeyCode;
			Type m_Type;
		};

		Keyboard() noexcept = default;
		~Keyboard() = default;

		Keyboard(const Keyboard& other) = delete;
		Keyboard& operator=(const Keyboard& other) noexcept = delete;
		Keyboard(Keyboard&& other) = delete;
		Keyboard& operator=(Keyboard&& other) noexcept = delete;

		//Key Events
		[[nodiscard]] bool IsKeyPressed(unsigned char keyCode) const noexcept;
		Event PollKeyEvent() noexcept;
		[[nodiscard]] bool IsEventQueueEmpty() const noexcept;
		void FlushEventQueue() noexcept;

		//Char Events
		char ReadChar() noexcept;
		[[nodiscard]] bool IsCharQueueEmpty() const noexcept;
		void FlushCharQueue() noexcept;

		//Reset
		void FlushAll() noexcept;

		//Autorepeat controls
		void SetAutorepeat(bool enabled);
		[[nodiscard]] bool IsAutorepeatEnabled() const noexcept;
		

	private:
		/* DATA MEMBERS */
		static constexpr unsigned int KEY_COUNT{ 256u };
		static constexpr unsigned int BUFFER_SIZE{ 16u };

		bool m_AutorepeatEnabled{};
		std::bitset<KEY_COUNT> m_KeyStates{};
		std::queue<Event> m_KeyboardEvents{};
		std::queue<char> m_CharQueue{};

		/* PRIVATE METHODS */
		void OnKeyDown(unsigned char keyCode) noexcept;
		void OnKeyUp(unsigned char keyCode) noexcept;
		void OnChar(char character) noexcept;
		void TrimKeybordEventBuffer() noexcept;
		void TrimCharBuffer() noexcept;
		void ClearState() noexcept;
	};

	class Mouse
	{
		friend class WindowHandler;
	public:
		class Event
		{
		public:
			enum class Type
			{
				LPress,
				LRelease,
				RPress,
				RRelease,
				MPress,
				MRelease,
				WheelUp,
				WheelDown,
				Move,
				Enter,
				Leave,
				Invalid
			};
			
			Event() noexcept;
			Event(Type type, int x, int y) noexcept;
			~Event() = default;

			Event(const Event& other) noexcept = default;
			Event& operator=(const Event& other) noexcept = default;
			Event(Event&& other) noexcept = default;
			Event& operator=(Event&& other) noexcept = default;

			[[nodiscard]] bool IsValid() const noexcept;
			[[nodiscard]] Type GetType() const noexcept;
			[[nodiscard]] std::pair<int, int> GetPos() const noexcept;
			[[nodiscard]] int GetPosX() const noexcept;
			[[nodiscard]] int GetPosY() const noexcept;

		private:
			/* DATA MEMBERS */
			Type m_Type;
			int m_X;
			int m_Y;
		};

		Mouse() noexcept;
		~Mouse() = default;

		Mouse(const Mouse& other) = delete;
		Mouse& operator=(const Mouse& other) noexcept = delete;
		Mouse(Mouse&& other) = delete;
		Mouse& operator=(Mouse&& other) noexcept = delete;

		[[nodiscard]] std::pair<int, int> GetPos() const noexcept;
		[[nodiscard]] int GetPosX() const noexcept;
		[[nodiscard]] int GetPosY() const noexcept;
		[[nodiscard]] bool IsLeftPressed() const noexcept;
		[[nodiscard]] bool IsRightPressed() const noexcept;
		[[nodiscard]] bool IsMiddlePressed() const noexcept;
		[[nodiscard]] bool IsInWindow() const noexcept;
		Event Read() noexcept;
		[[nodiscard]] bool IsBufferEmpty() const noexcept;
		void Flush() noexcept;


	private:
		/* DATA MEMBERS */
		static constexpr unsigned int BUFFER_SIZE{ 16u };

		int m_X;
		int m_Y;
		bool m_IsLeftPressed;
		bool m_IsRightPressed;
		bool m_IsMiddlePressed;
		bool m_IsInWindow;
		int m_WheelDeltaCarry;
		std::queue<Event> m_MouseEvents;

		/* PRIVATE METHODS */
		void OnLeftPresed(int x, int y) noexcept;
		void OnLeftReleased(int x, int y) noexcept;
		void OnRightPressed(int x, int y) noexcept;
		void OnRightReleased(int x, int y) noexcept;
		void OnMiddlePressed(int x, int y) noexcept;
		void OnMiddleReleased(int x, int y) noexcept;
		void OnWheelUp(int x, int y) noexcept;
		void OnWheelDown(int x, int y) noexcept;
		void OnWheelDelta(int x, int y, int delta) noexcept;
		void OnMouseMove(int x, int y) noexcept;
		
		void OnMouseEnter() noexcept;
		void OnMouseLeave() noexcept;
		void TrimBuffer() noexcept;
	};

	InputManager() noexcept = default;
	~InputManager() override = default;

	InputManager(const InputManager& other) noexcept = delete;
	InputManager& operator=(const InputManager& other) noexcept = delete;
	InputManager(InputManager&& other) noexcept = delete;
	InputManager& operator=(InputManager&& other) noexcept = delete;

	[[nodiscard]] Keyboard& GetKeyboard() noexcept;
	[[nodiscard]] Mouse& GetMouse() noexcept;

private:
	/* DATA MEMBERS */
	Keyboard m_Keyboard{};
	Mouse m_Mouse{};

	/* PRIVATE METHODS */
	
};

