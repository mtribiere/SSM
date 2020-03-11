#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include <string.h>
#include "tree.h"

int regexTest(const char* string, const char* regex, const int size);

typedef int (*fptr)(const char*);
int etoile(fptr function, const char *s, int mini, int maxi, int shift);

#endif