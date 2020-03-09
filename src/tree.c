#include "tree.h"

Node* createNode(const char name[NAMESIZE]) {
//Crée un noeud à partir de son nom dans un arbre, renvoie un pointeur vers ce noeud
	Node* node = (Node*)malloc(sizeof(Node));
	if(node == NULL) perror("Problème d'allocation de mémoire");
	strcpy(node->name, name);
	node->childCount = 0;
	node->childList = NULL;
	return node;
}

void createChild(Node* parent, int count, const char names[][NAMESIZE]) {
	//Crée le nombre de fils indiqué au noeud dans l'arbre en utilisant la liste de nom donnée.
	//Renvoie une liste de pointeurs vers les fils créés.
	parent->childList = (Node **)malloc(sizeof(Node*) * count);
	if(parent->childList == NULL) perror("Problème d'allocation de mémoire");
	for(int i = 0; i < count; i++)
	{
		parent->childList[i] = createNode(names[i]);
		parent->childCount++;
	}
}

void printNode(Node* node)
{
	//Affiche les données contenues dans un noeud
	printf("\n\n");
	printf("Name\t\t:\t%s\n", node->name);
	printf("Content\t\t:\t%s\n", node->content);
	printf("ContentSize\t:\t%d\n", node->contentSize);
	printf("ChildCount\t:\t%d\n", node->childCount);
	printf("\n\n");
}

int treeLength(Node** root)
{
	//Renvoie le nombre de noeud dans l'arbre
	Node* explorer = *root;
	if(explorer->childCount == 0)
		return 1;
	else
	{
		int sum = 1;
		for(int i = 0; i < explorer->childCount; i++)
		{
			sum += treeLength((explorer->childList) + i);
		}
		return sum;
	}
}

Node* searchByName(Node** root, const char name[NAMESIZE]) {
	//A partir d'un pointeur vers la racine et d'un nom, trouve le noeud correspondant dans l'arbre
	//A CHANGER, DEVRAIT RENVOYER TOUS LES NOEUDS CORRESPONDANTS.
	Node* explorer = *root;
	int i = 0;
	Node* result = NULL;
	if(!strcmp(name, explorer->name))
		result = explorer;
	while(result == NULL && i < explorer->childCount)
	{
		result = searchByName(explorer->childList + i, name);
		i++;
	}
	return result;
}

void removeNode(Node* node)
//Pas encore testée
{
	Node* explorer = node;
	if(explorer->childCount == 0) free(explorer);
	else
	{
		for(int i = (explorer->childCount)-1; i >= 0; i--)
		{
			removeNode(explorer->childList[i]);
			explorer->childCount--;
		}
	}
}