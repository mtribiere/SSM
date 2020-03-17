#include "parser.h"

int regexTest(const char* string, const char* regex, const int size)
{
    int toReturn;
	regex_t preg;
	char* str = malloc(sizeof(char) * size);
	if(str == NULL) perror("Problème d'allocation de mémoire");
	strncpy(str, string, size);
    int err = regcomp (&preg, regex, REG_NOSUB  | REG_EXTENDED | REG_NEWLINE);
    if(!err)
    {
    	int match = regexec (&preg, str, 0, NULL, 0);
    	regfree (&preg);
    	if(!match) toReturn = TRUE;
    	else toReturn = FALSE;
    }
    free(str);
    return toReturn;
}

void etoile(functionArray functions, const char *s, int min, int max,Node *node)
//Le max est à -1 si il est infini
{
	int validCount = 0;
	int isMaxReached = FALSE; 
	int falseFound = FALSE;
	int totalSize = 0;
	(node->childCount) = 0;


	//Si on doit faire un OU
	if(functions.isOrFunction == TRUE){

		//Par defaut le contenu de la node est la chaine
		node->content = s;

		//Tant que le max n'est pas atteint et qu'aucun node n'est detecté faux
		while(!isMaxReached && !falseFound){	
		
			///////Executer toutes les fonctions
			int isCorrect = FALSE;
			//Creer une fonction temporaire et un node temporaire
			int (*tmpFunction)(const char *,Node *);
			Node *tmpNode;
		
			for(int i = 0;i<functions.functionCount && isCorrect == FALSE;i++){

				//Recuperer la bonne fonction
				tmpFunction = (functions.functions)[i];
				tmpNode = malloc(sizeof(Node));

				//Executer la fonction
				isCorrect = (*tmpFunction)(s,tmpNode);

				//Si la fonction n'est pas correcte
				if(!isCorrect)
					free(tmpNode);

			}

			//Si une fonction est correcte
			if(isCorrect == TRUE){
				//Inserer le node fils
				(node->childList)[validCount] = tmpNode;
				(node->childCount)++;

				//Ajouter un aux nodes trouvées
				validCount++;

				//Si on a depassé le maximum
				if(max != -1 && validCount >= max){
					isMaxReached = TRUE;
				}else{ //Sinon passer au node suivant
					s = s + (tmpNode->contentSize);

					//Sommer la taille
					totalSize = totalSize + (tmpNode->contentSize);
				}

			}else{ //Si la aucune fonction n'est correcte
				falseFound = TRUE;
			}

		}

		//Si un a trouvé un faux avant d'avoir le min
		if(min != -1 && validCount<min){
			
			//Liberer les enfants
			for(int i = 0;i<(node->childCount);i++){
				free((node->childList)[i]);
			}

			//Remettre le compteur à zero
			(node->childCount) = 0;
			
			//Mettre le contenu à vide
			node->content = NULL;
			node->contentSize = 0;

		}else{ //Si le node peut etre rempli

			//Declarer la taille
			node->contentSize = totalSize;
		}

	}
}