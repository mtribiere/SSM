#include "semantic.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <magic.h>
#include "httpparser.h"
#include "site.h"
#include "localRessource.h"

void buildResponse(_Token* root, char* reponse, int* taille, int* close)
//A partir de l'arbre de dérivation syntaxique, construit la réponse HTTP à envoyer
{
	_Token* field;
	Lnode* node;
	int erreur = 0;

	field = searchTree(root, "method");
	node = field->node;
	code(root,reponse, taille, &erreur);

	if (!erreur) {
		if(!erreur && (!strncmp(node->value, "GET", node->len) || !strncmp(node->value, "HEAD", node->len)))
		//ajouter des headers, un message-body ...
		{
			//Normaliser l'URI
			char* URI = normalisationURI(root, &erreur);
			//Si erreur
			if(erreur)
			{
				*taille = strlen(URI);
				*close = 1;
				strcpy(reponse, URI);
			}
			else
			{
				//Convertir l'URI en chemin local
				char* target = findRessource(URI, &erreur);

				//Si erreur
				if(erreur)
				{
					*close = 1;
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

					//Recuperer la resource
					int tailleFichier;
					char* ressource = writeRessource(target, &tailleFichier, &erreur);

					if(erreur)
					{
						*close = 1;
						*taille = strlen(ressource);
						strcpy(reponse, ressource);
						free(ressource);
						free(mime);
					}
					else
					{
						char tailleString[10];

						//Taille du fichier dans un char[]
						sprintf(tailleString, "%d", tailleFichier);
						addHeader(reponse, "Content-Length", tailleString, taille);

						//Passer au body de la réponse
						reponse[(*taille)++] = '\r';
						reponse[(*taille)++] = '\n';

						if(!erreur && !strncmp(node->value, "GET", node->len))
						{
							for(int i = *taille; i < (*taille + tailleFichier); i++)
								reponse[i] = ressource[i - *taille];
							free(ressource);
							*taille += tailleFichier;
						}
						free(mime);
					}
				}
				free(target);
			}
		}
		//Liberer la réponse
		purgeElement(&field);
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
	//Liberer la réponse
	purgeElement(&field);

	if(node->value[5] != '1') //Version majeure différente de 1
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

		} else {
			node = field->node;

			//Liberer la réponse
			purgeElement(&field);

			int length = strtol(node->value, NULL, 10);
			field = searchTree(root, "message_body");
			node = field->node;

			//Liberer la réponse
			purgeElement(&field);

			if(length != node->len)
			{
				*erreur = 1;
				code = 400;
			} else { //Content-Length correct
				code = 202;
			}
		}
	}

	else if(!headerUnique(root))
	//Refuser si plusieurs fois le même header
	{
		*erreur = 1;
		code = 400;
	}

	//char* message = ;
	strcpy(reponse, codeMessage(code));
	*taille += strlen(reponse);
	//free(message);

	return code;
}

char* codeMessage(int code)
//Selon le code à envoyer, forme la start-line
{
	char* toSend = (char*)malloc(sizeof(char)*500000);
	int toSendSize = 0;
	switch(code)
	{
		case(200): free(toSend); return "HTTP/1.1 200 OK\r\n";
		case(202): free(toSend); return "HTTP/1.1 202 Accepted\r\nContent-Length: 31\r\nContent-Type: text/plain\r\n\r\nPOST valide recu : 202 Accepted";
		case(400):
			strcpy(toSend, "HTTP/1.1 400 Bad Request\r\nConnection: close\r\nContent-Type: text/html\r\n\r\n");
			break;
		case(404):
			strcpy(toSend, "HTTP/1.1 404 Not Found\r\nConnection: close\r\nContent-Type: text/html\r\n\r\n");
			break;
		case(411):
			strcpy(toSend, "HTTP/1.1 411 Length Required\r\nConnection: close\r\nContent-Type: text/html\r\n\r\n");
			break;
		case(418):
			strcpy(toSend, "HTTP/1.1 418 I m a teapot\r\nConnection: close\r\nContent-Type: text/html\r\n\r\n");
			break;
		case(501):
			strcpy(toSend, "HTTP/1.1 501 Not Implemented\r\nConnection: close\r\nContent-Type: text/html\r\n\r\n");
			break;
		case(505):
			strcpy(toSend, "HTTP/1.1 505 HTTP Version Not Supported\r\nConnection: close\r\nContent-Type: text/html\r\n\r\n");
			break;
		default:
			strcpy(toSend, "HTTP/1.1 500 Internal Server Error\r\nConnection: close\r\nContent-Type: text/html\r\n\r\n");
	}
	toSendSize += strlen(toSend);

	_Token *root;
	root = getRootTree();
	_Token *host;
	host = searchTree(root, "Host"); //Champ Host

	int trouve = 0;

	if(host != NULL) //Si il y a un champ Host
	{

		Lnode* a = host->node;
		char *el = a->value; //Contenu du champ Host

		char *ptr2;
		ptr2 = strtok(el, ":");

		Site* s = multisitesConf;

		int ignored;
		int ignored2;
		char* ressource;

		while (s != NULL) {
			if (!strcmp(s->fqdn, ptr2)) { // Si on a trouvé la conf du bon site
				switch(code) { // Si un fichier est présent, on recopie la valeur à la fin de toSend
					case(400):
						if (strcmp(s->e400, "")) {
							trouve = 1;
							ressource = writeRessource(s->e400, &ignored, &ignored2);
							strcpy(toSend+toSendSize, ressource);
						}
						break;
					case(404):
						if (strcmp(s->e404, "")) {
							trouve = 1;
							ressource = writeRessource(s->e404, &ignored, &ignored2);
							strcpy(toSend+toSendSize, ressource);
						}
						break;
					case(411):
						if (strcmp(s->e411, "")) {
							trouve = 1;
							ressource = writeRessource(s->e411, &ignored, &ignored2);
							strcpy(toSend+toSendSize, ressource);
						}
						break;
					case(418):
						if (strcmp(s->e418, "")) {
							trouve = 1;
							ressource = writeRessource(s->e418, &ignored, &ignored2);
							strcpy(toSend+toSendSize, ressource);
						}
						break;
					case(501):
						if (strcmp(s->e501, "")) {
							trouve = 1;
							ressource = writeRessource(s->e501, &ignored, &ignored2);
							strcpy(toSend+toSendSize, ressource);
						}
						break;
					case(505):
						if (strcmp(s->e505, "")) {
							trouve = 1;
							ressource = writeRessource(s->e505, &ignored, &ignored2);
							strcpy(toSend+toSendSize, ressource);
						}
						break;
				}
			}
			s = s->next;
		}
	}

	// Valeurs par défaut, si pas de champ HOST ou pas trouvé
	if (!trouve) {
		switch(code) {
			case(400):
				strcpy(toSend+toSendSize, "400 Bad Request");
				break;
			case(404):
				strcpy(toSend+toSendSize, "404 Not Found");
				break;
			case(411):
				strcpy(toSend+toSendSize, "411 Length Required");
				break;
			case(418):
				strcpy(toSend+toSendSize, "418 I'm a teapot");
				break;
			case(501):
				strcpy(toSend+toSendSize, "501 Not Implemented");
				break;
			case(505):
				strcpy(toSend+toSendSize, "505 HTTP Version Not Supported");
				break;
			default:
				strcpy(toSend+toSendSize, "500 Internal Server Error");
				break;
		}
	}
	printf("%s\n", toSend);
	return(toSend);
}

char* normalisationURI(_Token* root, int* erreur)
//Normalise l'URI de la requête : enlève les encodages de type %XY et enlève les /./ /../ en trop
{
	_Token* URIfield = searchTree(root, "origin_form");
	//Cas où l'on ne trouve pas
	if(URIfield == NULL)
	{
		*erreur = 1;
		return codeMessage(400);
	}

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

	URI[j] = '\0';
	purgeElement(&URIfield);
	return URI;
}

void addHeader(char* reponse, const char* headerField, const char* headerValue, int* taille)
//Ajoute l'entête indiquée et son contenu à la réponse en cours de construction
{
	//Copier la reponse temporairement
	char *temp = malloc(sizeof(char) * MAX_RESPONSE_SIZE);
	strcpy(temp,reponse);

	//Ajouter le header (Necessaire pour respecter la doc)
	sprintf(reponse, "%s%s: %s\r\n",temp,headerField,headerValue);

	//Ajouter la taille
	*taille += strlen(headerField) + 4 + strlen(headerValue);

	//Librer la mémoire
	free(temp);
}

int headerUnique(_Token* root)
//0 si il y a plusieurs fois le même header, 1 sinon
{
	int valide = 1;
	char* headerName[13] = {"Connection_header", "Content_Length_header", "Content_Type_header", "Cookie_header", "Transfer_Encoding-header", "Expect_header", "Host_header", "Accept_header", "Accept_Charset_header", "Accept_Encoding_header", "Accept_Language_header", "Referer_header", "User_Agent_header"};
	_Token* field;

	for(int i = 0; i < 13; i++)
	{
		field = searchTree(root, headerName[i]);

		if(field != NULL && field->next != NULL)
		{
			valide = 0;
		}

		purgeElement(&field);
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
				close = 1;
		}
	}
	if(close == 0)
	//Par défaut, on envoie "Connection: keep-alive"
		addHeader(reponse, "Connection", "keep-alive", taille);
	else
	//Si le client demande de fermer la connection, on envoie un "Connection: close"
		addHeader(reponse, "Connection", "close", taille);

	return close;
}
