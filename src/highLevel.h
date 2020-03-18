#ifndef HIGH_LEVEL_H
#define HIGH_LEVEL_H
#include "tree.h"
#include "parser.h"

int HTTPname(const char *s, Node* node);
int HTTPVersion(const char *s, Node* node);
int pchar(const char *s, Node* node);
int segment(const char *s, Node* node);
int absolutePath(const char *s, Node* node);
int query(const char *s, Node* node);
int originForm(const char *s, Node* node);
int requestLine(const char *s, Node* node);

int subDelims(const char *s, Node* node);
int unreserved(const char* s, Node* node);
int tchar(const char* s, Node* node);
int token(const char *s, Node* node);
int method(const char *s, Node* node);

#endif