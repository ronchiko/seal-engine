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
	$ENV{SEAL_BIN_DIR}/$<CONFIG>/SealFramework.lib
)

## Benckmarks
add_executable(benchmark_ecs benchmarks/benchmark_ecs.c)
target_include_directories(benchmark_ecs PUBLIC $ENV{SEAL_BIN_DIR}/$<CONFIG>/SealFramework.lib)
target_link_libraries(benchmark_ecs PUBLIC $ENV{SEAL_BIN_DIR}/$<CONFIG>/SealFramework.lib SealEngine)

## Tests
enable_testing()
add_subdirectory(tests)

if (DEFINED SEAL_BUILD_GLOBAL)
	MoveExecutable(SealEngine lib engine)
endif()