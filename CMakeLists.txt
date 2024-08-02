cmake_minimum_required(VERSION 3.0.0)
project(OpenGLFlightSimulator VERSION 0.2024.1)
set(CMAKE_BUILD_TYPE Release)

if(WIN32)
	set(OS_WINDOWS TRUE)
elseif(UNIX)
	set(OS_LINUX TRUE)
else()
	message(FATAL_ERROR "Platform Unsupported")
endif()


set(TARGET_TYPE )
# if(OS_WINDOWS)
# 	set(TARGET_TYPE WIN32)
# endif()


include_directories(include)

file(GLOB_RECURSE sources
	"include/*.hpp"
	"src/*.hpp"
	"src/*.cpp"
)

add_executable(opengl_flight_simulator ${TARGET_TYPE} ${sources})