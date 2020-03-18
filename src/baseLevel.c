#include "baseLevel.h"
#include "tree.h"
#include "parser.h"

//Définitions de base de l'ABNF
int DIGIT(const char* s, Node* node) {
	int toReturn = regexTest(s,"^[0-9]",1);
    if(node != NULL)
    {
		strcpy(node->name,"DIGIT");
		node->content = s; 
		node->contentSize = 1;
		node->childCount = 0;
    }
    return toReturn;
}
int ALPHA(const char* s, Node* node) {
	int toReturn = regexTest(s,"^[A-Za-z]",1);
    if(node != NULL)
    {
		strcpy(node->name,"ALPHA");
		node->content = s; 
		node->contentSize = 1;
		node->childCount = 0;
    }
    return toReturn;
}

int BIT(const char* s, Node* node) {
	int toReturn = regexTest(s,"^[01]",1);
    if(node != NULL)
    {
        strcpy(node->name,"BIT");
        node->content = s; 
        node->contentSize = 1;
        node->childCount = 0;
    }
    return toReturn;
}

int HEXDIG(const char* s, Node* node) {
	int toReturn = regexTest(s,"^[0-9A-Fa-f]",1);
    if(node != NULL)
    {
		strcpy(node->name,"HEXDIG");
		node->content = s; 
		node->contentSize = 1;
		node->childCount = 0;
    }
    return toReturn;
}

int CHAR_(const char* s, Node* node) {
	int toReturn = regexTest(s,"^[-]",1);
    if(node != NULL)
    {
        strcpy(node->name,"CHAR");
        node->content = s; 
        node->contentSize = 1;
        node->childCount = 0;
    }
    return toReturn;
}

int CR(const char* s, Node* node) {
	int toReturn = regexTest(s,"^\r",1);
    if(node != NULL)
    {
        strcpy(node->name,"CR");
        node->content = s; 
        node->contentSize = 1;
        node->childCount = 0;
    }
    return toReturn;
}
int LF(const char* s, Node* node) {
	int toReturn = regexTest(s,"^\n",1);
    if(node != NULL)
    {
        strcpy(node->name,"LF");
        node->content = s; 
        node->contentSize = 1;
        node->childCount = 0;
    }
    return toReturn;
}
int DQUOTE(const char* s, Node* node) {
	int toReturn = regexTest(s,"DQUOTE",1);
    if(node != NULL)
    {
        strcpy(node->name,"^\"");
        node->content = s; 
        node->contentSize = 1;
        node->childCount = 0;
    }
    return toReturn;
}
int HTAB(const char* s, Node* node) {
	int toReturn = regexTest(s,"^\t",1);
    if(node != NULL)
    {
        strcpy(node->name,"HTAB");
        node->content = s; 
        node->contentSize = 1;
        node->childCount = 0;
    }
    return toReturn;
}
int OCTET(const char* s, Node* node) {
	int toReturn = regexTest(s,"^\0-ÿ",1);
    if(node != NULL)
    {
        strcpy(node->name,"OCTET");
        node->content = s; 
        node->contentSize = 1;
        node->childCount = 0;
    }
    return toReturn;
}
int SP(const char* s, Node* node) {
	int toReturn = regexTest(s,"^ ",1);
    if(node != NULL)
    {
        strcpy(node->name,"SP");
        node->content = s; 
        node->contentSize = 1;
        node->childCount = 0;
    }
    return toReturn;
}
int VCHAR(const char* s, Node* node) {
	int toReturn = regexTest(s,"^[\\!-~]",1);
    if(node != NULL)
    {
        strcpy(node->name,"VCHAR");
        node->content = s; 
        node->contentSize = 1;
        node->childCount = 0;
    }
    return toReturn;
}

int slash(const char* s, Node* node) {
	int toReturn = regexTest(s,"^/",1);
    if(node != NULL)
    {
        strcpy(node->name,"/");
        node->content = s; 
        node->contentSize = 1;
        node->childCount = 0;
    }
    return toReturn;
}

int interrogation(const char* s, Node* node) {
	int toReturn = regexTest(s,"^\\?",1);
    if(node != NULL)
    {
        strcpy(node->name,"?");
        node->content = s; 
        node->contentSize = 1;
        node->childCount = 0;
    }
    return toReturn;
}

int dot(const char* s, Node* node) {
	int toReturn = regexTest(s,"^\\.",1);
    if(node != NULL)
    {
        strcpy(node->name,".");
        node->content = s; 
        node->contentSize = 1;
        node->childCount = 0;
    }
    return toReturn;
}


int percent(const char* s, Node* node) {
	int toReturn = regexTest(s,"^%",1);
    if(node != NULL)
    {
        strcpy(node->name,"%");
        node->content = s; 
        node->contentSize = 1;
        node->childCount = 0;
    }
    return toReturn;
}


int unreserved(const char* s, Node* node) {
	int toReturn = regexTest(s,"^([A-Za-z0-9]|\\-|\\.|_|~)",1);
    if(node != NULL)
    {
		strcpy(node->name,"unreserved");
		node->content = s; 
		node->contentSize = 1;
		node->childCount = 0;
    }
    return toReturn;
}



int subDelims(const char* s, Node* node) {
	int toReturn = regexTest(s,"^(\\!|\\$|&|'|\\(|\\)|\\*|\\+|,|;|\\=)",1);
    if(node != NULL)
    {
		strcpy(node->name,"sub-delims");
		node->content = s; 
		node->contentSize = 1;
		node->childCount = 0;
    }

    return toReturn;
}

int colon(const char* s, Node* node) {
	int toReturn = regexTest(s,"^:",1);
    if(node != NULL)
    {
		strcpy(node->name,":");
		node->content = s; 
		node->contentSize = 1;
		node->childCount = 0;
    }
    return toReturn;
}

int at(const char* s, Node* node) {
	int toReturn = regexTest(s,"^@",1);
    if(node != NULL)
    {
		strcpy(node->name,"@");
		node->content = s; 
		node->contentSize = 1;
		node->childCount = 0;
    }
    return toReturn;
}

int tchar(const char* s, Node* node) {
	int toReturn = regexTest(s,"^(\\!|#|\\$|%|&|'|\\*|\\+|\\-|\\.|\\^|_|`|\\||~|[0-9A-Za-z])",1);
    if(node != NULL)
    {
        strcpy(node->name,"tchar");
        node->content = s; 
        node->contentSize = 1;
        node->childCount = 0;
    }
    return toReturn;
}


int OWS(const char *s, Node* node) {
    strcpy(node->name,"OWS");
    int toReturn = TRUE;

	functionArray chooseFrom;
	chooseFrom.functions[0] = &SP;
	chooseFrom.functions[1] = &HTAB;
	chooseFrom.functionCount = 2;
	chooseFrom.isOrFunction = TRUE;

	etoile(chooseFrom,s,0,-1,node);

    return toReturn;
}

int RWS(const char *s, Node* node) {
	strcpy(node->name,"RWS");
    int toReturn = TRUE;

    functionArray chooseFrom;
    chooseFrom.functions[0] = &SP;
    chooseFrom.functions[1] = &HTAB;
    chooseFrom.functionCount = 2;
    chooseFrom.isOrFunction = TRUE;

    etoile(chooseFrom,s,1,-1,node);

    if(node->childCount == 0)
        toReturn = FALSE;

    return toReturn;
}

int BWS(const char *s, Node* node) {
    int toReturn = OWS(s, node);
    strcpy(node->name,"BWS");
    return toReturn;
}

int CRLF(const char *s, Node* node){
	strcpy(node->name,"CRLF");
    int toReturn = TRUE;

    functionArray functions;
    (functions.functions)[0] = CR;
    (functions.functions)[1] = LF;

    functions.functionCount = 2;
    functions.isOrFunction = FALSE;
    functions.optionnal = NULL;
    etoile(functions, s,1, 1, node);

    if(node->childCount == 0)
        toReturn = FALSE;
    return toReturn;
}


int token(const char *s, Node* node){
	strcpy(node->name,"token");
    int toReturn = TRUE;

    functionArray chooseFrom;
    chooseFrom.functions[0] = tchar;
    chooseFrom.functionCount = 1;
    chooseFrom.isOrFunction = TRUE;

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