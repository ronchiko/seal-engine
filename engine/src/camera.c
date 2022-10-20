
#include <string.h>
#include <stdlib.h>

#include <seal/types/camera.h>

#define _NO_MAIN_CAMERA 	-1

typedef struct {
	struct {
		Seal_Bool active;
	} metadata;

	Seal_CameraObject camera;
} Camera;

static struct {
	Camera *array;
	Seal_Size used;
} gCameras = { NULL, 0 };

static Seal_Camera gMainCamera = _NO_MAIN_CAMERA;

static Seal_Camera _SealGetEmptyCameraId() {
	for(Seal_Camera i = 0; i < gCameras.used; ++i) {
		if (!gCameras.array[i].metadata.active)
			return i;
	}

	gCameras.array = realloc(gCameras.array, sizeof(Camera) * (gCameras.used + 1));
	memset(gCameras.array + gCameras.used, 0, sizeof(Camera)); 

	return gCameras.used++;
}

static Seal_Bool _SealIsCamera(Seal_Camera cam) {
	if(cam < 0 || cam >= gCameras.used) return SEAL_FALSE;

	return gCameras.array[cam].metadata.active;
}

static void _ReassignMainCamera(void) {
	for(Seal_Camera i = 0; i < gCameras.used; ++i) {
		if (gCameras.array[i].metadata.active) {
			gMainCamera = i;
			return;
		}
	}

	Seal_LogWarning("No cameras can be set as main, no main camera at the moment");
	gMainCamera = _NO_MAIN_CAMERA;
}

Seal_Camera Seal_AddCamera(Seal_Vector2 position, Seal_CameraObject **out) {
	Seal_Camera id = _SealGetEmptyCameraId();
	if (gMainCamera <= _NO_MAIN_CAMERA) gMainCamera = id;

	Seal_LogInfo("Creating camera with id %d, at position (%f, %f)", id, position.x, position.y);
	Camera *inserted = gCameras.array + id;
	inserted->metadata.active = SEAL_TRUE;

	inserted->camera.position = position;
	inserted->camera.angle = 0;
	inserted->camera.zoom  = 1;
	
	if(out) *out = &inserted->camera;

	return id; 
}

Seal_CameraObject *Seal_GetCamera(Seal_Camera cam) {
	return _SealIsCamera(cam) ? &gCameras.array[cam].camera : NULL;	
}

Seal_Camera Seal_GetMainCamera(void) {
	return gMainCamera;
}

void Seal_SetMainCamera(Seal_Camera c) {
	if(!_SealIsCamera(c)) {
		Seal_LogWarning("Can't set camera %d to main camera, because its not a valid camera", c);
		return;
	}

	Seal_LogInfo("Setting camera %d to main camera", c);
	gMainCamera = c;
} 

void Seal_RemoveCamera(Seal_Camera c) {
	if (!_SealIsCamera(c))  {
		Seal_LogWarning("Camera %s is not a valid camera", c);
		return;
	}

	Camera *cam = gCameras.array + c;
	cam->metadata.active = SEAL_FALSE;

	if (gMainCamera == c) {
		_ReassignMainCamera();
	} 
}

void Seal_CleanCameras(void) {
	free(gCameras.array);
	gCameras.used = 0;
	gCameras.array = NULL;
	gMainCamera = _NO_MAIN_CAMERA;
}