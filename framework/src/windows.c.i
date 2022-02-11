
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <seal/debug.h>

struct _Seal_FileIterator {
	HANDLE findHandle;
	WIN32_FIND_DATA findData;
	int flags;
	bool closed;
	char *root;

	struct _Seal_FileIterator *child;
};

Seal_FileIterator Seal_CreateFileIterator(const char *dir, int flags) {
	Seal_FileIterator iterator = malloc(sizeof(struct _Seal_FileIterator));
	iterator->flags = flags;
	iterator->closed = false;
	iterator->child = NULL;
	iterator->root = strdup(dir);
	
	char glob[2049];
	snprintf(glob, 2048, "%s/*.*", dir);

	if((iterator->findHandle = FindFirstFile(glob, &iterator->findData)) == INVALID_HANDLE_VALUE) {
		iterator->closed = true;
		return iterator;
	}

	// Skip '..' & '.' entries
	int hasNext = 1;
	while(hasNext && (!strcmp(iterator->findData.cFileName, ".") || !strcmp(iterator->findData.cFileName, ".."))) {
		hasNext = FindNextFile(iterator->findHandle, &iterator->findData);
	}
	
	// If the only enteries are the default then we are done
	if(!hasNext)
		iterator->closed = true;
	
	return iterator;
}

static int _FileIteratorIterateChild(Seal_FileIterator iterator, char *path, int n) {
	int code;
	if(iterator->child->closed || (code = Seal_FileIteratorNext(iterator->child, path, n)) <= 0) {
		Seal_FreeFileIterator(iterator->child);
		iterator->child = NULL;
		return FILE_ITER_RESULT_END;
	}

	return code;
}

int Seal_FileIteratorNext(Seal_FileIterator iterator, char *path, int n) {
skip_file:
	*path = 0;
	if(iterator->findHandle == INVALID_HANDLE_VALUE || (iterator->closed && !iterator->child)) {
		return FILE_ITER_RESULT_END;
	}

	// Currently inside child iterator
	if(iterator->child) {
		int result;
		if((result = _FileIteratorIterateChild(iterator, path, n)) > 0)
			return result;
	
		if(iterator->closed) return FILE_ITER_RESULT_END;
	}

	if(iterator->findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {	// Got directory		
		snprintf(path, n, "%s/%s", iterator->root, iterator->findData.cFileName);

		if(iterator->flags & FILE_ITER_RECURSIVE) {	// In case recursion is on then create child iterator
			Seal_FreeFileIterator(iterator->child);
			iterator->child = Seal_CreateFileIterator(path, iterator->flags);
		}

		if(!FindNextFile(iterator->findHandle, &iterator->findData)) iterator->closed = true;

		if(iterator->flags & FILE_ITER_INCLUDE_DIRECTORIES) return FILE_ITER_RESULT_DIR;
		
		goto skip_file;
	}

	snprintf(path, n, "%s/%s", iterator->root, iterator->findData.cFileName);
	if(!FindNextFile(iterator->findHandle, &iterator->findData)) iterator->closed = true;
	
	return FILE_ITER_RESULT_FILE;
}

void Seal_FreeFileIterator(Seal_FileIterator iter) {
	if(!iter) return;
	
	FindClose(iter->findHandle);
	if(iter->child) {
		Seal_FreeFileIterator(iter->child);
	}

	free(iter->root);
	free(iter);
}

int Seal_CreateDirectory(const char *path) {
	return CreateDirectory(path, NULL);
}

int Seal_DirectoryExists(const char *path) {
	DWORD attribs = GetFileAttributes(path);

	return attribs != INVALID_FILE_ATTRIBUTES && (attribs & FILE_ATTRIBUTE_DIRECTORY);
}

int Seal_DeleteChildren(const char *path, int flags) {
	Seal_FileIterator iterator = Seal_CreateFileIterator(path, flags);
	
	char _path[512];
	for(;Seal_FileIteratorNext(iterator, _path, 511) >= 0;) {
		remove(_path);
	}

	Seal_FreeFileIterator(iterator);
	return 0;
}