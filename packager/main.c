#include <stdio.h>

#include "config.h"

#include "argv.h"
#include "filesystem.h"

int main(int argc, char *argv[]) {
	/*
	 * Expected input: seal-pkg [OPTIONS] --lookup <lookupname>.c [asset folders...]
	 */
	printf("Seal Packager Version %d.%d\n", VERSION_MAJOR, VERSION_MINOR);
	// Parse arguments
	Seal_PackagerArgs *args = Seal_ParseArgv(argc, argv);
	printf("LOOKUP: %s\n", args->lookup);

	for(int i = 0; i < args->assets.count; ++i)
		printf("%s\n", args->assets.patterns[i]);

	// Run packager
	char path[2049];
	int type, total = 0;
	Seal_FileIterator iter = Seal_CreateFileIterator("src/", FILE_ITER_RECURSIVE);
	while((type = Seal_FileIteratorNext(iter, path, 2048)) > 0) {
		printf("\t%s: %s\n", type == FILE_ITER_RESULT_FILE ? "FILE" : "DIR", path);
		++total;
	}
	Seal_FreeFileIterator(iter);
	printf("%d\n", total);

	// Generate lookup
	Seal_FreeArgv(args);
	return 0;
}