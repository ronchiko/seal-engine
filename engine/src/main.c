
#include <time.h>

#include "seal.h"
#include "seal/render2d/batcher.h"

extern int Seal_InitializeUserSystems(void);
extern int Seal_ActivateUserSystems(void);

extern char *Seal_ProgramName;

Seal_TimeUnit Seal_BeginTime, Seal_DeltaTimeMs; 
Seal_Float Seal_DeltaTime;

int main(int argc, char **argv) {
	if(!Seal_Init()) {
		return 1;
	}

	Seal_InitializeSystems();
	Seal_Window *window = Seal_CreateWindow(700, 700, Seal_ProgramName, NULL); 

	Seal_InitTransformBuffer();
	if(Seal_InitializeUserSystems()) {
		Seal_LogError("Failed to initialize user systems", SEAL_TRUE);
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

		printf("FPS: %3d\r", (int)(1 / Seal_DeltaTime));
	}

	Seal_TerminateSystems();
	Seal_FreeAllBatcher2ds();

	Seal_DestroyWindow(window);

	return 0;
}