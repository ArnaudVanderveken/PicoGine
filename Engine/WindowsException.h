#pragma once
#include "CleanedWindows.h"
#include "PicoGineException.h"

#define PGWND_EXCEPTION(hr) WindowsException(__LINE__, __WFILE__, hr)
#define PGWND_THROW_IF_FAILED(hr) if (FAILED(hr)) throw WindowsException(__LINE__, __WFILE__, hr)
#define PGWND_LAST_EXCEPTION() WindowsException(__LINE__, __WFILE__, GetLastError())
#define BUFFER_SIZE 512

class WindowsException : public PicoGineException
{
public:
	WindowsException(int line, const wchar_t* file, HRESULT hr) noexcept;
	~WindowsException() override = default;

	WindowsException(const WindowsException& other) = delete;
	WindowsException& operator=(const WindowsException& other) noexcept = delete;
	WindowsException(WindowsException&& other) = delete;
	WindowsException& operator=(WindowsException&& other) noexcept = delete;

	[[nodiscard]] const wchar_t* wwhat() const noexcept override;
	[[nodiscard]] const wchar_t* GetType() const noexcept override;
	static std::wstring TranslateErrorCode(HRESULT hr) noexcept;
	HRESULT GetErrorCode() const noexcept;

protected:
private:
	/* DATA MEMBERS */
	HRESULT m_Hr;

	/* PRIVATE METHODS */
	
};

