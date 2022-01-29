#pragma once

#include <stdint.h>

typedef struct _Seal_LookupTable *Seal_LookupTable;

Seal_LookupTable Seal_CreateNewLookup(void);

int Seal_LookupGetOrAddArchive(Seal_LookupTable lookup, const char *archive);
int Seal_LookupAddLookup(Seal_LookupTable lookup, const char *path, int archive);
void Seal_LookSetEncryptionKey(Seal_LookupTable lookup, const char *key);

/* Also frees the lookup object*/
void Seal_WriteLookup(Seal_LookupTable lookup, const char *out);