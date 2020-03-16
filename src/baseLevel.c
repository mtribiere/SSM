#include "baseLevel.h"
#include "tree.h"
#include "parser.h"

//Définitions de base de l'ABNF
int DIGIT(const char* s, Node* node) {
	int toReturn = regexTest(s,"[0-9]",1);
    if(node != NULL)
    {
    	if(toReturn) {node->content = s; node->contentSize = 1;}
    	else removeNode(node);
    }
    return toReturn;
}
int ALPHA(const char* s, Node* node) {
	int toReturn = regexTest(s,"[A-Za-z]",1);
    if(node != NULL)
    {
    	if(toReturn) {node->content = s; node->contentSize = 1;}
    	else removeNode(node);
    }
    return toReturn;
}
int BIT(const char* s, Node* node) {
	int toReturn = regexTest(s,"[01]",1);
    if(node != NULL)
    {
    	if(toReturn) {node->content = s; node->contentSize = 1;}
    	else removeNode(node);
    }
    return toReturn;
}
int HEXDIG(const char* s, Node* node) {
	int toReturn = regexTest(s,"[0-9A-Fa-f]",1);
    if(node != NULL)
    {
    	if(toReturn) {node->content = s; node->contentSize = 1;}
    	else removeNode(node);
    }
    return toReturn;
}
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

int unreserved(const char* s, Node* node) {
	int toReturn = regexTest(s,"^[A-Za-z0-9]|\\-|\\.|_|~$",1);
    if(node != NULL)
    {
    	if(toReturn) {node->content = s; node->contentSize = 1;}
    	else removeNode(node);
    }
    return toReturn;
}
int subDelims(const char* s, Node* node) {
	int toReturn = regexTest(s,"^(\\!|\\$|&|'|\\(|\\)|\\*|\\+|,|;|\\=)",1);
    if(node != NULL)
    {
    	if(toReturn) {node->content = s; node->contentSize = 1;}
    	else removeNode(node);
    }
    return toReturn;
}

int colon(const char* s, Node* node) {
	int toReturn = regexTest(s,"^:",1);
    if(node != NULL)
    {
    	if(toReturn) {node->content = s; node->contentSize = 1;}
    	else removeNode(node);
    }
    return toReturn;
}

int at(const char* s, Node* node) {
	int toReturn = regexTest(s,"^@",1);
    if(node != NULL)
    {
    	if(toReturn) {node->content = s; node->contentSize = 1;}
    	else removeNode(node);
    }
    return toReturn;
}
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
	result OWSaux(const char *s, Node* node) {return (result){(SP(s, NULL) || HTAB(s, NULL)), 1};} //Fonction utilisée par etoile
	result ret = etoile(OWSaux, s, 0, -1); //On appelle etoile, on regarde si il y a 0 ou plus occurrences
	if(node != NULL){ //Si node vaut NULL, on renvoie juste le booléen
		if(ret.boolean == TRUE) //Si le booléen est vrai, on crée les fils
		{
			createChild(node, ret.number, NULL); //etoile renvoie le nombre d'occurences
			for(int i = 0; i < ret.number; i++)
			{
				if(SP(s+i, NULL)) {addChild(node, "SP"); SP(s+i, node->childList[0]);} //Si l'occurrence correspond à un espace, on rajoute le fils selon la fonction espace
				else if(HTAB(s+i, NULL)) {addChild(node, "HTAB"); HTAB(s+i, node->childList[0]);} //Pareil pour HTAB
			}
			node->content = s; //On règle les valeurs du noeud père.
			node->contentSize = ret.number;
		} else {
			removeNode(node); //Si le booléen est faux, on supprime le noeud
		}
	}
	return ret.boolean;
}

int RWS(const char *s, Node* node) {
	result RWSaux(const char *s, Node* node) {return (result){(SP(s, NULL) || HTAB(s, NULL)), 1};}
	result ret = etoile(RWSaux, s, 1, -1);
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
	result ret = etoile(tcharaux, s, 1, -1);
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