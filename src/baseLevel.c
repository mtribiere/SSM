#include "baseLevel.h"
#include "tree.h"
#include "parser.h"

//Définitions de base de l'ABNF
int DIGIT(const char* s, Node* node) {
	int toReturn = regexTest(s,"^[0-9]",1);
    if(node != NULL)
    {
		//Si le caratère est correcte
    	if(toReturn) {
			strcpy(node->name,"DIGIT");
			node->content = s; 
			node->contentSize = 1;
			node->childCount = 0;
		}
    }
    return toReturn;
}
int ALPHA(const char* s, Node* node) {
	int toReturn = regexTest(s,"^[A-Za-z]",1);
    if(node != NULL)
    {
    	if(toReturn) {
			strcpy(node->name,"ALPHA");
			node->content = s; 
			node->contentSize = 1;
			node->childCount = 0;
		}
    }
    return toReturn;
}
/*
int BIT(const char* s, Node* node) {
	int toReturn = regexTest(s,"[01]",1);
    if(node != NULL)
    {
    	if(toReturn) {node->content = s; node->contentSize = 1;}
    	else removeNode(node);
    }
    return toReturn;
}
*/
int HEXDIG(const char* s, Node* node) {
	int toReturn = regexTest(s,"^[0-9A-Fa-f]",1);
    if(node != NULL)
    {
    	if(toReturn) {
			strcpy(node->name,"HEXDIG");
			node->content = s; 
			node->contentSize = 1;
			node->childCount = 0;
		}
    }
    return toReturn;
}

int pct_encoded(const char *s,Node *node){
	int toReturn = regexTest(s,"^%[0-9A-Fa-f][0-9A-Fa-f]",3);
	if(node != NULL){
		if(toReturn){
			strcpy(node->name,"pct-encoded");
			node->content = s;
			node->contentSize = 3;
			node->childCount = 0;
		}
	}

	return toReturn;
}
/*
int CHAR_(const char* s, Node* node) {
	int toReturn = regexTest(s,"[-]",1);
    if(node != NULL)
    {
    	if(toReturn) {node->content = s; node->contentSize = 1;}
    	else removeNode(node);
    }
    return toReturn;
}
int CR(const char* s, Node* node) {
	int toReturn = regexTest(s,"\r",1);
    if(node != NULL)
    {
    	if(toReturn) {node->content = s; node->contentSize = 1;}
    	else removeNode(node);
    }
    return toReturn;
}
int LF(const char* s, Node* node) {
	int toReturn = regexTest(s,"[\n]",1);
    if(node != NULL)
    {
    	if(toReturn) {node->content = s; node->contentSize = 1;}
    	else removeNode(node);
    }
    return toReturn;
}
int DQUOTE(const char* s, Node* node) {
	int toReturn = regexTest(s,"\"",1);
    if(node != NULL)
    {
    	if(toReturn) {node->content = s; node->contentSize = 1;}
    	else removeNode(node);
    }
    return toReturn;
}
int HTAB(const char* s, Node* node) {
	int toReturn = regexTest(s,"\t",1);
    if(node != NULL)
    {
    	if(toReturn) {node->content = s; node->contentSize = 1;}
    	else removeNode(node);
    }
    return toReturn;
}
int OCTET(const char* s, Node* node) {
	int toReturn = regexTest(s,"\0-ÿ",1);
    if(node != NULL)
    {
    	if(toReturn) {node->content = s; node->contentSize = 1;}
    	else removeNode(node);
    }
    return toReturn;
}
int SP(const char* s, Node* node) {
	int toReturn = regexTest(s," ",1);
    if(node != NULL)
    {
    	if(toReturn) {node->content = s; node->contentSize = 1;}
    	else removeNode(node);
    }
    return toReturn;
}
int VCHAR(const char* s, Node* node) {
	int toReturn = regexTest(s,"[\\!-~]",1);
    if(node != NULL)
    {
    	if(toReturn) {node->content = s; node->contentSize = 1;}
    	else removeNode(node);
    }
    return toReturn;
}

int slash(const char* s, Node* node) {
	int toReturn = regexTest(s,"/",1);
    if(node != NULL)
    {
    	if(toReturn) {node->content = s; node->contentSize = 1;}
    	else removeNode(node);
    }
    return toReturn;
}

int interrogation(const char* s, Node* node) {
	int toReturn = regexTest(s,"\\?",1);
    if(node != NULL)
    {
    	if(toReturn) {node->content = s; node->contentSize = 1;}
    	else removeNode(node);
    }
    return toReturn;
}

int dot(const char* s, Node* node) {
	int toReturn = regexTest(s,"\\.",1);
    if(node != NULL)
    {
    	if(toReturn) {node->content = s; node->contentSize = 1;}
    	else removeNode(node);
    }
    return toReturn;
}

int percent(const char* s, Node* node) {
	int toReturn = regexTest(s,"%",1);
    if(node != NULL)
    {
    	if(toReturn) {node->content = s; node->contentSize = 1;}
    	else removeNode(node);
    }
    return toReturn;
}
*/

int unreserved(const char* s, Node* node) {
	int toReturn = regexTest(s,"^[A-Za-z0-9]|\\-|\\.|_|~$",1);
    if(node != NULL)
    {
    	if(toReturn) {
			strcpy(node->name,"unreserved");
			node->content = s; 
			node->contentSize = 1;
			node->childCount = 0;
		}
    }
    return toReturn;
}



int subDelims(const char* s, Node* node) {
	int toReturn = regexTest(s,"^(\\!|\\$|&|'|\\(|\\)|\\*|\\+|,|;|\\=)",1);
    if(node != NULL)
    {
    	if(toReturn) {
			strcpy(node->name,"sub-delimiter");
			node->content = s; 
			node->contentSize = 1;
			node->childCount = 0;
		}
    }

    return toReturn;
}

int colon(const char* s, Node* node) {
	int toReturn = regexTest(s,"^:",1);
    if(node != NULL)
    {
    	if(toReturn) {
			strcpy(node->name,"COLON");
			node->content = s; 
			node->contentSize = 1;
			node->childCount = 0;
		}
    }
    return toReturn;
}

int at(const char* s, Node* node) {
	int toReturn = regexTest(s,"^@",1);
    if(node != NULL)
    {
    	if(toReturn) {
			strcpy(node->name,"AT");
			node->content = s; 
			node->contentSize = 1;
			node->childCount = 0;
		}
    }
    return toReturn;
}
/*
int tchar(const char* s, Node* node) {
	int toReturn = regexTest(s,"(\\!|#|\\$|%|&|'|\\*|\\+|\\-|\\.|\\^|_|`|\\||~|[0-9A-Za-z])",1);
    if(node != NULL)
    {
    	if(toReturn) {node->content = s; node->contentSize = 1;}
    	else removeNode(node);
    }
    return toReturn;
}


int OWS(const char *s, Node* node) {

	functionArray chooseFrom;
	chooseFrom.functions[0] = &SP;
	chooseFrom.functions[1] = &HTAB;
	chooseFrom.functionCount = 2;
	chooseFrom.isOrFunction = TRUE;

	etoile(chooseFrom,s,-1,-1,node);
}

int RWS(const char *s, Node* node) {
	result RWSaux(const char *s, Node* node) {return (result){(SP(s, NULL) || HTAB(s, NULL)), 1};}
	results ret = etoile(RWSaux, s, 1, -1);
	if(node != NULL){
		if(ret.boolean == TRUE)
		{
			createChild(node, ret.number, NULL);
			for(int i = 0; i < ret.number; i++)
			{
				if(SP(s+i, NULL)){addChild(node, "SP"); SP(s+i, node->childList[0]);}
				else if(HTAB(s+i, NULL)){addChild(node, "HTAB"); HTAB(s+i, node->childList[0]);}
			}
			node->content = s;
			node->contentSize = ret.number;
		} else {
			removeNode(node);
		}
	}
	return ret.boolean;
}

int BWS(const char *s, Node* node) {return OWS(s, node);}

int CRLF(const char *s, Node* node){
	int toReturn = strlen(s) > 1 && (CR(s, node->childList[0]) && LF(s + 1, node->childList[1]));
	if(node != NULL)
	{
		if(toReturn)
		{
			createChild(node, 2, (char[][NAMESIZE]){"CR","LF"});
			CR(s, node->childList[0]);
			LF(s + 1, node->childList[1]);
		} else removeNode(node);
	}
    return toReturn;
}


int token(const char *s, Node* node){
	result tcharaux(const char *s, Node* node){return (result){tchar(s, node), 1};}
	results ret = etoile(tcharaux, s, 1, -1);
	if(ret.boolean == TRUE)
	{
		createChild(node, ret.number, NULL);
		for(int i = 0; i < ret.number; i++)
		{
			addChild(node, "tchar"); tchar(s+i, node->childList[i]);
		}
		node->content = s;
		node->contentSize = ret.number;
	} else {
		removeNode(node);
	}
	return ret.boolean;
}

int method(const char *s, Node* node) { return token(s, node); }
*/