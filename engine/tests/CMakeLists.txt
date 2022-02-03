
file(GLOB_RECURSE Tests CONFIGURE_DEPENDS *.c)

message(STATUS "Generating tests ${Tests}")
foreach(File ${Tests})
	string(REGEX REPLACE "${CMAKE_SOURCE_DIR}" "" Testname ${File})
	string(REPLACE "/" "." Testname ${Testname})

	add_executable(${Testname} ${File})
	target_link_libraries(${Testname} PRIVATE 
		SealEngine
		$ENV{SEAL_BIN_DIR}/$<CONFIG>/SealFramework.lib
	)
	target_include_directories(${Testname} PRIVATE $ENV{SEAL_INCLUDE} include)
	
	message(STATUS "Adding ${Testname} - ${File}")
	add_test(Test-${Testname} ${Testname})
endforeach()