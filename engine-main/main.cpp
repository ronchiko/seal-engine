
#include <time.h>

#include "seal.h"

#define APPPROC extern "C"

APPPROC int Seal_InitializeUserSystems(void);
APPPROC int Seal_ActivateUserSystems(void);

APPPROC char *Seal_ProgramName;

extern "C" void Seal_FreeInputBuffering(void);
extern "C" void Seal_UpdateInputBuffers(void);

extern "C" Seal_Bool Seal_ExitFlag;

Seal_TimeUnit Seal_BeginTime, Seal_DeltaTimeMs; 
Seal_Float Seal_DeltaTime;

int main(int argc, char **argv) {
	if(!Seal_Init()) {
		return 1;
	}

	seal::window::options options = {
		SEAL_TRUE,
		SEAL_FALSE,
		SEAL_TRUE,
		SEAL_TRUE,
		SEAL_FALSE,
		nullptr
	};

	seal::window window(700, 700, Seal_ProgramName, options);
	Seal_SetMainCamera(Seal_AddCamera(Seal_Vector2{0, 0}, nullptr));

	Seal_InitializeSystems();
	
	Seal_InitTransformBuffer(seal::component_buffer<Seal_Transform2d>::instance().id());

	if(Seal_InitializeUserSystems()) {
		Seal_LogFatal("Failed to initialize user systems");
		return 1;
	}
	
	if(Seal_ActivateUserSystems()) {
		Seal_LogFatal("Failed to activate user systems");
		return 1;
	}

	Seal_BeginTime = clock(), Seal_DeltaTimeMs = 0;
	Seal_DeltaTime = 0;
	for(;!Seal_ExitFlag && window.should_continue();) {
		clock_t fs = clock();

		Seal_UpdateLoop();
		window.swap_buffers();
		
		Seal_DeltaTimeMs = clock() - fs;
		Seal_DeltaTime = Seal_DeltaTimeMs / 1000.f;
		Seal_UpdateInputBuffers();
	}

	Seal_FreeInputBuffering();
	Seal_TerminateSystems();

	Seal_CleanCameras();

	Seal_Cleanup();
	return 0;
}