#pragma once

#include <seal/common.h>

typedef struct _Seal_FileIterator *Seal_FileIterator;

typedef enum {
	FILE_ITER_RESULT_END 	= -1,
	FILE_ITER_RESULT_FILE	= 1,
	FILE_ITER_RESULT_DIR	= 2,
} Seal_FileIteratorResult;

typedef enum {
	FILE_ITER_RECURSIVE = Seal_Bit(0),				// Recursively search for files
	FILE_ITER_INCLUDE_DIRECTORIES = Seal_Bit(1),	// Include directories in results 
} Seal_FileIteratorFlags;

int Seal_GetFileName(const char *path, int maxPathLen, char *out, int outSize);
int Seal_GetFileExt(const char *path, int maxPathLen, char *out, int outSize);

Seal_FileIterator Seal_CreateFileIterator(const char *glob, int flags);
int Seal_FileIteratorNext(Seal_FileIterator iterator, char *path, int n);
void Seal_FreeFileIterator(Seal_FileIterator iter);

int Seal_CreateDirectory(const char *path);
int Seal_DirectoryExists(const char *path);

int Seal_DeleteChildren(const char *path, int flags);
