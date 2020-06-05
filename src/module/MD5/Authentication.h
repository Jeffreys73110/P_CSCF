/* TAKEN from rcf2617.txt */

#ifndef _DIGCALC_H_
#define _DIGCALC_H_

#include	"../../StdHeader.h"
#include "MD5.h"
#ifdef	OS_LINUX
// #include	"../../definition.h"
#ifndef BYTE
typedef unsigned char BYTE;
#endif

#ifndef _stricmp
#define _stricmp	strcasecmp
#endif

#ifndef _strnicmp
#define _strnicmp	strncasecmp
#endif

#include <string.h>
#endif

#define HASHLEN 16
typedef char HASH[HASHLEN];
#define HASHHEXLEN 32
typedef char HASHHEX[HASHHEXLEN+1];
#define IN
#define OUT

int	MD5(char *Algorithm,    char *Method,char *From_Name,char *From_URL,char *From_Password,char *Realm, char *Nonce,    char *Response);
void CvtHex( IN HASH Bin, OUT HASHHEX Hex);	// Add with Jeffreys

/* calculate H(A1) as per HTTP Digest spec */
void DigestCalcHA1(
    IN char * pszAlg,
    IN char * pszUserName,
    IN char * pszRealm,
    IN char * pszPassword,
    IN char * pszNonce,
    IN char * pszCNonce,
    OUT HASHHEX SessionKey
    );

/* calculate request-digest/response-digest as per HTTP Digest spec */
void DigestCalcResponse(
    IN HASHHEX HA1,           /* H(A1) */
    IN char * pszNonce,       /* nonce from server */
    IN char * pszNonceCount,  /* 8 hex digits */
    IN char * pszCNonce,      /* client nonce */
    IN char * pszQop,         /* qop-value: "", "auth", "auth-int" */
    IN char * pszMethod,      /* method from the request */
    IN char * pszDigestUri,   /* requested URL */
    IN HASHHEX HEntity,       /* H(entity body) if qop="auth-int" */
    OUT HASHHEX Response      /* request-digest or response-digest */
    );



#endif
