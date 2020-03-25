#include "header.h"

//headers supportés = {Referer-header, Accept-header, Content-Type-header, Cookie-header, Accept-Language-header, Expect-header}

/*
Referer-header = "Referer" ":" OWS Referer OWS 
	Referer = absolute-URI / partial-URI 
		absolute-URI = scheme ":" hier-part [ "?" query ] 
			scheme = ALPHA * ( ALPHA / DIGIT / "+" / "-" / "." ) 	
*/

int doubleSlash(const char *s, Node* node)
{
	int toReturn = regexTest(s,"^//",2);
    if(node != NULL)
    {
		strcpy(node->name,"//");
		node->content = s;
		node->contentSize = 2;
		node->childCount = 0;
    }
    return toReturn;
}

int hierPart(const char *s, Node* node){
	//Remplir le node
	strcpy(node->name,"hier-part");
	int toReturn = TRUE;

	functionArray functions;
	(functions.functions[0]) = doubleSlash; 
	(functions.functions[1]) = authority;
	(functions.functions[2]) = pathAbempty;

	functions.functionCount = 3;
	functions.isOrFunction = FALSE;

	//Executer etoile
	(node->childCount) = 0;
	(node->contentSize) = 0;
	etoile(functions,s,1,1,node);

	if(node->childCount == 0){
		(functions.functions[0]) = pathAbsolute; 
		(functions.functions[1]) = pathRootless;
		(functions.functions[2]) = pathEmpty;
		functions.isOrFunction = TRUE;

		etoile(functions,s,1,1,node);

		if(node->childCount == 0)
			toReturn = FALSE;
	}

	return toReturn;
}

int interrogationQuery(const char *s, Node* node){
	//Remplir le node
	strcpy(node->name,"/!\\ A changer");
	int toReturn = TRUE;

	functionArray functions;
	(functions.functions[0]) = interrogation; 
	(functions.functions[1]) = query;

	functions.optionnal = NULL;
	functions.functionCount = 2;
	functions.isOrFunction = FALSE;

	//Executer etoile
	(node->childCount) = 0;
	(node->contentSize) = 0;
	etoile(functions,s,1,1,node);

	if(node->childCount == 0)
		toReturn = FALSE;
	return toReturn;
}

int partialURI(const char *s, Node* node){
	//Remplir le node
	strcpy(node->name,"partial-URI");
	int toReturn = TRUE;

	functionArray functions;
	functions.optionnal = malloc(MAX_FUNCTION_NUMBER*sizeof(int));
  	memset(functions.optionnal,MAX_FUNCTION_NUMBER,MAX_FUNCTION_NUMBER*sizeof(int));
	(functions.functions[0]) = relativePart; 
	(functions.functions[1]) = interrogationQuery; functions.optionnal[0] = 1;

	functions.functionCount = 2;
	functions.isOrFunction = FALSE;

	//Executer etoile
	(node->childCount) = 0;
	(node->contentSize) = 0;
	etoile(functions,s,1,1,node);

	if(node->childCount == 0)
		toReturn = FALSE;
	return toReturn;
}

int relativePart(const char *s, Node* node){
	//Remplir le node
	strcpy(node->name,"relative-part");
	int toReturn = TRUE;

	functionArray functions;
	(functions.functions[0]) = doubleSlash; 
	(functions.functions[1]) = authority;
	(functions.functions[2]) = pathAbempty;

	functions.functionCount = 3;
	functions.isOrFunction = FALSE;

	//Executer etoile
	(node->childCount) = 0;
	(node->contentSize) = 0;
	etoile(functions,s,1,1,node);

	if(node->childCount == 0){
		(functions.functions[0]) = pathAbsolute; 
		(functions.functions[1]) = pathNoscheme;
		(functions.functions[2]) = pathEmpty;
		functions.isOrFunction = TRUE;

		etoile(functions,s,1,1,node);

		if(node->childCount == 0)
			toReturn = FALSE;
	}

	return toReturn;
}


int userinfoAt(const char *s, Node* node){
	//Remplir le node
	strcpy(node->name,"/!\\ A changer");
	int toReturn = TRUE;

	functionArray functions;
	(functions.functions[0]) = userinfo; 
	(functions.functions[1]) = at;

	functions.functionCount = 2;
	functions.isOrFunction = FALSE;

	//Executer etoile
	(node->childCount) = 0;
	(node->contentSize) = 0;
	etoile(functions,s,1,1,node);

	if(node->childCount == 0)
		toReturn = FALSE;

	return toReturn;
}

int colonPort(const char *s, Node* node){
	//Remplir le node
	strcpy(node->name,"/!\\ A changer");
	int toReturn = TRUE;

	functionArray functions;
	(functions.functions[0]) = colon; 
	(functions.functions[1]) = port;

	functions.functionCount = 2;
	functions.isOrFunction = FALSE;

	//Executer etoile
	(node->childCount) = 0;
	(node->contentSize) = 0;
	etoile(functions,s,1,1,node);

	if(node->childCount == 0)
		toReturn = FALSE;

	return toReturn;
}

int authority(const char *s, Node* node){
	//Remplir le node
	strcpy(node->name,"authority");
	int toReturn = TRUE;

	functionArray functions;
	functions.optionnal = malloc(MAX_FUNCTION_NUMBER*sizeof(int));
  	memset(functions.optionnal,MAX_FUNCTION_NUMBER,MAX_FUNCTION_NUMBER*sizeof(int));
	(functions.functions[0]) = userinfoAt; functions.optionnal[0] = 0;
	(functions.functions[1]) = host;
	(functions.functions[2]) = colonPort; functions.optionnal[1] = 2;

	functions.functionCount = 3;
	functions.isOrFunction = FALSE;

	//Executer etoile
	(node->childCount) = 0;
	(node->contentSize) = 0;
	etoile(functions,s,1,1,node);

	if(node->childCount == 0)
		toReturn = FALSE;

	return toReturn;
}

int userinfo(const char *s, Node* node){
	//Remplir le node
	strcpy(node->name,"userinfo");
	int toReturn = TRUE;

	functionArray functions;
	(functions.functions[0]) = unreserved;
	(functions.functions[1]) = pct_encoded;
	(functions.functions[2]) = subDelims;
	(functions.functions[3]) = colon;

	functions.functionCount = 4;
	functions.isOrFunction = TRUE;

	//Executer etoile
	(node->childCount) = 0;
	(node->contentSize) = 0;
	etoile(functions,s,0,-1,node);

	return toReturn;
}

int host(const char *s, Node* node){
	//Remplir le node
	strcpy(node->name,"host");
	int toReturn = TRUE;

	functionArray functions;
	(functions.functions[0]) = IPliteral;
	(functions.functions[1]) = IPv4address;
	(functions.functions[2]) = regName;

	functions.functionCount = 3;
	functions.isOrFunction = TRUE;

	//Executer etoile
	(node->childCount) = 0;
	(node->contentSize) = 0;
	etoile(functions,s,1,1,node);

	if(node->childCount == 0)
		toReturn = FALSE;
	return toReturn;
}

int IPliteral(const char *s, Node* node){
	//Remplir le node
	strcpy(node->name,"IP-literal");
	int toReturn = TRUE;

	functionArray functions;
	(functions.functions[0]) = openBracket;
	(functions.functions[1]) = IPv6address;
	(functions.functions[2]) = closeBracket;

	functions.optionnal = NULL;
	functions.functionCount = 3;
	functions.isOrFunction = FALSE;

	//Executer etoile
	(node->childCount) = 0;
	(node->contentSize) = 0;
	etoile(functions,s,1,1,node);

	if(node->childCount == 0)
	{
		(functions.functions[1]) = IPvFuture;

		//Executer etoile
		etoile(functions,s,1,1,node);
	}

	//Si le node n'a de fils, déclarer la node fausse
	if(node->childCount == 0)
		toReturn = FALSE;

	return toReturn;
}

void checkls32(functionArray functions, const char *s, Node* node)
{
	if(node->childCount > 0){
		int backChildCount = node->childCount;
		(functions.functions[0]) = ls32;
		functions.functionCount = 1;
		functions.isOrFunction = TRUE;
		etoile(functions,s,1,1,node); //ls32 

		if(node->childCount - backChildCount != 1)
		{
			(node->childCount) = 0;
			(node->contentSize) = 0;
		}
			
	}
}

int colonh16(const char *s, Node* node){
	//Remplir le node
	strcpy(node->name,"/!\\ A changer");
	int toReturn = TRUE;

	functionArray functions;
	(functions.functions[0]) = colon;
	(functions.functions[1]) = h16;

	functions.optionnal = NULL;
	functions.functionCount = 2;
	functions.isOrFunction = FALSE;

	//Executer etoile
	(node->childCount) = 0;
	(node->contentSize) = 0;
	etoile(functions,s,1,1,node);

	//Si le node n'a de fils, déclarer la node fausse
	if(node->childCount == 0)
		toReturn = FALSE;

	return toReturn;
}

int doubleColon(const char *s, Node* node){
	int toReturn = regexTest(s,"^\\:\\:",2);
    if(node != NULL)
    {
		strcpy(node->name,"::");
		node->content = s;
		node->contentSize = 2;
		node->childCount = 0;
    }
    return toReturn;
}

int IPv6address(const char *s, Node* node){
	//Remplir le node
	strcpy(node->name,"IPv6address");
	int toReturn = TRUE;
	int backChildCount;

	functionArray functions;
	(functions.functions[0]) = h16;
	(functions.functions[1]) = colon;	

	functions.optionnal = NULL;
	functions.functionCount = 2;
	functions.isOrFunction = FALSE;

	//Executer etoile
	(node->childCount) = 0;
	(node->contentSize) = 0;
	etoile(functions,s,6,6,node); //6 ( h16 ":" )

	if(node->childCount == 0){
		(functions.functions[0]) = doubleColon;
		functions.functionCount = 1;
		functions.isOrFunction = TRUE;
		etoile(functions,s,1,1,node); //"::"

		if(node->childCount > 0){
			(functions.functions[0]) = h16;
			(functions.functions[1]) = colon;
			functions.functionCount = 2;
			functions.isOrFunction = FALSE;
			backChildCount = node->childCount;
			etoile(functions,s,5,5,node); //5 ( h16 ":" )

			if(node->childCount - backChildCount == 0)
			{
				(node->childCount) = 0;
				(node->contentSize) = 0;
			}
			checkls32(functions, s, node);
		}
	}
	if(node->childCount == 0){
		functions.optionnal = malloc(MAX_FUNCTION_NUMBER*sizeof(int));
  		memset(functions.optionnal,MAX_FUNCTION_NUMBER,MAX_FUNCTION_NUMBER*sizeof(int));
  		(functions.functions[0]) = h16; functions.optionnal[0] = 0;
		(functions.functions[1]) = doubleColon;
		functions.functionCount = 2;
		functions.isOrFunction = FALSE;
		etoile(functions,s,1,1,node); //[ h16 ] "::"

		if(node->childCount > 0){
			if(functions.optionnal != NULL) {free(functions.optionnal); functions.optionnal = NULL;}
			(functions.functions[0]) = h16;
			(functions.functions[1]) = colon;
			functions.functionCount = 2;
			functions.isOrFunction = FALSE;
			backChildCount = node->childCount;
			etoile(functions,s,4,4,node); //4 ( h16 ":" )

			if(node->childCount - backChildCount == 0)
			{
				(node->childCount) = 0;
				(node->contentSize) = 0;
			}
			checkls32(functions, s, node);
		}
	}
	if(node->childCount == 0){
		if(functions.optionnal != NULL) {free(functions.optionnal); functions.optionnal = NULL;}
		functions.optionnal = malloc(MAX_FUNCTION_NUMBER*sizeof(int));
  		memset(functions.optionnal,MAX_FUNCTION_NUMBER,MAX_FUNCTION_NUMBER*sizeof(int));
  		(functions.functions[0]) = h16; functions.optionnal[0] = 0;
  		(functions.functions[1]) = colonh16; functions.optionnal[1] = 1;
		(functions.functions[2]) = doubleColon;
		functions.functionCount = 3;
		functions.isOrFunction = FALSE;
		etoile(functions,s,1,1,node); //[ h16 *1 ( ":" h16 ) ] "::"

		if(node->childCount > 0){
			if(functions.optionnal != NULL) {free(functions.optionnal); functions.optionnal = NULL;}
			(functions.functions[0]) = h16;
			(functions.functions[1]) = colon;
			functions.functionCount = 2;
			functions.isOrFunction = FALSE;
			backChildCount = node->childCount;
			etoile(functions,s,3,3,node); //3 ( h16 ":" )

			if(node->childCount - backChildCount == 0)
			{
				(node->childCount) = 0;
				(node->contentSize) = 0;
			}
			checkls32(functions, s, node);
		}
	}
	if(node->childCount == 0){
		if(functions.optionnal != NULL) {free(functions.optionnal); functions.optionnal = NULL;}
		functions.optionnal = malloc(MAX_FUNCTION_NUMBER*sizeof(int));
  		memset(functions.optionnal,MAX_FUNCTION_NUMBER,MAX_FUNCTION_NUMBER*sizeof(int));
  		(functions.functions[0]) = h16; functions.optionnal[0] = 0;
  		(functions.functions[1]) = colonh16; functions.optionnal[1] = 1;
  		(functions.functions[2]) = colonh16; functions.optionnal[2] = 2;
		(functions.functions[3]) = doubleColon;
		functions.functionCount = 4;
		functions.isOrFunction = FALSE;
		etoile(functions,s,1,1,node); //[ h16 *2 ( ":" h16 ) ] "::"

		if(node->childCount > 0){
			if(functions.optionnal != NULL) {free(functions.optionnal); functions.optionnal = NULL;}
			(functions.functions[0]) = h16;
			(functions.functions[1]) = colon;
			functions.functionCount = 2;
			functions.isOrFunction = FALSE;
			backChildCount = node->childCount;
			etoile(functions,s,2,2,node); //2 ( h16 ":" )

			if(node->childCount - backChildCount == 0)
			{
				(node->childCount) = 0;
				(node->contentSize) = 0;
			}
			checkls32(functions, s, node);
		}
	}
	if(node->childCount == 0){
		if(functions.optionnal != NULL) {free(functions.optionnal); functions.optionnal = NULL;}
		functions.optionnal = malloc(MAX_FUNCTION_NUMBER*sizeof(int));
  		memset(functions.optionnal,MAX_FUNCTION_NUMBER,MAX_FUNCTION_NUMBER*sizeof(int));
  		(functions.functions[0]) = h16; functions.optionnal[0] = 0;
  		(functions.functions[1]) = colonh16; functions.optionnal[1] = 1;
  		(functions.functions[2]) = colonh16; functions.optionnal[2] = 2;
  		(functions.functions[3]) = colonh16; functions.optionnal[3] = 3;
		(functions.functions[4]) = doubleColon;
		functions.functionCount = 5;
		functions.isOrFunction = FALSE;
		etoile(functions,s,1,1,node); //[ h16 *3 ( ":" h16 ) ] "::"

		if(node->childCount > 0){
			if(functions.optionnal != NULL) {free(functions.optionnal); functions.optionnal = NULL;}
			(functions.functions[0]) = h16;
			(functions.functions[1]) = colon;
			functions.functionCount = 2;
			functions.isOrFunction = FALSE;
			backChildCount = node->childCount;
			etoile(functions,s,1,1,node); // h16 ":"

			if(node->childCount - backChildCount == 0)
			{
				(node->childCount) = 0;
				(node->contentSize) = 0;
			}
			checkls32(functions, s, node);
		}
	}
	if(node->childCount == 0){
		if(functions.optionnal != NULL) {free(functions.optionnal); functions.optionnal = NULL;}
		functions.optionnal = malloc(MAX_FUNCTION_NUMBER*sizeof(int));
  		memset(functions.optionnal,MAX_FUNCTION_NUMBER,MAX_FUNCTION_NUMBER*sizeof(int));
  		(functions.functions[0]) = h16; functions.optionnal[0] = 0;
  		(functions.functions[1]) = colonh16; functions.optionnal[1] = 1;
  		(functions.functions[2]) = colonh16; functions.optionnal[2] = 2;
  		(functions.functions[3]) = colonh16; functions.optionnal[3] = 3;
  		(functions.functions[4]) = colonh16; functions.optionnal[4] = 4;
		(functions.functions[5]) = doubleColon;
		functions.functionCount = 6;
		functions.isOrFunction = FALSE;
		backChildCount = node->childCount;
		etoile(functions,s,1,1,node); //[ h16 *4 ( ":" h16 ) ] "::"

		if(node->childCount - backChildCount == 0)
			{
				(node->childCount) = 0;
				(node->contentSize) = 0;
			}
		checkls32(functions, s, node);
	}
	if(node->childCount == 0){
		if(functions.optionnal != NULL) {free(functions.optionnal); functions.optionnal = NULL;}
		functions.optionnal = malloc(MAX_FUNCTION_NUMBER*sizeof(int));
  		memset(functions.optionnal,MAX_FUNCTION_NUMBER,MAX_FUNCTION_NUMBER*sizeof(int));
  		(functions.functions[0]) = h16; functions.optionnal[0] = 0;
  		(functions.functions[1]) = colonh16; functions.optionnal[1] = 1;
  		(functions.functions[2]) = colonh16; functions.optionnal[2] = 2;
  		(functions.functions[3]) = colonh16; functions.optionnal[3] = 3;
  		(functions.functions[4]) = colonh16; functions.optionnal[4] = 4;
  		(functions.functions[5]) = colonh16; functions.optionnal[5] = 5;
		(functions.functions[6]) = doubleColon;
		functions.functionCount = 7;
		functions.isOrFunction = FALSE;
		etoile(functions,s,1,1,node); //[ h16 *5 ( ":" h16 ) ] "::"

		if(node->childCount == 0){
			(node->childCount) = 0;
			(node->contentSize) = 0;
		}
		if(node->childCount > 0){
			int backChildCount = node->childCount;
			(functions.functions[0]) = h16;
			functions.functionCount = 1;
			functions.isOrFunction = TRUE;
			etoile(functions,s,1,1,node); //h16 

			if(node->childCount - backChildCount != 1)
			{
				(node->childCount) = 0;
				(node->contentSize) = 0;
			}	
		}
	}
	if(node->childCount == 0){
		if(functions.optionnal != NULL) {free(functions.optionnal); functions.optionnal = NULL;}
		functions.optionnal = malloc(MAX_FUNCTION_NUMBER*sizeof(int));
  		memset(functions.optionnal,MAX_FUNCTION_NUMBER,MAX_FUNCTION_NUMBER*sizeof(int));
  		(functions.functions[0]) = h16; functions.optionnal[0] = 0;
  		(functions.functions[1]) = colonh16; functions.optionnal[1] = 1;
  		(functions.functions[2]) = colonh16; functions.optionnal[2] = 2;
  		(functions.functions[3]) = colonh16; functions.optionnal[3] = 3;
  		(functions.functions[4]) = colonh16; functions.optionnal[4] = 4;
  		(functions.functions[5]) = colonh16; functions.optionnal[5] = 5;
  		(functions.functions[6]) = colonh16; functions.optionnal[6] = 6;
		(functions.functions[7]) = doubleColon;
		functions.functionCount = 8;
		functions.isOrFunction = FALSE;
		etoile(functions,s,1,1,node); //[ h16 *6 ( ":" h16 ) ] "::"

		if(node->childCount == 0){
			(node->childCount) = 0;
			(node->contentSize) = 0;
		}
	}
	if(node->childCount == 0)
		toReturn = FALSE;
	return toReturn;
}

int ls32(const char *s, Node* node){
	//Remplir le node
	strcpy(node->name,"ls32");
	int toReturn = TRUE;

	functionArray functions;
	(functions.functions[0]) = h16;
	(functions.functions[1]) = colon;
	(functions.functions[2]) = h16;

	functions.optionnal = NULL;
	functions.functionCount = 3;
	functions.isOrFunction = FALSE;

	//Executer etoile
	(node->childCount) = 0;
	(node->contentSize) = 0;
	etoile(functions,s,1,1,node);

	if(node->childCount == 0){
		(functions.functions[0]) = IPv4address;
		functions.functionCount = 1;
		functions.isOrFunction = TRUE;

		etoile(functions,s,1,1,node);

		if(node->childCount == 0)
			toReturn = FALSE;
	}

	return toReturn;
}

int h16(const char *s, Node* node){
	//Remplir le node
	strcpy(node->name,"h16");
	int toReturn = TRUE;

	functionArray functions;
	(functions.functions[0]) = HEXDIG;

	functions.functionCount = 1;
	functions.isOrFunction = TRUE;

	//Executer etoile
	(node->childCount) = 0;
	(node->contentSize) = 0;
	etoile(functions,s,1,4,node);

	if(node->childCount == 0)
		toReturn = FALSE;

	return toReturn;
}

int IPv4address(const char *s, Node* node){
	//Remplir le node
	strcpy(node->name,"IPv4address");
	int toReturn = TRUE;

	functionArray functions;
	(functions.functions[0]) = decOctet;
	(functions.functions[1]) = dot;
	(functions.functions[2]) = decOctet;
	(functions.functions[3]) = dot;
	(functions.functions[4]) = decOctet;
	(functions.functions[5]) = dot;
	(functions.functions[6]) = decOctet;

	functions.optionnal = NULL;
	functions.functionCount = 7;
	functions.isOrFunction = FALSE;

	//Executer etoile
	(node->childCount) = 0;
	(node->contentSize) = 0;
	etoile(functions,s,1,1,node);

	if(node->childCount == 0)
		toReturn = FALSE;

	return toReturn;
}

int twentyFive(const char *s, Node* node){
	int toReturn = regexTest(s,"^25",2);
    if(node != NULL)
    {
		strcpy(node->name,"25");
		node->content = s;
		node->contentSize = 2;
		node->childCount = 0;
    }
    return toReturn;
}

int one(const char *s, Node* node){
	int toReturn = regexTest(s,"^1",1);
    if(node != NULL)
    {
		strcpy(node->name,"1");
		node->content = s;
		node->contentSize = 1;
		node->childCount = 0;
    }
    return toReturn;
}

int two(const char *s, Node* node){
	int toReturn = regexTest(s,"^2",1);
    if(node != NULL)
    {
		strcpy(node->name,"2");
		node->content = s;
		node->contentSize = 1;
		node->childCount = 0;
    }
    return toReturn;
}

int DIGITtoFive(const char* s, Node* node) {
	int toReturn = regexTest(s,"^[0-5]",1);
    if(node != NULL)
    {
		strcpy(node->name,"%x30-35");
		node->content = s;
		node->contentSize = 1;
		node->childCount = 0;
    }
    return toReturn;
}

int DIGITtoFour(const char* s, Node* node) {
	int toReturn = regexTest(s,"^[0-4]",1);
    if(node != NULL)
    {
		strcpy(node->name,"%x30-34");
		node->content = s;
		node->contentSize = 1;
		node->childCount = 0;
    }
    return toReturn;
}

int DIGITwithoutZero(const char* s, Node* node) {
	int toReturn = regexTest(s,"^[1-9]",1);
    if(node != NULL)
    {
		strcpy(node->name,"%x31-39");
		node->content = s;
		node->contentSize = 1;
		node->childCount = 0;
    }
    return toReturn;
}

int decOctet(const char *s, Node* node){
	//Remplir le node
	strcpy(node->name,"dec-octet");
	int toReturn = TRUE;

	functionArray functions;
	(functions.functions[0]) = twentyFive;
	(functions.functions[1]) = DIGITtoFive;

	functions.optionnal = NULL;
	functions.functionCount = 2;
	functions.isOrFunction = FALSE;

	//Executer etoile
	(node->childCount) = 0;
	(node->contentSize) = 0;
	etoile(functions,s,1,1,node);

	if(node->childCount == 0){
		(functions.functions[0]) = two;
		(functions.functions[1]) = DIGITtoFour;
		(functions.functions[2]) = DIGIT;

	functions.functionCount = 3;
		etoile(functions,s,1,1,node);

		if(node->childCount == 0){
			(functions.functions[0]) = one;
			(functions.functions[1]) = DIGIT;
			(functions.functions[2]) = DIGIT;

			etoile(functions,s,1,1,node);

			if(node->childCount == 0){
				(functions.functions[0]) = DIGITwithoutZero;
				(functions.functions[1]) = DIGIT;

				functions.functionCount = 2;
				etoile(functions,s,1,1,node);
				if(node->childCount == 0){
					(functions.functions[0]) = DIGIT;

					functions.functionCount = 1;
					etoile(functions,s,1,1,node);
					if(node->childCount == 0)
						toReturn = FALSE;
				}
			}
		}
	}

	return toReturn;
}

int v(const char *s, Node* node)
{
	int toReturn = regexTest(s,"^v",1);
    if(node != NULL)
    {
		strcpy(node->name,"v");
		node->content = s;
		node->contentSize = 1;
		node->childCount = 0;
    }
    return toReturn;
}

int IPvFuture(const char *s, Node* node){
	//Remplir le node
	strcpy(node->name,"IPvFuture");
	int toReturn = FALSE;

	functionArray functions;
	(functions.functions[0]) = v;

	functions.functionCount = 1;
	functions.isOrFunction = TRUE;

	//Executer etoile
	(node->childCount) = 0;
	(node->contentSize) = 0;
	etoile(functions,s,1,1,node);

	if(node->childCount > 0)
	{
		int backChildCount = node->childCount;
		(functions.functions[0]) = HEXDIG;
		etoile(functions,s,1,-1,node);

		if(node->childCount - backChildCount > 0)
		{
			backChildCount = node->childCount;
			(functions.functions[0]) = dot;
			etoile(functions,s,1,1,node);

			if(node->childCount - backChildCount > 0)
			{
				backChildCount = node->childCount;
				(functions.functions[0]) = unreserved;
				(functions.functions[1]) = subDelims;
				(functions.functions[2]) = colon;

				functions.functionCount = 3;
				etoile(functions,s,1,-1,node);

				if(node->childCount - backChildCount > 0)
					toReturn = TRUE;
			}
		}
	}
	return toReturn;
}

int regName(const char *s, Node* node){
	//Remplir le node
	strcpy(node->name,"reg-name");
	int toReturn = TRUE;

	functionArray functions;
	(functions.functions[0]) = unreserved;
	(functions.functions[1]) = pct_encoded;
	(functions.functions[2]) = subDelims;

	functions.functionCount = 3;
	functions.isOrFunction = TRUE;

	//Executer etoile
	(node->childCount) = 0;
	(node->contentSize) = 0;
	etoile(functions,s,0,-1,node);

	return toReturn;
}

int port(const char *s, Node* node){
	//Remplir le node
	strcpy(node->name,"port");
	int toReturn = TRUE;

	functionArray functions;
	(functions.functions[0]) = DIGIT;

	functions.functionCount = 1;
	functions.isOrFunction = TRUE;

	//Executer etoile
	(node->childCount) = 0;
	(node->contentSize) = 0;
	etoile(functions,s,0,-1,node);

	return toReturn;
}

int pathAbempty(const char *s, Node* node){
	//Remplir le node
	strcpy(node->name,"path-abempty");
	int toReturn = TRUE;

	functionArray functions;
	functions.optionnal = NULL;
	(functions.functions[0]) = slash;
	(functions.functions[1]) = segment;

	functions.functionCount = 2;
	functions.isOrFunction = FALSE;

	//Executer etoile
	(node->childCount) = 0;
	(node->contentSize) = 0;
	etoile(functions,s,0,-1,node);

	return toReturn;
}

int pathAbsolute(const char *s, Node* node){
	//Remplir le node
	strcpy(node->name,"path-absolute");
	int toReturn = TRUE;

	functionArray functions;
	functions.optionnal = malloc(MAX_FUNCTION_NUMBER*sizeof(int));
  	memset(functions.optionnal,MAX_FUNCTION_NUMBER,MAX_FUNCTION_NUMBER*sizeof(int));
	(functions.functions[0]) = slash;
	(functions.functions[1]) = pathRootless; functions.optionnal[0] = 1;

	functions.functionCount = 2;
	functions.isOrFunction = FALSE;

	//Executer etoile
	(node->childCount) = 0;
	(node->contentSize) = 0;
	etoile(functions,s,1,1,node);

	if(node->childCount == 0)
		toReturn = FALSE;

	return toReturn;
}

int pathRootless(const char *s, Node* node){
	//Remplir le node
	strcpy(node->name,"path-rootless");
	int toReturn = FALSE;

	functionArray functions;
	(functions.functions[0]) = segmentNz;

	functions.functionCount = 1;
	functions.isOrFunction = TRUE;

	//Executer etoile
	(node->childCount) = 0;
	(node->contentSize) = 0;
	etoile(functions,s,1,1,node);

	if(node->childCount > 0)
	{
		functions.optionnal = NULL;
		(functions.functions[0]) = slash;
		(functions.functions[1]) = segment;

		functions.functionCount = 2;
		functions.isOrFunction = FALSE;

		//Executer etoile
		etoile(functions,s,0,-1,node);
		
			//Si le node n'a de fils, déclarer la node fausse
		if(node->childCount > 0)
			toReturn = TRUE;
	}

	return toReturn;
}

int pathEmpty(const char *s, Node* node){
	//Remplir le node
	strcpy(node->name,"path-empty");
	
	node->childCount = 0;
	node->contentSize = 0;

	return TRUE;
}

int pathNoscheme(const char *s, Node* node){
	//Remplir le node
	strcpy(node->name,"path-noscheme");
	int toReturn = FALSE;

	functionArray functions;
	(functions.functions[0]) = segmentNzNc;

	functions.functionCount = 1;
	functions.isOrFunction = TRUE;

	//Executer etoile
	(node->childCount) = 0;
	(node->contentSize) = 0;
	etoile(functions,s,1,1,node);

	if(node->childCount > 0)
	{
		functions.optionnal = NULL;
		(functions.functions[0]) = slash;
		(functions.functions[1]) = segment;

		functions.functionCount = 2;
		functions.isOrFunction = FALSE;

		//Executer etoile
		etoile(functions,s,0,-1,node);
		
			//Si le node n'a de fils, déclarer la node fausse
		if(node->childCount > 0)
			toReturn = TRUE;
	}

	return toReturn;
}

int segmentNz(const char *s, Node* node){
	//Remplir le node
	strcpy(node->name,"segment-nz");
	int toReturn = TRUE;

	functionArray functions;
	(functions.functions[0]) = pchar;

	functions.functionCount = 1;
	functions.isOrFunction = TRUE;

	//Executer etoile
	(node->childCount) = 0;
	(node->contentSize) = 0;
	etoile(functions,s,1,-1,node);

	//Si le node n'a de fils, déclarer la node fausse
	if(node->childCount == 0)
		toReturn = FALSE;

	return toReturn;
}

int segmentNzNc(const char *s, Node* node){
	//Remplir le node
	strcpy(node->name,"segment-nz-nc");
	int toReturn = TRUE;

	functionArray functions;
	(functions.functions[0]) = unreserved;
	(functions.functions[1]) = pct_encoded;
	(functions.functions[2]) = subDelims;
	(functions.functions[3]) = at;

	functions.functionCount = 4;
	functions.isOrFunction = TRUE;

	//Executer etoile
	(node->childCount) = 0;
	(node->contentSize) = 0;
	etoile(functions,s,1,-1,node);

	//Si le node n'a de fils, déclarer la node fausse
	if(node->childCount == 0)
		toReturn = FALSE;

	return toReturn;
}


//ACCEPT HEADER

int acceptHeader(const char *s, Node* node){
	//Remplir le node
	strcpy(node->name,"Accept-header");
	int toReturn = TRUE;

	functionArray functions;
	(functions.functions[0]) = acceptHeaderName;
	(functions.functions[1]) = colon;
	(functions.functions[2]) = OWS;
	(functions.functions[3]) = accept;
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

int acceptHeaderName(const char *s, Node* node)
{
	int toReturn = regexTestInsensitive(s,"^Accept",6);
    if(node != NULL)
    {
		strcpy(node->name,"Accept");
		node->content = s; 
		node->contentSize = 6;
		node->childCount = 0;
    }
    return toReturn;
}

//Problème de def ? En réalité, il n'y a jamais d'acceptParams car parameter de media-range prend le dessus
// int mediaRangeAcceptParams(const char *s, Node* node){
// 	//Remplir le node
// 	strcpy(node->name,"/!\\ A changer");
// 	int toReturn = TRUE;

// 	functionArray functions;
// 	functions.optionnal = malloc(MAX_FUNCTION_NUMBER*sizeof(int));
//   	memset(functions.optionnal,MAX_FUNCTION_NUMBER,MAX_FUNCTION_NUMBER*sizeof(int));
// 	(functions.functions[0]) = mediaRange;
// 	(functions.functions[1]) = acceptParams; functions.optionnal[0] = 1;

// 	functions.functionCount = 2;
// 	functions.isOrFunction = FALSE;

// 	//Executer etoile
// 	(node->childCount) = 0;
// 	(node->contentSize) = 0;
// 	etoile(functions,s,1,1,node);

// 	if(node->childCount == 0)
// 		toReturn = FALSE;

// 	return toReturn;
// }

int accept(const char *s, Node* node){
	//Remplir le node
	strcpy(node->name,"Accept");
	int toReturn = TRUE;

	functionArray functions;
	(functions.functions[0]) = coma;
	(functions.functions[1]) = mediaRange/*AcceptParams*/;
	
	functions.functionCount = 2;
	functions.isOrFunction = TRUE;

	//Executer etoile
	(node->childCount) = 0;
	(node->contentSize) = 0;
	etoile(functions,s,1,1,node);

	if(node->childCount > 0)
	{
		functions.optionnal = malloc(MAX_FUNCTION_NUMBER*sizeof(int));
  		memset(functions.optionnal,MAX_FUNCTION_NUMBER,MAX_FUNCTION_NUMBER*sizeof(int));
		(functions.functions[0]) = OWS;
		(functions.functions[1]) = coma;
		(functions.functions[2]) = OWS; functions.optionnal[0] = 2;
		(functions.functions[3]) = mediaRange/*AcceptParams*/; functions.optionnal[1] = 3;
		
		functions.functionCount = 4;
		functions.isOrFunction = FALSE;

		//Executer etoile
		etoile(functions,s,0,-1,node);
	}
	//Toujours vrai car tout est optionnel
	return toReturn;
}

//media-range = ( "*SLASH*" / ( type "/" subtype ) / ( type "SLASH*" ) ) * ( OWS ";" OWS parameter ) 
//media-range = ( type "/" subtype ) * ( OWS ";" OWS parameter ) 			
int mediaRange(const char *s, Node* node){
	//Remplir le node
	strcpy(node->name,"media-range");
	int toReturn = TRUE;

	functionArray functions;
	(functions.functions[0]) = type;
	(functions.functions[1]) = slash;
	(functions.functions[2]) = subtype;

	functions.functionCount = 3;
	functions.isOrFunction = FALSE;

	//Executer etoile
	(node->childCount) = 0;
	(node->contentSize) = 0;
	etoile(functions,s,1,1,node);

	if(node->childCount > 0)
	{
		(functions.functions[0]) = OWS;
		(functions.functions[1]) = semiColon;
		(functions.functions[2]) = OWS;
		(functions.functions[3]) = parameter;

		functions.functionCount = 4;
		functions.isOrFunction = FALSE;
		functions.optionnal = NULL;
		etoile(functions,s,0,-1,node);
	}
	//Si le node n'a de fils, déclarer la node fausse
	if(node->childCount == 0)
		toReturn = FALSE;

	return toReturn;	
}

int acceptParams(const char *s, Node* node){
	//Remplir le node
	strcpy(node->name,"accept-params");
	int toReturn = TRUE;

	functionArray functions;
	(functions.functions[0]) = weight;

	functions.functionCount = 1;
	functions.isOrFunction = TRUE;

	//Executer etoile
	(node->childCount) = 0;
	(node->contentSize) = 0;
	etoile(functions,s,1,1,node);

	if(node->childCount > 0)
	{
		(functions.functions[0]) = acceptExt;
		etoile(functions,s,0,-1,node);
	}
	//Si le node n'a de fils, déclarer la node fausse
	if(node->childCount == 0)
		toReturn = FALSE;

	return toReturn;	
}

int equalTokenOrQuotedString(const char *s, Node* node){
	//Remplir le node
	strcpy(node->name,"/!\\ A changer");
	int toReturn = FALSE;

	functionArray functions;
	(functions.functions[0]) = equal;
	functions.functionCount = 1;
	functions.isOrFunction = TRUE;

	(node->childCount) = 0;
	(node->contentSize) = 0;
	etoile(functions, s, 1, 1, node);

	if(node->childCount >= 1)
	{
		
		(functions.functions[0]) = token;
		(functions.functions[1]) = quotedString;
		functions.functionCount = 2;
		
		//Executer etoile
		// (node->childCount) = 0;
		// (node->contentSize) = 0;
		etoile(functions,s,1,1,node);

		if(node->childCount >= 2)
		//Si le node a au moins un fils, déclarer la node vraie
			toReturn = TRUE;
	}
	return toReturn;
}

int acceptExt(const char *s, Node* node){
	//Remplir le node
	strcpy(node->name,"accept-ext");
	int toReturn = TRUE;

	functionArray functions;
	functions.optionnal = malloc(MAX_FUNCTION_NUMBER*sizeof(int));
  	memset(functions.optionnal,MAX_FUNCTION_NUMBER,MAX_FUNCTION_NUMBER*sizeof(int));
	(functions.functions[0]) = OWS;
	(functions.functions[1]) = semiColon;
	(functions.functions[2]) = OWS;
	(functions.functions[3]) = token;
	(functions.functions[4]) = equalTokenOrQuotedString; functions.optionnal[0] = 4;

	functions.functionCount = 5;
	functions.isOrFunction = FALSE;

	//Executer etoile
	(node->childCount) = 0;
	(node->contentSize) = 0;
	etoile(functions,s,1,1,node);

	//Si le node n'a de fils, déclarer la node fausse
	if(node->childCount == 0)
		toReturn = FALSE;

	return toReturn;
}

//ACCEPT-LANGUAGE HEADER

int cookieHeader(const char *s, Node* node){
	//Remplir le node
	strcpy(node->name,"Cookie-header");
	int toReturn = TRUE;

	functionArray functions;
	(functions.functions[0]) = cookieHeaderName;
	(functions.functions[1]) = colon;
	(functions.functions[2]) = OWS;
	(functions.functions[3]) = cookieString;
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

int cookieHeaderName(const char *s, Node* node)
{
	int toReturn = regexTestInsensitive(s,"^Cookie",6);
    if(node != NULL)
    {
		strcpy(node->name,"Cookie");
		node->content = s; 
		node->contentSize = 6;
		node->childCount = 0;
    }
    return toReturn;
}

int cookieString(const char *s, Node* node)
{
	strcpy(node->name,"cookie-string");
	int toReturn = TRUE;

	functionArray functions;
	(functions.functions[0]) = cookiePair;

	functions.functionCount = 1;
	functions.isOrFunction = TRUE;

	//Executer etoile
	(node->childCount) = 0;
	(node->contentSize) = 0;
	etoile(functions,s,1,1,node);

	//Si le node n'a de fils, déclarer la node fausse
	if(node->childCount == 0)
		toReturn = FALSE;

	else
	{
		int backChildCount = (node->childCount);
		int backContentSize = (node->contentSize);

		(functions.functions[0]) = semiColon;
		(functions.functions[1]) = SP;
		(functions.functions[2]) = cookiePair;

		functions.functionCount = 3;
		functions.isOrFunction = FALSE;
		functions.optionnal = NULL;

		etoile(functions,s,0,-1,node);

		if(node->childCount - backChildCount <= 0)
		{
			(node->childCount) = backChildCount;
			(node->contentSize) = backContentSize;
		}
	}

	return toReturn;
}

int cookiePair(const char *s, Node* node)
{
	//Remplir le node
	strcpy(node->name,"cookie-pair");
	int toReturn = TRUE;

	functionArray functions;
	(functions.functions[0]) = cookieName;
	(functions.functions[1]) = equal;
	(functions.functions[2]) = cookieValue;

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

	return toReturn;
}

int cookieName(const char *s, Node* node)
{
    int toReturn = token(s, node);
    strcpy(node->name,"cookie-name");
    return toReturn;
}

int cookieValue(const char *s, Node* node){
//Remplir le node
	strcpy(node->name,"cookie-value");
	int toReturn = TRUE;

	functionArray functionsQuote;
	(functionsQuote.functions[0]) = DQUOTE;

	functionsQuote.functionCount = 1;
	functionsQuote.isOrFunction = TRUE;

	node->childCount = 0;
	node->contentSize = 0;
	etoile(functionsQuote, s,1, 1, node);

	if(node->childCount == 0)
		toReturn = FALSE;

	functionArray functions;
	(functions.functions[0]) = cookieOctet;

	functions.functionCount = 1;
	functions.isOrFunction = TRUE;

	if(toReturn == TRUE)
	{
		//Executer etoile
		etoile(functions,s,0,-1,node);


		int backChildCount = (node->childCount);
		etoile(functionsQuote, s, 1,1, node);

		if(node->childCount - backChildCount == 0)
			toReturn = FALSE;
	} 
	if(toReturn == FALSE)
	{
		node->childCount = 0;
		node->contentSize = 0;
		etoile(functions,s,0,-1,node);

		if(node->childCount >= 0)
			toReturn = TRUE;
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

int hastagToPlus(const char *s, Node* node)
{
	int toReturn = regexTest(s,"^[#-']",1) || regexTest(s,"^\\(",1) | regexTest(s,"^\\)",1) || regexTest(s,"^\\*",1) || regexTest(s,"^\\+",1);
    if(node != NULL)
    {
        strcpy(node->name,"%x23-2B");
        node->content = s; 
        node->contentSize = 1;
        node->childCount = 0;
    }
    return toReturn;
}

int minusToColon(const char *s, Node* node)
{
	int toReturn = regexTest(s,"^[.-9]",1) || regexTest(s,"^\\-",1) || regexTest(s,"^\\:",1);
    if(node != NULL)
    {
        strcpy(node->name," %x2D-3A");
        node->content = s; 
        node->contentSize = 1;
        node->childCount = 0;
    }
    return toReturn;
}

int lesserToLeftBracket(const char *s, Node* node)
{
	int toReturn = regexTest(s,"^[<-Z]",1) || regexTest(s, "^\\[", 1);
    if(node != NULL)
    {
        strcpy(node->name," %x3C-5B");
        node->content = s; 
        node->contentSize = 1;
        node->childCount = 0;
    }
    return toReturn;
}

int cookieOctet(const char *s, Node* node){
	//Remplir le node
	strcpy(node->name,"cookie-octet");
	int toReturn = TRUE;

	functionArray functions;
	(functions.functions[0]) = exclamation;
	(functions.functions[1]) = hastagToPlus;
	(functions.functions[2]) = minusToColon;
	(functions.functions[3]) = lesserToLeftBracket;
	(functions.functions[4]) = rightBracketToTilde;

	functions.functionCount = 5;
	functions.isOrFunction = TRUE;

	//Executer etoile
	(node->childCount) = 0;
	(node->contentSize) = 0;
	etoile(functions,s,1,1,node);

	//Si le node n'a de fils, déclarer la node fausse
	if(node->childCount == 0)
		toReturn = FALSE;

	return toReturn;
}

//ACCEPT-LANGUAGE HEADER

int acceptLanguageHeader(const char *s, Node* node){
	//Remplir le node
	strcpy(node->name,"Accept-Language-header");
	int toReturn = TRUE;

	functionArray functions;
	(functions.functions[0]) = acceptLanguageHeaderName;
	(functions.functions[1]) = colon;
	(functions.functions[2]) = OWS;
	(functions.functions[3]) = acceptLanguage;
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

int acceptLanguageHeaderName(const char *s, Node* node)
{
	int toReturn = regexTestInsensitive(s,"^Accept-Language",15);
    if(node != NULL)
    {
		strcpy(node->name,"Accept-Language");
		node->content = s; 
		node->contentSize = 15;
		node->childCount = 0;
    }
    return toReturn;
}


int languageWeight(const char *s, Node* node){
	//Remplir le node
	strcpy(node->name,"A changer /!\\");
 	int toReturn = TRUE;
 	int backChildCount = (node->childCount);
 	int backContentSize = (node->contentSize);

  	functionArray functions;
  	functions.optionnal = malloc(MAX_FUNCTION_NUMBER*sizeof(int));
  	memset(functions.optionnal,MAX_FUNCTION_NUMBER,MAX_FUNCTION_NUMBER*sizeof(int));
  	(functions.functions[0]) = languageRange;
  	(functions.functions[1]) = weight; functions.optionnal[0] = 1;

  	functions.functionCount = 2;
  	functions.isOrFunction = FALSE;

  	//Executer etoile
  	node->childCount = 0;
  	node->contentSize = 0;
  	etoile(functions,s,1,1,node);

  	if(node->childCount - backChildCount == 0)
  		toReturn = FALSE;	
  	if(!toReturn)
  	{
  		(node->childCount) = backChildCount;
 		(node->contentSize) = backContentSize;
  	}

  	return toReturn;
}

int acceptLanguage(const char *s, Node* node){
 	//Remplir le node
  	strcpy(node->name,"Accept-Language");
  	int toReturn = TRUE;

  	functionArray functions;
  	(functions.functions[0]) = coma;
  	(functions.functions[1]) = OWS;

  	functions.functionCount = 2;
  	functions.isOrFunction = FALSE;
  	functions.optionnal = NULL;

  	//Executer etoile
  	(node->childCount) = 0;
  	(node->contentSize) = 0;
  	etoile(functions,s,0,-1,node);

  	toReturn = languageWeight(s, node);

  	if(toReturn){

  		functions.optionnal = malloc(MAX_FUNCTION_NUMBER*sizeof(int));
  		memset(functions.optionnal,MAX_FUNCTION_NUMBER,MAX_FUNCTION_NUMBER*sizeof(int));
  		(functions.functions[0]) = OWS;
  		(functions.functions[1]) = coma;
  		(functions.functions[2]) = OWS; functions.optionnal[0] = 2;
  		(functions.functions[3]) = languageWeight; functions.optionnal[1] = 3;

  		functions.functionCount = 4;
  		functions.isOrFunction = FALSE;
		
  		etoile(functions, s, 0, -1, node);
  	}
  	return toReturn;
}

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
	node->contentSize = 0;
	etoile(chooseFrom,s,1,1,node);

	//Si le node n'est pas de fils
	if(node->childCount == 0)
		toReturn = FALSE;

	//S'il est utile de verifier la prochaine partie
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