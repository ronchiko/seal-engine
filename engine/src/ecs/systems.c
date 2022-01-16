
#include <stdlib.h>

#include "seal/gl.h"
#include "seal/ecs/system.h"

#define CONTAINER_GROWTH 		10

#define TryStatelessInvokation(invoke)	if((invoke)) (*invoke)()

struct Seal_SystemsContainer {
	Seal_System *systems;
	Seal_Size systemsCount, allocted;
};

static struct Seal_SystemsContainer gSystems[__SEAL_SYSTEM_TYPES__] = {
	0
};

void Seal_InitializeSystems(void) {
	for(int i = 0; i < __SEAL_SYSTEM_TYPES__; ++i) {
		gSystems[i].systems = NULL;
		gSystems[i].systemsCount = gSystems[i].allocted = 0;
	}
}

void Seal_ActivateSystem(Seal_System *system) {
	typedef struct Seal_SystemsContainer Container;

	Container *container = gSystems + system->type;

	if (container->systemsCount >= container->allocted) {
		container->allocted += CONTAINER_GROWTH;
		container->systems = realloc(container->systems, sizeof(Seal_System) * container->allocted);
	}

	container->systems[container->systemsCount] = *system;
	container->systems[container->systemsCount++].required.components = 
		Seal_DeepCopy(system->required.components, system->required.count * sizeof(Seal_ComponentBuffer));

}

void Seal_TerminateSystems(void) {
	typedef struct Seal_SystemsContainer Container;
	for(Seal_Size i = 0; i < __SEAL_SYSTEM_TYPES__; ++i) {
		Container *container = gSystems + i;

		for(Seal_Size s = 0; s < container->systemsCount; ++s) {
			TryStatelessInvokation(container->systems[s].stateless_handlers.cleanup);
			free(container->systems[i].required.components);
		}

		free(container->systems);
	}
}

#define MAX_BUFFERS 		50

void Seal_UpdateLoop(void) {
	typedef struct Seal_SystemsContainer Container;
	Container *container = gSystems + SEAL_UPDATE_SYSTEM;
	
	GLCall(glClear(GL_COLOR_BUFFER_BIT));
	struct Seal_IterativeBuffer buffers[MAX_BUFFERS] = { 0 };

	for(Seal_Size i = 0; i < container->systemsCount; ++i) {
		Seal_ComponentsQuery query = container->systems[i].required;

		for(Seal_Size j = 0; j < query.count && j < MAX_BUFFERS; ++j)
			buffers[j] = Seal_BufferOf(query.components[j]);

		Seal_QueryContext context = {
			.buffers = buffers,
			.count = query.count,
			.hasNext = SEAL_FALSE
		};

		Seal_Component *components[MAX_BUFFERS];  
		Seal_Size size;

		TryStatelessInvokation(container->systems[i].stateless_handlers.prepare);

		// Entity events
		while(size = Seal_QueryComponents(&context, components, MAX_BUFFERS)) {
			(*container->systems[i].handler)(components, size);
		}

		TryStatelessInvokation(container->systems[i].stateless_handlers.finalize);
	}
}

// void Seal_InvokeSystems(Seal_SystemType type, Seal_Component *components, const Seal_Size count) {
// 	typedef struct Seal_SystemsContainer Container;
// 	Container *container = gSystems + type;

// 	for(int i = 0; i < container->used; ++i) {
// 		if(container->systems[i].required)
// 	}
// }