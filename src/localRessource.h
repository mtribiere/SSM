#ifndef LOCAL_RESSOURCE_H
#define LOCAL_RESSOURCE_H

char* findRessource(const char* URI, int* erreur);
char* writeRessource(const char* target, int* tailleFichier, int* erreur);
char* MIMEtype(const char* ressource);

#endif