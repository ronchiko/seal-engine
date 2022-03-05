#pragma once

#include <seal/ecs/component.h>


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

Seal_Int Seal_GetComponentBufferSize(Seal_ComponentBuffer buffer);