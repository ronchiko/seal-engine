
#include "tests/component.h"

static const int FIRST_PARENT_ID = 100;

static void _InitializeTestComponent(TestComponent *comp, int pid) {
	comp->base.componentId = 100;
	comp->base.parentId = pid;
	comp->floatComponent = 3.0;
	comp->integerComponent = 10412;
	comp->stringComponent = "Hello!";
}

int TestComponentBuffer_AddSingleComponent(void) {
	// Arrange
	int result = 0;
	Seal_ComponentBuffer buffer = Seal_CreateComponentBuffer(sizeof(TestComponent));
	
	TestComponent component;
	_InitializeTestComponent(&component, FIRST_PARENT_ID);

	// Act + Assert
	ASSERT(!memcmp(&component, Seal_AddComponent(buffer, (Seal_Component *)&component), sizeof(TestComponent)));
	ASSERT(_Seal_TestBufferSize(buffer, 1));

cleanup:
	Seal_FreeComponentBuffers();
	ASSERT_LEAK;

	return result;
}

int TestComponentBuffer_AddMultipleCompnents(void) {
	// Arrange
	int result = 0;
	Seal_ComponentBuffer buffer = Seal_CreateComponentBuffer(sizeof(TestComponent));
	
	TestComponent component1, component2;
	_InitializeTestComponent(&component1, FIRST_PARENT_ID);
	_InitializeTestComponent(&component2, FIRST_PARENT_ID + 1);

	// Assert + Act
	ASSERT(!memcmp(&component1, Seal_AddComponent(buffer, (Seal_Component *)&component1), sizeof(TestComponent)));
	ASSERT(!memcmp(&component2, Seal_AddComponent(buffer, (Seal_Component *)&component2), sizeof(TestComponent)));
	ASSERT(_Seal_TestBufferSize(buffer, 2));

cleanup:
	Seal_FreeComponentBuffers();
	ASSERT_LEAK;

	return result;
}

int TestComponentBuffer_WhenEmptyBuffer_ThenNoLeak(void) {
	// Arrange
	int result = 0;
	Seal_ComponentBuffer buffer = Seal_CreateComponentBuffer(sizeof(TestComponent));
	
	// Act + Assert
	ASSERT(_Seal_TestBufferSize(buffer, 0));

cleanup:
	Seal_FreeComponentBuffers();
	ASSERT_LEAK;
	return result;
}


int TestComponentBufferQuery_OnSingleBuffer_ThenReturnAll(void) {
	// Arrange
	int result = 0;
	Seal_ComponentBuffer buffer = Seal_CreateComponentBuffer(sizeof(TestComponent));

	TestComponent components[2];
	_InitializeTestComponent(components, FIRST_PARENT_ID);
	_InitializeTestComponent(components + 1, FIRST_PARENT_ID + 1);

	Seal_AddComponent(buffer, (Seal_Component *)components);
	Seal_AddComponent(buffer, (Seal_Component *)(components + 1));
	ASSERT(_Seal_TestBufferSize(buffer, 2));

	Seal_IterativeBuffer buffers[1] = { Seal_BufferOf(buffer) };

	Seal_QueryContext query = {
		.buffers = buffers,
		.count = 1,
		.hasNext = SEAL_TRUE
	};

	// Act
	Seal_Component *matches[20];
	Seal_Size matchCount;

	int i = 0;
	for(; matchCount = Seal_QueryComponents(&query, matches, 20); ++i) {
		ASSERT(matchCount == 1);
		ASSERT(!memcmp(components + i, matches[0], sizeof(TestComponent)));
	}
	
	// Assert
	ASSERT(i == 2);
cleanup:
	Seal_FreeComponentBuffers();
	ASSERT_LEAK;
	
	return result;
}


int TestComponentBufferQuery_OnMultipleBuffersSingleMatch_ThenReturnMatch(void) {
	// Arrange
	int result = 0;
	Seal_ComponentBuffer buffer1 = Seal_CreateComponentBuffer(sizeof(TestComponent));
	Seal_ComponentBuffer buffer2 = Seal_CreateComponentBuffer(sizeof(TestComponent));

	TestComponent components[3];
	_InitializeTestComponent(components, FIRST_PARENT_ID);
	_InitializeTestComponent(components + 1, FIRST_PARENT_ID + 1);
	_InitializeTestComponent(components + 2, FIRST_PARENT_ID - 1);

	Seal_AddComponent(buffer1, (Seal_Component *)components);
	Seal_AddComponent(buffer1, (Seal_Component *)(components + 1));

	Seal_AddComponent(buffer2, (Seal_Component *)components);
	Seal_AddComponent(buffer2, (Seal_Component *)(components + 2));

	ASSERT(_Seal_TestBufferSize(buffer1, 2));
	ASSERT(_Seal_TestBufferSize(buffer2, 2));

	Seal_IterativeBuffer buffers[2] = { Seal_BufferOf(buffer1), Seal_BufferOf(buffer2) };

	Seal_QueryContext query = {
		.buffers = buffers,
		.count = 2,
		.hasNext = SEAL_TRUE
	};

	// Act
	Seal_Component *matches[20];
	Seal_Size matchCount;

	int i = 0;
	for(; matchCount = Seal_QueryComponents(&query, matches, 20); ++i) {
		ASSERT(matchCount == 2);
		ASSERT(!memcmp(components, matches[0], sizeof(TestComponent)));
		ASSERT(!memcmp(components, matches[1], sizeof(TestComponent)));
	}
	
	// Assert
	ASSERT(i == 1);
cleanup:
	Seal_FreeComponentBuffers();
	ASSERT_LEAK;
	
	return result;
}


int TestComponentBufferQuery_OnMultipleBuffers_ThenReturnMatches(void) {
	// Arrange
	int result = 0;
	Seal_ComponentBuffer buffer1 = Seal_CreateComponentBuffer(sizeof(TestComponent));
	Seal_ComponentBuffer buffer2 = Seal_CreateComponentBuffer(sizeof(TestComponent));

	TestComponent components[5];
	_InitializeTestComponent(components, FIRST_PARENT_ID);
	_InitializeTestComponent(components + 1, FIRST_PARENT_ID + 1);
	_InitializeTestComponent(components + 2, FIRST_PARENT_ID - 1);
	_InitializeTestComponent(components + 3, FIRST_PARENT_ID - 2);
	_InitializeTestComponent(components + 4, FIRST_PARENT_ID + 2);

	Seal_AddComponent(buffer1, (Seal_Component *)components);
	Seal_AddComponent(buffer1, (Seal_Component *)(components + 1));
	Seal_AddComponent(buffer1, (Seal_Component *)(components + 4));
	Seal_AddComponent(buffer1, (Seal_Component *)(components + 3));

	Seal_AddComponent(buffer2, (Seal_Component *)components);
	Seal_AddComponent(buffer2, (Seal_Component *)(components + 2));
	Seal_AddComponent(buffer2, (Seal_Component *)(components + 3));
	Seal_AddComponent(buffer2, (Seal_Component *)(components + 4));

	ASSERT(_Seal_TestBufferSize(buffer1, 4));
	ASSERT(_Seal_TestBufferSize(buffer2, 4));

	Seal_IterativeBuffer buffers[2] = { Seal_BufferOf(buffer1), Seal_BufferOf(buffer2) };

	Seal_QueryContext query = {
		.buffers = buffers,
		.count = 2,
		.hasNext = SEAL_TRUE
	};

	// Act
	Seal_Component *matches[20];
	Seal_Size matchCount;

	const int EXPECTED_MATCHES [4]  = {3, 0, 4, -1};
	const int *currentMatch = EXPECTED_MATCHES;

	int i = 0;
	for(; matchCount = Seal_QueryComponents(&query, matches, 20); ++i, ++currentMatch) {
		ASSERT(*currentMatch >= 0);
		ASSERT(matchCount == 2);

		ASSERT(!memcmp(components + *currentMatch, matches[0], sizeof(TestComponent)));
		ASSERT(!memcmp(components + *currentMatch, matches[1], sizeof(TestComponent)));
	}
	
	// Assert
	ASSERT(i == 3);
cleanup:
	Seal_FreeComponentBuffers();
	ASSERT_LEAK;
	
	return result;
}


int main() {
	int result = 0;

	// Insertion
	RUN_TEST(TestComponentBuffer_AddSingleComponent);
	RUN_TEST(TestComponentBuffer_AddMultipleCompnents);
	RUN_TEST(TestComponentBuffer_WhenEmptyBuffer_ThenNoLeak);
	// Query
	RUN_TEST(TestComponentBufferQuery_OnSingleBuffer_ThenReturnAll);
	RUN_TEST(TestComponentBufferQuery_OnMultipleBuffersSingleMatch_ThenReturnMatch);
	RUN_TEST(TestComponentBufferQuery_OnMultipleBuffers_ThenReturnMatches);

	return result;
}