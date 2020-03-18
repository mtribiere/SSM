#include <stdlib.h>
#include <stdio.h>
#include "fileManager.h"

int loadFile(char *filename,char *dest){
	
	//Ouvrir le fichier en lecture
	FILE *file = NULL;
	file = fopen(filename,"r");

	//Si le fichier n'est pas ouvert
	if( file == NULL){
		printf("Erreur lors de l'ouverture du fichier\n");
		exit(1);
	}

	//Parcourir tout les caracteres
	int counter = 0;
	char currentChar;
	while(currentChar != EOF){
		//Prendre le caractère suivant
		currentChar = fgetc(file);
		
		//Si c'est un retour à la ligne et qu'il n'y a pas de '\r'
		if(currentChar == '\n' && dest[counter-1] != '\r'){
			dest[counter] = '\r';
			counter++;
		}
		
		//Ajouter le caractère courant
		dest[counter] = currentChar;
		
		counter++;
	}

	//Fermer le fichier 
	fclose(file);

	return counter-1;
}