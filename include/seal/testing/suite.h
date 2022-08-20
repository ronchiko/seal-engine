#pragma once

#include <stdio.h>
#include <stdlib.h>

#include <seal/debug.h>

#ifdef __cplusplus
extern "C" {
#endif

// Mocks
const char *Seal_ProgramName = "Test";
int Seal_InitializeUserSystems(void) { return 0; }
int Seal_ActivateUserSystems(void) { return 0; }

// Utils
#define LEN(arr) (sizeof(arr) / sizeof(arr[0]))
#define STREQ(a, b) (strcmp((a), (b)) == 0)

#define ASSERT(expr) if(!(expr)) { printf("\033[31;1m"__FILE__":%d raised assert failed\nFailure: "#expr"\033[0m\n", __LINE__); \
	result = 1; goto cleanup; }
#define RUN_TEST(name) printf("Running test "#name"\n"); result |= (name)();  

#if  defined(_WIN32) || defined(_WIN64)
#include <windows.h>

#define WORKDIR(w) SetCurrentDirectory((w))
#endif

static int ArrayContainsStr(const char ** array, int len, const char *str) {
	for(int i = 0; i < len; ++i) {
		if(STREQ(array[i], str))
			return 1;
	}

	return 0;
}

#define ASSERT_LEAK if(Seal_LeakCheck()) {\
 printf("\033[31;1m"__FILE__":%d Memory leak detected\033[0m\n", __LINE__); Seal_DumpLeak(stdout);\
 result = 1; }

#ifdef __cplusplus
}
#endif
