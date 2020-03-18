#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include <string.h>
#include "tree.h"
#include "baseLevel.h"

#define MAX_FUNCTION_NUMBER 50

int regexTest(const char* string, const char* regex, const int size);

typedef struct functionArray{
	void *functions[MAX_FUNCTION_NUMBER];
	int functionCount;
	int isOrFunction;
	int *optionnal;
} functionArray;


void etoile(functionArray functions, const char *s, int mini, int maxi, Node *node);
int optionnal(functionArray functions, int i);

#endif