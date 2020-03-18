#include "highLevel.h"
#include "baseLevel.h"
#include "parser.h"
#include "tree.h"

//HTTP-name = %x48.54.54.50
int HTTPname(const char* s, Node* node) {
	int toReturn = regexTest(s,"^HTTP",4);
    if(node != NULL)
    {
		strcpy(node->name,"HTTP-name");
		node->content = s; 
		node->contentSize = 4;
		node->childCount = 0;
    }
    return toReturn;
}

//HTTP-version = HTTP-name "/" DIGIT "." DIGIT
int HTTPVersion(const char *s, Node* node){
	strcpy(node->name,"HTTP-version");
    int toReturn = TRUE;

    functionArray functions;
    (functions.functions)[0] = HTTPname;
    (functions.functions)[1] = slash;
    (functions.functions)[2] = DIGIT;
    (functions.functions)[3] = dot;
    (functions.functions)[4] = DIGIT;

    functions.functionCount = 5;
    functions.isOrFunction = FALSE;

    etoile(functions, s,1, 1, node);

    if(node->childCount == 0)
        toReturn = FALSE;
    return toReturn;
	
}

//pct-encoded = "%" HEXDIG HEXDIG
int pct_encoded(const char *s,Node *node){
    strcpy(node->name,"pct-encoded");
    int toReturn = TRUE;

    functionArray functions;
    (functions.functions)[0] = percent;
    (functions.functions)[1] = HEXDIG;
    (functions.functions)[2] = HEXDIG;

    functions.functionCount = 3;
    functions.isOrFunction = FALSE;

    etoile(functions, s, 1,1, node);

    if(node->childCount == 0)
        toReturn = FALSE;
    return toReturn;
}


//pchar = unreserved / pct-encoded / sub-delims / ":" / "@"
int pchar(const char *s, Node* node){
	
	//Remplir le node
	strcpy(node->name,"pchar");
	int toReturn = TRUE;

	//Declarer l'ensemble des fonctions possibles
	functionArray functions;
	(functions.functions)[0] = &unreserved;
	(functions.functions)[1] = &pct_encoded;
	(functions.functions)[2] = &subDelims;
	(functions.functions)[3] = &colon;
	(functions.functions)[4] = &at;

	functions.functionCount = 5;
	functions.isOrFunction = TRUE;

	//Creer le(s) fils
	etoile(functions,s,1,1,node);

	//Si etoile ne trouve pas de fils
	if(node->childCount == 0)
		toReturn = FALSE;

	return toReturn;
	
}

//segment = * pchar
int segment(const char *s, Node* node){
	
	//Remplir le node
	strcpy(node->name,"segment");
	int toReturn = TRUE;

	//Declarer l'ensemble des fonctions possibles
	functionArray functions;
	(functions.functions)[0] = &pchar;

	functions.functionCount = 1;
	functions.isOrFunction = TRUE;

	//Creer le fils
	etoile(functions,s,0,-1,node);

	return toReturn;

}

//absolute-path = 1* ( "/" segment )
int absolutePath(const char *s, Node* node)
{
	//Remplir le node
	strcpy(node->name,"absolute-path");
	int toReturn = TRUE;

	functionArray chooseFrom;
	chooseFrom.functions[0] = &slash;
	chooseFrom.functions[1] = &segment;

	chooseFrom.functionCount = 2;
	chooseFrom.isOrFunction = FALSE;

	etoile(chooseFrom,s,1,-1,node);

	if(node->childCount == 0)
		toReturn = FALSE;

	return toReturn;
}
/*
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