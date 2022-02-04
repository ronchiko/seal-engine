#define _USE_BASE_MALLOC
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <seal/common.h>

#ifdef _DEBUG
#include <seal/debug.h>

#define LIST_GROWTH_RATE 200

#define _MUT_WAIT(mut) while((mut));
#define _MUT_LOCK(mut) mut = SEAL_TRUE;
#define _MUT_FREE(mut) mut = SEAL_FALSE;

struct Seal_DbgAddr { 
	void *addr;
	int line;
	const char *file;
	int pcl;
	size_t bytes;
};

struct Seal_DbgAddresserSet {
	struct Seal_DbgAddr *addresses;
	int allocated;
	int used;
};

static Seal_Bool gMemorySetMutex = SEAL_FALSE;
static struct Seal_DbgAddresserSet gMemorySet = { 
	.addresses = 0, 
	.allocated = 0,
	.used = 0
};

static Seal_Size PRIME = 0; 
static Seal_Size PRIMES[14] = {
	2, 53, 97, 193, 389, 769, 1543, 3079, 12289, 49157, 786433, 12582917, 402653189, 1610612741
};

inline static Seal_Size Seal_HashAddress(void *addr, Seal_Size wp) {
	return ((uintptr_t)addr + 3) % wp;
}

static void Seal_PutAddr(void *addr, size_t s, const char *file, int line) {
	const int length = gMemorySet.allocated;
	int position = Seal_HashAddress(addr, length);

	for(int offset = 0; offset < gMemorySet.allocated; ++offset) {
		if (gMemorySet.addresses[(offset + position) % length].addr) continue;

		gMemorySet.addresses[(offset + position) % length] = (struct Seal_DbgAddr) {
			.addr = addr,
			.file = file,
			.line = line,
			.pcl = 0,
			.bytes = s
		};

		gMemorySet.addresses[position].pcl = max(gMemorySet.addresses[position].pcl, offset);
		++gMemorySet.used;
		break;
	} 
}

static void _RescaleSetUnsafe() {
	if (gMemorySet.used >= gMemorySet.allocated) {
		gMemorySet.allocated = PRIMES[PRIME++];
		struct Seal_DbgAddr *oSet = gMemorySet.addresses;
		int oUsed = gMemorySet.used;

		gMemorySet.addresses = malloc(gMemorySet.allocated * sizeof(struct Seal_DbgAddr));
		gMemorySet.used = 0;

		memset(gMemorySet.addresses, 0, gMemorySet.allocated * sizeof(struct Seal_DbgAddr));
		for (int i = 0; i < oUsed; ++i) {
			if(oSet[i].addr) 
				Seal_PutAddr(oSet[i].addr, oSet[i].bytes ,oSet[i].file, oSet[i].line);
		}
	}
}

void *Seal_DbgAllocate(size_t w, const char *file, int line) {
	void *addr = malloc(w);

	_MUT_WAIT(gMemorySetMutex);
	_MUT_LOCK(gMemorySetMutex);
	_RescaleSetUnsafe();

	Seal_PutAddr(addr, w, file, line);
	_MUT_FREE(gMemorySetMutex);
	return addr;
}

void *Seal_DbgCleanAllocate(size_t count, size_t size, const char *file, int line) {
	void *addr = calloc(count, size);

	_MUT_WAIT(gMemorySetMutex);
	_MUT_LOCK(gMemorySetMutex);

	_RescaleSetUnsafe();
	Seal_PutAddr(addr, count * size, file, line);
	
	_MUT_FREE(gMemorySetMutex);
	return addr;
}

void Seal_DbgRelease(void *addr) {
	const int length = gMemorySet.allocated;
	int position = Seal_HashAddress(addr, length);

	_MUT_WAIT(gMemorySetMutex);
	_MUT_LOCK(gMemorySetMutex);

	int *pcl = &gMemorySet.addresses[position].pcl;
	for(int offset = 0; offset <= *pcl; ++offset) {
		if(gMemorySet.addresses[(position + offset) % length].addr != addr) continue;
		
		gMemorySet.addresses[(position + offset) % length].addr = NULL;
		if(offset == *pcl)
			*pcl--;
		
		--gMemorySet.used;
		break;
	}

	_MUT_FREE(gMemorySetMutex);
}

void *Seal_DbgReallocate(void *addr, size_t size, const char *file, int line) {
	if(!addr) {
		return Seal_DbgAllocate(size, file, line);
	}

	const int length = gMemorySet.allocated;
	int position = Seal_HashAddress(addr, length);
	
	_MUT_WAIT(gMemorySetMutex);
	_MUT_LOCK(gMemorySetMutex);

	int *pcl = &gMemorySet.addresses[position].pcl, offset;
	for(offset = 0; offset <= *pcl; ++offset) {
		void **aAddr = &gMemorySet.addresses[(position + offset) % length].addr;
		if(*aAddr != addr) continue;
		
		*aAddr = NULL;
		addr = realloc(*aAddr, size);
		if(!addr) {
			Seal_LogError("Out of memory", SEAL_FALSE);
			goto cleanup;
		}

		Seal_PutAddr(addr, size, file, line);
		
		--gMemorySet.used;

		_MUT_FREE(gMemorySetMutex);
		return addr;
	}

	Seal_LogError("Failed to realloc object", SEAL_FALSE);
cleanup:
	_MUT_FREE(gMemorySetMutex);
	return NULL;
}

Seal_Bool Seal_LeakCheck(void) {
	return gMemorySet.used > 0;
}

void Seal_DumpLeak(void *stream) {
	if(gMemorySet.used <= 0) return;

	fprintf(stream, "\033[31;1mMemory leak detected:\033[0m\n");
	for(int i = 0; i < gMemorySet.allocated; ++i) {
		struct Seal_DbgAddr rAddr = gMemorySet.addresses[i];
		if(rAddr.addr)
			fprintf(stream, "  UNFREED MEMORY: %zu bytes lost -> \033[33m%s:%d\033[0m\n", rAddr.bytes, rAddr.file, rAddr.line);
	}
}

void Seal_CleanDebugData() {
	PRIME = 0;
	for(int i = 0; i < gMemorySet.allocated; ++i) {
		struct Seal_DbgAddr rAddr = gMemorySet.addresses[i];
		if(rAddr.addr) free(rAddr.addr);
	}
	
	free(gMemorySet.addresses);
	gMemorySet.addresses = NULL;
	gMemorySet.used = gMemorySet.allocated = 0;
}

#endif