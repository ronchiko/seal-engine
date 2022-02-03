#pragma once

#define GLOB_MISMATCH		-2
#define GLOB_INTERNAL_ERR	-1
#define GLOB_MATCHED		0

typedef struct Seal_GlobExpr *Seal_GlobExpr;

Seal_GlobExpr Seal_ParseGlob(const char *glob);
void Seal_FreeGlobExpr(Seal_GlobExpr expr);

int Seal_GlobMatch(Seal_GlobExpr expr, const char *str);

