
#include "test-common.h"

#include "seal/ecs/component.h"

TEST_MAIN {
	Seal_ComponentBuffer buffer1 = Seal_CreateComponentBuffer(sizeof(TestComponent));

	// Populate buffers
	for(int i = 0; i < 20; ++i) {
		Seal_Entity entity = Seal_CreateEntity();
		
		TestComponent c1;
		Seal_AttachComponent(entity, &c1.base);
		Seal_AddComponent(buffer1, &c1.base);

	}

	struct Seal_IterativeBuffer buffers[1] = {
		Seal_BufferOf(buffer1),
	};

	Seal_QueryContext ctx = {
		.buffers = buffers,
		.count = 1,
		.hasNext = SEAL_TRUE
	};

	Seal_Component *components[20];
	Seal_Size size;

	int results = 1;
	while((size = Seal_QueryComponents(&ctx, components, 20))) {
		if (results > 20) {
			Seal_Log("Too many results");
			return -1;
		}

		if (results != components[0]->parentId) {
			Seal_Log("Results not matching");
			return -1;
		}

		++results;
	}

	return results != 21;
}