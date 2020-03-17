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

#endif