#include "Engine.h"

#include <thread>

#include "Renderer.h"
#include "SceneManager.h"
#include "TimeManager.h"
#include "WindowHandler.h"

using std::cout, std::endl;

void Engine::Run()
{
	cout << "Creating window" << endl;

	/* --- REFERENCES --- */
	auto& renderer = Renderer::Get();
	auto& sceneManager = SceneManager::Get();
	auto& time = TimeManager::Get();

	/* --- INITIALIZATION --- */
	renderer.Init();
	sceneManager.Init();
	time.Init();

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
			sceneManager.FixedUpdate();
			lag -= time.GetFixedTimeStep();
		}

		/* --- UPDATE --- */
		sceneManager.Update();

		/* --- LATE UPDATE --- */
		sceneManager.LateUpdate();

		/* --- RENDER --- */
		renderer.BeginFrame();
		sceneManager.Render();
		renderer.EndFrame();

		cout << 1.0f / time.GetElapsedTime() << endl;

		std::this_thread::sleep_for(TimeManager::Get().GetTimeToNextFrame());
	}
}