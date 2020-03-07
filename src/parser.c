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
int CHAR_(const char *s){return regexTest(s, "[-]",1);}
int CR(const char *s){return regexTest(s,"\r",1);}
int LF(const char *s){return regexTest(s,"\n",1);}
int DQUOTE(const char *s){return regexTest(s,"\"",1);}
int HTAB(const char *s){return regexTest(s,"\t",1);}
int OCTET(const char *s){return regexTest(s,"\0-ÿ",1);}
int SP(const char *s){return regexTest(s," ",1);}
int VCHAR(const char *s){return regexTest(s,"[\\!-~]",1);}

int OWS(const char *s){return etoile(ou(SP,HTAB), s, 0, -1);} //Renvoie toujours 1...
int RWS(const char *s){return etoile(ou(SP,HTAB), s, 1, -1);}
int BWS(const char *s){return OWS(s);}
int CRLF(const char *s){if(strlen(s) > 1) return (CR(s) && LF(s+1)); else return FALSE;}

int etoile(fptr function, const char *s, int mini, int maxi)
//Prend en argument une fonction booléenne sur un caractere, une chaine de caractère et un nombre minimal et maximal d'occurence
//Le max est à -1 si il est infini
//Renvoie TRUE si pour chaque caractère de la chaine de caractère, la fonction boolèenne renvoie TRUE entre le nombre minimal et maximal de fois.
{
    int i = 0;
    while(function(s+i)) {
        i++;
    }
    if((maxi == -1) && i < mini) return FALSE;
    if((i < mini ||i > maxi) && maxi != -1) return FALSE;
    return TRUE;
}

struct parameters {
    fptr function1, function2;
} param;

int orFunction(const char *s){return(param.function1(s) || param.function2(s));}

fptr ou(fptr function1, fptr function2) {
    param.function1 = function1;
    param.function2 = function2;
    return orFunction;
}

int andFunction(const char *s){return(param.function1(s) && param.function2(s));}

fptr et(fptr function1, fptr function2) {
    param.function1 = function1;
    param.function2 = function2;
    return andFunction;
}