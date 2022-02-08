#pragma once

#include <stdint.h>

#include "argv.h"

#include "lookup.h"

typedef enum {
	SEAL_GROUP_SHADER = 0,
	SEAL_GROUP_TEXTURE,

	SEAL_GROUP_GENERIC_TEXT,
	SEAL_GROUP_GENERIC_BIN,

	__SEAL_GROUP_COUNT__,

	SEAL_GROUP_IGNORE
} Seal_FileGroup;

typedef struct {	
	size_t deflatedSize;

	char **vector;
	size_t count;
	size_t alloced;
} Seal_FileVector;

typedef struct Seal_AssetArchive {
	Seal_FileVector vectors[__SEAL_GROUP_COUNT__];
} Seal_AssetArchive;

Seal_FileGroup Seal_GetFileGroup(const char *path, int maxl);
char *Seal_GenerateEncryptionKey(void);

void Seal_CreateArchive(Seal_PackagerArgs *args);