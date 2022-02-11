#pragma once

#include <seal/common.h>

typedef union Seal_Vector4 {
	struct {
		Seal_Float x;
		Seal_Float y;
		Seal_Float z;
		Seal_Float w;
	};
	Seal_Float array[4];
} Seal_Vector4;