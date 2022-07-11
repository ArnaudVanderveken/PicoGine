#include "WindowHandler.h"

WindowHandler::WindowHandler()
	: m_hInstance{ GetModuleHandle(nullptr) }
{
}

WindowHandler::~WindowHandler()
{
}

bool WindowHandler::ProcessMessages()
{
	return false;
}
