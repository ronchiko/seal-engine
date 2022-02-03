#pragma once

#include <stdio.h>
#include <stdlib.h>

#include <seal/debug.h>
#include <seal/ecs/component.h>
#include <seal/testing/suite.h>

#define ASSERT_LEAK if(Seal_LeakCheck()) {\
 printf("\033[31;1m"__FILE__":%d Memory leak detected\033[0m\n", __LINE__); Seal_DumpLeak(stdout);\
 result = 1; goto cleanup; }

typedef struct TestComponent {
	Seal_Component base;
	int integerComponent;
	const char *stringComponent;
	double floatComponent;
} TestComponent;

extern Seal_Bool _Seal_TestBufferSize(Seal_ComponentBuffer buffer, Seal_Size compareTo);