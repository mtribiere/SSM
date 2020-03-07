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
int CHAR_(const char *s);
int CR(const char *s);
int LF(const char *s);
int DQUOTE(const char *s);
int HTAB(const char *s);
int OCTET(const char *s);
int SP(const char *s);
int VCHAR(const char *s);

int OWS(const char *s);
int RWS(const char *s);
int BWS(const char *s);
int CRLF(const char *s);

typedef int (*fptr)(const char*);
int etoile(fptr function, const char *s, int mini, int maxi);
fptr ou(fptr function1, fptr function2);
fptr et(fptr function1, fptr function2);