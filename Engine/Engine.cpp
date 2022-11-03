#include "Engine.h"

#include "Renderer.h"
#include "WindowHandler.h"

using std::cout, std::endl;

void Engine::Run()
{
	cout << "Creating window" << endl;

	//Initialization
	Renderer::Get().Init();

	bool running{ true };
	while (running)
	{
		//Process Windows messages
		if (!WindowHandler::Get().ProcessMessages())
		{
			cout << "ClosingWindow" << endl;
			running = false;
		}

		//Render
		Renderer::Get().BeginFrame();
		Renderer::Get().EndFrame();

		Sleep(10);
	}
}