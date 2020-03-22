#include <stdlib.h>
#include <stdio.h>
#include "parser.h"
#include "fileManager.h"
#include "tree.h"
#include "highLevel.h"
#include "baseLevel.h"
#include "header.h"

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
	
	//char s[] = "Accept-Language: da, en-gb;q=0.8, en;q=0.7"; 
	char s[] = "Cookie: SID=31d4d96e407aad42; lang=en-US";
	Node testNode;
	testNode.childList = malloc(sizeof(Node)*MAX_CHILD_COUNT);
	testNode.childCount = 0;
	testNode.contentSize = 0;

	if(cookieHeader(s,&testNode))
		printTree(&testNode,0);
	else printf("FAUX\n");

	removeNode(&testNode);
	
	return 0;
}