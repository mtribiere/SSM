#include "parser.h"

int main()
{
    printf("Exactement 3 digits : %s,%d,%d\t:\t%d\n", "352", 3, 3, etoile(DIGIT, "352",3,3));
    printf("Entre 3 et 10 digits : %s,%d,%d\t:\t%d\n", "352", 3, 10, etoile(DIGIT, "352",3,10));
    printf("Entre 3 et 4 digits : %s,%d,%d\t:\t%d\n", "35a2", 3, 4, etoile(DIGIT, "35a2",3,3));
    printf("Entre 10 et 3 digits : %s,%d,%d\t:\t%d\n", "352", 10, 3, etoile(DIGIT, "352",10,3));
    printf("Plus de 0 digits : %s,%d,%d\t:\t%d\n", "0123456789", 0, -1, etoile(DIGIT, "0123456789",0,-1));
    printf("Plus de 3 digits : %s,%d,%d\t:\t%d\n", "25", 3, -1, etoile(DIGIT, "25",3,-1));
    printf("Plus de 3 digits : %s,%d,%d\t:\t%d\n", "2552", 3, -1, etoile(DIGIT, "2552",3,-1));
    printf("Moins de 3 digits : %s,%d,%d\t:\t%d\n", "2552", 0, 3, etoile(DIGIT, "2552",0,3));
    printf("Moins de 3 digits : %s,%d,%d\t:\t%d\n", "25", 0, 3, etoile(DIGIT, "25",0,3));
    return 0;
}
