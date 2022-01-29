#pragma once

void Seal_LoadOrCreateCache(void);
void Seal_FinializeCache(void);

const char *Seal_GetEncryptionKey(void);
void Seal_SetEncryptionKey(const char *key);

void Seal_CleanCache(void);
