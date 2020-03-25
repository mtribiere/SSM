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

int segmentNzNc(const char *s, Node* node);
int segmentNz(const char *s, Node* node);
int pathNoscheme(const char *s, Node* node);
int pathEmpty(const char *s, Node* node);
int pathRootless(const char *s, Node* node);
int pathAbsolute(const char *s, Node* node);
int pathAbempty(const char *s, Node* node);
int port(const char *s, Node* node);
int regName(const char *s, Node* node);
int IPvFuture(const char *s, Node* node);
int decOctet(const char *s, Node* node);
int IPv4address(const char *s, Node* node);
int h16(const char *s, Node* node);
int ls32(const char *s, Node* node);
int IPv6address(const char *s, Node* node);
int IPliteral(const char *s, Node* node);
int host(const char *s, Node* node);
int userinfo(const char *s, Node* node);

int acceptExt(const char *s, Node* node);
int acceptParams(const char *s, Node* node);
int mediaRange(const char *s, Node* node);
int accept(const char *s, Node* node);
int acceptHeaderName(const char *s, Node* node);
int acceptHeader(const char *s, Node* node);

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
