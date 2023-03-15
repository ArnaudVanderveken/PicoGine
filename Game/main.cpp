#include "Engine.h"
#include "WindowHandler.h"
#include "WindowsException.h"

#include <vld.h>

int main()
{
	try
	{
		Engine e;
		e.Run();
		return WindowHandler::Get().GetExitCode();
	}
	catch (const PicoGineException& e)
	{
		MessageBox(nullptr, e.wwhat(), e.GetType(), MB_OK | MB_ICONEXCLAMATION);
	}
	catch (const std::exception& e)
	{
		MessageBoxA(nullptr, e.what(), "Standard Exception", MB_OK | MB_ICONEXCLAMATION);
	}
	catch (...)
	{
		MessageBoxA(nullptr, "No Details Available", "Unknown Exception", MB_OK | MB_ICONEXCLAMATION);
	}

	return -1;
}