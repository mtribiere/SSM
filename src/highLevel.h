#ifndef HIGH_LEVEL_H
#define HIGH_LEVEL_H
#include "tree.h"
#include "parser.h"
#include "header.h"

int HTTPmessage(const char *s, Node* node);

int messageBody(const char *s, Node* node);
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
int fieldVchar(const char *s, Node *node);
int fieldContent(const char *s, Node *node);
int fieldValue(const char *s, Node *node);
int fieldName(const char *s, Node *node);
int headerField(const char *s, Node *node);
int fieldVchar(const char *s, Node* node);
int obsFold(const char *s, Node* node);
int method(const char *s, Node* node);

#endif
