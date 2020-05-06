#ifndef ___SEMANTIC___
#define ___SEMANTIC___

#define MAX_RESPONSE_SIZE 150000000

typedef struct _site {
	char* fqdn;
	char* dossier;
	char* e400;
	char* e404;
	char* e408;
	char* e411;
	char* e418;
	char* e501;
	char* e505;
	struct _site* next;
} Site;

Site* multisitesConf;
void loadMultisitesConf();
void unloadMultiSitesConf();

char* reponse;
void buildResponse(_Token* root, char* reponse, int* taille, int *close);

int code(_Token* root, char* reponse, int* taille, int* erreur);
char* codeMessage(int code);
char* normalisationURI(_Token* root, int* erreur);
char* findRessource(const char* URI, int* erreur);
char* writeRessource(const char* target, int* tailleFichier, int* erreur);
char* MIMEtype(const char* ressource);
void addHeader(char* reponse, const char* headerField, const char* headerValue, int* taille);
int headerUnique(_Token* root);
int connexion(_Token* root, char* reponse, int* taille);

#endif
