#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include <string.h>

#define FALSE 0
#define TRUE 1


int regexTest(const char* string, const char* regex, const int size);
int DIGIT(const char* s);
int ALPHA(const char *s);
int BIT(const char *s);
int HEXDIG(const char *s);
int etoile(int (*fonction)(const char* s), const char* s, int mini, int maxi);
