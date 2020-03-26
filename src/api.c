#include "api.h"
#include "tree.h"
#include "highLevel.h"

#define MAX_REQUEST_SIZE 799

static Node *staticTreeRoot = NULL;

//Fonction qui retourne la racine de l'arbre
void *getRootTree(){
	return (void *)staticTreeRoot;
}


//Fonction qui créer l'arbre. 
//Retourne 1 en cas de succes
int parseur(char *req, int len){

	//Verifier que la requete ne soit pas trop grande
	if(len > MAX_REQUEST_SIZE){
		printf("Erreur : La requete est trop grande \n");
		exit(1);
	}

	int toReturn = 1;

	//Creer l'arbre
	Node *treeRoot = malloc(sizeof(Node));
	treeRoot->childList = malloc(sizeof(Node)*MAX_CHILD_COUNT);
	treeRoot->childCount = 0;
	treeRoot->contentSize = 0;

	HTTPmessage(req,treeRoot);

	//Si l'arbre n'a pas été créé
	if(treeRoot == NULL){
		toReturn = 0;
	}else{
		staticTreeRoot = treeRoot;
	}

	return toReturn;

}

//Fonction qui retourne un pointeur vers le contenu de la node et sa taille
char *getElementValue(void *node,int *len){

	char *toReturn = NULL;

	//Convetir l'argument en Node *
	Node *myNode = (Node *) node;
	
	//Si la node n'est pas vide
	if(myNode != NULL){
		
		toReturn = (char *)myNode->content;
		*len = myNode->contentSize;
		
	}

	return toReturn;
}

//Fonction qui retourne un pointeur vers le nom  de la node et sa taille
char *getElementTag(void *node,int *len){

	char *toReturn = NULL;

	//Convetir l'argument en Node *
	Node *myNode = (Node *) node;
	
	//Si la node n'est pas vide
	if(myNode != NULL){
		
		toReturn = (char *)myNode->name;
		*len = strlen(toReturn);
		
	}

	return toReturn;
}

//Fonction qui recherche un node avec un nom spécifique
_Token *searchTree(void *start,char *name){ // /!\ On suppose que name contient un \0

	//Si start est NULL
	if(start == NULL)
		start = getRootTree();
	
	//Creer la structure de retour
	_Token *toReturn = NULL;

	//Lancer la recherche
	searchByName(start,&toReturn,name);

	return toReturn;
}

//Fonction qui supprime une liste chainée de _token
void purgeElement(_Token **r){
	
	//Si la liste n'est vide
	if(*r != NULL){

		//Si la liste ne contient pas qu'un element
		if( (*r)->next != NULL ){

			_Token *toDelete = (*r);
			_Token *backNext = (*r)->next;

			do{

				free(toDelete); //Liberer le noeud
				toDelete = backNext; //Passer au noeud suivant
				backNext = backNext->next;

			}while(backNext != NULL); // Tant qu'il ne reste plus qu'un element a liberer

			free(toDelete);
			*r = NULL;

		}else{ //Si la liste ne contient qu'un element

			free(*r); //Le supprimer
			*r = NULL;

		}

	}

}

//Fonction qui supprime tout l'arbre
void purgeTree(void *root){
	removeNode((Node *) root);
}