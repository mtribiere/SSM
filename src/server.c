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

		if ((res=parseur(requete->buf,requete->len))) { //Si la syntaxe est correcte
			
			//Creer l'arbre sythaxique
			_Token *root;
			root = getRootTree(); 

<<<<<<< HEAD
			//Contruire la réponse
			buildResponse(root, reponse, &tailleRequete);

			//Envoyer la réponse au client
			writeDirectClient(requete->clientId,reponse,tailleRequete); 

			//Liberer la mémorie
			tailleRequete = 0;
			free(reponse);	
			purgeTree(root); 
=======
			buildResponse(root, reponse, &tailleRequete, &close);

			//printf("%s\n", reponse);

			writeDirectClient(requete->clientId,reponse,tailleRequete); 
			
			tailleRequete = 0;
			free(reponse);
			purgeTree(root);


>>>>>>> e2cc28cbaa3354aa1e0e4b8c4f379b917a7b9fdd
		}
		else //Syntaxe incorrecte
		{
			//Envoyer un message d'erreur 
			strcpy(reponse, codeMessage(400));
			writeDirectClient(requete->clientId,reponse,strlen(reponse));
<<<<<<< HEAD

			//Liberer la mémoire
			free(reponse);
		}

		//Se deconnecter du client
		endWriteDirectClient(requete->clientId); 
		requestShutdownSocket(requete->clientId); 

		// on ne se sert plus de requete a partir de maintenant, on peut donc liberer... 
		freeRequest(requete); 
=======
			close = 1;

			free(reponse);
		}

		// on ne se sert plus de requete a partir de maintenant, on peut donc liberer... 
		freeRequest(requete); 

		endWriteDirectClient(requete->clientId);
		if(!close)
		{ 
			requestShutdownSocket(requete->clientId); 
		}
>>>>>>> e2cc28cbaa3354aa1e0e4b8c4f379b917a7b9fdd
	}
	return (1);
}
