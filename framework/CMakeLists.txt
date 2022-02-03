cmake_minimum_required(VERSION 3.21)

project(Seal_Framework VERSION 0.1)

file(GLOB_RECURSE SOURCE CONFIGURE_DEPENDS src/*.c)
message(STATUS ${SOURCE})

add_library(SealFramework STATIC ${SOURCE})

target_include_directories(
	SealFramework PUBLIC
	$ENV{SEAL_INCLUDE}
)

if (DEFINED SEAL_BUILD_GLOBAL)
	MoveExecutable(SealFramework lib framework)
endif()

enable_testing()
add_subdirectory(tests)