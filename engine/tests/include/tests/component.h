#pragma once

#include <stdio.h>
#include <stdlib.h>

#include <seal/debug.h>
#include <seal/ecs/component.h>
#include <seal/testing/suite.h>

typedef struct TestComponent {
	Seal_Component base;
	int integerComponent;
	const char *stringComponent;
	double floatComponent;
} TestComponent;

extern Seal_Bool _Seal_TestBufferSize(Seal_ComponentBuffer buffer, Seal_Size compareTo);