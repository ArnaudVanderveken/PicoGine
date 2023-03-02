#include "Engine.h"

#include <thread>

#include "TimeManager.h"
#include "Renderer.h"
#include "WindowHandler.h"

using std::cout, std::endl;

void Engine::Run()
{
	cout << "Creating window" << endl;

	/* --- REFERENCES --- */
	auto& time = TimeManager::Get();
	auto& renderer = Renderer::Get();

	/* --- INITIALIZATION --- */
	time.Init();
	renderer.Init();
	//TODO init the scene

	bool running{ true };
	float lag{};
	while (running)
	{
		/* --- TIME --- */
		time.Update();

		/* --- INPUT --- */
		//TODO move this to an proper input manager
		//Process Windows messages
		if (!WindowHandler::Get().ProcessMessages())
		{
			cout << "ClosingWindow" << endl;
			running = false;
		}

		/* --- FIXED UPDATE --- */
		lag += time.GetElapsedTime();
		while (lag >= time.GetFixedTimeStep())
		{
			//TODO Fixed update the scene
			lag -= time.GetFixedTimeStep();
		}

		/* --- UPDATE --- */
		//TODO update the scene

		/* --- LATE UPDATE --- */
		//TODO late update the scene

		/* --- RENDER --- */
		renderer.BeginFrame();
		//TODO render the scene
		renderer.EndFrame();

		cout << 1.0f / time.GetElapsedTime() << endl;

		std::this_thread::sleep_for(TimeManager::Get().GetTimeToNextFrame());
	}
}