#include "highLevel.h"
#include "baseLevel.h"
#include "parser.h"
#include "tree.h"

int HTTPname(const char* s, Node* node) {
	int toReturn = regexTest(s,"HTTP",4);
    if(node != NULL)
    {
    	if(toReturn) {node->content = s; node->contentSize = 4;}
    	else removeNode(node);
    }
    return toReturn;
}


int HTTPVersion(const char *s, Node* node){
	int toReturn = (strlen(s) > 7 && (HTTPname(s, NULL) && slash(s+4, NULL) && DIGIT(s+5, NULL) && dot(s+6 , NULL) && DIGIT(s+7, NULL))); //On vérifie qu'on a bien tous les éléments
	if(node != NULL) //Si le noeud est NULL, on renvoie juste le booléen
	{
		if(toReturn){ //Si le booléen est vrai, on crée les noeuds dans l'arbre
			createChild(node, 5, (char[][NAMESIZE]){"HTTP-name","/","DIGIT",".","DIGIT"}); //on initialise les noeuds fils
		    HTTPname(s, node->childList[0]); //on rajoute à chaque fils les bonnes infos
		    slash(s+4, node->childList[1]);
		    DIGIT(s+5, node->childList[2]);
		    dot(s+6 , node->childList[3]);
		    DIGIT(s+7, node->childList[4]);
		    node->content = s; //On règle les valeurs du noeud père
		    node->contentSize = 8;
		    }else removeNode(node);	//Si le booléen est faux, on supprime le noeud
		} 
    return toReturn;
}


int pctEncoded(const char *s, Node* node){
	int toReturn = (strlen(s) > 2 && (percent(s, NULL) && HEXDIG(s+1, NULL) && HEXDIG(s+2, NULL)));
	if(node != NULL){
		if(toReturn){
			createChild(node, 3, (char[][NAMESIZE]){"%","HEXDIG","HEXDIG"});
			percent(s, node->childList[0]);
			HEXDIG(s+1, node->childList[1]);
			HEXDIG(s+2, node->childList[2]);
			node->content = s;
	        node->contentSize = 3;
		} else removeNode(node);
    }
    return toReturn;
}

result pchar(const char *s, Node* node){
	result toReturn; //On renvoie un result car les fonctions suivantes auront besoin de connaître la taille du pchar
	if(pctEncoded(s, NULL)) toReturn = ((result){TRUE, 3}); //Si on a un pct-encoded, la taille est de 3
	else toReturn = ((result){unreserved(s, NULL) || subDelims(s, NULL) || colon(s, NULL) || at(s,NULL),1}); //Sinon la taille est de 1
	if(node != NULL){ //Si la node est NULL, on renvoie juste le booléen(la taille sera inutile)
		if(toReturn.boolean){ //Si le booléen est vrai, on crée le noeud dans l'arbre
			createChild(node, 1, NULL);
			if(pctEncoded(s, NULL)) {addChild(node, "pct-encoded"); pctEncoded(s, node->childList[0]);} //on regarde quel règle à été vérifiée puis on rajoute les bonnes infos au noeud
			else if(subDelims(s, NULL)) {addChild(node, "sub-delims"); subDelims(s, node->childList[0]);}
			else if(colon(s, NULL)) {addChild(node, ":"); colon(s, node->childList[0]);}
			else if(at(s, NULL)) {addChild(node, "@"); at(s, node->childList[0]);}
			else if(unreserved(s, NULL)) {addChild(node, "unreserved"); unreserved(s, node->childList[0]);}
			node->content = s; //On règle les valeurs du noeud père
			node->contentSize = node->childList[0]->contentSize;
		} else removeNode(node); //Si le booléen est faux, on supprime le noeud père
	}	
	return toReturn;
}

result segment(const char *s, Node* node){
	result ret = etoile(pchar, s, 0, -1);
	if(node == NULL) return (result){ret.boolean, ret.number}; //A priori cette ligne est fausse, ret.number n'est pas la taille du segment mais le nombre d'élements qui le compose
	if(node != NULL)
	{
		int j = 0;
		if(ret.boolean == TRUE)
		{
			createChild(node, ret.number, NULL);
			for(int i = 0; i < ret.number; i++)
			{
				addChild(node, "pchar"); pchar(s+j, node->childList[i]);
				j += node->childList[i]->contentSize;
			}
			node->content = s;
			int size = 0;
			for(int i =0; i < node->childCount; i++) size += node->childList[i]->contentSize;
			node->contentSize = size; //On calcule la taille du père à partir de la taille de ses fils
		} else removeNode(node);
	}
	return (result){ret.boolean, node->contentSize};
}
