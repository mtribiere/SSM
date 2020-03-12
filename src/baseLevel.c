#include "baseLevel.h"
#include "tree.h"
#include "parser.h"

//Définitions de base de l'ABNF
int DIGIT(const char* s, Node* node) {
	int toReturn = FALSE;
    if(regexTest(s,"[0-9]",1)){
    	if(node != NULL)
    	{
    		node->content = s;
        	node->contentSize = 1;
    	}
        toReturn =  TRUE;
    } else {
        removeNode(node);
    }
    return toReturn;
}
int ALPHA(const char *s, Node* node) {
	int toReturn = FALSE;
	if(regexTest(s,"[A-Za-z]",1)){
		if(node != NULL)
    	{
    		node->content = s;
        	node->contentSize = 1;
    	}
		toReturn =  TRUE;
	}else{
		removeNode(node);
	}
	return toReturn;
}
int BIT(const char *s, Node* node) {
	int toReturn = FALSE;
	if(regexTest(s,"[01]",1)){
		if(node != NULL)
    	{
    		node->content = s;
        	node->contentSize = 1;
    	}
		toReturn =  TRUE;
	}else{
		removeNode(node);
	}
	return toReturn;
}
int HEXDIG(const char *s, Node* node) {
	int toReturn = FALSE;
	if(regexTest(s,"[0-9A-Fa-f]",1)){
		if(node != NULL)
    	{
    		node->content = s;
        	node->contentSize = 1;
    	}
		toReturn =  TRUE;
	}else{
		removeNode(node);
	}
	return toReturn;
}
int CHAR_(const char *s, Node* node) {
	int toReturn = FALSE;
	if(regexTest(s,"[-]",1)){
		if(node != NULL)
    	{
    		node->content = s;
        	node->contentSize = 1;
    	}
		toReturn =  TRUE;
	}else{
		removeNode(node);
	}
	return toReturn;
}
int CR(const char *s, Node* node) {
	int toReturn = FALSE;
	if(regexTest(s,"\r",1)){
		if(node != NULL)
    	{
    		node->content = s;
        	node->contentSize = 1;
    	}
		toReturn =  TRUE;
	}else{
		removeNode(node);
	}
	return toReturn;
}
int LF(const char *s, Node* node) {
	int toReturn = FALSE;
	if(regexTest(s,"\n",1)){
		if(node != NULL)
    	{
    		node->content = s;
        	node->contentSize = 1;
    	}
		toReturn =  TRUE;
	}else{
		removeNode(node);
	}
	return toReturn;
}
int DQUOTE(const char *s, Node* node) {
	int toReturn = FALSE;
	if(regexTest(s,"\"",1)){
		if(node != NULL)
    	{
    		node->content = s;
        	node->contentSize = 1;
    	}
		toReturn =  TRUE;
	}else{
		removeNode(node);
	}
	return toReturn;
}
int HTAB(const char *s, Node* node) {
	int toReturn = FALSE;
	if(regexTest(s,"\t",1)){
		if(node != NULL)
    	{
    		node->content = s;
        	node->contentSize = 1;
    	}
		toReturn =  TRUE;
	}else{
		removeNode(node);
	}
	return toReturn;
}
int OCTET(const char *s, Node* node) {
	int toReturn = FALSE;
	if(regexTest(s,"\0-ÿ",1)){
		if(node != NULL)
    	{
    		node->content = s;
        	node->contentSize = 1;
    	}
		toReturn =  TRUE;
	}else{
		removeNode(node);
	}
	return toReturn;
}
int SP(const char *s, Node* node) {
	int toReturn = FALSE;
	if(regexTest(s," ",1)){
		if(node != NULL)
    	{
    		node->content = s;
        	node->contentSize = 1;
    	}
		toReturn =  TRUE;
	}else{
		removeNode(node);
	}
	return toReturn;
}
int VCHAR(const char *s, Node* node) {
	int toReturn = FALSE;
	if(regexTest(s,"[\\!-~]",1)){
		if(node != NULL)
    	{
    		node->content = s;
        	node->contentSize = 1;
    	}
		toReturn =  TRUE;
	}else{
		removeNode(node);
	}
	return toReturn;
}



int OWS(const char *s, Node* node) {
	int OWSaux(const char *s, Node* node) {return (SP(s, NULL) || HTAB(s, NULL));}
	result ret = etoile(OWSaux, s, 0, -1, 1);
	createChild(node, ret.number, NULL);
	for(int i = 0; i < ret.number; i++)
	{
		if(SP(s+i, NULL))
			addChild(node, "SP");
		else if(HTAB(s+i, NULL))
			addChild(node, "HTAB");
		node->childList[i]->content = s + i;
        node->childList[i]->contentSize = 1;
	}
	return ret.boolean;
}

int RWS(const char *s, Node* node) {
	int RWSaux(const char *s, Node* node) {return (SP(s, NULL) || HTAB(s, NULL));}
	result ret = etoile(RWSaux, s, 1, -1, 1);
	createChild(node, ret.number, NULL);
	for(int i = 0; i < ret.number; i++)
	{
		if(SP(s+i, NULL))
			addChild(node, "SP");
		else if(HTAB(s+i, NULL))
			addChild(node, "HTAB");
		node->childList[i]->content = s + i;
        node->childList[i]->contentSize = 1;
	}
	return ret.boolean;
}

int BWS(const char *s, Node* node) {return OWS(s, node);}

int CRLF(const char *s, Node* node){
	int toReturn = FALSE;
	createChild(node, 2, (char[][NAMESIZE]){"CR","LF"});
    if(strlen(s) > 1) toReturn = (CR(s, node->childList[0]) && LF(s + 1, node->childList[1])); 
    return toReturn;
}

//Définitions que l'on retrouve souvent
int tchar(const char *s, Node* node) {
	int toReturn = FALSE;
	if(regexTest(s,"(\\!|#|\\$|%|&|'|\\*|\\+|\\-|\\.|\\^|_|`|\\||~|[0-9A-Za-z])",1)){
		if(node != NULL)
    	{
    		node->content = s;
        	node->contentSize = 1;
    	}
		toReturn =  TRUE;
	}else{
		removeNode(node);
	}
	return toReturn;
}

int token(const char *s, Node* node){
	result ret = etoile(tchar, s, 1, -1, 1);
	createChild(node, ret.number, NULL);
	for(int i = 0; i < ret.number; i++)
	{
		addChild(node, "tchar");
		node->childList[i]->content = s + i;
        node->childList[i]->contentSize = 1;
	}
	return ret.boolean;
}


//Début message HTTP
int slash(const char *s, Node* node){
	int toReturn = FALSE;
    if(regexTest(s,"/",1)){
        if(node != NULL)
    	{
    		node->content = s;
        	node->contentSize = 1;
    	}
		toReturn =  TRUE;
    } else {
        removeNode(node);
    }
    return toReturn;
}
int dot(const char *s, Node* node){
	int toReturn = FALSE;
    if(regexTest(s,".",1)){
        if(node != NULL)
    	{
    		node->content = s;
        	node->contentSize = 1;
    	}
		toReturn =  TRUE;
    } else {
        removeNode(node); 
    }
    return toReturn;
}