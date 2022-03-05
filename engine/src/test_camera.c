
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <seal/types/camera.h>

#include "seal/testing/suite.h"

int TestCameraAdd_OnOutGiven() {
	int result = 0;

	Seal_CameraObject *object;
	Seal_Camera cam = Seal_AddCamera((Seal_Vector2){0, 0}, &object);

	ASSERT(object != NULL);
	ASSERT(cam == 0);
	
	ASSERT(object->angle == 0);
	ASSERT(object->zoom == 1);
	ASSERT(object->position.x == 0 && object->position.y == 0);

cleanup:
	Seal_CleanCameras();

	ASSERT_LEAK;
	return result;
}

int TestCameraAdd_OnNoOutGiven() {
	int result = 0;

	Seal_Camera cam = Seal_AddCamera((Seal_Vector2){0, 0}, NULL);
	ASSERT(cam == 0);
	
cleanup:
	Seal_CleanCameras();

	ASSERT_LEAK;
	return result;
}


int TestCameraGet_WhenCameraIsValid_ThenReturnCameraObject() {
	int result = 0;

	Seal_Camera cam = Seal_AddCamera((Seal_Vector2){1, 0}, NULL);
	ASSERT(cam == 0);

	Seal_CameraObject *object = Seal_GetCamera(cam);
	
	ASSERT(object != NULL);
	ASSERT(object->angle == 0);
	ASSERT(object->zoom == 1);
	ASSERT(object->position.x == 1 && object->position.y == 0);

cleanup:
	Seal_CleanCameras();

	ASSERT_LEAK;
	return result;
}


int TestCameraGet_WhenCameraIsInvalid_ThenReturnNull() {
	int result = 0;

	Seal_Camera cam = Seal_AddCamera((Seal_Vector2){1, 0}, NULL);
	ASSERT(cam == 0);

	Seal_CameraObject *object = Seal_GetCamera(2);
	ASSERT(object == NULL);

cleanup:
	Seal_CleanCameras();

	ASSERT_LEAK;
	return result;
}

int TestCameraGet_WhenCameraIsDestroyed_ThenReturnNull() {
	int result = 0;

	Seal_Camera cam = Seal_AddCamera((Seal_Vector2){1, 0}, NULL);
	ASSERT(cam == 0);

	Seal_RemoveCamera(cam);
	Seal_CameraObject *object = Seal_GetCamera(cam);
	
	ASSERT(object == NULL);

cleanup:
	Seal_CleanCameras();

	ASSERT_LEAK;
	return result;
}


int TestCameraGetMain_WhenMainIsSet_ThenReturnMainCameraId() {
	int result = 0;

	Seal_Camera cam1 = Seal_AddCamera((Seal_Vector2){1, 0}, NULL);
	Seal_Camera cam2 = Seal_AddCamera((Seal_Vector2){0, 1}, NULL);
	Seal_Camera cam3 = Seal_AddCamera((Seal_Vector2){1, 1}, NULL);

	Seal_SetMainCamera(cam2);

	Seal_Camera main = Seal_GetMainCamera();
	ASSERT(main == cam2);

cleanup:
	Seal_CleanCameras();

	ASSERT_LEAK;
	return result;
}


int TestCameraGetMain_WhenMainIsUnset_ThenReturnFirstCameraId() {
	int result = 0;

	Seal_Camera cam1 = Seal_AddCamera((Seal_Vector2){1, 0}, NULL);
	Seal_Camera cam2 = Seal_AddCamera((Seal_Vector2){0, 1}, NULL);
	Seal_Camera cam3 = Seal_AddCamera((Seal_Vector2){1, 1}, NULL);

	Seal_Camera main = Seal_GetMainCamera();
	ASSERT(main == cam1);

cleanup:
	Seal_CleanCameras();

	ASSERT_LEAK;
	return result;
}


int TestCameraGetMain_WhenMainIsRemoved_ThenReturnFirstAvailableCamera() {
	int result = 0;

	Seal_Camera cam1 = Seal_AddCamera((Seal_Vector2){1, 0}, NULL);
	Seal_Camera cam2 = Seal_AddCamera((Seal_Vector2){0, 1}, NULL);
	Seal_Camera cam3 = Seal_AddCamera((Seal_Vector2){1, 1}, NULL);

	Seal_RemoveCamera(cam1);
	Seal_RemoveCamera(cam2);

	Seal_Camera main = Seal_GetMainCamera();
	ASSERT(main == cam3);

cleanup:
	Seal_CleanCameras();

	ASSERT_LEAK;
	return result;
}


int TestCameraGetMain_WhenAllCamerasRemoved_ThenReturnFailure() {
	int result = 0;

	Seal_Camera cam1 = Seal_AddCamera((Seal_Vector2){1, 0}, NULL);
	Seal_Camera cam2 = Seal_AddCamera((Seal_Vector2){0, 1}, NULL);
	Seal_Camera cam3 = Seal_AddCamera((Seal_Vector2){1, 1}, NULL);

	Seal_RemoveCamera(cam1);
	Seal_RemoveCamera(cam2);
	Seal_RemoveCamera(cam3);

	Seal_Camera main = Seal_GetMainCamera();
	ASSERT(main == -1);

cleanup:
	Seal_CleanCameras();

	ASSERT_LEAK;
	return result;
}


int TestCameraGetMain_WhenNoCameras_ThenReturnFailure() {
	int result = 0;

	Seal_Camera main = Seal_GetMainCamera();
	ASSERT(main == -1);

cleanup:
	Seal_CleanCameras();

	ASSERT_LEAK;
	return result;
}


int TestRemoveCamera_WhenValidCamera_ThenRemoveCamera() {
	int result = 0;

	Seal_Camera cam1 = Seal_AddCamera((Seal_Vector2){1, 0}, NULL);
	Seal_RemoveCamera(cam1);

	Seal_CameraObject *cam = Seal_GetCamera(cam1);
	ASSERT(cam == NULL);

cleanup:
	Seal_CleanCameras();

	ASSERT_LEAK;
	return result;
}


int main() {
	int result = 0;

	// Seal_AddCamera
	RUN_TEST(TestCameraAdd_OnNoOutGiven);
	RUN_TEST(TestCameraAdd_OnOutGiven);

	// Seal_GetCamera
	RUN_TEST(TestCameraGet_WhenCameraIsValid_ThenReturnCameraObject);
	RUN_TEST(TestCameraGet_WhenCameraIsInvalid_ThenReturnNull);
	RUN_TEST(TestCameraGet_WhenCameraIsDestroyed_ThenReturnNull);

	// Seal_GetMainCamera
	RUN_TEST(TestCameraGetMain_WhenMainIsSet_ThenReturnMainCameraId);
	RUN_TEST(TestCameraGetMain_WhenMainIsUnset_ThenReturnFirstCameraId);
	RUN_TEST(TestCameraGetMain_WhenMainIsRemoved_ThenReturnFirstAvailableCamera);
	RUN_TEST(TestCameraGetMain_WhenAllCamerasRemoved_ThenReturnFailure);
	RUN_TEST(TestCameraGetMain_WhenNoCameras_ThenReturnFailure);

	// Seal_RemoveCamera
	RUN_TEST(TestRemoveCamera_WhenValidCamera_ThenRemoveCamera);

	return result;
}