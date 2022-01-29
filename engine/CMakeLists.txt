cmake_minimum_required(VERSION 3.21)

project(SealEngineLib VERSION 0.1)

find_package(OpenGL REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(GLEW REQUIRED)
find_package(libzip CONFIG REQUIRED)

# Build seal library

file(GLOB_RECURSE Source CONFIGURE_DEPENDS src/*.c)
add_library(SealEngine STATIC ${Source})

target_include_directories(SealEngine PUBLIC $ENV{SEAL_INCLUDE})
target_link_libraries(SealEngine PUBLIC 
	GLEW::GLEW 
	glfw 
	glu32 
	opengl32
	libzip::zip
)

## Tests
enable_testing()
add_subdirectory(tests)

if (DEFINED SEAL_BUILD_GLOBAL)
	MoveExecutable(SealEngine lib engine)
endif()