
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <seal/fs.h>

#include "cache.h"

struct CacheObject {
	char key[33];
} Seal_Cache = {
	.key = { 0 }
};

void Seal_LoadOrCreateCache(void) {
	// Create temporary directory
	if(!Seal_DirectoryExists(".seal")) {
		Seal_CreateDirectory(".seal");
	}

	FILE *cachefile = NULL;
	if(cachefile = fopen(".seal/packager.cache", "r")) {
		char key[17], value[33];

		while(fscanf(cachefile, "%16s = %32s\n", key, value) > 0) {
			if(strcmp(key, "KEY") == 0) {
				memcpy(Seal_Cache.key, value, 33 * sizeof(char));
			}
		}
		
		fclose(cachefile);
	}
}

void Seal_FinializeCache(void) {
	FILE *cachefile = NULL;
	if(cachefile = fopen(".seal/packager.cache", "w+")) {
		if(Seal_Cache.key[0] != 0)	// Save key only if there is a key
			fprintf(cachefile, "KEY = %s\n", Seal_Cache.key);

		fclose(cachefile);
	}
}

const char *Seal_GetEncryptionKey(void) {
	return Seal_Cache.key[0] ? Seal_Cache.key : NULL;
}

void Seal_SetEncryptionKey(const char *key) {
	memcpy(Seal_Cache.key, key, 32);
	Seal_Cache.key[32] = 0; 
}


void Seal_CleanCache(void) {
	Seal_Cache.key[0] = 0;
}
