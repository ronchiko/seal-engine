cmake_minimum_required(VERSION 3.21)

project(SealPackager VERSION 0.1)

find_package(libzip CONFIG REQUIRED)
configure_file(config.h.i config.h)

include_directories( 
	${PROJECT_BINARY_DIR} 
	include
	$ENV{SEAL_INCLUDE}
)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.c)
list(FILTER SOURCES EXCLUDE REGEX "test_.*\.c")

add_library(seal-pkg-lib STATIC 
	${SOURCES}
)

add_executable(seal-pkg main.c)
target_link_libraries(seal-pkg PRIVATE 
	seal-pkg-lib 
	libzip::zip
	$ENV{SEAL_BIN_DIR}/$<CONFIG>/SealFramework.lib
)

if (DEFINED SEAL_BUILD_GLOBAL)
	MoveExecutable(seal-pkg-lib lib packager)
	MoveExecutable(seal-pkg 	exe packager)
endif()

# Tests
enable_testing()
file(GLOB_RECURSE TESTS CONFIGURE_DEPENDS test_*.c)

message(STATUS "Building tests for packager")
foreach(FILE ${TESTS})
	string(REGEX REPLACE "${CMAKE_SOURCE_DIR}" "" TEST ${FILE})
	string(REPLACE "/" "." TEST ${TEST})
	string(REPLACE "test_" "" TEST ${TEST})

	add_executable(test${TEST} ${FILE})
	target_link_libraries(test${TEST} PRIVATE seal-pkg-lib)
	target_include_directories(test${TEST} PRIVATE include)

	add_test(test${TEST} test${TEST} ${CMAKE_CURRENT_LIST_DIR})
endforeach()
