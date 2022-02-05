#pragma once

#include "seal/common.h"

#include "seal/input.h"
#include "seal/fs.h"

/* \brief This function open a file and dumps its content into a string */
char *SealIO_ReadFile(const char *path);