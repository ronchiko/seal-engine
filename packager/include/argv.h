#pragma once

#include <stddef.h>

typedef struct {
	struct {
		char **patterns;
		size_t count;
	} assets;

	char *lookup;
} Seal_PackagerArgs;

Seal_PackagerArgs *Seal_ParseArgv(int argc, const char **argv);
void Seal_FreeArgv(Seal_PackagerArgs *);