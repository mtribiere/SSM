#ifndef TREE_H
#define TREE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "api.h"

#define FALSE 0
#define TRUE 1

#define NAMESIZE 50 //Taille maximale du nom d'un noeud
#define MAX_CHILD_COUNT 50

typedef struct Node {
	char name[NAMESIZE];
	const char* content;
	int contentSize; 
	struct Node** childList;
	int childCount;
} Node;

void removeNode(Node* node);

void searchFunction(Node** root, const char name[NAMESIZE], Node** foundList);
void searchByName(Node* root, _Token **returnChain ,const char *name);
void insertHead(_Token **head, Node *toInsertNode);

int treeLength(Node** root);

void printNode(Node* node,int deep);
void printTree(Node* root,int deep);
void printStringWithLimit(const char *s,int limit);

void printTokenChain(_Token *chain);

#endif