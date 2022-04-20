
#include <stdio.h>
#include <stdlib.h>

#include <seal/math/rect.h>
#include <seal/testing/suite.h>

#pragma region RectFromPoints
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
#pragma endregion

#pragma region RectContains
int TestRectContains_WhenPointInsideRect_ThenTrue(void) {
	int result = 0;

	Seal_Rect rect = { .x = 1, .y = 1, .w = 5, .h = 5 };
	Seal_Bool contains = Seal_RectContains(&rect, (Seal_Vector2){2, 5});

	ASSERT(contains);

cleanup:
	return result;
}

int TestRectContains_WhenPointOnEdgeOfRect_ThenTrue(void) {
	int result = 0;

	Seal_Rect rect = { .x = 1, .y = 1, .w = 5, .h = 5 };
	Seal_Bool contains = Seal_RectContains(&rect, (Seal_Vector2){2, 6});

	ASSERT(contains);

cleanup:
	return result;
}

int TestRectContains_WhenPointOnCornerOfRect_ThenTrue(void) {
	int result = 0;

	Seal_Rect rect = { .x = 1, .y = 1, .w = 5, .h = 5 };
	Seal_Bool contains = Seal_RectContains(&rect, (Seal_Vector2){1, 1});

	ASSERT(contains);

cleanup:
	return result;
}

int TestRectContains_WhenPointOutOfRect_ThenFalse(void) {
	int result = 0;

	Seal_Rect rect = { .x = 1, .y = 1, .w = 5, .h = 5 };
	Seal_Bool contains = Seal_RectContains(&rect, (Seal_Vector2){-1, 189.37});

	ASSERT(!contains);

cleanup:
	return result;
}

int TestRectContains_WhenPointOutOfOnlyOnXRect_ThenFalse(void) {
	int result = 0;

	Seal_Rect rect = { .x = 1, .y = 1, .w = 5, .h = 5 };
	Seal_Bool contains = Seal_RectContains(&rect, (Seal_Vector2){-1, 3.0});

	ASSERT(!contains);

cleanup:
	return result;
}

int TestRectContains_WhenPointOutOfOnlyOnYRect_ThenFalse(void) {
	int result = 0;

	Seal_Rect rect = { .x = 1, .y = 1, .w = 5, .h = 5 };
	Seal_Bool contains = Seal_RectContains(&rect, (Seal_Vector2){1, -6});

	ASSERT(!contains);

cleanup:
	return result;
}

int TestRectContains_WhenRectNull_ThenFalse(void) {
	int result = 0;

	Seal_Bool contains = Seal_RectContains(SEAL_NULL, (Seal_Vector2){1, -6});

	ASSERT(!contains);

cleanup:
	return result;
}
#pragma endregion

#pragma region RectIntersects

int TestRectIntersects_WhenAIsB_ThenReturnTrue(void) {
	int result = 0;

	Seal_Rect rect = { .x = 1, .y = 1, .w = 5, .h = 5 };
	Seal_Bool intersects = Seal_RectIntersects(&rect, &rect);

	ASSERT(intersects);

cleanup:
	return result;
}

int TestRectIntersects_WhenAContainsB_ThenReturnTrue(void) {
	int result = 0;

	Seal_Rect rectA = { .x = 1, .y = 1, .w = 5, .h = 5 };
	Seal_Rect rectB = { .x = 2, .y = 2, .w = 2, .h = 2};
	Seal_Bool intersects = Seal_RectIntersects(&rectA, &rectB);

	ASSERT(intersects);

cleanup:
	return result;
}

int TestRectIntersects_WhenAOverlapsB_ThenReturnTrue(void) {
	int result = 0;

	Seal_Rect rectA = { .x = 1, .y = 1, .w = 5, .h = 5 };
	Seal_Rect rectB = { .x = 3, .y = 2, .w = 8, .h = 2};
	Seal_Bool intersects = Seal_RectIntersects(&rectA, &rectB);

	ASSERT(intersects);

cleanup:
	return result;
}

int TestRectIntersects_WhenAOutsideOfB_ThenReturnFalse(void) {
	int result = 0;

	Seal_Rect rectA = { .x = 1, .y = 1, .w = 5, .h = 5 };
	Seal_Rect rectB = { .x = 3, .y = 9, .w = 8, .h = 2};
	Seal_Bool intersects = Seal_RectIntersects(&rectA, &rectB);

	ASSERT(!intersects);

cleanup:
	return result;
}

#pragma endregion

int main() {
	int result = 0;

	// RectFromPoints
	RUN_TEST(TestRectFromPoints_OnALesserThenB);
	RUN_TEST(TestRectFromPoints_OnALesserThenBOnOnlyXAxis);
	RUN_TEST(TestRectFromPoints_OnALesserThenBOnOnlyYAxis);
	RUN_TEST(TestRectFromPoints_OnALesserThenBOnBothAxis);

	// RectContains
	RUN_TEST(TestRectContains_WhenPointInsideRect_ThenTrue);
	RUN_TEST(TestRectContains_WhenPointOnCornerOfRect_ThenTrue);
	RUN_TEST(TestRectContains_WhenPointOnEdgeOfRect_ThenTrue);
	RUN_TEST(TestRectContains_WhenPointOutOfOnlyOnXRect_ThenFalse);
	RUN_TEST(TestRectContains_WhenPointOutOfOnlyOnYRect_ThenFalse);
	RUN_TEST(TestRectContains_WhenPointOutOfRect_ThenFalse);
	RUN_TEST(TestRectContains_WhenRectNull_ThenFalse);

	// RectIntersects
	RUN_TEST(TestRectIntersects_WhenAIsB_ThenReturnTrue);
	RUN_TEST(TestRectIntersects_WhenAContainsB_ThenReturnTrue);
	RUN_TEST(TestRectIntersects_WhenAOverlapsB_ThenReturnTrue);
	RUN_TEST(TestRectIntersects_WhenAOutsideOfB_ThenReturnFalse);

	return result;
}