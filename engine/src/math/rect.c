
#include <seal/math.h>
#include <seal/math/rect.h>

Seal_Rect Seal_RectFromPoints(Seal_Vector2 a, Seal_Vector2 b) {
	// Treat a as origin
	Seal_Vector2 size = Seal_V2V2Sub(b, a);
	if (size.x < 0) {
		a.x = b.x;
		size.x *= -1;
	}

	if(size.y < 0) {
		a.y = b.y;
		size.y *= -1;
	}

	return (Seal_Rect){
		.position = a,
		.size = size
	};
}

Seal_Bool Seal_RectContains(const Seal_Rect *a, Seal_Vector2 p) {
	if(!a) {
		Seal_LogError("Seal_RectContains was called with null");
		return SEAL_FALSE;
	}

	return Seal_IsBetween(a->x, p.x, a->x + a->w) && Seal_IsBetween(a->y, p.y, a->y + a->h); 
}

Seal_Bool Seal_RectIntersects(const Seal_Rect *a, const Seal_Rect *b) {
	if(!a || !b) {
		Seal_Log("Seal_RectIntersects called with null");
		return SEAL_FALSE;
	}

	return (Seal_IsBetween(a->x, b->x, a->x + a->w) || Seal_IsBetween(a->x, b->x + b->w, a->x + a->w)) &&
		   (Seal_IsBetween(a->y, b->y, a->y + a->h) || Seal_IsBetween(a->y, b->y + b->h, a->y + a->h));
}

Seal_Bool Seal_RectToV2Series(Seal_V2Series series, const Seal_Rect *rect) {
	if (!rect) return SEAL_FALSE;
	
	return Seal_V2SeriesPut(series, 0, (Seal_Vector2){rect->x, rect->y}) &&
		   Seal_V2SeriesPut(series, 1, (Seal_Vector2){rect->x + rect->w, rect->y}) &&
		   Seal_V2SeriesPut(series, 2, (Seal_Vector2){rect->x, rect->y + rect->h}) &&
		   Seal_V2SeriesPut(series, 3, (Seal_Vector2){rect->x + rect->w, rect->y + rect->h});
}