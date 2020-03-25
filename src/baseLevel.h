#ifndef BASE_LEVEL_H
#define BASE_LEVEL_H
#include "tree.h"
#include "parser.h"

int DIGIT(const char* s, Node* node);
int ALPHA(const char *s, Node* node);
int BIT(const char *s, Node* node);
int HEXDIG(const char *s, Node* node);
int CHAR_(const char *s, Node* node);
int CR(const char *s, Node* node);
int LF(const char *s, Node* node);
int DQUOTE(const char *s, Node* node);
int HTAB(const char *s, Node* node);
int OCTET(const char *s, Node* node);
int SP(const char *s, Node* node);
int VCHAR(const char *s, Node* node);

int OWS(const char *s, Node* node);
int RWS(const char *s, Node* node);
int BWS(const char *s, Node* node);
int CRLF(const char *s, Node* node);

int pct_encoded(const char *s,Node *node);

int slash(const char *s, Node* node);
int interrogation(const char *s, Node* node);
int dot(const char *s, Node* node);
int percent(const char *s, Node* node);
int exclamation(const char* s, Node* node);
int octothorp(const char* s, Node* node);
int dollar(const char* s, Node* node);
int and(const char* s, Node* node);
int quote(const char* s, Node* node);
int star(const char* s, Node* node);
int plus(const char* s, Node* node);
int minus(const char* s, Node* node);
int chineseHat(const char* s, Node* node);
int underScore(const char* s, Node* node);
int inverseQuote(const char* s, Node* node);
int pipe(const char* s, Node* node);
int tilt(const char* s, Node* node);
int tiret(const char* s, Node* node);
int openParenthese(const char* s, Node* node);
int closeParenthese(const char* s, Node* node);
int coma(const char* s, Node* node);
int semiColon(const char* s, Node* node);
int equal(const char* s, Node* node);
int colon(const char *s, Node* node);
int at(const char *s, Node* node);
int openBracket(const char* s, Node* node);
int closeBracket(const char* s, Node* node);
int backSlash(const char *s,Node* node);
int ZERO(const char* s, Node* node);
int ONE(const char* s, Node* node);

#endif
