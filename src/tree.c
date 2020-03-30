#include "tree.h"
#include "api.h"

void printNode(Node* node,int deep)
{
	//Affiche les données contenues dans un noeud
	printf("\n");
	for(int i = 0;i<deep;i++)
		printf("\t");
	printf("[Name\t:\t%s]\n", node->name);

	for(int i = 0;i<deep;i++)
		printf("\t");
	printf("Content\t:\t");
	printStringWithLimit(node->content,node->contentSize);
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

//Affiche le contenu d'une liste chainée de token
void printTokenChain(_Token *chain){

	if(chain != NULL){
		printTokenChain(chain->next);
		printNode((Node *) (chain->node),0);
	}

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

//Fonction qui recherche les nodes dans l'arbre avec un nom
void searchByName(Node* root, _Token **returnChain ,const char *name)
{
	//Si le noeud à le bon nom
	if(strcmp(name,(root->name)) == 0){
		insertHead(returnChain,root);
	}

	//Parmis les fils chercher si certains ont le bon nom
	for(int i = 0;i<(root->childCount);i++){
		searchByName((root->childList)[i],returnChain,name);
	}

}

//Insertion d'un node en tête d'une liste chainée de token
void insertHead(_Token **head, Node *toInsertNode){

	//Creer l'element a inserer
	_Token *toInsert = malloc(sizeof(_Token));
	(toInsert->node) = (void *) toInsertNode;
	(toInsert->next) = NULL;

	//Si la liste est vide
	if(*head == NULL){

		*head = toInsert;

	}else{ //Sinon on insere avant le premier element

		(toInsert->next) = *head;
		*head = toInsert;

	}

}

void removeNode(Node* node)
//Libère la mémoire du noeud donné
{
	//Si le node est déjà supprimé
	if(node == NULL) return;

	//Liberer les enfants
	for(int i = 0;i<(node->childCount);i++){
		removeNode((node->childList)[i]);
	}
	(node->childCount) = 0;

	//Liberer le tableau des enfants
	free(node->childList);

	//Librer le node
	free(node);
	node = NULL;
}
