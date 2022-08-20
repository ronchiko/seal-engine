#pragma once

#include <seal/common.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef union Seal_Vector4 {
	struct {
		Seal_Float x;
		Seal_Float y;
		Seal_Float z;
		Seal_Float w;
	};
	Seal_Float array[4];
} Seal_Vector4;

#ifdef __cplusplus
}
#endif
