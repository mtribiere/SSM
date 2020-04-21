#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include "httpparser.h" 
#include "libparser/api.h" 
#include "semantic.h"

void buildResponse(_Token* root, char* reponse, int* taille)
//A partir de l'arbre de dérivation syntaxique, construit la réponse HTTP à envoyer
{
	int erreur = 0;
	code(root,reponse, taille, &erreur);
	if(!erreur)
	//ajouter des headers, un message-body ...
	{
		char* URI = normalisationURI(root);

		char* target = findRessource(URI, &erreur);
		if(erreur)
		{
			*taille = strlen(target);
			strcpy(reponse, target);
		}
		else
		{
			//AJOUTER LE HEADER Content-Type
			char* mime = MIMEtype(target);
			addHeader(reponse, "Content-Type", mime, taille);

			reponse[(*taille)++] = '\r';
			reponse[(*taille)++] = '\n';

			int tailleDebut = *taille;
			char* ressource = writeRessource(target, taille, &erreur);
			if(erreur)
			{
				*taille = strlen(ressource);
				strcpy(reponse, ressource);
			} else
			{
				for(int i = tailleDebut; i < *taille; i++)
					reponse[i] = ressource[i - tailleDebut];

				free(ressource);
			}
			free(mime);
		}
		free(target);
	}	

	//Si Connection: close alors Connection:close
	//Sinon Connection: keep-alive
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
	
	field = searchTree(root, "method"); //Méthode différente de GET,HEAD ou POST
	node = field->node;
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
			int length = strtol(node->value, NULL, 10);
			field = searchTree(root, "message_body");
			node = field->node;
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
	for(i = 0; i < node->len; i++) //Dot segment removal
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
	
	char* target = malloc(500); //Changer la taille
	if(target == NULL)
	{
		*erreur = 1;
		return codeMessage(500); /* 500 (Internal Server Error) */
	}

	//+ regarder champ Host, + query ?

	strcpy(target, "www");
	strcpy(target + 3, URI);

	if(!strcmp(target, "www/"))
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

char* MIMEtype(const char* ressource)
//Retourne une chaîne de caractère contenant le type MIME de la ressource demandée
{
	char* extension = strchr(ressource, (int)'.');
	char* mime = malloc(sizeof(char) * 40);

	if(extension == NULL) strcpy(mime, "text/plain");
	else
	{
		extension++;
		if     (!strcmp(extension, "jpg" ))  strcpy(mime, "image/jpeg");
		else if(!strcmp(extension, "jpeg"))  strcpy(mime, "image/jpeg");
		else if(!strcmp(extension, "png" ))  strcpy(mime, "image/png");
		else if(!strcmp(extension, "gif" ))  strcpy(mime, "image/gif");
		else if(!strcmp(extension, "html"))  strcpy(mime, "text/html; charset=UTF-8");
		else if(!strcmp(extension, "php" ))  strcpy(mime, "text/html; charset=UTF-8");
		else if(!strcmp(extension, "pdf" ))  strcpy(mime, "application/pdf");
		else if(!strcmp(extension, "txt" ))  strcpy(mime, "text/plain");
		else if(!strcmp(extension, "css" ))  strcpy(mime, "text/css");
		else if(!strcmp(extension, "js"  ))  strcpy(mime, "application/javascript");
		else if(!strcmp(extension, "mp4" ))  strcpy(mime, "video/mp4");
		else if(!strcmp(extension, "ico" ))  strcpy(mime, "image/x-icon");
	}
	return mime;
}

void addHeader(char* reponse, const char* headerField, const char* headerValue, int* taille)
//Ajoute l'entête indiquée et son contenu à la réponse en cours de construction
{
	sprintf(reponse, "%s%s: %s\r\n", reponse, headerField, headerValue);

	*taille += strlen(headerField) + 4 + strlen(headerValue);
}