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

int main(int argc, char *argv[])
{
	message *requete; 
	int tailleRequete = 0;
	int res; 
	int close = 1;

	while ( 1 ) {
		char* reponse = calloc(sizeof(char), sizeof(char) * 50000000); //Voir pour mieux régler la taille
		// on attend la reception d'une requete HTTP requete pointera vers une ressource allouée par librequest. 
		if ((requete=getRequest(8080)) == NULL )
		{
			free(reponse);
			return -1; 
		} 

		if ((res=parseur(requete->buf,requete->len))) { //Si la syntaxe est correcte
			_Token *root; 			

			root = getRootTree(); 

			buildResponse(root, reponse, &tailleRequete, &close);

			//printf("%s\n", reponse);

			writeDirectClient(requete->clientId,reponse,tailleRequete); 
			
			tailleRequete = 0;
			free(reponse);
			purgeTree(root);


		}
		else //Syntaxe incorrecte
		{
			strcpy(reponse, codeMessage(400));
			writeDirectClient(requete->clientId,reponse,strlen(reponse));
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
	}
	return (1);
}
