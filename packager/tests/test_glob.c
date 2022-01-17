#include <stdlib.h>

#include "filesystem.h"

#include "suite.h"

int TestParseGlob_OnValidGlobExpr() {
	int result = 0;
	const char *glob = "src/*.c";

	Seal_GlobExpr expr = Seal_ParseGlob(glob);

	ASSERT(expr != NULL);

	ASSERT(Seal_GlobMatch(expr, "src/main.c") == GLOB_MATCHED);
	ASSERT(Seal_GlobMatch(expr, "src/limit.c") == GLOB_MATCHED);
	ASSERT(Seal_GlobMatch(expr, "src/fs/glob.c") == GLOB_MATCHED);
	ASSERT(Seal_GlobMatch(expr, "src/fs/glob.clb") == GLOB_MISMATCH);
	ASSERT(Seal_GlobMatch(expr, "src/fs/glob.win.c") == GLOB_MATCHED);
	ASSERT(Seal_GlobMatch(expr, "build/fs/glob.win.c") == GLOB_MISMATCH);

cleanup:
	Seal_FreeGlobExpr(expr);
	return result;
}

int TestParseGlob_OnPerfectMatchExpr() {
	int result = 0;
	const char *glob = "src/main.c";

	Seal_GlobExpr expr = Seal_ParseGlob(glob);

	ASSERT(expr != NULL);

	ASSERT(Seal_GlobMatch(expr, "src/main.c") == GLOB_MATCHED);
	ASSERT(Seal_GlobMatch(expr, "main.c") == GLOB_MISMATCH);
	ASSERT(Seal_GlobMatch(expr, "my*main.c") == GLOB_MISMATCH);
	ASSERT(Seal_GlobMatch(expr, "src/big/main.c") == GLOB_MISMATCH);

cleanup:
	Seal_FreeGlobExpr(expr);
	return result;
}

int TestParseGlob_OnMatchAllExpr() {
	int result = 0;
	const char *glob = "*";

	Seal_GlobExpr expr = Seal_ParseGlob(glob);

	ASSERT(expr != NULL);

	ASSERT(Seal_GlobMatch(expr, "src/main.c") == GLOB_MATCHED);
	ASSERT(Seal_GlobMatch(expr, "main.c") == GLOB_MATCHED);
	ASSERT(Seal_GlobMatch(expr, "project/service/main.c") == GLOB_MATCHED);
	ASSERT(Seal_GlobMatch(expr, "") == GLOB_MATCHED);

cleanup:
	Seal_FreeGlobExpr(expr);
	return result;
}

int TestParseGlob_EndsWithWildcardExpr() {
	int result = 0;
	const char *glob = "dir/file*";

	Seal_GlobExpr expr = Seal_ParseGlob(glob);

	ASSERT(expr != NULL);

	ASSERT(Seal_GlobMatch(expr, "src/main.c") == GLOB_MISMATCH);
	ASSERT(Seal_GlobMatch(expr, "dir/file") == GLOB_MATCHED);
	ASSERT(Seal_GlobMatch(expr, "dir/file.c") == GLOB_MATCHED);
	ASSERT(Seal_GlobMatch(expr, "dir/filename") == GLOB_MATCHED);
	ASSERT(Seal_GlobMatch(expr, "base/filename") == GLOB_MISMATCH);

cleanup:
	Seal_FreeGlobExpr(expr);
	return result;
}

int TestParseGlob_StartsWithWildcardExpr() {
	int result = 0;
	const char *glob = "*.c";

	Seal_GlobExpr expr = Seal_ParseGlob(glob);

	ASSERT(expr != NULL);

	ASSERT(Seal_GlobMatch(expr, "src/main.c") == GLOB_MATCHED);
	ASSERT(Seal_GlobMatch(expr, "system.c") == GLOB_MATCHED);
	ASSERT(Seal_GlobMatch(expr, "dir/file") == GLOB_MISMATCH);
	ASSERT(Seal_GlobMatch(expr, "dir/file.c") == GLOB_MATCHED);
	ASSERT(Seal_GlobMatch(expr, "dir/filename") == GLOB_MISMATCH);
	ASSERT(Seal_GlobMatch(expr, "base/filename") == GLOB_MISMATCH);

cleanup:
	Seal_FreeGlobExpr(expr);
	return result;
}

int TestParseGlob_MultiWildcardExpr() {
	int result = 0;
	const char *glob = "*/build/*.*";

	Seal_GlobExpr expr = Seal_ParseGlob(glob);

	ASSERT(expr != NULL);

	ASSERT(Seal_GlobMatch(expr, "src/build/main.c") == GLOB_MATCHED);
	ASSERT(Seal_GlobMatch(expr, "system.c") == GLOB_MISMATCH);
	ASSERT(Seal_GlobMatch(expr, "dir/file") == GLOB_MISMATCH);
	ASSERT(Seal_GlobMatch(expr, "dir/file.c") == GLOB_MISMATCH);
	ASSERT(Seal_GlobMatch(expr, "dir/filename") == GLOB_MISMATCH);
	ASSERT(Seal_GlobMatch(expr, "base/build/filename.c") == GLOB_MATCHED);
	ASSERT(Seal_GlobMatch(expr, "dir/build/MYFILE.ucx") == GLOB_MATCHED);

cleanup:
	Seal_FreeGlobExpr(expr);
	return result;
}


int TestParseGlob_OnInvalidGlobExpr() {
	int result = 0;
	const char *glob = "src/**.c";

	Seal_GlobExpr expr = Seal_ParseGlob(glob);

	ASSERT(expr == NULL);

cleanup:
	Seal_FreeGlobExpr(expr);
	return result;
}

int main() {
	int result = 0;

	RUN_TEST(TestParseGlob_OnValidGlobExpr);
	RUN_TEST(TestParseGlob_OnMatchAllExpr);
	RUN_TEST(TestParseGlob_OnPerfectMatchExpr);
	RUN_TEST(TestParseGlob_EndsWithWildcardExpr);
	RUN_TEST(TestParseGlob_OnInvalidGlobExpr);

	return result;
}