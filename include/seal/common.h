#pragma once

#include <stdint.h>
#include <time.h>

/* Double pointer */
#define PPTR(type)	type**

/* \brief clarification the variable doesnt need a value and can be null */
#define SEAL_OPTIONAL
#define SEAL_NULL NULL

typedef enum {
	SEAL_FALSE,
	SEAL_TRUE
} Seal_Bool;

typedef int8_t		Seal_Byte;
typedef uint8_t		Seal_UByte;
typedef int32_t  	Seal_Size;
typedef uint32_t 	Seal_ID;

typedef int32_t		Seal_Int;
typedef int64_t		Seal_Long;
typedef float 		Seal_Float;

typedef uint32_t 	Seal_BitMask;

typedef Seal_Int Seal_Camera;
typedef Seal_ID Seal_Entity;

typedef union {
	struct {
		Seal_Int x;
		Seal_Int y;
	};
	Seal_Int array[2];
} Seal_Int2;

#define SEAL_ID_INVALID 		0

#define SEAL_UINT32_MAX		0xFFFFFFFF

#define Seal_Bit(n) 	(1 << (n))

#include "seal/debug.h"

Seal_Bool Seal_Init(void);
void Seal_Close(void);

#pragma region Logging

void Seal_Log(const char *fmt, ...);
void Seal_LogInfo(const char *fmt, ...);
void Seal_LogWarning(const char *fmt, ...);
void Seal_LogError(const char *fmt, ...);
void Seal_LogFatal(const char *fmt, ...);

#pragma endregion

void *Seal_DeepCopy(void *src, size_t size);

Seal_Size Seal_FConcatString(char **str, const char *o, Seal_Size strlen, Seal_Size olen);

typedef clock_t Seal_TimeUnit;

extern Seal_TimeUnit Seal_DeltaTimeMs;
extern Seal_Float Seal_DeltaTime;

inline Seal_Float Seal_Time() {
	extern Seal_TimeUnit Seal_BeginTime;
	return (clock() - Seal_BeginTime) / 1000.f;
}

typedef struct { Seal_Float rgba[4]; } Seal_Color;
