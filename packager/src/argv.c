#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "argv.h"

typedef int (*Seal_OptionHandler)(Seal_PackagerArgs *args, int *i, int n, const char **argv);

static int HandleLookupOption(Seal_PackagerArgs *args, int *i, int n, const char **argv) {
	if(++(*i) >= n)
		return -1;
	
	int length = strlen(argv[*i]);
	args->lookup = malloc((length + 1)* sizeof(char));
	memcpy(args->lookup, argv[*i], length * sizeof(char));
	args->lookup[length] = 0;
	
	return 0;
}

static int HandleArchiveSizeOption(Seal_PackagerArgs *args, int *i, int n, const char **argv) {
	if(++(*i) >= n)
		return -1;
	
	uint64_t limit = strtoull(argv[*i], NULL, 10);
	args->archiveSizeLimit = limit;
	
	return 0;
}

static const struct {char *key; Seal_OptionHandler handler;} OPTIONS[] = {
	{"--lookup", 	&HandleLookupOption},
	{"--arch-size", &HandleArchiveSizeOption},
};
static const int OPTIONS_COUNT = sizeof(OPTIONS) / sizeof(OPTIONS[0]);

static const struct {char *key; size_t offset;} FLAGS[] = {
	{"--verbose", 	offsetof(Seal_PackagerArgs, verbose)},
	{"-v", 			offsetof(Seal_PackagerArgs, verbose)},
	{"-e", 			offsetof(Seal_PackagerArgs, encrypt)},
	{"--encrypt",	offsetof(Seal_PackagerArgs, encrypt)},
	{"--new-key",	offsetof(Seal_PackagerArgs, regenerateKey)},
	{"--zip", 		offsetof(Seal_PackagerArgs, exportZips)},
	{"--clean",		offsetof(Seal_PackagerArgs, cleanCache)}
};
static const int FLAGS_COUNT = sizeof(FLAGS) / sizeof(FLAGS[0]);

Seal_OptionHandler Seal_IsOption(const char *arg) {
	for(int i = 0; i < OPTIONS_COUNT; ++i) 
		if(strcmp(arg, OPTIONS[i].key) == 0) 
			return OPTIONS[i].handler;
		
	return NULL;
}

int Seal_IsFlags(const char *arg) {
	for(int i = 0; i < FLAGS_COUNT; ++i)
		if(strcmp(arg, FLAGS[i].key) == 0) return FLAGS[i].offset;

	return -1;
}

Seal_PackagerArgs *Seal_ParseArgv(int n, const char **argv) {	
	Seal_PackagerArgs *args = malloc(sizeof(Seal_PackagerArgs));
	memset(args, 0, sizeof(Seal_PackagerArgs));

	args->archiveSizeLimit = DEFAULT_MAX_ZIP_ARCHIVE_SIZE_BYTES;

	for (int i = 1; i < n; ++i) {
		Seal_OptionHandler handler;
		int offset;

		if(handler = Seal_IsOption(argv[i])) {
			if(handler(args, &i, n, argv) < 0) {
				free(args);
				return NULL;
			}
		} else if((offset = Seal_IsFlags(argv[i])) >= 0) {
			*(args_flag_t *)((char *)args + offset) = 1;
		}else {
			// What is not is a path			
			args->assets.patterns = realloc(args->assets.patterns, (++args->assets.count) * sizeof(char *));
			args->assets.patterns[args->assets.count - 1] = strdup(argv[i]); 
		}
	}

	return args;
}

void Seal_FreeArgv(Seal_PackagerArgs *args) {
	if(!args) return;

	for(int i = 0; i < args->assets.count; ++i)
		free(args->assets.patterns[i]);
	
	free(args->assets.patterns);
	free(args->lookup);
	
	free(args);
}