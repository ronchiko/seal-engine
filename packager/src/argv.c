
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "argv.h"

typedef int (*Seal_OptionHandler)(Seal_PackagerArgs *args, int *i, int n, const char **argv);

static int HandleLookupOption(Seal_PackagerArgs *args, int *i, int n, const char **argv) {
	if(++(*i) >= n) {
		return -1;
	}
	
	int length = strlen(argv[*i]);
	args->lookup = malloc((length + 1)* sizeof(char));
	memcpy(args->lookup, argv[*i], length * sizeof(char));
	args->lookup[length] = 0;
	
	return 0;
}

static const struct {char *key; Seal_OptionHandler handler;} OPTIONS[] = {
	{"--lookup", &HandleLookupOption} 
};
static const int OPTIONS_COUNT = sizeof(OPTIONS) / sizeof(OPTIONS[0]);

Seal_OptionHandler Seal_IsOption(const char *arg) {
	for(int i = 0; i < OPTIONS_COUNT; ++i) 
		if(strcmp(arg, OPTIONS[i].key) == 0) 
			return OPTIONS[i].handler;
		
	return NULL;
}

Seal_PackagerArgs *Seal_ParseArgv(int n, const char **argv) {	
	Seal_PackagerArgs *args = malloc(sizeof(Seal_PackagerArgs));
	args->assets.count = 0;
	args->assets.patterns = NULL;
	args->lookup = NULL;
	
	for (int i = 1; i < n; ++i) {
		Seal_OptionHandler handler;

		if(handler = Seal_IsOption(argv[i])) {
			if(handler(args, &i, n, argv) < 0) {
				free(args);
				return NULL;
			}
		} else {
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