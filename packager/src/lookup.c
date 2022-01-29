
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "lookup.h"

typedef struct _Seal_LookupElement {
	char *key;
	int archive;
} Seal_LookupElement;

struct _Seal_LookupTable {
	struct {
		char **raw;
		size_t count;
		size_t alloc;
	} archives;

	struct {
		size_t count;
		size_t alloc;
		Seal_LookupElement *raw;
	} table;

	char *encryptionKey;
};

Seal_LookupTable Seal_CreateNewLookup(void) {
	Seal_LookupTable table = malloc(sizeof(struct _Seal_LookupTable));
	if(!table) 	// OOM
		return NULL;

	table->archives.alloc = table->archives.count = 0;
	table->table.alloc = table->table.count = 0;
	table->archives.raw = (void *)(table->table.raw = NULL);

	table->encryptionKey = NULL;

	return table;
}

#define HASH_FUNCTION_DEF(w) #w; w

static const char *gHashFunctionStr = HASH_FUNCTION_DEF(
	uint64_t _HashString(const char *str) {
		uint64_t hash = 5381;
		
		int c;
		while(c = *str++) hash = ((hash << 5) + hash) + c;

		return hash;
	}
);

void Seal_WriteLookup(Seal_LookupTable lookup, const char *path) {
	if(!lookup) return;
	
	FILE *file;
	if(file = fopen(path, "w+")) {
		char encryptionKeyString[40];
		memcpy(encryptionKeyString, "NULL", sizeof(char) * 5);
		if (lookup->encryptionKey)
			snprintf(encryptionKeyString, 39, "\"%s\"", lookup->encryptionKey);

		fprintf(file, "\n#include <stdint.h>\n#include <stdlib.h>\n\nconst char *ENCRYPTION_KEY = %s;\n\n", encryptionKeyString);
		fprintf(file, "%s\n\n", gHashFunctionStr);

		fprintf(file, "const char *Seal_GetFileArchive(const char *source) {\n\tswitch(_HashString(source)) {\n\n");
		for(int i = 0; i < lookup->table.count; ++i) {
			uint64_t hash = _HashString(lookup->table.raw[i].key);
			int archiveIndex = lookup->table.raw[i].archive;
			
			if (archiveIndex >= lookup->archives.count) {
				// Fail!
				continue;
			}

			const char *archive = lookup->archives.raw[archiveIndex];

			fprintf(file, "\tcase %zu:\treturn \"%s\";\t// %s\n", hash, archive, lookup->table.raw[i].key);
		}
		fprintf(file, "\n\tdefault: return NULL;\n\t}\n}");

		
		fclose(file);
	}

	// Free lookup
	for(int i = 0; i < lookup->archives.count; ++i) free(lookup->archives.raw[i]);
	for(int i = 0; i < lookup->table.count; ++i) free(lookup->table.raw[i].key);

	free(lookup->archives.raw);
	free(lookup->table.raw);
	free(lookup->encryptionKey);

	free(lookup);
}

int Seal_LookupGetOrAddArchive(Seal_LookupTable lookup, const char *archive) {
	for(int i = 0; i < lookup->archives.count; ++i) {
		if(strcmp(archive, lookup->archives.raw[i]) == 0) return i;
	}
	
	if(lookup->archives.alloc <= lookup->archives.count)
		lookup->archives.raw = realloc(lookup->archives.raw, sizeof(char *) * (lookup->archives.alloc += 5));
	
	lookup->archives.raw[lookup->archives.count] = strdup(archive);
	return lookup->archives.count++;
}

int Seal_LookupAddLookup(Seal_LookupTable lookup, const char *path, int archive) {
	if(archive < 0 || archive >= lookup->archives.alloc) return 0;

	if(lookup->table.alloc <= lookup->archives.count) {
		lookup->table.raw = realloc(lookup->table.raw, sizeof(Seal_LookupElement) * (lookup->table.alloc += 25));
	}

	lookup->table.raw[lookup->table.count].archive = archive;
	lookup->table.raw[lookup->table.count++].key = strdup(path);

	return 1;
}

void Seal_LookSetEncryptionKey(Seal_LookupTable lookup, const char *key) {
	lookup->encryptionKey = key ? strdup(key) : NULL;
}