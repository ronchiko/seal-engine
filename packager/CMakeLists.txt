cmake_minimum_required(VERSION 3.21)

project(SealPackager VERSION 0.1)

configure_file(config.h.i config.h)

include_directories( 
	${PROJECT_BINARY_DIR} 
	include
	../include
)

file(GLOB_RECURSE SOURCE CONFIGURE_DEPENDS src/*.c)
add_library(seal-pkg-lib STATIC ${SOURCE})

add_executable(seal-pkg main.c)
target_link_libraries(seal-pkg PRIVATE seal-pkg-lib)


enable_testing()
add_subdirectory(tests)
