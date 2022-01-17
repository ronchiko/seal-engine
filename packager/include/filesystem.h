#pragma once

#define GLOB_MISMATCH		-2
#define GLOB_INTERNAL_ERR	-1
#define GLOB_MATCHED		0

typedef struct _Seal_FileIterator *Seal_FileIterator;
typedef struct Seal_GlobExpr *Seal_GlobExpr;

Seal_GlobExpr Seal_ParseGlob(const char *glob);
void Seal_FreeGlobExpr(Seal_GlobExpr expr);

int Seal_GlobMatch(Seal_GlobExpr expr, const char *str);

Seal_FileIterator Seal_ExecuteGlob(const char *glob);
Seal_FileIterator Seal_ExecuteGlobRec(const char *glob);

void Seal_FreeFileIterator(Seal_FileIterator iter);