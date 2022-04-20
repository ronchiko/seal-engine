#pragma once

#include <float.h>

#define CREATE_SWAP_FUNCTION(type, extension) \
static inline void Seal_Swap##extension(type *a, type *b) {type t = *a; *a = *b; *b = t; } 

#define CREATE_MIN_MAX_FUNCTIONS(type, extension) \
static inline type Seal_Max##extension(type a, type b) { return a < b ? b : a; } \
static inline type Seal_Min##extension(type a, type b) { return a > b ? b : a; }

/* Linear interpolation (0 <= t <= 1) */
static inline float Seal_Lerp(float a, float b, float t) { return b * t + a * (1 - t); }

float Seal_Clampf(float v, float min, float max);
int Seal_Clamp(int v, int min, int max);

CREATE_MIN_MAX_FUNCTIONS(int, );
CREATE_SWAP_FUNCTION(int, );

CREATE_MIN_MAX_FUNCTIONS(float, f);
CREATE_SWAP_FUNCTION(float, f);

/* Rounds to the nearest integer */
float Seal_Round(float a);

float Seal_Ciel(float a);
float Seal_Floor(float b);

/* Computes 2 ** N where n is a positive integer */
#define Seal_2PowN(n) (2 << (n))

/* Computes the inverse sqrt of n, using the quake fast inverse square root */
float Seal_FastInverseSqrt(float n);

#define SEAL_MAX_FLOAT		FLT_MAX
#define SEAL_MIN_FLOAT		FLT_MIN
#define SEAL_EPSILON		FLT_EPSILON

#define Seal_IsBetween(min, n, max) ((min) <= (n) && (n) <= (max))  

// Additional maths
#include "seal/math/vector2.h"
#include "seal/math/vector4.h"

#include "seal/math/rect.h"

#include "seal/math/matrix2.h"
#include "seal/math/matrix3.h"
#include "seal/math/matrix4.h"