file(GLOB_RECURSE TESTS CONFIGURE_DEPENDS *.c)

make_directory("tests/test_folder/empty")

message(STATUS "Building tests for packager")
foreach(FILE ${TESTS})
	string(REGEX REPLACE "${CMAKE_SOURCE_DIR}" "" TEST ${FILE})
	string(REPLACE "/" "." TEST ${TEST})

	add_executable(${TEST} ${FILE})
	target_link_libraries(${TEST} PRIVATE SealFramework)
	target_include_directories(${TEST} PRIVATE include $ENV{SEAL_INCLUDE})

	add_test(Test-${TEST} ${TEST} ${CMAKE_CURRENT_LIST_DIR})
endforeach()
