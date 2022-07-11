#include "Engine.h"
#include "Test.h"
#include "WindowHandler.h"

using std::cout, std::cin, std::endl;

void Engine::Run()
{
	cout << "Creating window" << endl;

	bool running{ true };
	while (running)
	{
		if (!WindowHandler::Get().ProcessMessages())
		{
			cout << "ClosingWindow" << endl;
			running = false;
		}

		//Render

		Sleep(10);
	}
}
