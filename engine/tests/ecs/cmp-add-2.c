
#include "test-common.h"

#include "seal/ecs/component.h"

TEST_MAIN {
	Seal_ComponentBuffer buffer = Seal_CreateComponentBuffer(sizeof(TestComponent));

	TestComponent compo = {
		.base = {
			.parentId = 200,
			.componentId = 100
		},
		.test1 = 10,
		.test2 = "Hello",
		.test3 = 0.3
	};

	Seal_AddComponent(buffer, &compo.base);
	compo.base.parentId++;
	Seal_AddComponent(buffer, &compo.base);
	Seal_RemoveComponent(buffer, compo.base.parentId);

	Seal_TrimComponentBuffer(buffer);

	FAIL_IF(_Seal_TestBufferSize(buffer, 1));

	Seal_FreeComponentBuffers();
	LEAK_CHECK;

	SUCCEED;
}