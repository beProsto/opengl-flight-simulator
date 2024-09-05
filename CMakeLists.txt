cmake_minimum_required(VERSION 3.0.0)
project(OpenGLFlightSimulator VERSION 0.2024.1)
set(CMAKE_BUILD_TYPE Release)
set(appname opengl_flight_simulator)

if(WIN32)
	set(OS_WINDOWS TRUE)
elseif(UNIX)
	set(OS_LINUX TRUE)
else()
	message(FATAL_ERROR "Platform Unsupported")
endif()

set(target_type )
# if(OS_WINDOWS)
# 	set(TARGET_TYPE WIN32)
# endif()

add_subdirectory(ext/assimp)
link_libraries(assimp)

set(BUILD_EXAMPLES NO)
add_subdirectory(ext/OWL)
link_libraries(OWL)

set(glad_source ext/glad/src/glad.c)


include_directories(include)
include_directories(ext/assimp/include)
include_directories(ext/OWL/include)
include_directories(ext/glad/include)

file(GLOB_RECURSE sources
	"include/*.hpp"
	"src/*.hpp"
	"src/*.cpp"
)
add_executable(${appname} ${target_type} ${sources} ${glad_source})

add_custom_command(TARGET ${appname} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "$<TARGET_FILE:OWL>"
    $<TARGET_FILE_DIR:${appname}>
)
add_custom_command(TARGET ${appname} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "$<TARGET_FILE:assimp>"
    $<TARGET_FILE_DIR:${appname}>
)


# This only needs to happen because this is a game lol

file(GLOB_RECURSE resources
    "resources/*.obj"
    "resources/*.mtl"
)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/res)
file(MAKE_DIRECTORY res)
add_custom_command(TARGET ${appname} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${resources}
    ${CMAKE_CURRENT_BINARY_DIR}/res
)
add_custom_command(TARGET ${appname} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${resources}
    ${CMAKE_CURRENT_SOURCE_DIR}/res
)