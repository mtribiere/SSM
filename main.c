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
	
	char s[] = "GET /images/logo.png?q=now HTTP/1.1\r\n"; 
	Node testNode;
	testNode.childList = malloc(sizeof(Node)*MAX_CHILD_COUNT);

	if(requestLine(s,&testNode))
		printTree(&testNode,0);
	else printf("FAUX\n");

	removeNode(&testNode);
	
	return 0;
}