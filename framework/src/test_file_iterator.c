
#include <stdlib.h>

#include <seal/io/fs.h>
#include <seal/testing/suite.h>

int TestFileIterator_WhenNoFlagsOn() {
	int result = 0;
	const char *expected[] = {
		"myfile"
	};
	const int expectedCount = LEN(expected);

	char path[256], name[64];
	int resultCount, resultType = 0;
	Seal_FileIterator iter = Seal_CreateFileIterator("test_folder", 0);

	for(resultCount = 0; (resultType = Seal_FileIteratorNext(iter, path, 255)) > 0; ++resultCount) {
		Seal_GetFileName(path, 255, name, 63);

		ASSERT(ArrayContainsStr(expected, expectedCount, name));
		ASSERT(resultType != FILE_ITER_RESULT_DIR);
	}

	ASSERT(resultType == FILE_ITER_RESULT_END);
	ASSERT(resultCount == expectedCount);

cleanup:
	Seal_FreeFileIterator(iter);
	return result;
}

int TestFileIterator_WhenRecursiveFlagOn() {
	int result = 0;
	const char *expected[] = {
		"myfile",
		"file2"
	};
	const int expectedCount = LEN(expected);

	char path[256], name[64];
	int resultCount, resultType = 0;
	Seal_FileIterator iter = Seal_CreateFileIterator("test_folder", FILE_ITER_RECURSIVE);

	for(resultCount = 0; (resultType = Seal_FileIteratorNext(iter, path, 255)) > 0; ++resultCount) {
		Seal_GetFileName(path, 255, name, 63);

		ASSERT(ArrayContainsStr(expected, expectedCount, name));
		ASSERT(resultType != FILE_ITER_RESULT_DIR);
	}

	ASSERT(resultType == FILE_ITER_RESULT_END);
	ASSERT(resultCount == expectedCount);
cleanup:
	Seal_FreeFileIterator(iter);
	return result;
}

int TestFileIterator_WhenIncludeDirectoriesFlagOn() {
	int result = 0;
	const char *expected[] = {
		"myfile",
		"empty",
		"sub"
	};
	const int expectedCount = LEN(expected);

	char path[256], name[64];
	int resultCount, resultType;
	Seal_FileIterator iter = Seal_CreateFileIterator("test_folder", FILE_ITER_INCLUDE_DIRECTORIES);

	for(resultCount = 0; (resultType = Seal_FileIteratorNext(iter, path, 255)) > 0; ++resultCount) {
		Seal_GetFileName(path, 255, name, 63);

		ASSERT(ArrayContainsStr(expected, expectedCount, name));
	}

	ASSERT(resultType == FILE_ITER_RESULT_END);
	ASSERT(resultCount == expectedCount);
cleanup:
	Seal_FreeFileIterator(iter);
	return result;
}

int TestFileIterator_WhenIncludeDirectoriesAndRecursiveFlagsOn() {
	int result = 0;
	const char *expected[] = {
		"myfile",
		"empty",
		"sub",
		"file2"
	};
	const int expectedCount = LEN(expected);

	char path[256], name[64];
	int resultCount, resultType;
	Seal_FileIterator iter = Seal_CreateFileIterator("test_folder", FILE_ITER_INCLUDE_DIRECTORIES | FILE_ITER_RECURSIVE);

	for(resultCount = 0; (resultType = Seal_FileIteratorNext(iter, path, 255)) > 0; ++resultCount) {
		Seal_GetFileName(path, 255, name, 63);

		ASSERT(ArrayContainsStr(expected, expectedCount, name));
	}

	ASSERT(resultType == FILE_ITER_RESULT_END);
	ASSERT(resultCount == expectedCount);
cleanup:
	Seal_FreeFileIterator(iter);
	return result;
}

int main(int argc, char *argv[]) {
	if(!WORKDIR(argv[1])) {
		return 1;
	}
	int result = 0;

	RUN_TEST(TestFileIterator_WhenNoFlagsOn);
	RUN_TEST(TestFileIterator_WhenRecursiveFlagOn);
	RUN_TEST(TestFileIterator_WhenIncludeDirectoriesFlagOn);

	return result;
}

