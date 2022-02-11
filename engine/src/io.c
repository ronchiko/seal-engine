
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <zip.h>

#include "seal/io.h" 

extern const char *Seal_GetFileArchive(const char *source); // This method is automatically genereted in lookup.c

Seal_Size Seal_FConcatString(char **src, const char *cnt, Seal_Size srclen, Seal_Size cntlen) {
	char *nSrc = realloc(*src, srclen + cntlen + 1);	
	
	memmove(nSrc + srclen, cnt, cntlen);
	*(nSrc + srclen + cntlen) = 0;
	
	*src = nSrc;
	return srclen + cntlen;
}

static Seal_Size SealIO_ReadBuffer(FILE *file, char *line, Seal_Size len) {	

	Seal_Size index = 0;
	do {
		int current = fgetc(file);
		if(current == EOF) break;

		line[index] = current;
		++index;
	} while(index < len);

	return (Seal_Size)index;
}

static char *_OpenFileFromArchive(const char *archiveName, const char *path, Seal_Size *len) {
	
	*len = 0;
	int errorp;
	zip_t *archive = zip_open(archiveName, ZIP_RDONLY, &errorp);
	if(!archive) {
		Seal_LogError("Failed to open file %s from archive", SEAL_FALSE, path);
		return NULL;
	}

	zip_file_t *source = zip_fopen(archive, path, 0);
	if(!source) {
		Seal_LogError("Failed to open file %s from archive, %s", SEAL_FALSE, path, zip_strerror(archive));
		return NULL;
	}

	zip_stat_t stats = { 0 };
	if(zip_stat(archive, path, ZIP_STAT_SIZE, &stats)) {
		Seal_LogError("Failed to read data on file %s from archive, %s", SEAL_FALSE, path, zip_strerror(archive));
		return NULL;
	}
	
	*len = stats.size;

	char *buffer = malloc((stats.size + 1) * sizeof(char));
	if(!buffer) {
		Seal_LogError("Out of memory, expected  size of file %s is %zu", SEAL_FALSE, path, stats.size);
		return NULL;
	}

	if(zip_fread(source, buffer, stats.size) < 0) {
		Seal_LogError("Failed to read file %s", SEAL_FALSE, path);
		return NULL;
	}
	buffer[stats.size] = 0;

	return buffer;
}

char *_SealIO_ReadFile(const char *path, const char *readPolicy, Seal_Size *len) {
	// Check if the path is of an embbeded file or points to an archive file
	const char *archive = NULL;
	if (archive = Seal_GetFileArchive(path)) {
		char *buffer = _OpenFileFromArchive(archive, path, len);
		return buffer;
	}

	FILE *file = fopen(path, readPolicy);
	if (!file) {
		Seal_LogError("Failed to open file '%s'", SEAL_FALSE, path);
		return NULL;
	}

	Seal_Size bufferSize = 0, dynbufferSize = 0;
	char staticBuffer[512];
	
	char *dynamicBuffer = NULL;
	while((bufferSize = SealIO_ReadBuffer(file, staticBuffer, 512)) > 0) {
		dynbufferSize = Seal_FConcatString(&dynamicBuffer, staticBuffer, dynbufferSize, bufferSize);
	}

	fclose(file);

	*len = dynbufferSize;
	return dynamicBuffer; 
}

Seal_File SealIO_ReadFile(const char *path) { 
	Seal_Size length = 0;
	char *buffer = _SealIO_ReadFile(path, "r", &length); 

	return (Seal_File){.data = buffer, .length = length};
}
Seal_File SealIO_ReadFileRaw(const char *path)  { 
	Seal_Size length = 0;
	char *buffer = _SealIO_ReadFile(path, "rb", &length); 

	return (Seal_File){.data = buffer, .length = length};
}