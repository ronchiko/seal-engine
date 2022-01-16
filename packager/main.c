#include <stdio.h>

#include "config.h"

#include "argv.h"

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

	// Generate lookup
	Seal_FreeArgv(args);
	return 0;
}