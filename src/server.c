#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

// for librequest
#include "request.h"

// for parser
// this will declare internal type used by the parser
#include "httpparser.h"
#include "libparser/api.h"

#include "semantic.h"

#define MAX_RESPONSE_SIZE 500000000

int main(int argc, char *argv[])
{
	message *requete;
	int tailleRequete = 0;
	int res;
	int close = 1;

	while ( 1 ) {
		char* reponse = calloc(sizeof(char), sizeof(char) * MAX_RESPONSE_SIZE); //Voir pour mieux régler la taille
		// on attend la reception d'une requete HTTP requete pointera vers une ressource allouée par librequest.
		if ((requete=getRequest(8080)) == NULL )
		{
			free(reponse);
			return -1;
		}
		//printf(requete->buf);

		if ((res=parseur(requete->buf,requete->len))) { //Si la syntaxe est correcte

			//Creer l'arbre sythaxique
			_Token *root;
			root = getRootTree();

			//Contruire la réponse
			buildResponse(root, reponse, &tailleRequete,&close);

			//Envoyer la réponse au client
			writeDirectClient(requete->clientId,reponse,tailleRequete);

			//Liberer la mémorie
			tailleRequete = 0;
			free(reponse);
			purgeTree(root);
		}
		else //Syntaxe incorrecte
		{
			//Envoyer un message d'erreur
			strcpy(reponse, codeMessage(400));
			writeDirectClient(requete->clientId,reponse,strlen(reponse));

			//Liberer la mémoire
			free(reponse);
		}

		//Se deconnecter du client
		endWriteDirectClient(requete->clientId);
		requestShutdownSocket(requete->clientId);

		// on ne se sert plus de requete a partir de maintenant, on peut donc liberer...
		freeRequest(requete);
	}
	return (1);
}
