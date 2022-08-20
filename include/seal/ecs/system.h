#pragma once

#include "seal/ecs/component.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*Seal_SystemHandler)(Seal_Component **components, const Seal_Size count);
typedef void (*Seal_SystemStatelessHandler)(void);

typedef enum Seal_SystemType {
	SEAL_UPDATE_SYSTEM,				// Invoked every frame
	SEAL_DESTRUCTION_SYSTEM,		// Invoked when an entity is destroyed
	SEAL_CREATION_SYSTEM,			// Invoked when an entity is created
	SEAL_INITIALIZATION_SYSTEM,		// Invoked when the engine starts
	SEAL_CLOSING_SYSTEM,				// Invoked when the engine closes

	__SEAL_SYSTEM_TYPES__
} Seal_SystemType;

typedef struct Seal_System {
	Seal_SystemType type;

	Seal_ComponentsQuery required;
	Seal_SystemHandler handler;
	struct  {
		Seal_SystemStatelessHandler prepare;
		Seal_SystemStatelessHandler finalize;
		Seal_SystemStatelessHandler cleanup;
	} stateless_handlers;
} Seal_System;

void Seal_InitializeSystems(void);
void Seal_ActivateSystem(Seal_System *system);
void Seal_TerminateSystems(void);

void Seal_InvokeSystems(Seal_SystemType type, Seal_Component *components, const Seal_Size count);

void Seal_UpdateLoop(void);

#define SEAL_SYSTEM(name, ...)
#define SEAL_SYSTEM_FINALIZE(name)
#define SEAL_SYSTEM_PREPARE(name)
#define SEAL_SYSTEM_CLEANUP(name)

#define SEAL_ADD_SYSTEM(name, update, finalize, prepare, cleanup, ...)

#ifdef __cplusplus
}
#endif
