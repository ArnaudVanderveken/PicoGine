add_library(Engine Engine.cpp)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++latest /W4 /WX")
set(EngineIncludeDir "${CMAKE_CURRENT_SOURCE_DIR}" PARENT_SCOPE)