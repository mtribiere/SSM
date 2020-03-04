#include <stdio.h>
#include <stdlib.h>

#define FALSE 0
#define TRUE 1

int DIGIT(const char* c);
int etoile(int (*fonction)(const char* c), const char* s, int mini, int maxi);
