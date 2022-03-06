

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