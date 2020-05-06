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
	_Token* field;
	Lnode* node;
	int erreur = 0;

	field = searchTree(root, "method");
	node = field->node;
	code(root,reponse, taille, &erreur);
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

int code(_Token* root, char* reponse, int* taille, int* erreur)
//Détermine le code à envoyer
{
	int code = 200;
	_Token* field;
	Lnode* node;

	field = searchTree(root, "HTTP_version");
	node = field->node;
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
		case(202): return "HTTP/1.1 202 Accepted\r\nContent-Length: 31\r\nContent-Type: text/plain\r\n\r\nPOST valide recu : 202 Accepted";
		case(400): return "HTTP/1.1 400 Bad Request\r\nConnection: close\r\nContent-Type: text/plain\r\n\r\n400 Bad Request";
		case(404): return "HTTP/1.1 404 Not Found\r\nConnection: close\r\nContent-Type: text/plain\r\n\r\n404 Not Found";
		case(408): return "HTTP/1.1 408 Request Timeout\r\nConnection: close\r\nContent-Type: text/plain\r\n\r\n408 Request Timeout";
		case(411): return "HTTP/1.1 411 Length Required\r\nConnection: close\r\nContent-Type: text/plain\r\n\r\n411 Length Required";
		case(418): return "HTTP/1.1 418 I m a teapot\r\nConnection: close\r\nContent-Type: text/plain\r\n\r\n418 I'm a teapot";
		case(501): return "HTTP/1.1 501 Not Implemented\r\nConnection: close\r\nContent-Type: text/plain\r\n\r\n501 Not Implemented";
		case(505): return "HTTP/1.1 505 HTTP Version Not Supported\r\nConnection: close\r\nContent-Type: text/plain\r\n\r\n505 HTTP Version Not Supported";
		default  : return "HTTP/1.1 500 Internal Server Error\r\nConnection: close\r\nContent-Type: text/plain\r\n\r\n500 Internal Server Errror";
	}
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

	return URI;
}

//S'exécute avant que le serveur soit lancé
// Charge la configuration contenue dans le fichier sites.conf dans une liste chaînée
void loadMultisitesConf()
{
	multisitesConf = NULL;

	// On ouvre le fichier
	FILE* fichierConf = fopen("sites.conf", "r");
	if(fichierConf == NULL)
	//Lors d'une erreur, on ferme le serveur
	//Pas besoin d'envoyer un 500 (Internal Server Error) car aucune connexion n'est ouverte
	{
		perror("Erreur lors de l'initialisation");
		exit(-1);
	}

	// On récupère la première ligne
	char* ligne = malloc(500*sizeof(char));
	if(ligne == NULL)
	{
		perror("Erreur lors de l'initialisation");
		exit(-1);
	}

	Site *site;


	// On malloc() les deux champs code et fichier
	char* code = malloc(sizeof(char)*50);
	if(code == NULL)
	{
		perror("Erreur lors de l'initialisation");
		exit(-1);
	}
	char* codeFree = code;
	char* fichier = malloc(sizeof(char)*50);
	if(fichier == NULL)
	{
		perror("Erreur lors de l'initialisation");
		exit(-1);
	}


	// On parcourt le fichier
	while (fgets(ligne, 500, fichierConf) != NULL) {
		// Si on enregistre un nouveau site
		if (ligne[0] != '\t') {
			// On malloc l'espace
			site = malloc(sizeof(Site));
			if(site == NULL)
			{
				perror("Erreur lors de l'initialisation");
				exit(-1);
			}
			//
			char* e400 = malloc(sizeof(char)*50);
			char* e404 = malloc(sizeof(char)*50);
			char* e408 = malloc(sizeof(char)*50);
			char* e411 = malloc(sizeof(char)*50);
			char* e418 = malloc(sizeof(char)*50);
			char* e501 = malloc(sizeof(char)*50);
			char* e505 = malloc(sizeof(char)*50);
			site->e400 = e400;
			site->e404 = e404;
			site->e408 = e408;
			site->e411 = e411;
			site->e418 = e418;
			site->e501 = e501;
			site->e505 = e505;

			// On malloc() les deux champs fqdn et dossier
			char* fqdn = malloc(sizeof(char)*50);
			if(fqdn == NULL)
			{
				perror("Erreur lors de l'initialisation");
				exit(-1);
			}
			char* dossier = malloc(sizeof(char)*50);
			if(dossier == NULL)
			{
				perror("Erreur lors de l'initialisation");
				exit(-1);
			}

			// On parse la ligne
			char *ptr = strtok(ligne, " ");
			strcpy(fqdn, ptr);
			ptr = strtok(NULL, " ");
			strcpy(dossier, ptr);

			// On retire les sauts à la ligne
			int c = 0;
			while (dossier[c] != '\0') {
				if ((dossier[c] == '\n') || (dossier[c] == '\r')) {
					dossier[c] = '\0';
				}
				c++;
			}

			// On enregistre les données parsées
			site->fqdn = fqdn;
			site->dossier = dossier;

			site->next = multisitesConf;
			multisitesConf = site;
		}
		// Si on enregistre un nouveau fichier d'erreur
		else {

			// On parse la ligne
			char *ptr = strtok(ligne, " ");
			strcpy(code, ptr);
			code++;
			ptr = strtok(NULL, " ");
			strcpy(fichier, ptr);

			// On retire les sauts à la ligne
			int c = 0;
			while (fichier[c] != '\0') {
				if ((fichier[c] == '\n') || (fichier[c] == '\r')) {
					fichier[c] = '\0';
				}
				c++;
			}

			// On enregistre les données parsées
			if      (!strcmp(code, "400")) strcpy(multisitesConf->e400, fichier);
			else if (!strcmp(code, "404")) strcpy(multisitesConf->e404, fichier);
			else if (!strcmp(code, "408")) strcpy(multisitesConf->e408, fichier);
			else if (!strcmp(code, "411")) strcpy(multisitesConf->e411, fichier);
			else if (!strcmp(code, "418")) strcpy(multisitesConf->e418, fichier);
			else if (!strcmp(code, "501")) strcpy(multisitesConf->e501, fichier);
			else if (!strcmp(code, "505")) strcpy(multisitesConf->e505, fichier);
		}
	}
	free(codeFree);
	free(fichier);
	free(ligne);
	fclose(fichierConf);
}

// void unloadMultiSitesConf()
// {
// 	Site* s = multisitesConf;
// 	Site* next;
// 	while(s != NULL)
// 	{
// 		free(site->e400);
// 		free(site->e404);
// 		free(site->e408);
// 		free(site->e411);
// 		free(site->e418);
// 		free(site->e501);
// 		free(site->e505);

// 		free(fdqn);
// 		free(dossier);

// 		next = s->next;
		
// 		free(site);
// 	}
// }

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

		char* elem = malloc(sizeof(char)*500);
		if(elem == NULL)
		{
			*erreur = 1;
			return codeMessage(500); /* 500 (Internal Server Error) */
		}
		for (int i = 0; i < a->len; i++) {
			elem[i] = el[i]; //Recopie du contenu du champ Host
		}

		Site* s = multisitesConf; //Tête de la liste chaînée

		char *ptr2;
		ptr2 = strtok(elem, " ");
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
