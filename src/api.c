#include "api.h"
#include "highLevel.h"

static Node *staticTreeRoot = NULL;


//Fonction qui retourne la racine de l'arbre
void *getRootTree(){
	return (void *)staticTreeRoot;
}


//Fonction qui créer l'arbre. 
//Retourne 1 en cas de succes
int parseur(char *req, int len){
	
	int toReturn = 1;

	//Ajouter le caratctère sentinel
	req[len-1] = '\0';
	
	//Creer l'arbre
	Node *treeRoot = malloc(sizeof(Node));
	treeRoot->childCount = 0;
	treeRoot->contentSize = 0;

	requestLine(req,treeRoot);

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
		
		//J'ai commenté en dessous pour enlever les WARNING
		// toReturn = myNode->content;
		// len = myNode->contentSize;
		
	}

	return toReturn;
}