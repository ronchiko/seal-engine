#pragma once

#include "seal/common.h"

/*
	This structure is the base of every component, the engine assumes the first 2 fields of 
	the component are parent ID and component ID, so include this structure as your field inside
	a component
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

typedef struct Seal_IterativeBuffer {
	char *buffer;
	int elementSize, elements;
} Seal_IterativeBuffer;

typedef struct Seal_QueryContext {
	struct Seal_IterativeBuffer	*buffers;
	int  						 count;

	Seal_Bool 					 hasNext;
} Seal_QueryContext;

/**
 * \brief Queries a list of component buffers for all of the entities that exists in them 
 * 
 * \param ctx Context of the query, pass NULL to start a new query
 * \param outBuffer The output buffer (size=required.count)
 * \param count The amount of components int the output buffer
 */
Seal_IterativeBuffer Seal_BufferOf(Seal_ComponentBuffer buffer);
Seal_Size Seal_QueryComponents(Seal_QueryContext *ctx, Seal_Component **outBuffer, Seal_Size maxCount);

Seal_Bool Seal_ComponentsMatch(Seal_ComponentsQuery query, Seal_Component **components, const Seal_Size compoCount);