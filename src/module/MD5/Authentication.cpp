
#include "Authentication.h"
#ifdef	OS_WINDOWS
#include "MD5.cpp"
#endif
#define AUTH_INT_SUPPORT



int	MD5(char *Algorithm,    char *Method,char *From_Name,char *From_URL,char *From_Password,char *Realm, char *Nonce,    char *Response)
{	
    char    *pszNonceCount = NULL;
    char    *pszQop = NULL;
	char    *pszCNonce=NULL;
    HASHHEX HA1 = "";
    HASHHEX HA2 = "";
    HASHHEX Response1;

    DigestCalcHA1(Algorithm, From_Name, Realm, From_Password, Nonce, pszCNonce, HA1);

    DigestCalcResponse(HA1, Nonce, pszNonceCount, pszCNonce, pszQop, Method, From_URL, HA2, Response1);
	strcpy(Response,Response1);

	return 0;
}


void CvtHex( IN HASH Bin, OUT HASHHEX Hex)
{
	unsigned short i;
	unsigned char j;
  
	for (i = 0; i < HASHLEN; i++) 
	{	j = (Bin[i] >> 4) & 0xf;
		if (j <= 9)
			Hex[i*2] = (j + '0');
		 else
			Hex[i*2] = (j + 'a' - 10);
		j = Bin[i] & 0xf;
		if (j <= 9)
			Hex[i*2+1] = (j + '0');
		else
			Hex[i*2+1] = (j + 'a' - 10);
	}
	Hex[HASHHEXLEN] = '\0';
};

/* calculate H(A1) as per spec */
void DigestCalcHA1(IN char * pszAlg, IN char * pszUserName, IN char * pszRealm, IN char * pszPassword, IN char * pszNonce, IN char * pszCNonce, OUT HASHHEX SessionKey)
{
	MD5_CTX Md5Ctx;
	HASH HA1;
  
	MD5Init(&Md5Ctx);
	MD5Update(&Md5Ctx, (BYTE *)pszUserName, strlen(pszUserName));
	MD5Update(&Md5Ctx, (BYTE *)":", 1);
	MD5Update(&Md5Ctx, (BYTE *)pszRealm, strlen(pszRealm));
	MD5Update(&Md5Ctx, (BYTE *)":", 1);
	MD5Update(&Md5Ctx, (BYTE *)pszPassword, strlen(pszPassword));
	MD5Final((BYTE *)HA1, &Md5Ctx);
	if ((pszAlg!=NULL)&&_stricmp(pszAlg, "md5-sess") == 0)
	{	MD5Init(&Md5Ctx);
		MD5Update(&Md5Ctx, (BYTE *)HA1, HASHLEN);
		MD5Update(&Md5Ctx, (BYTE *)":", 1);
		MD5Update(&Md5Ctx, (BYTE *)pszNonce, strlen(pszNonce));
		MD5Update(&Md5Ctx, (BYTE *)":", 1);
		MD5Update(&Md5Ctx, (BYTE *)pszCNonce, strlen(pszCNonce));
		MD5Final((BYTE *)HA1, &Md5Ctx);
	}
	CvtHex(HA1, SessionKey);
}

/* calculate request-digest/response-digest as per HTTP Digest spec */
void DigestCalcResponse(
			IN HASHHEX HA1,         /* H(A1) */
			IN char * pszNonce,     /* nonce from server */
			IN char * pszNonceCount,  /* 8 hex digits */
			IN char * pszCNonce,    /* client nonce */
			IN char * pszQop,       /* qop-value: "", "auth", "auth-int" */
			IN char * pszMethod,    /* method from the request */
			IN char * pszDigestUri, /* requested URL */
			IN HASHHEX HEntity,     /* H(entity body) if qop="auth-int" */
			OUT HASHHEX Response    /* request-digest or response-digest */
			)
{
	MD5_CTX Md5Ctx;
	HASH HA2;
	HASH RespHash;
	HASHHEX HA2Hex;
  
	// calculate H(A2)
	MD5Init(&Md5Ctx);
	MD5Update(&Md5Ctx, (BYTE *)pszMethod, strlen(pszMethod));
	MD5Update(&Md5Ctx, (BYTE *)":", 1);
	MD5Update(&Md5Ctx, (BYTE *)pszDigestUri, strlen(pszDigestUri));
 
	if (pszQop!=NULL)
    {	char *index = strchr(pszQop,'i');

#ifdef AUTH_INT_SUPPORT                   
		while (index!=NULL&&index-pszQop>=5&&strlen(index)>=3)
		{	if (_strnicmp(index-5, "auth-int",8) == 0)
			{	goto auth_withqop;	}
			index = strchr(index+1,'i');
		}

		//strchr(pszQop,'a');
		while (index!=NULL&&strlen(index)>=4)
		{
			if (_strnicmp(index-5, "auth",4) == 0)
			{
				goto auth_withqop;
			}
			index = strchr(index+1,'a');
		}
#endif
      goto auth_withoutqop;

	}
  
auth_withoutqop:
	MD5Final((BYTE *)HA2, &Md5Ctx);
	CvtHex(HA2, HA2Hex);

	// calculate response
	MD5Init(&Md5Ctx);
	MD5Update(&Md5Ctx, (BYTE *)HA1, HASHHEXLEN);
	MD5Update(&Md5Ctx, (BYTE *)":", 1);
	MD5Update(&Md5Ctx, (BYTE *)pszNonce, strlen(pszNonce));
	MD5Update(&Md5Ctx, (BYTE *)":", 1);

	goto end;

auth_withqop:
	MD5Update(&Md5Ctx, (BYTE *)":", 1);
	MD5Update(&Md5Ctx, (BYTE *)HEntity, HASHHEXLEN);
	MD5Final((BYTE *)HA2, &Md5Ctx);
	CvtHex(HA2, HA2Hex);

	// calculate response
	MD5Init(&Md5Ctx);
	MD5Update(&Md5Ctx, (BYTE *)HA1, HASHHEXLEN);
	MD5Update(&Md5Ctx, (BYTE *)":", 1);
	MD5Update(&Md5Ctx, (BYTE *)pszNonce, strlen(pszNonce));
	MD5Update(&Md5Ctx, (BYTE *)":", 1);
    MD5Update(&Md5Ctx, (BYTE *)pszNonceCount, strlen(pszNonceCount));
    MD5Update(&Md5Ctx, (BYTE *)":", 1);
    MD5Update(&Md5Ctx, (BYTE *)pszCNonce, strlen(pszCNonce));
    MD5Update(&Md5Ctx, (BYTE *)":", 1);
    MD5Update(&Md5Ctx, (BYTE *)pszQop, strlen(pszQop));
    MD5Update(&Md5Ctx, (BYTE *)":", 1);

end:
	MD5Update(&Md5Ctx, (BYTE *)HA2Hex, HASHHEXLEN);
	MD5Final((BYTE *)RespHash, &Md5Ctx);
	CvtHex(RespHash, Response);
};
