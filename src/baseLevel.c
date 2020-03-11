#include "baseLevel.h"
#include "tree.h"
#include "parser.h"

//Définitions de base de l'ABNF
int DIGIT(const char* s, Node* node) {
    if(regexTest(s,"[0-9]",1)){
        node->content = s;
        node->contentSize = 1;
        return TRUE;
    } else {
        removeNode(node);
        return FALSE;
    }
}
int ALPHA(const char *s, Node* node) {
	
	int toReturn = FALSE;
	if(regexTest(s,"[A-Za-z]",1)){
		node->content = s;
		node->contentSize = 1;
		return TRUE;
	}else{
		removeNode(node);
	}

	return toReturn;
	
}
int BIT(const char *s)   {return regexTest(s,"[01]",1);}
int HEXDIG(const char *s){return regexTest(s,"[0-9A-Fa-f]",1);}
int CHAR_(const char *s){return regexTest(s, "[-]",1);}
int CR(const char *s){return regexTest(s,"\r",1);}
int LF(const char *s){return regexTest(s,"\n",1);}
int DQUOTE(const char *s){return regexTest(s,"\"",1);}
int HTAB(const char *s){return regexTest(s,"\t",1);}
int OCTET(const char *s){return regexTest(s,"\0-ÿ",1);}
int SP(const char *s){return regexTest(s," ",1);}
int VCHAR(const char *s){return regexTest(s,"[\\!-~]",1);}

int OWS(const char *s){return (etoile(SP,s,0,-1,1) || etoile(HTAB,s,0,-1,1));} //Renvoie toujours 1...
int RWS(const char *s){return (etoile(SP,s,1,-1,1) || etoile(HTAB,s,1,-1,1));}
int BWS(const char *s){return OWS(s);}
int CRLF(const char *s){
    if(strlen(s) > 1) return (CR(s) && LF(s+1)); 
    else return FALSE;
}

//Définitions que l'on retrouve souvent
int tchar(const char *s){return regexTest(s,"(\\!|#|\\$|%|&|'|\\*|\\+|\\-|\\.|\\^|_|`|\\||~|[0-9A-Za-z])",1);}
int token(const char *s){return etoile(tchar, s, 1, -1, 1);}


//Début message HTTP
int slash(const char *s, Node* node){
    if(regexTest(s,"/",1)){
        node->content = s;
        node->contentSize = 1;
        return TRUE;
    } else {
        removeNode(node);
        return FALSE;
    }
}
int dot(const char *s, Node* node){
    if(regexTest(s,".",1)){
        node->content = s;
        node->contentSize = 1;
        return TRUE;
    } else {
        removeNode(node);
        return FALSE;
    }
}