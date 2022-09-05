#include "PicoGineException.h"
#include <sstream>

using std::wstring, std::endl;
typedef std::wstringstream wsstream;

PicoGineException::PicoGineException(int line, const wchar_t* file) noexcept
    : m_Line{ line }
	, m_File{ file }
{
}

const wchar_t* PicoGineException::wwhat() const noexcept
{
    wsstream wss;
    wss << GetType() << endl
        << GetOriginString();
    m_WhatBuffer = wss.str();
    return m_WhatBuffer.c_str();
}

const wchar_t* PicoGineException::GetType() const noexcept
{
    return L"PicoGine Exception";
}

int PicoGineException::GetLine() const noexcept
{
    return m_Line;
}

const std::wstring& PicoGineException::GetFile() const noexcept
{
    return m_File;
}

std::wstring PicoGineException::GetOriginString() const noexcept
{
    wsstream wss;
    wss << "[File] " << m_File << endl
        << "[Line] " << m_Line;
    return wss.str();
}
