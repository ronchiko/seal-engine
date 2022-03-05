cmake_minimum_required(VERSION 3.21)

project(Seal_Framework VERSION 0.1)

find_package(JPEG REQUIRED)
find_package(libpng CONFIG REQUIRED)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.c)
list(FILTER SOURCES EXCLUDE REGEX "test_.*\.c")

add_library(SealFramework STATIC ${SOURCES})

target_include_directories(
	SealFramework PRIVATE
	$ENV{SEAL_INCLUDE}
	${JPEG_INCLUDE_DIR}
)

target_link_libraries(SealFramework PRIVATE 
	png 
	${JPEG_LIBRARIES}
)

if (DEFINED SEAL_BUILD_GLOBAL)
	MoveExecutable(SealFramework lib framework)
endif()

enable_testing()
file(GLOB_RECURSE TESTS CONFIGURE_DEPENDS test_*.c)

make_directory("test_folder/empty")

message(STATUS "Building tests for framework")
foreach(FILE ${TESTS})
	string(REGEX REPLACE "${CMAKE_SOURCE_DIR}" "" TEST ${FILE})
	string(REPLACE "/" "." TEST ${TEST})

	add_executable(test${TEST} ${FILE})
	target_link_libraries(test${TEST} PRIVATE SealFramework)
	target_include_directories(test${TEST} PRIVATE include $ENV{SEAL_INCLUDE})

	add_test(test${TEST} test${TEST} ${CMAKE_CURRENT_LIST_DIR})
endforeach()