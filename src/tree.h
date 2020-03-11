#ifndef TREE_H
#define TREE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FALSE 0
#define TRUE 1

#define NAMESIZE 50 //Taille maximale du nom d'un noeud

typedef struct Node {
	char name[NAMESIZE];
	const char* content;
	int contentSize; 
	struct Node** childList;
	int childCount;
} Node;

Node* createNode(const char name[NAMESIZE]);
void createChild(Node* parent, int count, const char names[][NAMESIZE]);
Node* searchByName(Node** root, const char name[NAMESIZE]);
void removeNode(Node* node);

int treeLength(Node** root);

void printNode(Node* node);
void printStringWithLimit(const char *s,int limit);

#endif