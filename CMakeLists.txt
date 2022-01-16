cmake_minimum_required(VERSION 3.21)

set(CMAKE_CONFIGURATION_TYPES "Debug;Release")

function(MoveExecutable Target Extension Component) 
	## TODO: Fix for other platforms
	add_custom_command(TARGET ${Target} POST_BUILD
		COMMAND powershell.exe
		copy ${CMAKE_BINARY_DIR}/${Component}/$<CONFIG>/${Target}.${Extension} -D ${PROJECT_SOURCE_DIR}/lib/$<CONFIG>/${Target}.${Extension})
endfunction()

project(Seal-GE)

## Requirements:
# CMAKE
# VCPKG

# Create lib directory
make_directory(lib)
make_directory(lib/Release)
make_directory(lib/Debug)

set($ENV:{SEAL_INCLUDE} ${CMAKE_ROOT}/include)
# Build subproject
add_subdirectory(engine)
add_subdirectory(packager)