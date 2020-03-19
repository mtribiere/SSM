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
		media-range = ( "*SLASH*" / ( type "/" subtype ) / ( type "/*" ) ) * ( OWS ";" OWS parameter ) 
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

/*COMMUN A PLUSIEURS HEADERS:

type = token 
subtype = token
parameter = token "=" ( token / quoted-string )
	quoted-string = DQUOTE * ( qdtext / quoted-pair ) DQUOTE
		qdtext = HTAB / SP / "!" / %x23-5B / %x5D-7E / obs-text
		quoted-pair = "\" ( HTAB / SP / VCHAR / obs-text ) 
			obs-text = %x80-FF 