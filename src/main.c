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
		printf(" %d ",message[i]);s
		if(message[i] == '\n') printf("\n");
	}*/
	
	char s[] = "a3sdsAd "; 
	Node testNode;
	strcpy(testNode.name,"TestNode");

	functionArray functions;
	(functions.functions)[0] = &DIGIT;
	(functions.functions)[1] = &ALPHA;
	functions.functionCount = 2;
	functions.isOrFunction = TRUE;
	
	etoile(functions,s,1,-1,&testNode);
	printTree(&testNode);
	
	return 0;
}