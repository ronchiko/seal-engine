#pragma once

#include <stddef.h>
#include <stdint.h>

#define DEFAULT_MAX_ZIP_ARCHIVE_SIZE_BYTES 	10485760	// 10MB

typedef char args_flag_t;

typedef struct {
	struct {
		char **patterns;
		size_t count;
	} assets;

	char *lookup;
	uint64_t archiveSizeLimit;

	args_flag_t verbose;
	args_flag_t encrypt;
	args_flag_t exportZips;
	args_flag_t regenerateKey;
	args_flag_t cleanCache;
} Seal_PackagerArgs;

#define LOG(args, msg, ...) if(args->verbose) printf("[DEBUG] "##msg"\n", __VA_ARGS__, NULL)

Seal_PackagerArgs *Seal_ParseArgv(int argc, const char **argv);
void Seal_FreeArgv(Seal_PackagerArgs *);