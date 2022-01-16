
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
	if (!glfwSuccessful) Seal_LogError("Failed to initialize GLFW", SEAL_FALSE);

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

void Seal_Log(const char *fmt, ...) {
	va_list _args;
	va_start(_args, fmt);
	
	vprintf(fmt, _args);
	printf("\n");
	fflush(stdout);

	va_end(_args);
}

void Seal_LogError(const char *fmt, Seal_Bool fatal, ...) {
	va_list _args;
	va_start(_args, fatal);
	
	printf("\033[31mERROR\033[0m: ");
	vprintf(fmt, _args);
	printf("\n");
	fflush(stdout);
	
	va_end(_args);

	if (fatal) exit(1);
}

void Seal_LogWarning(const char *fmt, ...) {
	va_list _args;
	va_start(_args, fmt);
	
	printf("\033[33mWARNING\033[0m: ");
	vprintf(fmt, _args);
	printf("\n");
	fflush(stdout);
	
	va_end(_args);
}

void *Seal_DeepCopy(void *src, size_t size) {
	void *dst = malloc(size);
	return memcpy(dst, src, size);
}