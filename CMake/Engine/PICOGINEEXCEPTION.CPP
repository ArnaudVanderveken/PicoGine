#include "PicoGineException.h"
#include <sstream>

using std::string, std::endl;
typedef std::stringstream sstream;

PicoGineException::PicoGineException(int line, const char* file) noexcept
    : m_Line{ line }
	, m_File{ file }
{
}

const char* PicoGineException::what() const noexcept
{
    sstream ss;
    ss << GetType() << endl
        << GetOriginString();
    m_WhatBuffer = ss.str();
    return m_WhatBuffer.c_str();
}

const char* PicoGineException::GetType() const noexcept
{
    return "PicoGine Exception";
}

int PicoGineException::GetLine() const noexcept
{
    return m_Line;
}

const std::string& PicoGineException::GetFile() const noexcept
{
    return m_File;
}

std::string PicoGineException::GetOriginString() const noexcept
{
    sstream ss;
    ss << "[File] " << m_File << endl
        << "[Line] " << m_Line;
    return ss.str();
}
