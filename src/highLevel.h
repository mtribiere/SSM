#ifndef HIGH_LEVEL_H
#define HIGH_LEVEL_H
#include "tree.h"

int HTTPname(const char *s, Node* node);
int HTTPVersion(const char *s, Node* node);
int pctEncoded(const char *s, Node* node);
int pchar(const char *s, Node* node);
#endif