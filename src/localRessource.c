#include "localRessource.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <magic.h>
#include "site.h"
#include "semantic.h"
#include "httpparser.h"
#include "libparser/api.h"


char* findRessource(const char* URI, int* erreur)
//A partir d'une URI, trouve la ressource associée dans les répertoires du serveur
{
	char* target = malloc(MAX_URI_SIZE);
	//Chemin vers la ressource recherchée
	if(target == NULL)
	{
		*erreur = 1;
		return codeMessage(500); /* 500 (Internal Server Error) */
	}

	_Token *root;
	root = getRootTree();
	_Token *host;
	host = searchTree(root, "Host"); //Champ Host

	if(host != NULL) //Si il y a un champ Host
	{
		Lnode* a = host->node;
		char *el = a->value; //Contenu du champ Host

		Site* s = multisitesConf; //Tête de la liste chaînée

		char *ptr2 = NULL;
		ptr2 = strtok(el, " ");
		ptr2 = strtok(ptr2, ":"); //Récupération de la partie du type 127.0.0.1

		while(s != NULL && strcmp(ptr2, s->fqdn) != 0) //Parcours de la liste chaînée jusqu'à trouver une correspondance
			s = s->next;

		char* dossier = malloc(sizeof(char)*20);
		char* dossierSlash = malloc(sizeof(char)*20);
		if (s == NULL) { //S'il n'y a pas de correspondance, on renvoie la requête vers www/ (127.0.0.1)
			strcpy(dossier,"www");
		} else { //Sinon, on indique le dossier correspondant
			strcpy(dossier, s->dossier);
		}

		strcpy(dossierSlash, dossier);
		strcpy(dossierSlash+strlen(dossier), "/"); //Dossier trouvé avec un / à la fin
		strcpy(target, dossier);
		strcpy(target + strlen(dossier), URI); //"Dossier / URI"

		if(!strcmp(target, dossierSlash)) //Si l'URI est / , on rend www*/index.html
			strcat(target, "index.html");

		free(dossier);
		free(dossierSlash);
		purgeElement(&host);
	} else {
		strcpy(target, "www"); //S'il n'y a pas de champ Host, on renvoie la requête vers www/ (127.0.0.1)
		strcpy(target + 3, URI);

		if(!strcmp(target, "www/")) //Si l'URI est / , on rend www/index.html
			strcat(target, "index.html");
	}

	printf("Target : %s\n", target);
	return target;
}

char* writeRessource(const char* target, int* tailleFichier, int* erreur)
//A partir de l'emplacement de la ressource à envoyer, renvoie son contenu
{
	FILE* file;
	char* ressource;
	long fileLength;

	if(access(target, F_OK) == -1)
	/* 404 (Not Found) */
	{
		*erreur = 1;
		return codeMessage(404);
	}

	file = fopen(target, "rb");
	if(file == NULL)
	{
		*erreur = 1;
		return codeMessage(500); /*500 (Internal Server Error)*/
	}

	//Definir la taille du fichier
	fseek(file, 0, SEEK_END);
	fileLength = ftell(file);
	ressource = malloc(sizeof(char) * (fileLength + 1));

	if(ressource == NULL)
	{
		*erreur = 1;
		return codeMessage(500); /* 500 (Internal Server Error) */
	}

	rewind(file);
	if(fread(ressource, 1, fileLength, file) != fileLength)
	{
		*erreur = 1;
		free(ressource);
		return codeMessage(500); /* 500 (Internal Server Error) */
	}

	*tailleFichier = fileLength;
	ressource[fileLength] = '\0';

	fclose(file);

	return ressource;
}

char* MIMEtype(const char* ressource)
//Retourne une chaîne de caractère contenant le type MIME de la ressource demandée
{
	//Rechercher l'extention
 	char* query = strchr(ressource, (int)'?');
	if(query != NULL)
		*query = '\0';

	char* extension = strrchr(ressource, (int)'.');

	//Chercher la type MIME
	char* mime = malloc(sizeof(char) * MAX_MIME_SIZE);
	strcpy(mime, "text/plain");

	if(extension != NULL)
	{
		extension++;
		if     (!strcmp(extension, "jpg" ))  		strcpy(mime, "image/jpeg");
		else if(!strcmp(extension, "jpeg")) 		strcpy(mime, "image/jpeg");
		else if(!strcmp(extension, "png" )) 		strcpy(mime, "image/png");
		else if(!strcmp(extension, "gif" )) 		strcpy(mime, "image/gif");
		else if(!strcmp(extension, "html")) 		strcpy(mime, "text/html; charset=UTF-8");
		else if(!strcmp(extension, "php" )) 		strcpy(mime, "text/html; charset=UTF-8");
		else if(!strcmp(extension, "pdf" ))  		strcpy(mime, "application/pdf");
		else if(!strcmp(extension, "txt" ))  		strcpy(mime, "text/plain");
		else if(!strcmp(extension, "css" )) 		strcpy(mime, "text/css");
		else if(!strcmp(extension, "js"  )) 		strcpy(mime, "application/javascript");
		else if(!strcmp(extension, "mp4" )) 		strcpy(mime, "video/mp4");
		else if(!strcmp(extension, "ico" )) 		strcpy(mime, "image/x-icon");
		else if(!strcmp(extension, "ttf" )) 		strcpy(mime, "font/ttf");
		else if(!strcmp(extension, "woff" )) 		strcpy(mime, "font/woff");
		else if(!strcmp(extension, "woff2" )) 		strcpy(mime, "font/woff2");
	}
	if(extension == NULL || !strcmp(mime, "text/plain")){ //Si echec consulter libmagic
		//Ouvrir la librairie
		magic_t cookie = magic_open(MAGIC_MIME);

		//Si erreur
		if(cookie == NULL || magic_load(cookie,NULL) != 0){
			magic_close(cookie);
			printf("Erreur de la libmagic\n");
		} else {
			//Chercher le type de fichier
			strcpy(mime,magic_file(cookie, ressource));
		}
		//Liberer la librairie
		magic_close(cookie);
	}
	return mime;
}