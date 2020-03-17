#include "highLevel.h"
#include "baseLevel.h"
#include "parser.h"
#include "tree.h"
/*
//HTTP-name = %x48.54.54.50
int HTTPname(const char* s, Node* node) {
	int toReturn = regexTest(s,"HTTP",4);
    if(node != NULL)
    {
    	if(toReturn) {node->content = s; node->contentSize = 4;}
    	else removeNode(node);
    }
    return toReturn;
}

//HTTP-version = HTTP-name "/" DIGIT "." DIGIT
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

//pct-encoded = "%" HEXDIG HEXDIG
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

//pchar = unreserved / pct-encoded / sub-delims / ":" / "@"
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

//segment = * pchar
result segment(const char *s, Node* node){
	results ret = etoile(pchar, s, 0, -1);
	if(node != NULL)
	{
		if(ret.boolean == TRUE)
		{
			int j = 0;
			createChild(node, ret.number, NULL);
			for(int i = 0; i < ret.number; i++)
			{
				addChild(node, "pchar"); pchar(s+j, node->childList[i]);
				j += node->childList[i]->contentSize;
			}
			node->content = s;
			node->contentSize = ret.size;
		} else removeNode(node);
	}
	return (result){ret.boolean, ret.size};
}

//absolute-path = 1* ( "/" segment )
int absolutePath(const char *s, Node* node)
{
	functionArray chooseFrom;
	chooseFrom.functions[0] = &slash;
	chooseFrom.functions[1] = &segment;

	chooseFrom.functionsCount = 2;
	chooseFrom.isOrFunction = FALSE;

	etoile(chooseFrom,s,1,-1,node);
}

//query = * ( pchar / "/" / "?" )
int query(const char *s, Node* node) {
	result queryaux(const char *s, Node* node) {
		if(pchar(s, NULL).boolean) return (result){TRUE, pchar(s, NULL).number};
		else return (result){slash(s, NULL) || interrogation(s, NULL), 1};
	}
	results ret = etoile(queryaux, s, 0, -1);
	if(node != NULL){
		if(ret.boolean == TRUE)
		{
			int j = 0;
			createChild(node, ret.number, NULL);
			for(int i = 0; i < ret.number; i++)
			{
				if(slash(s+j, NULL)) {addChild(node, "/"); slash(s+j, node->childList[i]);}
				else if(interrogation(s+j, NULL)) {addChild(node, "?"); interrogation(s+j, node->childList[i]);}
				else if(pchar(s+j, NULL).boolean) {addChild(node, "pchar"); pchar(s+j, node->childList[i]);}
				j += node->childList[i]->contentSize;
			}
			node->content = s;
			node->contentSize = ret.size;
		} else {
			removeNode(node);
		}
	}
	return ret.boolean;
}
*/