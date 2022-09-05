#include "WindowsException.h"
#include <sstream>

using std::wstring, std::endl;
typedef std::wstringstream wsstream;

WindowsException::WindowsException(int line, const wchar_t* file, HRESULT hr) noexcept
	: PicoGineException(line, file)
	, m_Hr{ hr }
{
}

const wchar_t* WindowsException::wwhat() const noexcept
{
	wsstream wss;
	wss << GetType() << endl
		<< "[Error Code] " << GetErrorCode() << endl
		<< "[Description] " << TranslateErrorCode(m_Hr) << endl
		<< GetOriginString();
	m_WhatBuffer = wss.str();
	return m_WhatBuffer.c_str();
}

const wchar_t* WindowsException::GetType() const noexcept
{
	return L"PicoGine Windows Exception";
}

std::wstring WindowsException::TranslateErrorCode(HRESULT hr) noexcept
{
	wchar_t msgBuffer[BUFFER_SIZE];
	const DWORD msgLen = FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
	                                   nullptr,
	                                   hr,
	                                   0,
	                                   msgBuffer,
								       BUFFER_SIZE,
	                                   nullptr);

	wstring outStr = msgLen == 0 ? L"Unidentified error code" : msgBuffer;
	return outStr;
}

HRESULT WindowsException::GetErrorCode() const noexcept
{
	return m_Hr;
}
