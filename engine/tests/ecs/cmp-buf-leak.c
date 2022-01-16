
#include "test-common.h"

#include "seal/ecs/component.h"

TEST_MAIN {
	Seal_ComponentBuffer buffer = Seal_CreateComponentBuffer(sizeof(TestComponent));

	Seal_FreeComponentBuffers();
	LEAK_CHECK;

	SUCCEED;
}