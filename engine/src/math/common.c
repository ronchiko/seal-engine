
#include <stdint.h>

#include "seal/math.h"

inline float Seal_Round(float number) {
	int integer = (int)number;
	if (number - integer >= 0.5) return Seal_Ciel(number);
	return Seal_Floor(number); 
}

inline float Seal_Floor(float number) { return (float)((int)number); }
inline float Seal_Ciel(float number) { return Seal_Floor(number) + 1; }

inline float Seal_Clampf(float n, float mn, float mx) {
	return n > mx ? mx : n < mn ? mn : n;
}
inline int Seal_Clamp(int n, int mn, int mx) {
	return n > mx ? mx : n < mn ? mn : n;
}

float Seal_FastInverseSqrt(float number) {
	union {
		float 		f;
		uint32_t 	i;
	} value = { .f = number };
	
	value.i = 0x5f3759df - (value.i >> 1); 
	value.f = value.f * (1.5f - (0.5f * number * value.f * value.f));

	return value.f;
}
