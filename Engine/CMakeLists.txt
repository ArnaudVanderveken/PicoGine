add_library(Engine 
	BaseComponent.h BaseComponent.cpp
	BaseMaterial.h
	CameraComponent.h CameraComponent.cpp
	CleanedWindows.h
	ColorMaterial.h ColorMaterial.cpp
	ColorVS.hlsl ColorPS.hlsl
	EnginePCH.h
	Engine.h Engine.cpp
	GameObject.h GameObject.cpp
	GameSettings.h
	GameScene.h GameScene.cpp
	InputManager.h InputManager.cpp
	MaterialManager.h MaterialManager.cpp
	PicoGineException.h PicoGineException.cpp
	Renderer.h Renderer.cpp
	SceneManager.h SceneManager.cpp
	Singleton.h
	Structs.h
	TimeManager.h TimeManager.cpp
	TestVS.hlsl TestPS.hlsl
	Transform.h Transform.cpp
	WindowsException.h WindowsException.cpp
	WindowHandler.h WindowHandler.cpp
)

file(GLOB_RECURSE VERTEX_SHADERS
    *VS.hlsl
)

file(GLOB_RECURSE PIXEL_SHADERS
	*PS.hlsl
)

file(GLOB_RECURSE GEOMETRY_SHADERS
	*GS.hlsl
)

file(GLOB_RECURSE MATERIAL_FILES
	*Material.h
	*Material.cpp
)

file(GLOB_RECURSE MANAGER_FILES
	*Manager.h
	*Manager.cpp
)

source_group("Shaders/Vertex Shaders" FILES ${VERTEX_SHADERS})
source_group("Shaders/Pixel Shaders" FILES ${PIXEL_SHADERS})
source_group("Shaders/Geometry Shaders" FILES ${GEOMETRY_SHADERS})
source_group("Materials" FILES ${MATERIAL_FILES})
source_group("Managers" FILES ${MANAGER_FILES})

set_source_files_properties(${VERTEX_SHADERS} PROPERTIES VS_SHADER_OBJECT_FILE_NAME "$(ProjectDir)/Shaders/%(Filename).cso" VS_SHADER_TYPE Vertex VS_SHADER_MODEL 5.0)
set_source_files_properties(${PIXEL_SHADERS} PROPERTIES VS_SHADER_OBJECT_FILE_NAME "$(ProjectDir)/Shaders/%(Filename).cso" VS_SHADER_TYPE Pixel VS_SHADER_MODEL 5.0)
set_source_files_properties(${GEOMETRY_SHADERS} PROPERTIES VS_SHADER_OBJECT_FILE_NAME "$(ProjectDir)/Shaders/%(Filename).cso" VS_SHADER_TYPE Geometry VS_SHADER_MODEL 5.0)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++latest /W4 /WX")
target_precompile_headers(Engine PUBLIC ./EnginePCH.h)
set(EngineIncludeDir "${CMAKE_CURRENT_SOURCE_DIR}" PARENT_SCOPE)