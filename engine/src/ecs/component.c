
#include <stdlib.h>
#include <string.h>

#include "seal/ecs/component.h"

#define COMPONENT_BUFFER_BUFFER_GROWTH		100
#define COMPONENT_BUFFER_ARRAY_GROWTH		10

/* Virtual structures macros */
#define VSTRUCT_AT(b, i)					(b) + virtualStructure * (i)
#define VSTRUCT_H_AT(b, i)					((Seal_ComponentHeader *)(VSTRUCT_AT(b, i)))
#define VSTRUCT_STRUCT_AT(b, i)				((Seal_Component *)(VSTRUCT_AT(b, i) + sizeof(Seal_ComponentHeader)))
#define VSTRUCT_SIZE(l)						(l) * virtualStructure

typedef enum {
	SEAL_NO_FLAGS = 0,
	SEAL_DELETED_COMPONENT = Seal_Bit(0),
	SEAL_INACTIVE_COMPONENT = Seal_Bit(1),
} Seal_ComponentHeaderFlags;

/* Contains some metadata about the  */
typedef struct Seal_ComponentHeader {
	Seal_Byte flags;	
} Seal_ComponentHeader;

/* A Component buffer is a sorted array of components by their entity ids */
struct Seal_ComponentBuffer {
	/*
		Each component contains some metadata that is stored before the component data itself, 
		when using the buffer that must be taken to considiration
	*/
	Seal_Byte *buffer;
	Seal_Size sizeofComponent;
	Seal_Size allocted, used;
};

/* \brief All the buffers */
struct {
	struct Seal_ComponentBuffer *buffers;
	Seal_Size allocted, used; 
} gBuffers;

Seal_ComponentBuffer Seal_CreateComponentBuffer(Seal_Size componentSize) {
	typedef struct Seal_ComponentBuffer Buffer;

	if(gBuffers.allocted <= gBuffers.used) {
		gBuffers.allocted += COMPONENT_BUFFER_ARRAY_GROWTH;
		gBuffers.buffers = realloc(gBuffers.buffers, gBuffers.allocted * sizeof(Buffer));
	}

	Seal_ComponentBuffer id;
	gBuffers.buffers[id = gBuffers.used++] = (Buffer) {
		.sizeofComponent = componentSize,
		.allocted = 0,
		.used = 0,
		.buffer = NULL
	};

	Seal_Component cmpnnt = { .parentId = 0, .componentId = 0 };  
	Seal_AddComponent(id, &cmpnnt);
	return id;
}

void Seal_FreeComponentBuffers(void) {
	for(Seal_Size i = 0; i < gBuffers.used; ++i)
		free(gBuffers.buffers[i].buffer);

	free(gBuffers.buffers);
	gBuffers.used = gBuffers.allocted = 0;
	gBuffers.buffers = NULL;
}

static Seal_Size Seal_FindComponentWithId(Seal_Byte *buf, Seal_Int begin, Seal_Int length, Seal_Size componentSize, Seal_ID id) {
	const Seal_Size virtualStructure = componentSize + sizeof(Seal_ComponentHeader);

	if (length <= 0) return -(begin + 1);
	if (length == 1) {
		int diff = VSTRUCT_STRUCT_AT(buf, begin)->parentId - id;
		return diff ? -(begin + 2) : begin;
	}

	const Seal_Size _length = length >> 1;
	const Seal_Size idiff = VSTRUCT_STRUCT_AT(buf, begin + _length)->parentId - id;
	if (!idiff) return begin + _length;
	else if (idiff > 0) return Seal_FindComponentWithId(buf, begin, _length, componentSize, id);
	
	return Seal_FindComponentWithId(buf, begin + _length, length - _length, componentSize, id);
}

Seal_Component *Seal_AddComponent(Seal_ComponentBuffer buffer, const Seal_Component *component) {
	if(!component || buffer < 0 || buffer >= (Seal_ID)gBuffers.used) {
		Seal_LogError("%zu is not a valid component buffer", SEAL_FALSE, buffer);
		return SEAL_ID_INVALID;
	}

	struct Seal_ComponentBuffer *buf = gBuffers.buffers + buffer;
	const Seal_Size virtualStructure = buf->sizeofComponent + sizeof(Seal_ComponentHeader);

	if (buf->allocted <= buf->used) {
		buf->allocted += COMPONENT_BUFFER_BUFFER_GROWTH;
		buf->buffer = realloc(buf->buffer, buf->allocted * virtualStructure);
	} 

	Seal_Int index = Seal_FindComponentWithId(buf->buffer, 0, buf->used, buf->sizeofComponent, component->parentId);
	if (index < 0) index = -index - 1;

	memmove(VSTRUCT_AT(buf->buffer, index + 1), VSTRUCT_AT(buf->buffer, index), VSTRUCT_SIZE(buf->used - index));
	Seal_Component *ncmp = memcpy(VSTRUCT_STRUCT_AT(buf->buffer, index), component, buf->sizeofComponent);

	Seal_ComponentHeader *header = VSTRUCT_H_AT(buf->buffer, index);
	header->flags = SEAL_NO_FLAGS;

	++buf->used;
	return ncmp;
}

void Seal_RemoveComponent(Seal_ComponentBuffer buffer, Seal_ID componentId) {
	if(componentId <= 0 || buffer < 0 || (Seal_Size)buffer >= gBuffers.used) {
		Seal_LogError("Failed to remove component %d from buffer %d", componentId, buffer);
		return;
	}
	
	struct Seal_ComponentBuffer *buf = gBuffers.buffers + buffer;
	const Seal_Size virtualStructure = buf->sizeofComponent + sizeof(Seal_ComponentHeader);

	Seal_Size index = Seal_FindComponentWithId(buf->buffer, 0, buf->used, buf->sizeofComponent, componentId);
	if (index >= 0) {
		VSTRUCT_H_AT(buf->buffer, index)->flags |= SEAL_DELETED_COMPONENT;
	}
}

void Seal_TrimComponentBuffer(Seal_ComponentBuffer buffer) {
	if(buffer < 0 || (Seal_Size)buffer >= gBuffers.used){
		Seal_LogError("Failed to trim buffer %d", buffer);
		return;
	}
	
	int move = 0;
	struct Seal_ComponentBuffer *buf = gBuffers.buffers + buffer;
	const Seal_Size virtualStructure = buf->sizeofComponent + sizeof(Seal_ComponentHeader);

	for(Seal_Size i = 0; i < buf->used; ++i){
		if (VSTRUCT_H_AT(buf->buffer, i)->flags & SEAL_DELETED_COMPONENT) {
			++move;
			continue;
		}

		memmove(VSTRUCT_AT(buf->buffer, i - move), VSTRUCT_AT(buf->buffer, i), virtualStructure);
	}
	buf->used -= move;
}

Seal_Size Seal_CountComponentBuffers(void) {
	return gBuffers.used;
}

struct Seal_IterativeBuffer Seal_BufferOf(Seal_ComponentBuffer buffer) {
	if(buffer < 0 || (Seal_Size)buffer >= gBuffers.used){
		Seal_LogError("Failed to find buffer %d", buffer);
		return (struct Seal_IterativeBuffer){
			.buffer = NULL,
			.elements = 0,
			.elementSize = 0
		};
	}

	struct Seal_ComponentBuffer *buf = gBuffers.buffers + buffer;
	const Seal_Size vsize = buf->sizeofComponent + sizeof(Seal_ComponentHeader);
	return (struct Seal_IterativeBuffer) {
		.buffer = buf->buffer + vsize,
		.elements = buf->used - 1,
		.elementSize = vsize
	};
}

inline Seal_Component *Seal_AdvanceUntilIdOver(struct Seal_IterativeBuffer *buffer, Seal_ID id) {
	struct Seal_IterativeBuffer prestate = *buffer;
	for(;buffer->elements > 0;) {
		Seal_Component *component = (Seal_Component *)(buffer->buffer + sizeof(Seal_ComponentHeader));

		if(component->parentId > id) return NULL;

		else if(component->parentId == id) return component;

		--buffer->elements;
		buffer->buffer += buffer->elementSize;
	}

	*buffer = prestate;
	return NULL;
}

inline Seal_Component *Seal_GetNext(struct Seal_IterativeBuffer *buffer) {
	if(buffer->elements <= 0) return NULL;

	--buffer->elements;
	char *bufferState = buffer->buffer;
	buffer->buffer += buffer->elementSize;

	return (Seal_Component *)(bufferState + sizeof(Seal_ComponentHeader));
}

Seal_Size Seal_QueryComponents(Seal_QueryContext *ctx, Seal_Component **outBuffer, Seal_Size maxCount) {
	Seal_Size count;
	Seal_Bool missing;

	do {
		missing = SEAL_FALSE;
		count = 0;
		Seal_Component *component = Seal_GetNext(ctx->buffers);
		if (!component)
			return 0;
		
		outBuffer[count++] = component;
		for (int i = 1;component && i < ctx->count; ++i) {
			component = Seal_AdvanceUntilIdOver(ctx->buffers + i, component->parentId);
			
			if(count >= maxCount || !component) {
				missing = SEAL_TRUE;
				break;
			}

			outBuffer[count++] = component;
		}

	} while(missing);

	return count;
}


// *
// Internal testing method
// *

Seal_Bool _Seal_TestBufferSize(Seal_ComponentBuffer buffer, Seal_Size compareTo) {
	if (buffer < 0 || (Seal_Size)buffer >= gBuffers.used)
		return SEAL_FALSE;
	struct Seal_ComponentBuffer *buf = gBuffers.buffers + buffer;
	return buf->used == compareTo + 1;
}