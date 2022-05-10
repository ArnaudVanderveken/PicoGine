cmake_minimum_required(VERSION 3.22)

project(PicoGine)
add_subdirectory(Engine)
add_subdirectory(Game)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Game)

message(STATUS "include directory: ${PROJECT_SOURCE_DIR}")

target_include_directories(Game PUBLIC "${PROJECT_BINARY_DIR}" "${EngineIncludeDir}")
	
install(TARGETS Engine DESTINATION bin)

include(InstallRequiredSystemLibraries)
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/License.txt")
set(CPACK_PACKAGE_VERSION_MAJOR "0")
set(CPACK_PACKAGE_VERSION_MINOR "1")

set(CPACK_NSIS_MODIFY_PATH ON)
include(CPack)