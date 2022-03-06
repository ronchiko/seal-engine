#pragma once

#include "seal/common.h"

#define COMPONENT (Seal_Component *)

/*
	This structure is the base of every component, the engine assumes the first 2 fields of 
	the component are parent ID and component ID, so include this structure as the first field inside
	all your components
*/
typedef struct Seal_Component {
	Seal_ID parentId;
	Seal_ID componentId;
} Seal_Component;

typedef Seal_ID Seal_ComponentBuffer;

/* Attaches a component to an entity, returns the components ID */
Seal_ID Seal_AttachComponent(Seal_Entity entity, Seal_Component *component);

/* Allocates a new component buffer */
Seal_ComponentBuffer Seal_CreateComponentBuffer(Seal_Size componentSize);
/* Counts the amount of component buffers */
Seal_Size Seal_CountComponentBuffers(void);

/* Adds a component to a buffer */
Seal_Component *Seal_AddComponent(Seal_ComponentBuffer buffer, const Seal_Component *component);
/* Marks a component as deleted */
void Seal_RemoveComponent(Seal_ComponentBuffer buffer, Seal_ID entityId);
/* Deletes a marked components from a buffer */
void Seal_TrimComponentBuffer(Seal_ComponentBuffer buffer);
/* Frees a component buffer and all its data */
void Seal_FreeComponentBuffers(void);

/* Creates a new entity */
Seal_Entity Seal_CreateEntity(void);
/* Queues an entity to be deleted */
void Seal_DestroyEntity(Seal_Entity entity);
/* Deletes an entity and all of its components imeditatly */
void Seal_PurgeEntity(Seal_Entity entity);

typedef struct Seal_ComponentsQuery {
	Seal_ComponentBuffer *components;
	Seal_Size count;
} Seal_ComponentsQuery;
