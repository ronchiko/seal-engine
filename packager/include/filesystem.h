#pragma once

#define BIT(n) (1 << (n))

#define GLOB_MISMATCH		-2
#define GLOB_INTERNAL_ERR	-1
#define GLOB_MATCHED		0

typedef struct _Seal_FileIterator *Seal_FileIterator;
typedef struct Seal_GlobExpr *Seal_GlobExpr;

Seal_GlobExpr Seal_ParseGlob(const char *glob);
void Seal_FreeGlobExpr(Seal_GlobExpr expr);

int Seal_GlobMatch(Seal_GlobExpr expr, const char *str);

typedef enum {
	FILE_ITER_RESULT_END 	= -1,
	FILE_ITER_RESULT_FILE	= 1,
	FILE_ITER_RESULT_DIR	= 2,
} Seal_FileIteratorResult;

typedef enum {
	FILE_ITER_RECURSIVE = BIT(0),
	FILE_ITER_INCLUDE_DIRECTORIES = BIT(1)
} Seal_FileIteratorFlags;

int Seal_GetFileName(const char *path, int maxPathLen, char *buf, int bufSize);

Seal_FileIterator Seal_CreateFileIterator(const char *glob, int flags);
int Seal_FileIteratorNext(Seal_FileIterator iterator, char *path, int n);
void Seal_FreeFileIterator(Seal_FileIterator iter);