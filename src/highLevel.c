#include "highLevel.h"
#include "baseLevel.h"
#include "parser.h"
#include "tree.h"


int HTTPname(const char *s, Node* node){
	int toReturn = FALSE;
    if(regexTest(s,"HTTP",4)){
        if(node != NULL)
    	{
    		node->content = s;
        	node->contentSize = 4;
    	}
		toReturn =  TRUE;
    } else {
        removeNode(node); 
    }
    return toReturn;
}


int HTTPVersion(const char *s, Node* node){
	if(node == NULL){
		return(strlen(s) > 7 && (HTTPname(s, NULL) && slash(s+4, NULL) && DIGIT(s+5, NULL) && dot(s+6 , NULL) && DIGIT(s+7, NULL)));
	}
    createChild(node, 5, (char[][NAMESIZE]){"HTTP-name","/","DIGIT",".","DIGIT"});
    if(strlen(s) > 7 && (HTTPname(s, node->childList[0]) && slash(s+4, node->childList[1]) && DIGIT(s+5, node->childList[2]) && dot(s+6 , node->childList[3]) && DIGIT(s+7, node->childList[4]))) {
        node->content = s;
        node->contentSize = 8;
        return TRUE;
    }
    else {removeNode(node);return FALSE;}
}


int pctEncoded(const char *s, Node* node){
	if(node == NULL){
		return(strlen(s) > 2 && (percent(s, NULL) && HEXDIG(s+1, NULL) && HEXDIG(s+2, NULL)));
	}
    createChild(node, 3, (char[][NAMESIZE]){"%","HEXDIG","HEXDIG"});
    if(strlen(s) > 2 && (percent(s, node->childList[0]) && HEXDIG(s+1, node->childList[1]) && HEXDIG(s+2, node->childList[2]))) {
        node->content = s;
        node->contentSize = 3;
        return TRUE;
    }
    else {removeNode(node);return FALSE;}
}

//pchar = unreserved / pct-encoded / sub-delims / ":" / "@"
//pctEncoded(NULL) marche pas ...
int pchar(const char *s, Node* node){
	if(node == NULL){
		return(unreserved(s, NULL) || pctEncoded(s, NULL) || subDelims(s, NULL) || colon(s, NULL) || at(s,NULL));
	}
	createChild(node, 1, NULL);
	if(unreserved(s, NULL)) {addChild(node, "unreserved"); unreserved(s, node->childList[0]);}
	else if(pctEncoded(s, NULL)) {addChild(node, "pct-encoded"); pctEncoded(s, node->childList[0]);}
	else if(subDelims(s, NULL)) {addChild(node, "sub-delims"); subDelims(s, node->childList[0]);}
	else if(colon(s, NULL)) {addChild(node, "colon"); colon(s, node->childList[0]);}
	else if(at(s, NULL)) {addChild(node, "at"); at(s, node->childList[0]);}
	else {removeNode(node); return FALSE;}
	node->content = s;
	node->childList[0]->content = s;
	node->contentSize = node->childList[0]->contentSize;
	return TRUE;
}