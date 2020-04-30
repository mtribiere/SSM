#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
// #include <time.h>

// for librequest
#include "request.h"

// for parser
// this will declare internal type used by the parser
#include "httpparser.h"
#include "libparser/api.h"

#include "semantic.h"

int main(int argc, char *argv[])
{
	message *requete;
	int tailleRequete = 0;
	int res;
	int close = 0;

	loadMultisitesConf();
	// unloadMultiSitesConf pour décharger liste chainée

	while ( 1 ) {
		char* reponse = calloc(sizeof(char), sizeof(char) * MAX_RESPONSE_SIZE); //Voir pour mieux régler la taille
		// on attend la reception d'une requete HTTP requete pointera vers une ressource allouée par librequest.
		if ((requete=getRequest(8080)) == NULL )
		{
			free(reponse);
			return -1;
		}

		if ((res=parseur(requete->buf,requete->len))) { //Si la syntaxe est correcte

			//Creer l'arbre syntaxique
			_Token *root;
			root = getRootTree();

			//Construire la réponse
			buildResponse(root, reponse, &tailleRequete,&close);

			//Envoyer la réponse au client
			writeDirectClient(requete->clientId,reponse,tailleRequete);

			//Liberer la mémoire
			tailleRequete = 0;
			purgeTree(root);
		}
		else //Syntaxe incorrecte
		{
			//Envoyer un message d'erreur
			strcpy(reponse, codeMessage(400));
			writeDirectClient(requete->clientId,reponse,strlen(reponse));
		}

		endWriteDirectClient(requete->clientId);
		
		//Se deconnecter du client
		if(close)
			requestShutdownSocket(requete->clientId);

		//Liberer la mémoire
		free(reponse);
		// on ne se sert plus de requete a partir de maintenant, on peut donc liberer...
		freeRequest(requete);
		
	}
	return (1);
}