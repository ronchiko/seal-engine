#pragma once

#include <stdint.h>

#include "seal/common.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct Seal_Window Seal_Window;

typedef struct {
	Seal_Bool scalable;
	Seal_Bool fullscreen;
	Seal_Bool decorated;
	Seal_Bool centered;
	Seal_Bool show;
	const char *iconPath;
} Seal_WindowOptions;

Seal_Window *Seal_CreateWindow(Seal_Size w, Seal_Size h,  const char *title, SEAL_OPTIONAL Seal_WindowOptions *);
void Seal_DestroyWindow(SEAL_OPTIONAL Seal_Window *w);

Seal_Bool Seal_WindowContinue(SEAL_OPTIONAL Seal_Window *);
void Seal_WindowSwapBuffers(Seal_Window *);
void Seal_SetWindowFullscreen(Seal_Window *, Seal_Bool fullscreen);
Seal_Bool Seal_GetWindowFullscreen(Seal_Window *);

void Seal_GetWindowStats(Seal_Window* window, Seal_Size *width, Seal_Size *height, Seal_Float *ratio);

#ifdef __cplusplus
}
#endif