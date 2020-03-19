#ifndef HEADER_H
#define HEADER_H
#include "tree.h"
#include "parser.h"
#include "highLevel.h"

int type(const char *s, Node* node);
int subtype(const char *s, Node* node);
int obsText(const char *s, Node* node);

#endif
