#include <seal/testing/suite.h>

#define SMALL_BUFFER_SIZE	150
#define LARGE_BUFFER_SIZE	10000

int TestAllocate_WhenMemoryUnfreed_ThenRaiseLeak(void) {
	int result = 0;

	int *array = malloc(sizeof(int) * SMALL_BUFFER_SIZE);
	for(int i = 0; i < SMALL_BUFFER_SIZE; ++i) {
		array[i] = i;
	}

	ASSERT(Seal_LeakCheck());

	free(array);
cleanup:
	ASSERT_LEAK;
	Seal_CleanDebugData();
	return result;
}

int TestCleanAllocate_WhenAllocatedThenClean(void) {
	int result = 0;

	int *array = calloc(SMALL_BUFFER_SIZE, sizeof(int));
	for(int i = 0; i < SMALL_BUFFER_SIZE; ++i) {
		ASSERT(array[i] == 0);
	}

	free(array);
cleanup:
	ASSERT_LEAK;
	Seal_CleanDebugData();
	return result;
}

int TestAllocate_WhenMassCalled(void) {
	int result = 0;

	void *buffers[LARGE_BUFFER_SIZE] = { 0 };

	for(int j = 0; j < LARGE_BUFFER_SIZE; ++j ) {
		buffers[j] = malloc(100);
		
		ASSERT(buffers[j] != NULL);
	}

	for(int j = 0; j < LARGE_BUFFER_SIZE; ++j) free(buffers[j]);

cleanup:
	ASSERT_LEAK;
	Seal_CleanDebugData();
	return result;
} 

int TestReallocate_WhenLargeDataOverTime(void) {
	int result = 0;

	const int RESCALE_TIMES = 10;
	void *buffers[SMALL_BUFFER_SIZE] = { 0 };

	for(int j = 0; j < SMALL_BUFFER_SIZE; ++j ) {
		int baseSize = 2;
		buffers[j] = malloc(baseSize);
		
		for(int i = 0; i < RESCALE_TIMES; ++i) {
			baseSize *= 2;
			buffers[j] = realloc(buffers[j], baseSize);
			
			ASSERT(buffers[j] != NULL);
		}
	}

	for(int j = 0; j < SMALL_BUFFER_SIZE; ++j) free(buffers[j]);

cleanup:
	ASSERT_LEAK;
	Seal_CleanDebugData();
	return result;
} 

int main() {
	int result = 0;
	RUN_TEST(TestAllocate_WhenMemoryUnfreed_ThenRaiseLeak);
	RUN_TEST(TestCleanAllocate_WhenAllocatedThenClean);

	RUN_TEST(TestAllocate_WhenMassCalled);
	RUN_TEST(TestReallocate_WhenLargeDataOverTime);

	return result;
}