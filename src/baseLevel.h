#ifndef BASE_LEVEL_H
#define BASE_LEVEL_H
#include "tree.h"

int DIGIT(const char* s, Node* node);
int ALPHA(const char *s, Node* node);
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

int tchar(const char *s);
int token(const char *s);


int slash(const char *s, Node* node);
int dot(const char *s, Node* node);

#endif