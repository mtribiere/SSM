#include <stdlib.h>
#include <stdio.h>
#include "parser.h"
#include "fileManager.h"
#include "tree.h"
#include "highLevel.h"
#include "baseLevel.h"

#define MAX_FILE_SIZE 500

int main(int argc, char *argv[])
{	
	/*
	//Si pas assez d'arguments
	if(argc != 2){
		printf("Erreur d'arguments.\n Usage : ./ssm <filename>\n");
		exit(1);
	}

	//Charger le fichier
	char message[MAX_FILE_SIZE] = {0};
	int messageSize = loadFile(argv[1],message);

	//DEBUG
	printf("Size loaded : %d char\n",messageSize);
	for(int i = 0;i<messageSize;i++){
		printf(" %d ",message[i]);
		if(message[i] == '\n') printf("\n");
	}*/

	Node* node = createNode("token");
	printf("token : %d\n", token("~:e", node));
	printf("Taille de l'arbre : %d\n", treeLength(&node));
	
	Node** foundList = searchByName(&node, "tchar");
	if(foundList != NULL)
	{
		int i = 0;
		while(foundList[i] != NULL)
			printNode(foundList[i++]);
	} else printf("Pas de tchar\n");
	
	return 0;
}