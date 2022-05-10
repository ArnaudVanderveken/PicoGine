add_executable(Game 
	main.cpp
)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++latest /W4 /WX")
target_precompile_headers(Game PUBLIC GamePCH.h)
target_link_libraries(Game PUBLIC Engine)