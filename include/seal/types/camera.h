#pragma once

#include "seal/math.h"

typedef struct {
	Seal_Vector2 position;

	float angle;
	float zoom;
} Seal_CameraObject;

Seal_Camera Seal_AddCamera(Seal_Vector2 position, Seal_CameraObject **out);
void Seal_SetMainCamera(Seal_Camera camera);
void Seal_RemoveCamera(Seal_Camera camera);

Seal_CameraObject *Seal_GetCamera(Seal_Camera camera);

Seal_Camera Seal_GetMainCamera(void);
void Seal_CleanCameras(void);
