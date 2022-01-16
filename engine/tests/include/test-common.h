#pragma once

#include <stdio.h>
#include <stdlib.h>

#include "seal/ecs/component.h"

#define FAIL exit(-1)
#define SUCCEED exit(0)

#define FAIL_IF(cond) if((cond) == SEAL_FALSE) FAIL;
#define IF_SUCCEED(cond) if((cond) == SEAL_FALSE) FAIL; SUCCEED;

#define TEST_MAIN int main(void)

#define LEAK_CHECK if(Seal_LeakCheck()) { Seal_DumpLeak(stdout); FAIL; }

typedef struct TestComponent {
	Seal_Component base;
	int test1;
	const char *test2;
	double test3;
} TestComponent;

extern Seal_Bool _Seal_TestBufferSize(Seal_ComponentBuffer buffer, Seal_Size compareTo);