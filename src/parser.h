#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include <string.h>
#include "tree.h"

int regexTest(const char* string, const char* regex, const int size);

typedef struct result
{
	int boolean;
	int number;
} result;
typedef result (*fptr)(const char*, Node*);


result etoile(fptr function, const char *s, int mini, int maxi);


#endif