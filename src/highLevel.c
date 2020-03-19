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
    functions.optionnal = NULL;

	(node->contentSize) = 0;
	(node->childCount) = 0;
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
    functions.optionnal = NULL;

	(node->contentSize) = 0;
	(node->childCount) = 0;
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
	(node->contentSize) = 0;
	(node->childCount) = 0;
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

	//Creer les fils
	(node->contentSize) = 0;
	(node->childCount) = 0;
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
	chooseFrom.optionnal = NULL;

	(node->contentSize) = 0;
	(node->childCount) = 0;
	etoile(chooseFrom,s,1,-1,node);

	if(node->childCount == 0)
		toReturn = FALSE;

	return toReturn;
}

//query = * ( pchar / "/" / "?" )
int query(const char *s, Node* node) {
	//Remplir le node
	strcpy(node->name,"query");
	int toReturn = TRUE;

	//Declarer l'ensemble des fonctions possibles
	functionArray functions;
	(functions.functions)[0] = pchar;
	(functions.functions)[1] = slash;
	(functions.functions)[2] = interrogation;

	functions.functionCount = 3;
	functions.isOrFunction = TRUE;

	//Creer les fils
	(node->contentSize) = 0;
	(node->childCount) = 0;
	etoile(functions,s,0,-1,node);

	return toReturn;
}

//origin-form = absolute-path [ "?" query ]
int originForm(const char *s, Node* node) {
	//Remplir le node
	strcpy(node->name,"origin-form");
	int toReturn = TRUE;

	functionArray functions;
	functions.optionnal = calloc(MAX_FUNCTION_NUMBER, sizeof(int));
	(functions.functions)[0] = absolutePath;
	(functions.functions)[1] = interrogation; functions.optionnal[0] = 1;
	(functions.functions)[2] = query; functions.optionnal[1] = 2;

	functions.functionCount = 3;
	functions.isOrFunction = FALSE;

	(node->contentSize) = 0;
	(node->childCount) = 0;
	etoile(functions,s,1,1,node);

	if(node->childList[0]->contentSize == 0)
		toReturn = FALSE;

	return toReturn;
}

//request-line = method SP origin-form SP HTTP-version CRLF
int requestLine(const char *s, Node* node){
	//Remplir le node
	strcpy(node->name,"request-line");
	int toReturn = TRUE;

	functionArray functions;
	functions.optionnal = NULL;
	(functions.functions)[0] = method;
	(functions.functions)[1] = SP;
	(functions.functions)[2] = originForm; 
	(functions.functions)[3] = SP; 
	(functions.functions)[4] = HTTPVersion; 
	(functions.functions)[5] = CRLF; 

	functions.functionCount = 6;
	functions.isOrFunction = FALSE;

	(node->contentSize) = 0;
	(node->childCount) = 0;
	etoile(functions,s,1,1,node);

	if(node->childCount == 0)
		toReturn = FALSE;

	return toReturn;
}

int subDelims(const char *s, Node* node){
	
	//Copier le nom
	strcpy(node->name,"subDelimiter");
	int toReturn  = TRUE; 

	//Creer le tableau des fonctions
	functionArray chooseFrom;
	chooseFrom.functions[0] = &exclamation;
	chooseFrom.functions[1] = &dollar;
	chooseFrom.functions[2] = &and;
	chooseFrom.functions[3] = &quote;
	chooseFrom.functions[4] = &openParenthese;
	chooseFrom.functions[5] = &closeParenthese;
	chooseFrom.functions[6] = &star;
	chooseFrom.functions[7] = &plus;
	chooseFrom.functions[8] = &coma;
	chooseFrom.functions[9] = &semiColon;
	chooseFrom.functions[10] = &equal;

	chooseFrom.isOrFunction = TRUE;
	chooseFrom.functionCount = 11;

	//Executer la fonction etoile
	(node->contentSize) = 0;
	(node->childCount) = 0;
	etoile(chooseFrom,s,1,1,node);

	//Si le node n'a pas de fils
	if(node->childCount == 0){
		toReturn = FALSE;
	}

    return toReturn;
}


int tchar(const char* s, Node* node) {
	
	//Copier le nom
	strcpy(node->name,"tchar");
	int toReturn  = TRUE; 

	//Creer le tableau des fonctions
	functionArray chooseFrom;
	chooseFrom.functions[0] = &exclamation;
	chooseFrom.functions[1] = &octothorp;
	chooseFrom.functions[2] = &dollar;
	chooseFrom.functions[3] = &quote;
	chooseFrom.functions[4] = &star;
	chooseFrom.functions[5] = &plus;
	chooseFrom.functions[6] = &minus;
	chooseFrom.functions[7] = &dot;
	chooseFrom.functions[8] = &chineseHat;
	chooseFrom.functions[9] = &underScore;
	chooseFrom.functions[10] = &inverseQuote;
	chooseFrom.functions[11] = &pipe;
	chooseFrom.functions[12] = &tilt;
	chooseFrom.functions[13] = &DIGIT;
	chooseFrom.functions[14] = &ALPHA;

	chooseFrom.isOrFunction = TRUE;
	chooseFrom.functionCount = 15;

	//Executer la fonction etoile
	(node->contentSize) = 0;
	(node->childCount) = 0;
	etoile(chooseFrom,s,1,1,node);

	//Si le node n'a pas de fils
	if(node->childCount == 0){
		toReturn = FALSE;
	}

    return toReturn;
}

int unreserved(const char* s, Node* node) {
	
	//Copier le nom
	strcpy(node->name,"unreserved");
	int toReturn  = TRUE; 

	//Creer le tableau des fonctions
	functionArray chooseFrom;
	chooseFrom.functions[0] = &ALPHA;
	chooseFrom.functions[1] = &DIGIT;
	chooseFrom.functions[2] = &tiret;
	chooseFrom.functions[3] = &dot;
	chooseFrom.functions[4] = &underScore;
	chooseFrom.functions[5] = &tilt;

	chooseFrom.isOrFunction = TRUE;
	chooseFrom.functionCount = 6;

	//Executer la fonction etoile
	(node->contentSize) = 0;
	(node->childCount) = 0;
	etoile(chooseFrom,s,1,1,node);

	//Si le node n'a pas de fils
	if(node->childCount == 0){
		toReturn = FALSE;
	}

    return toReturn;
}

int token(const char *s, Node* node){
	strcpy(node->name,"token");
    int toReturn = TRUE;

    functionArray chooseFrom;
    chooseFrom.functions[0] = tchar;
    chooseFrom.functionCount = 1;
    chooseFrom.isOrFunction = TRUE;

	(node->contentSize) = 0;
	(node->childCount) = 0;
    etoile(chooseFrom,s,1,-1,node);

    if(node->childCount == 0)
        toReturn = FALSE;
    return toReturn;
}

int method(const char *s, Node* node) {
    int toReturn = token(s, node);
    strcpy(node->name,"method");
    return toReturn;
}