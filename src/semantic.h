#ifndef ___SEMANTIC___
#define ___SEMANTIC___

#include "libparser/api.h"

#define MAX_RESPONSE_SIZE 150000000
#define MAX_URI_SIZE 5000
#define MAX_MIME_SIZE 100

char* reponse;
void buildResponse(_Token* root, char* reponse, int* taille, int *close);

int code(_Token* root, char* reponse, int* taille, int* erreur);
char* codeMessage(int code);
char* normalisationURI(_Token* root, int* erreur);
void addHeader(char* reponse, const char* headerField, const char* headerValue, int* taille);
int headerUnique(_Token* root);
int connexion(_Token* root, char* reponse, int* taille);

#endif
