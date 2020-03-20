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
*/

int alphanum(const char *s, Node* node){
//Remplir le node
	strcpy(node->name,"alphanum");
	int toReturn = TRUE;

	functionArray functions;
	(functions.functions[0]) = ALPHA;
	(functions.functions[1]) = DIGIT;

	functions.functionCount = 2;
	functions.isOrFunction = TRUE;

	//Executer etoile
	(node->childCount) = 0;
	(node->contentSize) = 0;
	etoile(functions,s,1,1,node);

	if(node->childCount == 0)
		toReturn = FALSE;	

	return toReturn;
}

int qEquals(const char *s, Node* node){
    int toReturn = regexTest(s,"^q=",2);
    if(node != NULL)
    {
        strcpy(node->name,"q=");
        node->content = s; 
        node->contentSize = 2;
        node->childCount = 0;
    }
    return toReturn;
}

int weight(const char *s, Node* node){
	//Remplir le node
	strcpy(node->name,"weight");
	int toReturn = TRUE;

	functionArray functions;
	(functions.functions[0]) = OWS;
	(functions.functions[1]) = semiColon;
	(functions.functions[2]) = OWS; 
	(functions.functions[3]) = qEquals; 
	(functions.functions[4]) = qvalue; 

	functions.functionCount = 5;
	functions.isOrFunction = FALSE;
	functions.optionnal = NULL;

	//Executer etoile
	(node->childCount) = 0;
	(node->contentSize) = 0;
	etoile(functions,s,1,1,node);

	if(node->childCount == 0)
		toReturn = FALSE;	

	return toReturn;
}

int qvalue(const char *s, Node* node){
	//Remplir le node
	strcpy(node->name,"qvalue");
	int toReturn = TRUE;

	functionArray functions;
	functions.optionnal = malloc(MAX_FUNCTION_NUMBER*sizeof(int));
	memset(functions.optionnal,MAX_FUNCTION_NUMBER,MAX_FUNCTION_NUMBER*sizeof(int));

	(functions.functions[0]) = ZERO;
	(functions.functions[1]) = dot; functions.optionnal[0] = 1;
	(functions.functions[2]) = DIGIT; functions.optionnal[1] = 2;
	(functions.functions[3]) = DIGIT; functions.optionnal[2] = 3;
	(functions.functions[4]) = DIGIT; functions.optionnal[3] = 4;

	functions.functionCount = 5;
	functions.isOrFunction = FALSE;

	//Executer etoile
	(node->childCount) = 0;
	(node->contentSize) = 0;
	etoile(functions,s,1,1,node);

	if(node->childCount == 0){
		//On test l'autre partie du OU
		free(functions.optionnal);

		functions.optionnal = malloc(MAX_FUNCTION_NUMBER*sizeof(int));
		memset(functions.optionnal,MAX_FUNCTION_NUMBER,MAX_FUNCTION_NUMBER*sizeof(int));

		(functions.functions[0]) = ONE;
		(functions.functions[1]) = dot; functions.optionnal[0] = 1;
		(functions.functions[2]) = ZERO; functions.optionnal[1] = 2;
		(functions.functions[3]) = ZERO; functions.optionnal[2] = 3;
		(functions.functions[4]) = ZERO; functions.optionnal[3] = 4;

		functions.functionCount = 5;
		functions.isOrFunction = FALSE;

		//Executer etoile
		(node->childCount) = 0;
		(node->contentSize) = 0;
		etoile(functions,s,1,1,node);

		if(node->childCount == 0)
			toReturn = FALSE;
	}

	return toReturn;
}

//ACCEPT-LANGUAGE HEADER
int languageRange(const char *s,Node *node){

	//Remplir le node
	strcpy(node->name,"language-range");
	int toReturn = TRUE ;

	//PARTIE 0
	functionArray chooseFrom;
	
	(chooseFrom.functions)[0] = &star;

	(chooseFrom.functionCount) = 1;
	(chooseFrom.isOrFunction) = TRUE;
	(chooseFrom.optionnal) = NULL;

	//Executer etoile
	etoile(chooseFrom,s,1,1,node);

	//Si le node n'est pas de fils
	if(node->childCount == 0)
		toReturn = FALSE;

	//S'il est utile de verifier la prochiane partie
	if(toReturn == FALSE){
		toReturn = TRUE;

		//PARTIE 1
		(chooseFrom.functions)[0] = &ALPHA;

		(chooseFrom.functionCount) = 1;
		(chooseFrom.isOrFunction) = TRUE;
		(chooseFrom.optionnal) = NULL;

		//Executer etoile
		(node->childCount) = 0;
		(node->contentSize) = 0;
		etoile(chooseFrom,s,1,8,node);

		//Si le node n'a pas de fils
		if(node->childCount == 0)
			toReturn = FALSE;
		

		//PARTIE 2
		if(toReturn == TRUE)
			while(languageRangePart2(s,node) == TRUE);
		
	}

	return toReturn;

}

int languageRangePart2(const char *s,Node *node){
	
	int toReturn = TRUE;
	int backChildCount = (node->childCount);
	int backContentSize = (node->contentSize);

	////Si le premier caractère est un tiret
	functionArray chooseFrom;
	(chooseFrom.functions)[0] = &tiret;

	(chooseFrom.functionCount) = 1;
	(chooseFrom.isOrFunction) = TRUE;
	(chooseFrom.optionnal) = NULL;

	//Executer etoile
	etoile(chooseFrom,s,1,1,node);

	//Si le node n'as pas de fils en plus
	if((node->childCount) - backChildCount == 0)
		toReturn = FALSE;

	///////Si la suite est une suite de alphanum
	if(toReturn == TRUE){

		(chooseFrom.functions)[0] = &alphanum;

		(chooseFrom.functionCount) = 1;

		//Executer la fonction étoile
		etoile(chooseFrom,s,1,8,node);

		//Si le node n'a pas de fils en plus
		if((node->childCount) - (backChildCount + 1) == 0)
			toReturn = FALSE;

	}

	//Si la node est fausse
	if(toReturn == FALSE){
		(node->childCount) = backChildCount; //Restaurer le nombre de fils
		(node->contentSize) = backContentSize; //Restaurer le content size
	}

	return toReturn;
}


//CONTENT-TYPE HEADER

int contentTypeHeader(const char *s, Node* node){
	//Remplir le node
	strcpy(node->name,"Content-type-header");
	int toReturn = TRUE;

	functionArray functions;
	(functions.functions[0]) = contentTypeHeaderName;
	(functions.functions[1]) = colon;
	(functions.functions[2]) = OWS;
	(functions.functions[3]) = mediaType;
	(functions.functions[4]) = OWS;

	functions.functionCount = 5;
	functions.isOrFunction = FALSE;
	functions.optionnal = NULL;

	//Executer etoile
	(node->childCount) = 0;
	(node->contentSize) = 0;
	etoile(functions,s,1,1,node);

	//Si le node n'a de fils, déclarer la node fausse
	if(node->childCount == 0)
		toReturn = FALSE;

	return toReturn;
}

int contentTypeHeaderName(const char *s, Node* node)
{
	int toReturn = regexTestInsensitive(s,"^Content-Type",12);
    if(node != NULL)
    {
		strcpy(node->name,"Content-Type");
		node->content = s; 
		node->contentSize = 12;
		node->childCount = 0;
    }
    return toReturn;
}

int mediaType(const char *s, Node* node){
	//Remplir le node
	strcpy(node->name,"media-type");
	int toReturn = TRUE;

	functionArray functions;
	(functions.functions[0]) = type;
	(functions.functions[1]) = slash;
	(functions.functions[2]) = subtype;

	functions.functionCount = 3;
	functions.isOrFunction = FALSE;
	functions.optionnal = NULL;

	//Executer etoile
	(node->childCount) = 0;
	(node->contentSize) = 0;
	etoile(functions,s,1,1,node);

	//Si le node n'a de fils, déclarer la node fausse
	if(node->childCount == 0)
		toReturn = FALSE;

	//Si le node n'est pas déjà faux
	if(toReturn == TRUE){
		//Declarer l'ensemble des fonctions possibles
		(functions.functions)[0] = OWS;
		(functions.functions)[1] = semiColon;
		(functions.functions)[2] = OWS;
		(functions.functions)[3] = parameter;

		functions.functionCount = 4;
		functions.isOrFunction = FALSE;
		functions.optionnal = NULL;

		//Creer le(s) fils
		etoile(functions,s,0,-1,node);

		//Si etoile ne trouve pas de fils
		if((node->childCount) - 3 <= 0)
			toReturn = FALSE;
	}
	return toReturn;
}

//EXPECT-HEADER

int expectHeader(const char *s, Node* node)
{
	strcpy(node->name,"Expect-header");
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

//FONCTIONS PLUS GENERALES EN DESSOUS

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
	int toReturn = regexTest(s,"^[#-Z]",1) || regexTest(s,"^\\[",1);
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
	int toReturn = regexTest(s,"^[_-~]",1) || regexTest(s,"^\\]",1) || regexTest(s,"^\\^",1);
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


int quotedString(const char *s, Node* node){
	//Remplir le node
	strcpy(node->name,"quoted-string");
	int toReturn = TRUE;

	functionArray functions;
	(functions.functions[0]) = DQUOTE;
	functions.functionCount = 1;
	functions.isOrFunction = TRUE;
	//Executer etoile
	(node->childCount) = 0;
	(node->contentSize) = 0;
	etoile(functions,s,1,1,node);

	//Si le node n'a de fils, déclarer la node fausse
	if(node->childCount == 0)
		toReturn = FALSE;

	//Si le node n'est pas déjà faux
	if(toReturn == TRUE){
		//Declarer l'ensemble des fonctions possibles
		(functions.functions)[0] = qdtext;
		(functions.functions)[1] = quotedPair;

		functions.functionCount = 2;
		functions.isOrFunction = TRUE;

		//Creer le(s) fils
		etoile(functions,s,0,-1,node);

		//Si etoile ne trouve pas de fils
		if((node->childCount) == 0)
			toReturn = FALSE;

		if(toReturn == TRUE){
			//Declarer l'ensemble des fonctions possibles
			(functions.functions)[0] = DQUOTE;

			functions.functionCount = 1;
			functions.isOrFunction = TRUE;

			etoile(functions,s,1,1,node);

			if((node->childCount) == 0)
				toReturn = FALSE;
		}
	}
	return toReturn;
}

int quotedPair(const char *s,Node *node){
	
	//Remplir le node
	strcpy(node->name,"quoted-pair");
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
		//Sauvegarder l'ancien nombre de child
		/*int backChildCount = node->childCount;*/

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
		if((node->childCount) -1 <= 0)
			toReturn = FALSE;
	}

	return toReturn;
	
}

int parameter(const char *s,Node *node){
	
	//Remplir le node
	strcpy(node->name,"parameter");
	int toReturn = TRUE;

	///////Si premier caractère est un backslash
	//Declarer la fonction
	functionArray functions;
	(functions.functions)[0] = token;
	(functions.functions)[1] = equal;

	functions.functionCount = 2;
	functions.isOrFunction = FALSE;
	functions.optionnal = NULL;

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
		//Sauvegarder l'ancien nombre de child

		//Declarer l'ensemble des fonctions possibles
		(functions.functions)[0] = token;
		(functions.functions)[1] = quotedString;



		functions.functionCount = 2;
		functions.isOrFunction = TRUE;

		//Creer le(s) fils
		etoile(functions,s,1,1,node);

		//Si etoile ne trouve pas de fils
		if((node->childCount) - 2 <= 0)
			toReturn = FALSE;
	}

	return toReturn;
	
}