#pragma once

#include "common.h"

typedef enum {
	SEAL_KEY_LEFT,
	SEAL_KEY_RIGHT,
	SEAL_KEY_UP,
	SEAL_KEY_DOWN,
	SEAL_KEY_0,
	SEAL_KEY_1,
	SEAL_KEY_2,
	SEAL_KEY_3,
	SEAL_KEY_4,
	SEAL_KEY_5,
	SEAL_KEY_6,
	SEAL_KEY_7,
	SEAL_KEY_8,
	SEAL_KEY_9,
	SEAL_KEY_A,
	SEAL_KEY_B,
	SEAL_KEY_C,
	SEAL_KEY_D,
	SEAL_KEY_E,
	SEAL_KEY_F,
	SEAL_KEY_G,
	SEAL_KEY_H,
	SEAL_KEY_I,
	SEAL_KEY_L,
	SEAL_KEY_M,
	SEAL_KEY_N,
	SEAL_KEY_O,
	SEAL_KEY_P,
	SEAL_KEY_Q,
	SEAL_KEY_R,
	SEAL_KEY_S,
	SEAL_KEY_T,
	SEAL_KEY_U,
	SEAL_KEY_V,
	SEAL_KEY_W,
	SEAL_KEY_X,
	SEAL_KEY_Y,
	SEAL_KEY_Z,
} Seal_Key;

typedef enum {
	SEAL_INPUT_UP,			// Button is currently not pressed
	SEAL_INPUT_DOWN,		// Button is currently pressed
	SEAL_INPUT_PRESSED,		// Before SEAL_INPUT_DOWN, after SEAL_INPUT_UP
	SEAL_INPUT_RELEASED,	// Before SEAL_INPUT_UP, after SEAL_INPUT_DOWN
} Seal_InputButtonState;

typedef Seal_UByte Seal_KeyBinding;

int Seal_RegisterButtonKeyBinding(Seal_KeyBinding id, int *keys, int count);
int Seal_RegisterAxisKeyBindings(Seal_KeyBinding id, int *ps, int pc, int *ns, int nc);

Seal_Bool Seal_IsButtonPressed(Seal_KeyBinding binding);
Seal_Bool Seal_IsButtonDown(Seal_KeyBinding binding);
Seal_Bool Seal_IsButtonReleased(Seal_KeyBinding binding);
Seal_Bool Seal_IsButtonUp(Seal_KeyBinding binding);

float Seal_GetAxis(Seal_KeyBinding binding);