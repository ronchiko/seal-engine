#pragma once

#include <stdio.h>
#include <stdlib.h>

#include <seal/debug.h>
#include <seal/ecs/component.h>
#include <seal/testing/suite.h>

#include "query.h"

typedef struct TestComponent {
	Seal_Component base;
	int integerComponent;
	const char *stringComponent;
	double floatComponent;
} TestComponent;