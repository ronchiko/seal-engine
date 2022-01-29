
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <zip.h>

#include "seal/io.h" 

extern const char *Seal_GetFileArchive(const char *source); // This method is automatically genereted in lookup.c

Seal_Size Seal_FConcatString(char **src, const char *cnt, Seal_Size srclen, Seal_Size cntlen) {
	*src = realloc(*src, srclen + cntlen + 1);
	memmove(*src + srclen, cnt, cntlen);
	*(*src + srclen + cntlen) = 0;
	return srclen + cntlen;
}

static Seal_Size SealIO_ReadBuffer(FILE *file, char *line, Seal_Size len) {
	int current = fgetc(file);
	Seal_Size index = 0;
	while(current >= 0 && index < len) {
		line[index++] = current;
		current = fgetc(file);
	}

	return (Seal_Size)index;
}

static char *_OpenFileFromArchive(const char *archiveName, const char *path) {
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

char *SealIO_ReadFile(const char *path) {
	// Check if the path is of an embbeded file or points to an archive file
	const char *archive = NULL;
	if (archive = Seal_GetFileArchive(path)) {
		char *buffer = _OpenFileFromArchive(archive, path);
		return buffer;
	}

	FILE *file = fopen(path, "r");
	if (!file) {
		Seal_LogError("Failed to open file '%s'", SEAL_TRUE, path);
		return NULL;
	}

	Seal_Size bufferSize = 0, dynbufferSize = 0;
	char staticBuffer[512];
	char *dynamicBuffer = NULL;
	while((bufferSize = SealIO_ReadBuffer(file, staticBuffer, 512)) > 0) {
		dynbufferSize = Seal_FConcatString(&dynamicBuffer, staticBuffer, dynbufferSize, bufferSize);
	}

	return dynamicBuffer; 
}
