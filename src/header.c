#include "header.h"

//headers supportés = {Referer-header, Accept-header, Content-Type-header, Cookie-header, Accept-Language-header, Expect-header

/*
Referer-header = "Referer" ":" OWS Referer OWS 
	Referer = absolute-URI / partial-URI 
		absolute-URI = scheme ":" hier-part [ "?" query ] 
			scheme = ALPHA * ( ALPHA / DIGIT / "+" / "-" / "." ) 	
			hier-part = "//" authority path-abempty / path-absolute / path-rootless / path-empty 
		partial-URI = relative-part [ "?" query ] 
			relative-part = "//" authority path-abempty / path-absolute / path-noscheme / path-empty
				authority = [ userinfo "@" ] host [ ":" port ] 
					userinfo = * ( unreserved / pct-encoded / sub-delims / ":" ) 
					host = IP-literal / IPv4address / reg-name
						IPv4address = dec-octet "." dec-octet "." dec-octet "." dec-octet
							dec-octet = "25" %x30-35 / "2" %x30-34 DIGIT / "1" 2 DIGIT / %x31-39 DIGIT / DIGIT
						IP-literal = "[" ( IPv6address / IPvFuture ) "]" 
							IPv6address = 6 ( h16 ":" ) ls32 / "::" 5 ( h16 ":" ) ls32 / [ h16 ] "::" 4 ( h16 ":" ) ls32 / [ h16 *1 ( ":" h16 ) ] "::" 3 ( h16 ":" ) ls32 / [ h16 *2 ( ":" h16 ) ] "::" 2 ( h16 ":" ) ls32 / [ h16 *3 ( ":" h16 ) ] "::" h16 ":" ls32 / [ h16 *4 ( ":" h16 ) ] "::" ls32 / [ h16 *5 ( ":" h16 ) ] "::" h16 / [ h16 *6 ( ":" h16 ) ] "::" 
								h16 = 1*4 HEXDIG
								ls32 = ( h16 ":" h16 ) / IPv4address	  
							IPvFuture = "v" 1* HEXDIG "." 1* ( unreserved / sub-delims / ":" ) 
						reg-name = * ( unreserved / pct-encoded / sub-delims ) 
					port = * DIGIT
			path-abempty = * ( "/" segment ) 
			path-absolute = "/" [ segment-nz * ( "/" segment ) ]
			path-rootless = segment-nz * ( "/" segment ) 
			path-empty = "" 
			path-noscheme = segment-nz-nc * ( "/" segment ) 
				segment-nz = 1* pchar 
				segment-nz-nc = 1* ( unreserved / pct-encoded / sub-delims / "@" )
*/

/*
Accept-header = "Accept" ":" OWS Accept OWS 
	Accept = [ ( "," / ( media-range [ accept-params ] ) ) * ( OWS "," [ OWS ( media-range [ accept-params ] ) ] ) ] 
		media-range = ( "*SLASH*" / ( type "/" subtype ) / ( type "SLASH*" ) ) * ( OWS ";" OWS parameter ) 
		accept-params = weight * accept-ext 	 
			accept-ext = OWS ";" OWS token [ "=" ( token / quoted-string ) ] 
*/

/*
Content-Type-header = "Content-Type" ":" OWS media-type OWS
	media-type = type "/" subtype * ( OWS ";" OWS parameter )
*/

/*
Cookie-header = "Cookie:" OWS cookie-string OWS 
	cookie-string = cookie-pair * ( ";" SP cookie-pair ) 
		cookie-pair = cookie-name "=" cookie-value 
			cookie-name = token
			cookie-value = ( DQUOTE * cookie-octet DQUOTE ) / * cookie-octet 
				cookie-octet = %x21 / %x23-2B / %x2D-3A / %x3C-5B / %x5D-7E 
*/

/*
Accept-Language-header = "Accept-Language" ":" OWS Accept-Language OWS 
	Accept-Language = * ( "," OWS ) ( language-range [ weight ] ) * ( OWS "," [ OWS ( language-range [ weight ] ) ] ) 
		language-range = ( 1*8 ALPHA * ( "-" 1*8 alphanum ) ) / "*" 
			alphanum = ALPHA / DIGIT 
		weight = OWS ";" OWS "q=" qvalue 
			qvalue = ( "0" [ "." *3 DIGIT ] ) / ( "1" [ "." *3 "0" ] )
*/

/*
Expect-header = "Expect" ":" OWS Expect OWS 
	Expect = "100-continue"
*/

int expectHeader(const char *s, Node* node)
{
	strcpy(node->name,"qdtext");
    int toReturn = TRUE;

    functionArray chooseFrom;
    chooseFrom.optionnal = NULL;
    chooseFrom.functions[0] = expectHeaderName;
    chooseFrom.functions[1] = colon;
    chooseFrom.functions[2] = OWS;
    chooseFrom.functions[3] = expect;
    chooseFrom.functions[4] = OWS;
    chooseFrom.functionCount = 5;
    chooseFrom.isOrFunction = FALSE;

	(node->contentSize) = 0;
	(node->childCount) = 0;
    etoile(chooseFrom,s,1,1,node);

    if(node->childCount == 0)
        toReturn = FALSE;
    return toReturn;
}

int expectHeaderName(const char *s, Node* node)
{
	int toReturn = regexTestInsensitive(s,"^Expect",6);
    if(node != NULL)
    {
		strcpy(node->name,"Expect");
		node->content = s; 
		node->contentSize = 6;
		node->childCount = 0;
    }
    return toReturn;
}
int expect(const char *s, Node* node)
{
	int toReturn = regexTest(s,"^100-continue",12);
    if(node != NULL)
    {
		strcpy(node->name,"expect");
		node->content = s; 
		node->contentSize = 12;
		node->childCount = 0;
    }
    return toReturn;
}

int type(const char *s, Node* node)
{
    int toReturn = token(s, node);
    strcpy(node->name,"type");
    return toReturn;
}

int subtype(const char *s, Node* node)
{
    int toReturn = token(s, node);
    strcpy(node->name,"subtype");
    return toReturn;
}

int obsText(const char *s, Node* node)
{
	int toReturn = regexTest(s,"^[€-ÿ]",1);
    if(node != NULL)
    {
        strcpy(node->name,"obs-text");
        node->content = s; 
        node->contentSize = 1;
        node->childCount = 0;
    }
    return toReturn;
}

int hastagToLeftBracket(const char *s, Node* node)
{
	int toReturn = regexTest(s,"^[#-\\[]",1);
    if(node != NULL)
    {
        strcpy(node->name,"%x23-5B");
        node->content = s; 
        node->contentSize = 1;
        node->childCount = 0;
    }
    return toReturn;
}
int rightBracketToTilde(const char *s, Node* node)
{
	int toReturn = regexTest(s,"^[\\]-~]",1);
    if(node != NULL)
    {
        strcpy(node->name,"%x5D-7E");
        node->content = s; 
        node->contentSize = 1;
        node->childCount = 0;
    }
    return toReturn;
}

int qdtext(const char *s, Node* node)
{
	strcpy(node->name,"qdtext");
    int toReturn = TRUE;

    functionArray chooseFrom;
    chooseFrom.functions[0] = HTAB;
    chooseFrom.functions[1] = SP;
    chooseFrom.functions[2] = exclamation;
    chooseFrom.functions[3] = hastagToLeftBracket;
    chooseFrom.functions[4] = rightBracketToTilde;
    chooseFrom.functions[5] = obsText;
    chooseFrom.functionCount = 6;
    chooseFrom.isOrFunction = TRUE;

	(node->contentSize) = 0;
	(node->childCount) = 0;
    etoile(chooseFrom,s,1,1,node);

    if(node->childCount == 0)
        toReturn = FALSE;
    return toReturn;
}

/*COMMUN A PLUSIEURS HEADERS:

parameter = token "=" ( token / quoted-string )
	quoted-string = DQUOTE * ( qdtext / quoted-pair ) DQUOTE
		quoted-pair = "\" ( HTAB / SP / VCHAR / obs-text ) 
*/


int quotedPair(const char *s,Node *node){
	
	//Remplir le node
	strcpy(node->name,"quotedPair");
	int toReturn = TRUE;

	///////Si premier caractère est un backslash
	//Declarer la fonction
	functionArray functions;
	(functions.functions)[0] = &backSlash;

	functions.functionCount = 1;
	functions.isOrFunction = TRUE;

	//Executer etoile
	(node->childCount) = 0;
	(node->contentSize) = 0;
	etoile(functions,s,1,1,node);

	//Si le node n'a de fils
	if(node->childCount == 0)
		//Declarer la node fausse
		toReturn = FALSE;


	//Si le node n'est pas déjà faux
	if(toReturn == TRUE){
		//Sauvegerder l'ancien nombre de child
		int backChildCount = node->childCount;

		//Declarer l'ensemble des fonctions possibles
		(functions.functions)[0] = &HTAB;
		(functions.functions)[1] = &SP;
		(functions.functions)[2] = &VCHAR;
		(functions.functions)[3] = &obsText;


		functions.functionCount = 4;
		functions.isOrFunction = TRUE;

		//Creer le(s) fils
		(node->childCount) = 1;
		etoile(functions,s,1,1,node);

		//Si etoile ne trouve pas de fils
		if((node->childCount) - backChildCount == 0)
			toReturn = FALSE;

	}

	return toReturn;
	
}