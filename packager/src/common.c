#pragma once

#include <stdlib.h>
#include <time.h>

#define ENCRYPTION_KEY_SIZE 		16

char *Seal_GenerateEncryptionKey(void) {
	srand(time(NULL));

	char *key = malloc((ENCRYPTION_KEY_SIZE + 1)* sizeof(char));
	for(int i = 0; i < ENCRYPTION_KEY_SIZE; ++i)
		switch (rand() % 3)
		{
		case 0: key[i] = (rand() % ('Z' - 'A')) + 'A'; break;
		case 1: key[i] = (rand() % ('z' - 'a')) + 'a'; break;
		case 2: key[i] = (rand() % ('9' - '0')) + '0'; break;
		}
		
	key[ENCRYPTION_KEY_SIZE] = 0;
	return key;
}