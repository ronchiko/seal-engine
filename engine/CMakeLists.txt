cmake_minimum_required(VERSION 3.21)

message(STATUS $ENV{SEAL_INCLUDE})

find_package(OpenGL REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(GLEW REQUIRED)

# Build seal library

file(GLOB_RECURSE Source CONFIGURE_DEPENDS src/*.c)
add_library(SealEngine STATIC ${Source})

target_include_directories(SealEngine PUBLIC $ENV{SEAL_INCLUDE})
target_link_libraries(SealEngine PUBLIC GLEW::GLEW glfw glu32 opengl32)

## Tests
enable_testing()
add_subdirectory(tests)

MoveExecutable(SealEngine lib engine)