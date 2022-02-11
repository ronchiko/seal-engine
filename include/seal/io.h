#pragma once

#include "seal/common.h"

#include "seal/io/input.h"
#include "seal/io/img.h"
#include "seal/io/fs.h"

typedef struct  {
	Seal_Size length;
	char *data;
} Seal_File;

/* \brief This function open a file and dumps its content into a string */
Seal_File SealIO_ReadFile(const char *path);
Seal_File SealIO_ReadFileRaw(const char *path);