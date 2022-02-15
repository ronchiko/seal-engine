
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "seal/debug.h"

#include "seal/common.h"
#include "seal/ecs/component.h"

Seal_Bool Seal_Init(void) {
	Seal_Bool glfwSuccessful = glfwInit() != GLFW_TRUE ? SEAL_FALSE : SEAL_TRUE;
	if (!glfwSuccessful) Seal_LogError("Failed to initialize GLFW");

	return glfwSuccessful;
}

void Seal_Close(void) {
	Seal_FreeComponentBuffers();
	
#ifdef _DEBUG
	// Check for memory leaks
	if (Seal_LeakCheck()) {
		Seal_DumpLeak(stdout);
	}
#endif
	glfwTerminate();
}
