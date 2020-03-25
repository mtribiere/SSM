#include "tree.h"

Node* createNode(const char name[NAMESIZE]) {
//Crée un noeud à partir de son nom dans un arbre, renvoie un pointeur vers ce noeud
	Node* node = (Node*)malloc(sizeof(Node));
	if(node == NULL)
	{
		perror("Problème d'allocation de mémoire");
		exit(EXIT_FAILURE);
	} 
	strcpy(node->name, name);
	node->childCount = 0;
	node->childList = NULL;
	return node;
}

void createChild(Node* parent, int count, const char names[][NAMESIZE]) {
	//Crée le nombre de fils indiqué au noeud dans l'arbre en utilisant la liste de nom donnée.
	//Renvoie une liste de pointeurs vers les fils créés.
	parent->childList = (Node **)malloc(sizeof(Node*) * count);
	if(parent->childList == NULL)
	{
		perror("Problème d'allocation de mémoire");
		exit(EXIT_FAILURE);
	} 
	if(names != NULL)
	{
		for(int i = parent->childCount; i < count; i++)
		{
			parent->childList[i] = createNode(names[i]);
			parent->childCount++;
		}
	}
	//sinon les fils ne sont pas initialisés
	
}

void addChild(Node* parent, const char name[NAMESIZE])
{
	parent->childList[parent->childCount++] = createNode(name);
}

void printNode(Node* node,int deep)
{
	//Affiche les données contenues dans un noeud
	printf("\n\n");
	for(int i = 0;i<deep;i++)
		printf("\t");
	printf("[Name\t:\t%s]\n", node->name);

	for(int i = 0;i<deep;i++)
		printf("\t");
	printf("Content\t:\t");
	printStringWithLimit(node->content,node->contentSize);
	printf("\n");
/*
	for(int i = 0;i<deep;i++)
		printf("\t");
	printf("ContentSize\t:\t%d\n", node->contentSize);
		
	for(int i = 0;i<deep;i++)
		printf("\t");
	printf("ChildCount\t:\t%d\n", node->childCount);
	printf("\n\n");
	*/
}

void printTree(Node* root,int deep)
{
	deep++;
	printNode(root,deep);
	Node* explorer = root;
	for(int i = 0; i < explorer->childCount; i++)
	{
		printTree(explorer->childList[i],deep);
	}
}

void printStringWithLimit(const char *s,int limit){
	for(int i = 0;i<limit;i++)
		printf("%c",s[i]);
}

int treeLength(Node** root)
{
	//Renvoie le nombre de noeud dans l'arbre
	Node* explorer = *root;
	if(explorer == NULL) return 0;
	if(explorer->childCount == 0) return 1;
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

void searchFunction(Node** root, const char name[NAMESIZE], Node** foundList) {
//Cette fonction n'est pas faite pour être appelée toute seule
//Recherche toutes les occurences d'une node avec le nom name dans l'arbre ayant pour racine root
//Remplit le tableau foundList avec toutes les nodes correspondantes
	Node* explorer = *root;
	int i = 0;
	if(!strcmp(name, explorer->name))
		foundList[count++] = explorer;
	while(i < explorer->childCount)
	{
		searchFunction(explorer->childList + i, name, foundList);
		i++;
	}
}

Node** searchByName(Node** root, const char name[NAMESIZE])
//Utilise searchFunction() pour renvoyer un tableau de nodes appartenant à l'arbre de racine root et ayant pour nom name
{
	count = 0;
	Node** foundList = calloc(treeLength(root), sizeof(Node*));
	if(foundList == NULL)
	{
		perror("Problème d'allocation de mémoire");
		exit(EXIT_FAILURE);
	} 
	searchFunction(root, name, foundList);
	foundList = realloc(foundList, sizeof(Node*) * (count + 1));
	return foundList;
}

void removeNode(Node* node)
//Libère la mémoire du noeud donné
//Attention, on a pas node == NULL après avoir appelé cette fonction ...
{
	if(node == NULL) return;
	(node->childCount) = 0;
	free(node->childList);
	node = NULL;

	free(node);
}