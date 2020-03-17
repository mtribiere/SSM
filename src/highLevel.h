#ifndef HIGH_LEVEL_H
#define HIGH_LEVEL_H
#include "tree.h"
#include "parser.h"

int HTTPname(const char *s, Node* node);
int HTTPVersion(const char *s, Node* node);
int pctEncoded(const char *s, Node* node);
result pchar(const char *s, Node* node);
result segment(const char *s, Node* node);
result absolutePath(const char *s, Node* node);
result query(const char *s, Node* node);
result originForm(const char *s, Node* node);
int requestLine(const char *s, Node* node);

#endif
