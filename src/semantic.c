#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <magic.h>
#include "httpparser.h"
#include "libparser/api.h"
#include "semantic.h"

#define MAX_URI_SIZE 5000
#define MAX_MIME_SIZE 100

void buildResponse(_Token* root, char* reponse, int* taille, int* close)
//A partir de l'arbre de dérivation syntaxique, construit la réponse HTTP à envoyer
{
	int erreur = 0;
	code(root,reponse, taille, &erreur);
	if(!erreur)
	//ajouter des headers, un message-body ...
	{
		//Normaliser l'URI
		char* URI = normalisationURI(root);
		
		//Convertir l'URI en chemin local
		char* target = findRessource(URI, &erreur);

		//Si erreur
		if(erreur)
		{
			*taille = strlen(target);
			strcpy(reponse, target);
		}
		else
		{
			//AJOUTER LE HEADER CONNECTION
			*close = connexion(root, reponse, taille);

			//AJOUTER LE HEADER Content-Type
			char* mime = MIMEtype(target);
			addHeader(reponse, "Content-Type", mime, taille);

			//Passer au body de la réponse
			reponse[(*taille)++] = '\r';
			reponse[(*taille)++] = '\n';

			//Recuperer la resource
			int tailleDebut = *taille;
			char* ressource = writeRessource(target, taille, &erreur);

			if(erreur)
			{
				*taille = strlen(ressource);
				strcpy(reponse, ressource);
			} else
			{
				for(int i = tailleDebut; i < (*taille); i++)
					reponse[i] = ressource[i - tailleDebut];
				free(ressource);
			}

			free(mime);
		}
		free(target);
	}

}

int code(_Token* root, char* reponse, int* taille, int* erreur)
//Détermine le code à envoyer
{
	int code = 200;
	_Token* field;
	Lnode* node;

	field = searchTree(root, "HTTP_version");
	node = field->node;
	if(node->value[5] != '1') //Version majeure différent de 1
	{
		*erreur = 1;
		code = 505;
	}

	else if(node->value[7] == '1' && searchTree(root, "Host_header") == NULL) //HTTP 1.1 et pas de Host
	{
		*erreur = 1;
		code = 400;
	}

	//Liberer la réponse
	purgeElement(&field);

	field = searchTree(root, "method"); //Méthode différente de GET,HEAD ou POST
	node = field->node;

	//Liberer la réponse
	purgeElement(&field);

	if(strncmp(node->value, "GET", node->len) && strncmp(node->value, "HEAD", node->len) && strncmp(node->value, "POST", node->len))
	{
		*erreur = 1;
		code = 501;
	}

	else if(searchTree(root, "Content_Length") != NULL && searchTree(root, "Transfer_Encoding") != NULL)
	//Content-Length ET Transfer-Encoding
	{
		*erreur = 1;
		code = 400;
	}

	else if(!strncmp(node->value, "POST", node->len))
	{
		field = searchTree(root, "Content_Length");
		if(field == NULL) //POST et pas de Content-Length
		{
			*erreur = 1;
			code = 411;

		} else{
			node = field->node;

			//Liberer la réponse
			purgeElement(&field);

			int length = strtol(node->value, NULL, 10);
			field = searchTree(root, "message_body");
			node = field->node;

			//Liberer la réponse
			purgeElement(&field);

			if(length != node->len - 3) //Mauvais Content-Length, -3 car le parseur renvoie la mauvaise valeur ??
			{
				*erreur = 1;
				code = 400;
			} else { //Content-Length correct
				*erreur = 1;
				code = 201;
			}
		}
	}

	else if(!headerUnique(root))
	{
		*erreur = 1;
		code = 400;
	}

	//Refuser si plusieurs fois le même header

	strcpy(reponse, codeMessage(code));
	*taille += strlen(reponse);
	return code;
}

char* codeMessage(int code)
//Selon le code à envoyer, forme la start-line
{
	switch(code)
	{
		case(200): return "HTTP/1.1 200 OK\r\n";
		case(201): return "HTTP/1.1 201 Accepted\r\nContent-Type: text/plain\r\n\r\n";
		case(400): return "HTTP/1.1 400 Bad Request\r\nContent-Type: text/plain\r\n\r\n400 Bad Request";
		case(404): return "HTTP/1.1 404 Not Found\r\nContent-Type: text/plain\r\n\r\n404 Not Found";
		case(411): return "HTTP/1.1 411 Length Required\r\nContent-Type: text/plain\r\n\r\n411 Length Required";
		case(501): return "HTTP/1.1 501 Not Implemented\r\nContent-Type: text/plain\r\n\r\n501 Not Implemented";
		case(505): return "HTTP/1.1 505 HTTP Version Not Supported\r\nContent-Type: text/plain\r\n\r\n505 HTTP Version Not Supported";
		default  : return "HTTP/1.1 500 Internal Server Error\r\nContent-Type: text/plain\r\n\r\n500 Internal Server Errror";
	}
}

char* normalisationURI(_Token* root)
//Normalise l'URI de la requête : enlève les encodages de type %XY et enlève les /./ /../ en trop
{
	_Token* URIfield = searchTree(root, "origin_form");
	//Cas où l'on ne trouve pas
	//500 ? 501 ? 400 ?

	Lnode* node = URIfield->node;
	char* URI = node->value;

	int i, j = 0;
	char tmp[2];
	for(i = 0; i < node->len; i++) //Suppresion des caractères encodés
	{
		if(URI[i] == '%')
		{
			tmp[0] = URI[++i];
			tmp[1] = URI[++i];
			URI[j++] = (char)strtol(tmp, NULL, 16);
		}
		else
			URI[j++] = URI[i];
	}
	node->len = j;
	j = 0;
	for(i = 0; i < node->len; i++) //Dot segment removal <- A VERIFIER QUE CA MARCHE
	{
		if(URI[i] == '.')
		{
			if(i >= node->len - 1 || URI[i+1] == '/') i++;
			else if(URI[i+1] == '.')
			{
				if(i >= node->len - 2 || URI[i+2] == '/') i+=2;
				else
					URI[j++] = URI[i];
			}
			else
				URI[j++] = URI[i];
		}
		else
			URI[j++] = URI[i];
	}

	// node->len = j;
	// strcpy(node->value, URI);
	URI[j] = '\0';

	return URI;
}



char* findRessource(const char* URI, int* erreur)
//A partir d'une URI, trouve la ressource associée dans les répertoires du serveur
{

	char* target = malloc(MAX_URI_SIZE);
	if(target == NULL)
	{
		*erreur = 1;
		return codeMessage(500); /* 500 (Internal Server Error) */
	}

	FILE* fichierConf = fopen("sites.conf", "r");
	char* ligne = malloc(500*sizeof(char));
	Site *s = NULL;
	Site *t = NULL;
	//printf("SITES\n");
	while (fgets(ligne, 500, fichierConf) != NULL) {
		if (ligne[0] != '\t') {

			char *ptr = strtok(ligne, " ");
			char* fqdn = malloc(sizeof(char)*500);
			strcpy(fqdn, ptr);
			ptr = strtok(NULL, " ");
			char* dossier = malloc(sizeof(char)*500);
			strcpy(dossier, ptr);

			int c = 0;
			while (dossier[c] != '\0') {
				if ((dossier[c] == '\n') || (dossier[c] == '\r')) {
					dossier[c] = '\0';
				}
				c++;
			}

			t = malloc(sizeof(t));
			t->fqdn = fqdn;
			t->dossier = dossier;

			t->next = s;
			s = t;
		}
	}
	fclose(fichierConf);


	_Token *root;
	root = getRootTree();
	_Token *host;
	host = searchTree(root, "Host");
	Lnode* a = host->node;
	char *el = a->value;

	char* elem = malloc(sizeof(char)*500);
	for (int i = 0; i < a->len; i++) {
		elem[i] = el[i];
	}

	//+ regarder champ Host, + query ?

	char *ptr2;
	ptr2 = strtok(elem, " ");
	ptr2 = strtok(ptr2, ":");

	while(s != NULL && strcmp(ptr2, s->fqdn) != 0) {
		//printf("HOST: %s\n", ptr2);
		//printf("%s\n",s->fqdn);
		//printf("%s\n",s->dossier);
		s = s->next;
	}

	char* dossier = malloc(sizeof(char)*20);
	char* dossierSlash = malloc(sizeof(char)*20);
	if (s == NULL) {
		strcpy(dossier,"www"); // dossier par défaut
	}
	else {
		//printf("Trouve !");
		strcpy(dossier, s->dossier);
	}

	strcpy(dossierSlash, dossier);
	strcpy(dossierSlash+strlen(dossier), "/");
	strcpy(target, dossier);
	strcpy(target + strlen(dossier), URI);

	if(!strcmp(target, dossierSlash))
		strcat(target, "index.html");
	//Faire pareil pour .php ?

	return target;
}

char* writeRessource(const char* target, int* taille, int* erreur)
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
		printf("Ouïe\n");
		/*Codes d'erreur
	  Les droits ne sont pas remplis : 401 (Unauthorized)*/

	}

	//Definir la taille du fichier
	fseek(file, 0, SEEK_END);
	fileLength = ftell(file);
	ressource = malloc(sizeof(char) * (fileLength + 1));

	if(ressource == NULL)
	{
		*erreur = 1;
		free(ressource);
		return codeMessage(500); /* 500 (Internal Server Error) */
	}

	rewind(file);
	if(fread(ressource, 1, fileLength, file) != fileLength)
	{
		*erreur = 1;
		free(ressource);
		return codeMessage(500); /* 500 (Internal Server Error) */
	}

	*taille += fileLength;
	ressource[fileLength] = '\0';

	fclose(file);

	return ressource;
}

char* MIMEtype(const char* ressource){
//Retourne une chaîne de caractère contenant le type MIME de la ressource demandée

	//Rechercher l'extention
 	char* query = strchr(ressource, (int)'?');
	if(query != NULL)
		*query = '\0';

	char* extension = strrchr(ressource, (int)'.');

	//Chercher la type MIME
	char* mime = malloc(sizeof(char) * MAX_MIME_SIZE);

	if(extension == NULL) strcpy(mime, "text/plain");
	else
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
		else{ //Si echec consulter libmagic

			//Ouvrir la librairie
			magic_t cookie = magic_open(MAGIC_MIME);

			//Si erreur
			if(cookie == NULL || magic_load(cookie,NULL) != 0){
				magic_close(cookie);
				printf("Erreur de la libmagic\n");
				exit(1); // A changer
			}

				//Chercher le type de fichier
				strcpy(mime,magic_file(cookie, ressource));	

				//Liberer la librairie
				magic_close(cookie);

		} 										
	}

	printf("Extension : %s\n",mime);
	return mime; 
} 

void addHeader(char* reponse, const char* headerField, const char* headerValue, int* taille)
//Ajoute l'entête indiquée et son contenu à la réponse en cours de construction
{
	sprintf(reponse, "%s%s: %s\r\n", reponse, headerField, headerValue);

	*taille += strlen(headerField) + 4 + strlen(headerValue);

}

int headerUnique(_Token* root)
//0 si il y a plusieurs fois le même header, 1 sinon
{
	int valide = 1;
	char* headerName[13] = {"Connection_header", "Content_Length_header", "Content_Type_header", "Cookie_header", "Transfer_Encoding-header", "Expect_header", "Host_header", "Accept_header", "Accept_Charset_header", "Accept_Encoding_header", "Accept_Language_header", "Referer_header", "User_Agent_header"};
	// + Gérer "field_name" ?
	_Token* field;

	for(int i = 0; i < 13; i++)
	{
		field = searchTree(root, headerName[i]);

		if(field != NULL && field->next != NULL)
		{
			valide = 0;
		}
	}

	return valide;
}

int connexion(_Token* root, char* reponse, int* taille)
//1 si close, 0 sinon
{
	_Token* field;
	Lnode* node;
	int close = 0;
	char keep[] = "keep-alive";

	field = searchTree(root, "Connection");
	if(field != NULL)
	{
		node = field->node;
		//Si on a un "Connection: close"
		for(int i = 0; i < node->len; i++)
		{
			if(node->value[i] != keep[i])
			{
				printf("close\n");
				close = 1;
			}
		}
	}
	if(close == 0)
		addHeader(reponse, "Connection", "keep-alive", taille);
	else
		addHeader(reponse, "Connection", "close", taille); //Vérifier si ça fonctionne bien

	return close;
}
