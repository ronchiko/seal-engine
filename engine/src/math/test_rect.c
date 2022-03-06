
#include <stdio.h>
#include <stdlib.h>

#include <seal/math/rect.h>
#include <seal/testing/suite.h>

int TestRectFromPoints_OnALesserThenB(void) {
	int result = 0;

	Seal_Rect rect = Seal_RectFromPoints((Seal_Vector2){3, 1}, (Seal_Vector2){7, 9});

	ASSERT(rect.x == 3 && rect.y == 1);
	ASSERT(rect.w == 4 && rect.h == 8);

cleanup:
	return result;
}

int TestRectFromPoints_OnALesserThenBOnOnlyXAxis(void) {
	int result = 0;

	Seal_Rect rect = Seal_RectFromPoints((Seal_Vector2){3, 3}, (Seal_Vector2){-5, 9});

	ASSERT(rect.x == -5 && rect.y == 3);
	ASSERT(rect.w == 8 && rect.h == 6);

cleanup:
	return result;
}

int TestRectFromPoints_OnALesserThenBOnOnlyYAxis(void) {
	int result = 0;

	Seal_Rect rect = Seal_RectFromPoints((Seal_Vector2){3, 3}, (Seal_Vector2){10, 1});

	ASSERT(rect.x == 3 && rect.y == 1);
	ASSERT(rect.w == 7 && rect.h == 2);

cleanup:
	return result;
}

int TestRectFromPoints_OnALesserThenBOnBothAxis(void) {
	int result = 0;

	Seal_Rect rect = Seal_RectFromPoints((Seal_Vector2){6, 3}, (Seal_Vector2){-10, -1});

	ASSERT(rect.x == -10 && rect.y == -1);
	ASSERT(rect.w == 16 && rect.h == 4);

cleanup:
	return result;
}

int main() {
	int result = 0;

	// RectFromPoints
	RUN_TEST(TestRectFromPoints_OnALesserThenB);
	RUN_TEST(TestRectFromPoints_OnALesserThenBOnOnlyXAxis);
	RUN_TEST(TestRectFromPoints_OnALesserThenBOnOnlyYAxis);
	RUN_TEST(TestRectFromPoints_OnALesserThenBOnBothAxis);

	return result;
}