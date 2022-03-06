cmake_minimum_required(VERSION 3.21)

project(SealEngineLib VERSION 0.1)

find_package(OpenGL REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(GLEW REQUIRED)
find_package(libzip CONFIG REQUIRED)

# Build seal library

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.c)
list(FILTER SOURCES EXCLUDE REGEX "test_")

add_library(SealEngine STATIC ${SOURCES})

target_include_directories(SealEngine PUBLIC $ENV{SEAL_INCLUDE})
target_link_libraries(SealEngine PUBLIC 
	GLEW::GLEW 
	glfw 
	glu32 
	opengl32
	libzip::zip
	$ENV{SEAL_BIN_DIR}/$<CONFIG>/SealFramework.lib
)

## Tests
enable_testing()
file(GLOB_RECURSE TESTS CONFIGURE_DEPENDS test_*.c)

message(STATUS "Building tests for engine")
foreach(FILE ${TESTS})
	string(REGEX REPLACE "${CMAKE_SOURCE_DIR}" "" TEST ${FILE})
	string(REPLACE "/" "." TEST ${TEST})

	add_executable(test${TEST} ${FILE})
	target_link_libraries(test${TEST} PRIVATE 
		$ENV{SEAL_BIN_DIR}/$<CONFIG>/SealFramework.lib
		SealEngine
	)
	target_include_directories(test${TEST} PRIVATE include $ENV{SEAL_INCLUDE})

	add_test(test${TEST} test${TEST} ${CMAKE_CURRENT_LIST_DIR})
endforeach()

if (DEFINED SEAL_BUILD_GLOBAL)
	MoveExecutable(SealEngine lib engine)
endif()