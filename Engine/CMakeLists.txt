add_library(Engine 
	EnginePCH.h
	Engine.h Engine.cpp
	Singleton.h
	Test.h Test.cpp
	WindowHandler.h WindowHandler.cpp
)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++latest /W4 /WX")
target_precompile_headers(Engine PUBLIC ./EnginePCH.h)
set(EngineIncludeDir "${CMAKE_CURRENT_SOURCE_DIR}" PARENT_SCOPE)