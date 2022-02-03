#include <stdlib.h>
#include <stdio.h>
#include <zip.h>
#include <seal/fs.h>

#include "config.h"

#include "argv.h"
#include "packager.h"
#include "cache.h"

#define FILES_TO_DELETE_COUNT		8

int main(int argc, char *argv[]) {
	/*
	 * Expected input: seal-pkg [OPTIONS] --lookup <lookupname>.c [asset folders...]
	 */
	Seal_LoadOrCreateCache();

	atexit(&Seal_FinializeCache);
	printf("Seal Packager Version %d.%d\n", VERSION_MAJOR, VERSION_MINOR);
	// Parse arguments
	Seal_PackagerArgs *args = Seal_ParseArgv(argc, argv);
	
	if(args->cleanCache) {
		Seal_DeleteChildren(".seal", FILE_ITER_RECURSIVE | FILE_ITER_INCLUDE_DIRECTORIES);
	}
		
	// Run packager
	Seal_CreateArchive(args);

	// Generate lookup
	Seal_FreeArgv(args);
	return 0;
}