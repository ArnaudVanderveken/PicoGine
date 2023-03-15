add_library(Engine 
	BaseComponent.h BaseComponent.cpp
	CleanedWindows.h
	EnginePCH.h
	Engine.h Engine.cpp
	GameObject.h GameObject.cpp
	GameScene.h GameScene.cpp
	InputManager.h InputManager.cpp
	PicoGineException.h PicoGineException.cpp
	Renderer.h Renderer.cpp
	SceneManager.h SceneManager.cpp
	Singleton.h
	TimeManager.h TimeManager.cpp
	Transform.h Transform.cpp
	WindowsException.h WindowsException.cpp
	WindowHandler.h WindowHandler.cpp
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++latest /W4 /WX")
target_precompile_headers(Engine PUBLIC ./EnginePCH.h)
set(EngineIncludeDir "${CMAKE_CURRENT_SOURCE_DIR}" PARENT_SCOPE)