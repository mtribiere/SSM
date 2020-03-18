#include "highLevel.h"
#include "baseLevel.h"
#include "parser.h"
#include "tree.h"

headers headerSupportes[6] = {{"^Referer-header$",14}, {"^Accept-header$",13}, {"^Content-Type-header$",19}, {"^Cookie-header$",13}, {"^Accept-Language-header$",22}, {"^Expect-header$",13}};

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
			node->contentSize = toReturn.number;
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
result absolutePath(const char *s, Node* node)
{
	result absolutePathaux(const char *s, Node* node) {
		if(strlen(s) > 1) return (result){(slash(s, NULL) && segment(s+1, NULL).boolean), 1 + segment(s+1, NULL).number};
		else if(strlen(s) == 1) return (result){slash(s, NULL), 1};
		else return (result){FALSE, 0};
	}
	results ret = etoile(absolutePathaux, s, 1, -1);
	if(strlen(s) > 1) ret.number *= 2 ; //etoile repère le motif ("/" segment), pour la fonction on devra créer 2 fois plus de fils
	if(node != NULL){
		if(ret.boolean == TRUE)
		{
			int j = 0;
			createChild(node, ret.number, NULL);
			for(int i = 0; i < ret.number; i++)
			{
				if(slash(s+j, NULL)) {addChild(node, "/"); slash(s+j, node->childList[i]);}
				else if(segment(s+j, NULL).boolean) {addChild(node, "segment"); segment(s+j, node->childList[i]);}
				j += node->childList[i]->contentSize;
			}
			node->content = s;
			node->contentSize = ret.size;
		} else {
			removeNode(node);
		}
	}
	return (result){ret.boolean, ret.size};
}

//query = * ( pchar / "/" / "?" )
result query(const char *s, Node* node) {
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
	return (result){ret.boolean, ret.size};
}

//field-name = token
result fieldName(const char *s, Node* node) {
	return token(s, node);
}

//FAUX - A MODIFIER
result fieldValue(const char *s, Node* node) {
	return token(s, node);
}

//header-field =  Connection-header / Content-Length-header / Content-Type-header / Cookie-header / Transfer-Encoding-header / Expect-header / Host-header / Accept-header / Accept-Charset-header / Accept-Encoding-header / Accept-Language-header / Referer-header / User-Agent-header / ( field-name ":" OWS field-value OWS )
int headerField(const char *s, Node* node) {
	int toReturn = FALSE;
	int supporte = FALSE;
	result toFieldName = fieldName(s, NULL);
	int col = colon(s+toFieldName.number, NULL);
	int ows1 = 0; while (RWS(s+toFieldName.number+1+ows1, NULL)) ows1++;
	result toFieldValue = fieldValue(s+toFieldName.number+1+ows1, NULL);
	int ows2 = 0; while (RWS(s+toFieldName.number+1+ows1+toFieldValue.number, NULL)) ows2++;
	for (int i = 0; i < (sizeof(headerSupportes)/sizeof(headers)) ; i++) {
		if (regexTestInsensitive(s, headerSupportes[i].name, headerSupportes[i].size)) {
			supporte = TRUE;
			break;
		}
	}
	if (supporte && col && toFieldValue.boolean && toFieldValue.number > 1)
	{
		createChild(node, 2, NULL);
		addChild(node, "field-name"); fieldName(s, node->childList[0]);
		addChild(node, "field-content"); fieldValue(s+toFieldName.number+1+ows1, node->childList[1]);

		toReturn = TRUE;

		// A continuer...
	}

	return toReturn;
}

//origin-form = absolute-path [ "?" query ]
result originForm(const char *s, Node* node){
	result toReturn = absolutePath(s, NULL);
	if(strlen(s+toReturn.number) > 1 && interrogation(s+toReturn.number, NULL) && query(s+toReturn.number+1, NULL).boolean) toReturn.number += 1 + query(s+toReturn.number+1, NULL).number;
	if(node != NULL){
		if(toReturn.boolean){
			createChild(node, 3, NULL);
			addChild(node, "absolute-path"); absolutePath(s, node->childList[0]);
			int debutOpt = node->childList[0]->contentSize;
			if(interrogation(s+debutOpt, NULL) && query(s+debutOpt+1, NULL).boolean)
			{
				addChild(node, "?"); interrogation(s + debutOpt, node->childList[1]);
				addChild(node, "query"); query(s + debutOpt+1, node->childList[2]);
			}
			node->content = s;
			int size = 0;
			for(int i = 0; i < node->childCount; i++ ) size += node->childList[i]->contentSize;
	 	    node->contentSize = size;
		} else removeNode(node);
	}
	return toReturn;
}

//request-line = method SP origin-form SP HTTP-version CRLF
int requestLine(const char *s, Node* node){
	result ret1 = method(s, NULL);
 	int ret2    = SP(s+ret1.number, NULL);
 	result ret3 = originForm(s+ ret1.number+1, NULL);
 	int ret4    = SP(s + ret1.number + ret3.number + 1, NULL);
 	int ret5    = HTTPVersion(s+ ret1.number + ret3.number + 2, NULL);
 	int ret6    = CRLF(s+ ret1.number+ret3.number + 10, NULL);
 	int toReturn = ret1.boolean && ret2 && ret3.boolean && ret4 && ret5 && ret6;
 	if(node != NULL){
 		if(toReturn){
 			createChild(node, 6, (char[][NAMESIZE]){"method","SP","origin-form","SP","HTTP-version","CRLF"});
 			method(s, node->childList[0]);
 			SP(s+ret1.number, node->childList[1]);
 			originForm(s+ret1.number+1, node->childList[2]);
 			SP(s+ret1.number+1+ret3.number, node->childList[3]);
 			HTTPVersion(s+ret1.number+1+ret3.number+1, node->childList[4]);
 			CRLF(s+ret1.number+1+ret3.number+1+8, node->childList[5]);
 			node->content = s;
 	        int size = 0;
 			for(int i = 0; i < node->childCount; i++ ) size += node->childList[i]->contentSize;
 	 	    node->contentSize = size;
 		} else removeNode(node);
     }
     return toReturn;
}
