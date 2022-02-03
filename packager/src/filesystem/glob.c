#include <stdlib.h>
#include <string.h>

#include "filesystem.h"

#define MAX_MATCH_STR_SIZE	512

enum Seal_GlobNodeType {
	GLOB_MATCH,
	GLOB_WILDCARD
};

struct Seal_GlobNode {
	enum Seal_GlobNodeType type;
	union {
		char *match;
	};
};

struct Seal_GlobExpr {
	struct Seal_GlobNode *buffer;
	size_t count;
};

static int HandleSubExpr(Seal_GlobExpr expr, char *match, char **cur, const char *glob) {
	if(*glob == '*') {
		// Check for double wildcards
		if(*cur == match) {
			if(expr->count > 0 && expr->buffer[expr->count - 1].type == GLOB_WILDCARD) {
				return -1;
			}

			expr->buffer = realloc(expr->buffer, ++expr->count * sizeof(struct Seal_GlobNode));
			expr->buffer[expr->count - 1].type = GLOB_WILDCARD;
			return 1;
		}

		expr->buffer = realloc(expr->buffer, (expr->count + 2) * sizeof(struct Seal_GlobNode));
		**cur = 0;

		expr->buffer[expr->count].type 	= GLOB_MATCH;
		expr->buffer[expr->count].match = strdup(match);

		expr->buffer[expr->count + 1].type = GLOB_WILDCARD;

		expr->count += 2;
		*cur = match;
		return 1;
	}

	**cur = *glob;
	*cur += 1;

	return 1;
}

Seal_GlobExpr Seal_ParseGlob(const char *glob) {
	Seal_GlobExpr expr = malloc(sizeof(struct Seal_GlobExpr));
	expr->buffer = (struct Seal_GlobNode *)(expr->count = 0);

	char match[MAX_MATCH_STR_SIZE], *cursor = match;
	for(;*glob;++glob) {
		if(cursor == match + MAX_MATCH_STR_SIZE || HandleSubExpr(expr, match, &cursor, glob) < 0) {
			free(expr);
			return NULL;
		}
	}

	if(cursor != match) {
		expr->buffer = realloc(expr->buffer, ++expr->count * sizeof(struct Seal_GlobNode));
		*cursor = 0;

		if(match[1] == 0 && match[0] == '*') {
			expr->buffer[expr->count - 1].type 	= GLOB_WILDCARD;
		}else {
			expr->buffer[expr->count - 1].type 	= GLOB_MATCH;
			expr->buffer[expr->count - 1].match = strdup(match); 
		}
	}

	return expr;
}

void Seal_FreeGlobExpr(Seal_GlobExpr expr) {
	if(!expr) return;
	
	for(int i = 0; i < expr->count; ++i) {
		if(expr->buffer[i].type == GLOB_MATCH) free(expr->buffer[i].match);
	}

	free(expr);
}

static int Seal_GlobMatchFork(Seal_GlobExpr expr, int node, const char *str) {
	const char *currentMatch = expr->buffer[node].match;
	int i;

	for(i = node; *str && i < expr->count; ++str) {
		struct Seal_GlobNode *current = expr->buffer + i;
		
		switch (current->type) {
		case GLOB_WILDCARD:
			if(i < expr->count - 1 && expr->buffer[i + 1].match[0] == *str) {
				if (Seal_GlobMatchFork(expr, i + 1, str) == GLOB_MATCHED) 
					return GLOB_MATCHED;
			}
			break;
		case GLOB_MATCH:
			if(*currentMatch != *str) {
				return GLOB_MISMATCH;		// Mismatch
			}

			++currentMatch;
			if(*currentMatch == 0 && i < expr->count - 1) currentMatch = expr->buffer[++i].match;
			break;

		default:
			return GLOB_INTERNAL_ERR;	// Invalid node type
		}
	}

	if(*str != 0 || i != expr->count - 1)
		return GLOB_MISMATCH; // Mismatch
	
	return GLOB_MATCHED;
}

int Seal_GlobMatch(Seal_GlobExpr expr, const char *str) {
	return Seal_GlobMatchFork(expr, 0, str);
}
