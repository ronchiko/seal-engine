cmake_minimum_required(VERSION 3.21)

project(Seal_Cpp VERSION 0.1)

set(CMAKE_CXX_STANDARD 20)

find_package(JPEG REQUIRED)
find_package(libpng CONFIG REQUIRED)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)
list(FILTER SOURCES EXCLUDE REGEX "test_.*\.cpp")

add_library(SealCpp STATIC ${SOURCES})

target_include_directories(SealCpp PRIVATE
	$ENV{SEAL_INCLUDE}
	${JPEG_INCLUDE_DIR}
)

target_link_libraries(SealCpp PRIVATE 
	SealFramework
	SealEngine
)

if (DEFINED SEAL_BUILD_GLOBAL)
	MoveExecutable(SealCpp lib cpp-bindings)
endif()

enable_testing()
file(GLOB_RECURSE TESTS CONFIGURE_DEPENDS test_*.cpp)

message(STATUS "Building tests for cpp-bindings")
foreach(FILE ${TESTS})
	string(REGEX REPLACE "${CMAKE_SOURCE_DIR}" "" TEST ${FILE})
	string(REPLACE "/" "." TEST ${TEST})

	add_executable(test${TEST} ${FILE})
	target_link_libraries(test${TEST} PRIVATE SealCpp)
	target_include_directories(test${TEST} PRIVATE include $ENV{SEAL_INCLUDE})

	add_test(test${TEST} test${TEST} ${CMAKE_CURRENT_LIST_DIR})
endforeach()