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

int quotedPair(const char *s,Node *node);
int quotedString(const char *s, Node* node);
int parameter(const char *s,Node *node);

int mediaType(const char *s, Node* node);
int contentTypeHeaderName(const char *s, Node* node);
int contentTypeHeader(const char *s, Node* node);

int qvalue(const char *s, Node* node);
int weight(const char *s, Node* node);
int alphanum(const char *s, Node* node);

#endif
