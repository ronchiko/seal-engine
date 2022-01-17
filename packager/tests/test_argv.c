
#include <stdio.h>
#include <string.h>

#include "argv.h"

#include "suite.h"


int TestParseArgv_OnValidInputNoLookup(void) {
	// Arrange
	int result = 0;
	const char *argv[] = { "seal-pkg", "assets/*.png", "assets/*.vert"};

	// Act
	Seal_PackagerArgs *args = Seal_ParseArgv(LEN(argv), argv);

	// Assert
	ASSERT(args->lookup == NULL);
	ASSERT(args->assets.count == 2);
	ASSERT(STREQ(args->assets.patterns[0], "assets/*.png"));
	ASSERT(STREQ(args->assets.patterns[1], "assets/*.vert"));
	
cleanup:
	Seal_FreeArgv(args);

	return result; 
}

int TestParseArgv_OnValidInputWithLookup(void) {
	// Arrange
	int result = 0;
	const char *argv[] = { "seal-pkg", "assets/*.png", "--lookup", "lookup.c"};

	// Act
	Seal_PackagerArgs *args = Seal_ParseArgv(LEN(argv), argv);

	// Assert
	ASSERT(STREQ(args->lookup, "lookup.c"));
	ASSERT(args->assets.count == 1);
	ASSERT(STREQ(args->assets.patterns[0], "assets/*.png"));

cleanup:
	Seal_FreeArgv(args);

	return result; 
}

int TestParseArgv_OnValidInputWithEmptyLookup(void) {
	// Arrange
	int result = 0;
	const char *argv[] = { "seal-pkg", "assets/*.png", "--lookup"};

	// Act
	Seal_PackagerArgs *args = Seal_ParseArgv(LEN(argv), argv);

	// Assert
	ASSERT(args == NULL);

cleanup:
	Seal_FreeArgv(args);

	return result; 
}

int main() {
	int result = 0;

	RUN_TEST(TestParseArgv_OnValidInputNoLookup);
	RUN_TEST(TestParseArgv_OnValidInputWithLookup);
	RUN_TEST(TestParseArgv_OnValidInputWithEmptyLookup);

	return result;
}