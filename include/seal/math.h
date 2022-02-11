#pragma once

/* Linear interpolation (0 <= t <= 1) */
static inline float Seal_Lerp(float a, float b, float t) { return a * t + b * (1 - t); }

float Seal_Clampf(float v, float min, float max);
int Seal_Clamp(int v, int min, int max);

static inline int Seal_Max(int a, int b) { return a < b ? b : a; }
static inline int Seal_Min(int a, int b) { return a > b ? b : a; }

static inline float Seal_Maxf(float a, float b) { return a < b ? b : a; }
static inline float Seal_Minf(float a, float b) { return a > b ? b : a; }

/* Rounds to the nearest integer */
float Seal_Round(float a);

float Seal_Ciel(float a);
float Seal_Floor(float b);

/* Computes 2 ** N where n is a positive integer */
#define Seal_2PowN(n) (2 << (n))

/* Computes the inverse sqrt of n, using the quake fast inverse square root */
float Seal_FastInverseSqrt(float n);

// Additional maths
#include "seal/math/vector2.h"
#include "seal/math/vector4.h"

#include "seal/math/rect.h"

#include "seal/math/matrix2.h"
#include "seal/math/matrix3.h"
#include "seal/math/matrix4.h"