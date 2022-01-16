
#include "test-common.h"

#include "seal/ecs/component.h"

TEST_MAIN {
	Seal_ComponentBuffer buffer1 = Seal_CreateComponentBuffer(sizeof(TestComponent)),
						 buffer2 = Seal_CreateComponentBuffer(sizeof(TestComponent));

	// Populate buffers
	for(int i = 0; i < 100; ++i) {
		Seal_Entity entity = Seal_CreateEntity();
		
		TestComponent c1;
		if (!(i % 10)) {
			Seal_AttachComponent(entity, &c1.base);
			Seal_AddComponent(buffer1, &c1.base);
		}

		if (!(i % 3)) {
			Seal_AttachComponent(entity, &c1.base);
			Seal_AddComponent(buffer2, &c1.base);
		}
	}

	struct Seal_IterativeBuffer buffers[2] = {
		Seal_BufferOf(buffer1),
		Seal_BufferOf(buffer2)
	};

	Seal_QueryContext ctx = {
		.buffers = buffers,
		.count = 2,
		.hasNext = SEAL_TRUE
	};

	Seal_Component *components[20];
	Seal_Size size;

	int expectedIds[] = {
		1, 31, 61, 91, -1
	}, *currentId = expectedIds;

	while((size = Seal_QueryComponents(&ctx, components, 20))) {
		if (*currentId == -1) {
			Seal_Log("Out of ids!");
			return -1;
		}

		if (*currentId != components[0]->parentId) {
			Seal_Log("Ids not matching (%d != %d)", *currentId, components[0]->parentId);
			return -1;
		}
		++currentId;
	}

	return *currentId != -1;
}