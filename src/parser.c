#include "parser.h"

int regexTest(const char* string, const char* regex, const int size)
{
    int toReturn;
	regex_t preg;
	char* str = malloc(sizeof(char) * size);
	if(str == NULL) perror("Problème d'allocation de mémoire");
	strncpy(str, string, size);
    int err = regcomp (&preg, regex, REG_NOSUB | REG_EXTENDED);
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


result etoile(fptr function, const char *s, int mini, int maxi, int shift)
//Prend en argument une fonction booléenne sur un caractere, une chaine de caractère et un nombre minimal et maximal d'occurence
//Le max est à -1 si il est infini, shift est la taille d'un symbole
//Renvoie TRUE si pour chaque caractère de la chaine de caractère, la fonction boolèenne renvoie TRUE entre le nombre minimal et maximal de fois.
{
    int i = 0;
    while(function(s+i, NULL)) {
        i += shift;
    }
    i /= shift;
    if((maxi == -1) && i < mini) return (result){FALSE, i};
    if((i < mini ||i > maxi) && maxi != -1) return (result){FALSE, i};
    return (result){TRUE, i};
}