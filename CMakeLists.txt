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


set(BUILD_EXAMPLES NO)
add_subdirectory(ext/OWL)
link_libraries(OWL)

set(target_type )
# if(OS_WINDOWS)
# 	set(TARGET_TYPE WIN32)
# endif()


include_directories(include)
include_directories(ext/OWL/include)

file(GLOB_RECURSE sources
	"include/*.hpp"
	"src/*.hpp"
	"src/*.cpp"
)
add_executable(${appname} ${target_type} ${sources})