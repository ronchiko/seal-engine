cmake_minimum_required(VERSION 3.21)

project(SealMain VERSION 0.1)

find_package(OpenGL REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(GLEW REQUIRED)
find_package(libzip CONFIG REQUIRED)

set(CMAKE_CXX_STANDARD 20)

# Build seal library

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS *.cpp)

add_library(SealMain STATIC ${SOURCES})

target_include_directories(SealMain PUBLIC $ENV{SEAL_INCLUDE})
target_link_libraries(SealMain PUBLIC 
	GLEW::GLEW 
	glfw 
	glu32 
	opengl32
	libzip::zip
	$ENV{SEAL_BIN_DIR}/$<CONFIG>/SealFramework.lib
	$ENV{SEAL_BIN_DIR}/$<CONFIG>/SealEngine.lib
	$ENV{SEAL_BIN_DIR}/$<CONFIG>/SealCpp.lib
)

if (DEFINED SEAL_BUILD_GLOBAL)
	MoveExecutable(SealMain lib engine-main)
endif()