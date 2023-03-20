#include "Engine.h"

//#include <thread>

#include "Renderer.h"
#include "SceneManager.h"
#include "TimeManager.h"
#include "WindowHandler.h"

using std::cout, std::endl;
using std::chrono::duration_cast, std::chrono::milliseconds;

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

	/* --- GAME LOOP --- */
	bool running{ true };
	float lag{};
	const float fixedTimeStep{ time.GetFixedTimeStep() };
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
		while (lag >= fixedTimeStep)
		{
			sceneManager.FixedUpdate();
			lag -= fixedTimeStep;
		}

		/* --- UPDATE --- */
		sceneManager.Update();

		/* --- LATE UPDATE --- */
		sceneManager.LateUpdate();

		/* --- RENDER --- */
		renderer.BeginFrame();
		sceneManager.Render();
		renderer.RenderTestTriangle();
		renderer.EndFrame();

		//std::this_thread::sleep_for(duration_cast<milliseconds>(time.GetTimeToNextFrame()));
	}
}