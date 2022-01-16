#pragma once

#include "seal/common.h"

#ifdef _DEBUG

void *Seal_DbgAllocate(size_t bytes, const char *file, int line);
void *Seal_DbgReallocate(void *ptr, size_t bytes, const char *file, int line);
void  Seal_DbgRelease(void *ptr);

#ifndef _USE_BASE_MALLOC
#define malloc(l) 		Seal_DbgAllocate(l, __FILE__, __LINE__)
#define realloc(p,l) 	Seal_DbgReallocate(p, l, __FILE__, __LINE__)
#define free(p)			Seal_DbgRelease(p)
#endif

Seal_Bool Seal_LeakCheck(void);
void Seal_DumpLeak(void *stream);

void Seal_CleanDebugData(void);

#define DBG_WARN(msg, ...)   Seal_LogWarning("%s:%d: "##msg, __FILE__, __LINE__, __VA_ARGS__)
#define DBG_ERROR(msg, ...)  Seal_LogError("%s:%d: "##msg, SEAL_FALSE, __FILE__, __LINE__, __VA_ARGS__)

#else

#define DBG_WARN(msg, ...)
#define DBG_ERROR(msg, ...)  Seal_LogError(msg, SEAL_FALSE, __VA_ARGS__)

#define Seal_DumpLeak(...)
#define Seal_LeakCheck(...) 	1

#endif
