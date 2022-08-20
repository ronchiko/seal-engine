#pragma once

#include <seal/common.h>
#include <seal/math/vector2.h>

#include <seal/math/series/vector2.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef union Seal_Rect {
	Seal_Float pointer[4];
	struct {
		Seal_Float x;
		Seal_Float y;
		Seal_Float w;
		Seal_Float h;
	};
	struct {
		Seal_Vector2 position;
		Seal_Vector2 size;
	};
} Seal_Rect;

Seal_Rect Seal_RectFromPoints(Seal_Vector2 a, Seal_Vector2 b);
Seal_Bool Seal_RectContains(const Seal_Rect *rect, Seal_Vector2 point);
Seal_Bool Seal_RectIntersects(const Seal_Rect *a, const Seal_Rect *b);

/* 
	Puts a rectangle inside an initialezed series with at least 4 elements, if more are provided they will be untouched

	returns: True if it was successful, false if rect is null, series is uninitialzed or series has less then 4 elements
*/
Seal_Bool Seal_RectToV2Series(Seal_V2Series series, const Seal_Rect *rect);

#ifdef __cplusplus
}
#endif
