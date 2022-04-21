#pragma once

#include <seal/math/vector2.h>

typedef struct {
	Seal_Vector2 *elements;
	const Seal_Size length;
} Seal_V2Series;

Seal_V2Series Seal_V2SeriesFromBuffer(Seal_Vector2 *buffer, Seal_Size length);

void Seal_V2SeriesMove(Seal_V2Series series, Seal_Vector2 delta);
void Seal_V2SeriesRotate(Seal_V2Series series, float angle);
