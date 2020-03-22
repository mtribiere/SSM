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

int cookieOctet(const char *s, Node* node);
int cookieValue(const char *s, Node* node);
int cookieName(const char *s, Node* node);
int cookiePair(const char *s, Node* node);
int cookieString(const char *s, Node* node);
int cookieHeaderName(const char *s, Node* node);
int cookieHeader(const char *s, Node* node);

int languageRange(const char *s,Node *node);
int languageRangePart2(const char *s,Node *node);
int acceptLanguage(const char *s, Node* node);
int acceptLanguageHeader(const char *s, Node* node);
int acceptLanguageHeaderName(const char *s, Node* node);

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
