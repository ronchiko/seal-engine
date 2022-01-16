file(GLOB_RECURSE TESTS CONFIGURE_DEPENDS *.c)

message(STATUS "Building tests for packager")
foreach(FILE ${TESTS})
	string(REGEX REPLACE "${CMAKE_SOURCE_DIR}" "" TEST ${FILE})
	string(REPLACE "/" "." TEST ${TEST})

	add_executable(${TEST} ${FILE})
	target_link_libraries(${TEST} PRIVATE seal-pkg-lib)
	target_include_directories(${TEST} PRIVATE include)

	add_test(Test-${TEST} ${TEST})
endforeach()

