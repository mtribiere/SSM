#ifndef TREE_H
#define TREE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
void printNode(Node* node);
int treeLength(Node** root);
Node* searchByName(Node** root, const char name[NAMESIZE]);
void removeNode(Node* node);

#endif