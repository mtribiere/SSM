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

int DIGIT(const char* s) {return regexTest(s, "[0-9]", 1);}
int ALPHA(const char *s) {return regexTest(s,"[A-Za-z]",1);}
int BIT(const char *s)   {return regexTest(s,"[01]",1);}
int HEXDIG(const char *s){return regexTest(s,"[0-9A-Fa-f]",1);}


int etoile(int (*fonction)(const char* s), const char* s, int mini, int maxi)
//Prend en argument une fonction booléenne sur un caractere, une chaine de caractère et un nombre minimal et maximal d'occurence
//Le max est à -1 si il est infini
//Renvoie TRUE si pour chaque caractère de la chaine de caractère, la fonction boolèenne renvoie TRUE entre le nombre minimal et maximal de fois.
{
    int i = 0;
    while(fonction(s+i)) {
        i++;
    }
    if((maxi == -1) && i <= mini) return FALSE;
    if((i < mini ||i > maxi) && maxi != -1) return FALSE;
    return TRUE;
}

