## Requirements:
# CMAKE
# VCPKG
cmake_minimum_required(VERSION 3.21)

project(Seal-GE)

set(CMAKE_CONFIGURATION_TYPES "Debug;Release")

set(LIB_SOURCE_DIR ${PROJECT_SOURCE_DIR})
message(STATUS ${LIB_SOURCE_DIR})

function(MoveExecutable Target Extension Component) 
	## TODO: Fix for other platforms
	add_custom_command(TARGET ${Target} POST_BUILD
		COMMAND powershell.exe
		copy ${CMAKE_BINARY_DIR}/${Component}/$<CONFIG>/${Target}.${Extension} -D ${LIB_SOURCE_DIR}/lib/$<CONFIG>/${Target}.${Extension})
endfunction()

# Create lib directory
make_directory(lib)
make_directory(lib/Release)
make_directory(lib/Debug)

set(SEAL_BUILD_GLOBAL 1)

set($ENV:{SEAL_INCLUDE} ${CMAKE_ROOT}/include)
# Build subproject
add_subdirectory(framework)
add_subdirectory(engine)
add_subdirectory(packager)
add_subdirectory(cpp-bindings)