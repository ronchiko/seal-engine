
#include <stdlib.h>
#include <string.h>
#include <GLFW/glfw3.h>

#include <seal/common.h>
#include <seal/io/input.h>

typedef enum Seal_KeyActionType {
	SEAL_ACTION_UNASSINGED,
	SEAL_KEY_ACTION_BUTTON,
	SEAL_KEY_ACTION_AXIS
} Seal_KeyActionType;

typedef struct Seal_KeySet {
	int count;
	int *keys;
} Seal_KeySet;

typedef struct {
	Seal_KeyActionType type;
	union {	
		Seal_KeySet button;
		struct {
			Seal_KeySet positive;
			Seal_KeySet negative;
		} axis;
	};
} _Seal_KeyBinding;

static _Seal_KeyBinding gBindings[0x100] = { 0 };

int gKeyStates[GLFW_KEY_LAST + 1] = { SEAL_KEY_UP };

void _OnKeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
	if(key == GLFW_KEY_UNKNOWN) return;
	
	switch(action) {
	case GLFW_PRESS:
		gKeyStates[key] = SEAL_INPUT_PRESSED;
		break;
	case GLFW_RELEASE:
		gKeyStates[key] = SEAL_INPUT_RELEASED;
		break;
	}
}

void Seal_InitializeInputBuffering(GLFWwindow *window) {
	for(int i = 0; i < 0xFF; ++i) {
		gBindings[i].type = SEAL_ACTION_UNASSINGED;
	}

	glfwSetKeyCallback((GLFWwindow *)window, &_OnKeyCallback);
}

void Seal_FreeInputBuffering(void) {
	for(int i = 0; i < 0xFF; ++i) {
		switch(gBindings[i].type) {
		case SEAL_KEY_ACTION_AXIS:
			free(gBindings[i].axis.positive.keys);
			free(gBindings[i].axis.negative.keys);\
			break;
		case SEAL_KEY_ACTION_BUTTON:
			free(gBindings[i].button.keys);
			break;
		}
		
		gBindings[i].type = SEAL_ACTION_UNASSINGED;
	}
}

void Seal_UpdateInputBuffers(void) {
	for(int i = 0; i < GLFW_KEY_LAST + 1; ++i) {
		if(gKeyStates[i] == SEAL_INPUT_PRESSED) 
			gKeyStates[i] = SEAL_INPUT_DOWN;
		if(gKeyStates[i] == SEAL_INPUT_RELEASED) 
			gKeyStates[i] = SEAL_INPUT_UP;
	}
}

int Seal_RegisterButtonKeyBinding(Seal_KeyBinding id, int *keys, int count) {
	if (gBindings[id].type != SEAL_ACTION_UNASSINGED) return -1;

	const int keyBufferSize = sizeof(int) * count;

	gBindings[id].type = SEAL_KEY_ACTION_BUTTON;
	gBindings[id].button.count = count;
	gBindings[id].button.keys = malloc(keyBufferSize);

	memcpy(gBindings[id].button.keys, keys, keyBufferSize); 

	return 0;
}

int Seal_RegisterAxisKeyBindings(Seal_KeyBinding id, int *ps, int pc, int *ns, int nc) {
	if (gBindings[id].type != SEAL_ACTION_UNASSINGED) return -1;

	const int posBufferSize = sizeof(int) * pc;
	const int negBufferSize = sizeof(int) * nc;

	gBindings[id].type = SEAL_KEY_ACTION_AXIS;
	gBindings[id].axis.positive.count = pc;
	gBindings[id].axis.negative.count = nc;
	
	gBindings[id].axis.positive.keys = malloc(posBufferSize);
	gBindings[id].axis.negative.keys = malloc(negBufferSize);

	memcpy(gBindings[id].axis.positive.keys, ps, posBufferSize); 
	memcpy(gBindings[id].axis.negative.keys, ns, negBufferSize); 

	return 0;
}

Seal_Bool _IsOneKeyInState(Seal_KeySet set, Seal_InputButtonState state) {
	for(int i = 0; i < set.count; ++i) {
		if (gKeyStates[set.keys[i]] == state) return SEAL_TRUE;
	}
	return SEAL_FALSE;
}

#define _BUTTON_STATE_FNC(state, stateEnum)\
Seal_Bool Seal_IsButton##state(Seal_KeyBinding id) {\
	switch(gBindings[id].type){\
	case SEAL_KEY_ACTION_BUTTON: \
		return _IsOneKeyInState(gBindings[id].button, stateEnum);\
	default: return SEAL_FALSE; \
	}\
}

_BUTTON_STATE_FNC(Pressed, SEAL_INPUT_PRESSED);
_BUTTON_STATE_FNC(Down, SEAL_INPUT_DOWN);
_BUTTON_STATE_FNC(Released, SEAL_INPUT_RELEASED);
_BUTTON_STATE_FNC(Up, SEAL_INPUT_UP);

float Seal_GetAxis(Seal_KeyBinding id) {
	if (gBindings[id].type != SEAL_KEY_ACTION_AXIS) return 0;
	
	Seal_KeySet posSet = gBindings[id].axis.positive; 
	Seal_KeySet negSet = gBindings[id].axis.negative; 

	float pos = (int)(_IsOneKeyInState(posSet, SEAL_INPUT_DOWN));
	float neg = (int)(_IsOneKeyInState(negSet, SEAL_INPUT_DOWN));

	return pos - neg;
}