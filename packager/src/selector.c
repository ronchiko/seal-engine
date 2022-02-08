#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <zip.h>

#include <seal/io/fs.h>

#include "argv.h"
#include "packager.h"
#include "cache.h"

#define PATH_MAX_SIZE		512

static const int FILE_ITERATOR_FLAGS = FILE_ITER_RECURSIVE;

static void _NormalizeFileName(char *path, int nbytes) {	
	if(nbytes < 0) nbytes = strlen(path);
	const char *begin = path;

	for(int i = 0; *path && i < nbytes; ++i, ++path) {
		if(*path == '\\' || *path == '/') {
			*path = '/';

			for(++path, ++i;(*path == '\\' || *path == '/') && i < nbytes; ++i, ++path) {
				memmove(path, path + 1, nbytes - i);
			}
		}
	}
}

static char *_ConsumeFile(FILE *fp, zip_uint64_t *length) {
	char *buffer = NULL;
	char tbuffer[512];
	*length = 0;

	for(size_t read; read = fread(tbuffer, sizeof(char), 512, fp);) {
		buffer = realloc(buffer, (*length + read));

		memmove(buffer + *length, tbuffer, read);
		*length += read;
	}

	return buffer;
}

static void Seal_GroupAssets(Seal_PackagerArgs *args, Seal_AssetArchive *archive) {
	for(int i = 0; i < args->assets.count; ++i) {
		LOG(args, "Iterating over path '%s'", args->assets.patterns[i]);
		Seal_FileIterator iter = Seal_CreateFileIterator(args->assets.patterns[i], FILE_ITERATOR_FLAGS);
		char path[PATH_MAX_SIZE + 1];
		
		while(Seal_FileIteratorNext(iter, path, PATH_MAX_SIZE) >= 0) {
			Seal_FileGroup group = Seal_GetFileGroup(path, PATH_MAX_SIZE);

			if(group >= __SEAL_GROUP_COUNT__) continue;

			Seal_FileVector *vector = archive->vectors + group;
			if(vector->count >= vector->alloced)
				vector->vector = realloc(vector->vector, sizeof(char *) * (vector->alloced += 5));
			
			vector->vector[vector->count++] = strdup(path);
		}

		Seal_FreeFileIterator(iter);
	}
}

static zip_t *Seal_CreateZipArchiveForVector(
	Seal_PackagerArgs *args, Seal_FileVector *vector, Seal_FileGroup group, 
	const char *oldKey, const char *newKey, Seal_LookupTable lookup
) {
	static struct {const char *name; const char *readPolicy;} NAMES[] = {
		[SEAL_GROUP_GENERIC_TEXT] = {"data.pup", 	 "r"},
		[SEAL_GROUP_SHADER] = 		{"shaders.pup",  "r"},
		[SEAL_GROUP_TEXTURE] = 		{"textures.pup", "rb"},
		[SEAL_GROUP_GENERIC_BIN] = 	{"generic.pup",  "rb"},
	};

	char archiveName[PATH_MAX_SIZE + 1];
	strncpy(archiveName, NAMES[group].name, PATH_MAX_SIZE);
	LOG(args, "Creating archive %s", archiveName);

	const char *readPolicy = NAMES[group].readPolicy;

	if(args->exportZips)
		strncat(archiveName, ".zip", PATH_MAX_SIZE);

	if(args->cleanCache) remove(archiveName);

	int errorp;
	zip_t *zip = zip_open(archiveName, ZIP_CREATE, &errorp);
	if(zip == NULL) {
		zip_error_t error;
		zip_error_init_with_code(&error, errorp);
		printf("ERROR: %s\n", zip_error_strerror(&error));
		return NULL;
	}


	if(args->encrypt && oldKey)
		zip_set_default_password(zip, oldKey);

	int archiveIndex = Seal_LookupGetOrAddArchive(lookup, archiveName);

	zip_error_t error;
	zip_source_t *source;
	for(int i = 0; i < vector->count; ++i) {
		char *name = vector->vector[i];
		_NormalizeFileName(name, -1);

		FILE *fp = fopen(name, readPolicy);
		if(!fp) {
			printf("Failed to open file, %s\n", name);
			zip_close(zip);
			return NULL;
		}

		zip_uint64_t length = 0;
		char *fd = _ConsumeFile(fp, &length);

		fclose(fp);

		if((source = zip_source_buffer(zip, fd, length, 0)) == NULL) {
			printf("Failed to add file to zip '%s': %s\n", name, zip_strerror(zip));

			fclose(fp);
			zip_close(zip);
			return NULL;
		}
		
		zip_int64_t index, failed = 0;
		if((index = zip_name_locate(zip, name, 0)) < 0)
			failed = (index = zip_file_add(zip, vector->vector[i], source, 0)) < 0;
		else
			failed = zip_file_replace(zip, index, source, 0) < 0;

		if(failed) {
			printf("Failed to add file to zip '%s': %s\n", name, zip_strerror(zip));

			zip_source_close(source);
			zip_close(zip);
			return NULL;
		}
		
		Seal_LookupAddLookup(lookup, name, archiveIndex);

		if(args->encrypt && newKey)
			zip_file_set_encryption(zip, index, ZIP_EM_AES_128, newKey);

		free(fd);
	}

	zip_close(zip);
	
	LOG(args, "Created archive at %s", archiveName);
	return zip;
}

void Seal_CreateArchive(Seal_PackagerArgs *args) {
	Seal_AssetArchive *archive = malloc(sizeof(Seal_AssetArchive));
	memset(archive, 0, sizeof(Seal_AssetArchive));

	LOG(args, "Grouping files");
	Seal_GroupAssets(args, archive);

	const char *unlockKey = Seal_GetEncryptionKey();
	Seal_LookupTable lookup = Seal_CreateNewLookup();

	char encryptionKey[33] = { 0 };
	if(unlockKey)
		memcpy(encryptionKey, unlockKey, 32 * sizeof(char));

	const int fRegenerateKey = args->regenerateKey || unlockKey == NULL;

	if(args->encrypt && fRegenerateKey) {
		char *ek = Seal_GenerateEncryptionKey();
		memcpy(encryptionKey, ek, 32 * sizeof(char));
		encryptionKey[32] = 0;

		LOG(args, "New encryption key is %s", encryptionKey);
		free(ek);
	}

	// Create zip files
	for(int i = 0; i < __SEAL_GROUP_COUNT__; ++i) {
		Seal_CreateZipArchiveForVector(args, archive->vectors + i, (Seal_FileGroup)i, 
			unlockKey, encryptionKey[0] ? encryptionKey : NULL, lookup);
	}

	LOG(args, "Generating key");
	if(fRegenerateKey) {
		Seal_SetEncryptionKey(encryptionKey);
		Seal_LookSetEncryptionKey(lookup, encryptionKey);
	}

	LOG(args, "Generating lookup path");
	char lookupPath[513];
	snprintf(lookupPath, 512, ".seal/%s", args->lookup ? args->lookup : "lookup.c");
	lookupPath[512] = 0;
	LOG(args, "Creating lookup file at %s", lookupPath);

	Seal_WriteLookup(lookup, lookupPath);

	LOG(args, "Done creating archive");
	for(int i = 0; i < __SEAL_GROUP_COUNT__; ++i) {
		for(int j = 0; j < archive->vectors[i].count; ++j)
			free(archive->vectors[i].vector[j]);
		
		free(archive->vectors[i].vector);
	}
	free(archive);
}