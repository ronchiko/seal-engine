cmake_minimum_required(VERSION 3.21)

project(Seal_Framework VERSION 0.1)

find_package(JPEG REQUIRED)
find_package(libpng CONFIG REQUIRED)

file(GLOB_RECURSE SOURCE CONFIGURE_DEPENDS src/*.c)
message(STATUS ${SOURCE})

add_library(SealFramework STATIC ${SOURCE})

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
add_subdirectory(tests)