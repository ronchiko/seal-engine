
#include <math.h>

#include <seal/math.h>
#include <seal/math/series/vector2.h>

#define FOREACH_ELEMENT(series) for (int i = 0; i < series.length; ++i)

Seal_Bool Seal_V2SeriesPut(Seal_V2Series series, Seal_Size index, Seal_Vector2 v2) {
if  (series.length <= index || !series.elements) return SEAL_FALSE;

	series.elements[index] = v2;
	return SEAL_TRUE;
}

Seal_V2Series Seal_V2SeriesFromBuffer(Seal_Vector2 *buffer, Seal_Size length) {
	return (Seal_V2Series){
		.elements = buffer,
		.length = length
	};
}

void Seal_V2SeriesMove(Seal_V2Series series, Seal_Vector2 offset) {
	FOREACH_ELEMENT(series) {
		Seal_V2V2Add(series.elements[i], offset);
	}
}

void Seal_V2SeriesRotate(Seal_V2Series series, float angle) {
	float cosA = cosf(Deg2Rad(angle)), sinA = sinf(Deg2Rad(angle));

	FOREACH_ELEMENT(series) {
		Seal_Vector2 original = series.elements[i]; 
		series.elements[i] = (Seal_Vector2){ 
			cosA * original.x - sinA * original.y,
			sinA * original.x + cosA * original.y
		};
	}
}