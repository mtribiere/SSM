#include "parser.h"

int DIGIT(const char* c)
//Prend en argument une cha�ne de caract�re de taille 1, renvoie 1 si c'est un chiffre et 0 sinon
{
    char digits[] = "0123456789";
    for(int i = 0; i <= 9; ++i) {
        if(c[0] == digits[i]) return TRUE;
    }
    return FALSE;
}

int etoile(int (*fonction)(const char* c), const char* s, int mini, int maxi)
//Prend en argument une fonction bool�enne sur un caract�re, une cha�ne de caract�re et un nombre minimal et maximal d'occurence
//Le max est � -1 si il est infini
//Renvoie TRUE si pour chaque caract�re de la cha�ne de caract�re, la fonction bool�enne renvoie TRUE entre le nombre minimal et maximal de fois.
{
    int i = 0;
    while(fonction(s+i)) {
        i++;
    }
    if((maxi == -1) && i <= mini) return FALSE;
    if((i < mini ||i > maxi) && maxi != -1) return FALSE;
    return TRUE;
}

