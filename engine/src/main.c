
#include <time.h>

#include "seal.h"
#include "seal/render2d/batcher.h"

#define APPPROC extern

APPPROC int Seal_InitializeUserSystems(void);
APPPROC int Seal_ActivateUserSystems(void);

APPPROC char *Seal_ProgramName;

extern void Seal_FreeInputBuffering(void);
extern void Seal_UpdateInputBuffers(void);

Seal_TimeUnit Seal_BeginTime, Seal_DeltaTimeMs; 
Seal_Float Seal_DeltaTime;

int main(int argc, char **argv) {
	if(!Seal_Init()) {
		return 1;
	}

	Seal_WindowOptions opts = {
		.show = SEAL_TRUE,
		.fullscreen = SEAL_FALSE,
		.decorated = SEAL_TRUE,
		.centered = SEAL_TRUE,
		.iconPath = NULL
	};
	Seal_Window *window = Seal_CreateWindow(700, 700, Seal_ProgramName, &opts); 
	Seal_SetMainCamera(Seal_AddCamera((Seal_Vector2){0, 0}, NULL));

	Seal_InitializeSystems();
	
	Seal_InitTransformBuffer();
	if(Seal_InitializeUserSystems()) {
		Seal_LogFatal("Failed to initialize user systems");
		return 1;
	}
	
	Seal_ActivateUserSystems();

	Seal_BeginTime = clock(), Seal_DeltaTimeMs = 0;
	Seal_DeltaTime = 0;
	while(Seal_WindowContinue(window)) {
		clock_t fs = clock();

		Seal_UpdateLoop();
		Seal_WindowSwapBuffers(window);
		
		Seal_DeltaTimeMs = clock() - fs;
		Seal_DeltaTime = Seal_DeltaTimeMs / 1000.f;
		Seal_UpdateInputBuffers();
	}

	Seal_FreeInputBuffering();
	Seal_TerminateSystems();

	Seal_CleanCameras();

	Seal_DestroyWindow(window);

	Seal_Close();
	return 0;
}