#include <stdlib.h>
#include <stdio.h>
#include "parser.h"
#include "fileManager.h"
#include "tree.h"

#define MAX_FILE_SIZE 500

int main(int argc, char *argv[])
{	/*
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
	}
	*/
	Node* fatherNode = createNode("Père");
	char name[][NAMESIZE] = {"Fils 1", "Fils 2"};
	char name1[][NAMESIZE] = {"Fils 1-1", "Fils 1-2"};
	char name2[][NAMESIZE] = {"Fils 2-1", "Fils 2-2"};
	createChild(fatherNode, 2, name);
	createChild(fatherNode->childList[0], 2, name1);
	createChild(fatherNode->childList[1], 2, name2);
	Node *finder = searchByName(&fatherNode, "Fils 1-2");
	if(finder != NULL)
		printf("%s\n", finder->name);
	else
		printf("Pas trouvé\n");

	return 0;
}