#ifndef HEADER_H
#define HEADER_H
#include "tree.h"
#include "parser.h"
#include "baseLevel.h"
#include "highLevel.h"

int type(const char *s, Node* node);
int subtype(const char *s, Node* node);
int obsText(const char *s, Node* node);
int qdtext(const char *s, Node* node);
int expect(const char *s, Node* node);
int expectHeaderName(const char *s, Node* node);
int expectHeader(const char *s, Node* node);


#endif
