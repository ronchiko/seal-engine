#pragma once

int Seal_VerifyGlob(const char *glob);

typedef struct _Seal_FileIterator *Seal_FileIterator;

Seal_FileIterator Seal_ExecuteGlob(const char *glob);
Seal_FileIterator Seal_ExecuteGlobRec(const char *glob);

void Seal_FreeFileIterator(Seal_FileIterator iter);