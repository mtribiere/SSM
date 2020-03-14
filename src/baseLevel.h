#ifndef BASE_LEVEL_H
#define BASE_LEVEL_H
#include "tree.h"

int DIGIT(const char* s, Node* node);
int ALPHA(const char *s, Node* node);
int BIT(const char *s, Node* node);
int HEXDIG(const char *s, Node* node);
int CHAR_(const char *s, Node* node);
int CR(const char *s, Node* node);
int LF(const char *s, Node* node);
int DQUOTE(const char *s, Node* node);
int HTAB(const char *s, Node* node);
int OCTET(const char *s, Node* node);
int SP(const char *s, Node* node);
int VCHAR(const char *s, Node* node);

int OWS(const char *s, Node* node);
int RWS(const char *s, Node* node);
int BWS(const char *s, Node* node);
int CRLF(const char *s, Node* node);

int tchar(const char *s, Node* node);
int token(const char *s, Node* node);
int method(const char *s, Node* node);

int slash(const char *s, Node* node);
int dot(const char *s, Node* node);
int minus(const char *s, Node* node);
int underscore(const char *s, Node* node);
int tilde(const char *s, Node* node);
int unreserved(const char *s, Node* node);


#endif