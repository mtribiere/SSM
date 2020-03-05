#include <stdio.h>
#include <stdlib.h>
#include <regex.h>

#define FALSE 0
#define TRUE 1

int regexTest(const char* string, const char* regex, const int size);
int DIGIT(const char* s);
int etoile(int (*fonction)(const char* s), const char* s, int mini, int maxi);
