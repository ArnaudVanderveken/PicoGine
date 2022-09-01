#include "Engine.h"
#include "WindowHandler.h"

int main()
{
	Engine e;
	e.Run();
	return WindowHandler::Get().GetExitCode();
}