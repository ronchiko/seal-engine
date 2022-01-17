#pragma once

#include <stdio.h>

#define LEN(arr) (sizeof(arr) / sizeof(arr[0]))
#define STREQ(a, b) (strcmp((a), (b)) == 0)

#define ASSERT(expr) if(!(expr)) { printf("\033[31;1m"__FILE__":%d raised assert failed\nFailure: "#expr"\033[0m\n", __LINE__); \
	result = 1; goto cleanup; }
#define RUN_TEST(name) printf("Running test "#name"\n"); result |= (name)(); 