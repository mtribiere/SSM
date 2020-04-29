#ifndef ___SEMANTIC___
#define ___SEMANTIC___

void buildResponse(_Token* root, char* reponse, int* taille, int *close);

int code(_Token* root, char* reponse, int* taille, int* erreur);
char* codeMessage(int code);
char* normalisationURI(_Token* root);
char* findRessource(const char* URI, int* erreur);
char* writeRessource(const char* target, int* taille, int* erreur);
char* MIMEtype(const char* ressource);
void addHeader(char* reponse, const char* headerField, const char* headerValue, int* taille);
int headerUnique(_Token* root);
int connexion(_Token* root, char* reponse, int* taille);

#endif