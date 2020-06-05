#include "Functions.h"

bool replace_all_newline(std::string &str, const std::string &from, const std::string &to, int search_posi)
{
	size_t start_pos = str.find(from, search_posi);
	if (start_pos == std::string::npos)
		return false;
	str.replace(start_pos, from.length(), to);
	if (replace_all_newline(str, from, to, start_pos + to.length()) == false)
		return true;
	return true;
}

size_t Jstrlen(const char *string)
{
	size_t len;
	if (!string)
		return 0;
	for (len = 0; string[len] != '\0'; len++)
		;
	return len;
}

int Jstrnlen(const char *string, int buf_size)
{
	//    char*  p = (char*)memchr(string, 0, buf_size);
	//
	//    if (!p) return buf_size;
	//    else return (p - string);
	//	register const char *p;
	if (!string || buf_size <= 0)
		return 0;
	const char *p;
	int l = 0;
	for (p = string; *p && buf_size > 0; l++, buf_size--, ++p)
		;
	return l;
}

int Jatoi(const char *str, size_t str_size)
{
	bool minus_flag = false;
	int Value = 0, Digit;
	char c;
	if (!str)
		return Value;
	for (size_t i = 0; ((c = *str++) != '\0') && (i < str_size); i++)
	{
		if (c == '-' && i == 0)
			minus_flag = true;
		else
		{
			if (c >= '0' && c <= '9')
				Digit = (unsigned int)(c - '0');
			else
				break;
			Value = (Value * 10) + Digit;
		}
	}
	if (minus_flag)
		Value = -Value;
	return Value;
}

char *Jitoa(int d, char *buf, int base)
{
	char *p = buf;
	char *p1, *p2;
	unsigned long ud = d;
	int divisor = 10;

	/* If %d is specified and D is minus, put `-' in the head.  */
	if (base == 'd' && d < 0)
	{
		*p++ = '-';
		buf++;
		ud = -d;
	}
	else if (base == 'x')
	{
		divisor = 16;
	}

	/* Divide UD by DIVISOR until UD == 0.  */
	do
	{
		int remainder = ud % divisor;

		*p++ = (remainder < 10) ? remainder + '0' : remainder + 'a' - 10;
	} while (ud /= divisor);

	/* Terminate BUF.  */
	*p = 0;

	/* Reverse BUF.  */
	p1 = buf;
	p2 = p - 1;
	while (p1 < p2)
	{
		char tmp = *p1;
		*p1 = *p2;
		*p2 = tmp;
		p1++;
		p2--;
	}
	return buf;
}

// int Jstrntoi(const char *str, char **ptr, int base, size_t str_size)
// {
// 	int i;
// 	register const char *s = str;
// 	register int acc;
// 	register int c;

// 	register int cutoff;
// 	register int neg = 0, any, cutlim;

// 	do
// 	{
// 		c = *s++;
// 	}while (isspace(c));

// 	if(c == '-')
// 	{
// 		neg = 1;
// 		c = *s++;
// 	}
// 	else if (c == '+') c = *s++;

// 	if ((base == 0 || base == 16) && c == '0' && (*s == 'x' || *s == 'X'))
// 	{
// 		c = s[1];
// 		s += 2;
// 		base = 16;
// 	}

// 	if (base == 0) base = c == '0' ? 8 : 10;

// 	cutoff = (neg)? (-(int)INT_MIN) : (INT_MAX);
// 	cutlim = cutoff % (unsigned long)base;
// 	cutoff /= (unsigned long)base;

// 	for (acc = 0, any = 0, i=0; ; c = *s++, i++)
// 	{
// 		if (isdigit(c))  c -= '0';
// 		else if (isalpha(c)) c -= isupper(c) ? 'A' - 10 : 'a' - 10;
// 		else break;
// 		if (c >= base) break;
// 		if (any < 0 || acc > cutoff || (acc == cutoff && c > cutlim)) any = -1;
// 		else
// 		{
// 			any = 1;
// 			acc *= base;
// 			acc += c;
// 		}
// 	}
// 	if (any < 0)
// 	{
// 		acc = neg ? INT_MIN : INT_MAX;
// 	}
// 	else if (neg) acc = -acc;
// 	if (ptr != 0) *ptr = (char *) (any ? s - 1 : str);
// 	return (acc);
// }

char Filename_Mapper(char name)
{
	// 0 - 9
	for (int i = 48; i < 58; ++i)
	{
		if (name == i)
			return (char)i;
	}

	// 'A' - 'Z'
	for (int i = 65; i < 91; ++i)
	{
		if (name == i)
			return (char)i;
	}

	// 'a' - 'z'
	for (int i = 97; i < 123; ++i)
	{
		if (name == i)
			return (char)i;
	}

	// other valid characters
	switch (name)
	{
	case '/':
		return '/';
	case '.':
		return '.';
	case '-':
		return '-';
	case '_':
		return '_';
	case ' ':
		return ' ';
	case '@':
		return '@';
	case '$':
		return '$';
	case '(':
		return '(';
	case ')':
		return ')';
	case '~':
		return '~';
	case '+':
		return '+';
	case ':':
		return ':';
	}
	return -1;
}

bool Get_Clean_Filename(char *inName, char *outName, int outNameSize)
{
	int inNameLen = Jstrlen(inName);

	if (inNameLen > outNameSize)
		return false;

	for (int charIndex = 0; charIndex < inNameLen; charIndex++)
	{
		outName[charIndex] = Filename_Mapper(inName[charIndex]);
		if (outName[charIndex] < 0)
			return false;
	}

	return true;
}

/*
void Jsrand(unsigned int seed)
{
	srand(seed);
}
*/
int Jrand(int min, int max)
{
	std::random_device rd;
	std::default_random_engine gen = std::default_random_engine(rd());
	std::uniform_int_distribution<int> dis(min, max);
	return dis(gen);
}
void GenerateHashCode(char *hash, int hash_len)
{
	const char HASH_TABLE[] = {
		'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h',
		'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p',
		'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
		'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
		'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
		'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
		'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0'};
	const UINT HASH_TABLE_SIZE = sizeof(HASH_TABLE);

	int i, j;
	for (i = 0; i < hash_len; i++)
	{
		j = (Jrand(0, HASH_TABLE_SIZE - 1));
		hash[i] = HASH_TABLE[j];
	}
	hash[i] = 0;
}

char *Jstrnstr(const char *s, const char *find, size_t buf_size)
{
	char c, sc;
	size_t len;

	if ((c = *find++) != '\0')
	{
		len = strlen(find);
		do
		{
			do
			{
				if ((sc = *s++) == '\0' || buf_size-- < 1)
					return (NULL);
			} while (sc != c);
			if (len > buf_size)
				return (NULL);
		} while (strncmp(s, find, len) != 0);
		s--;
	}
	return ((char *)s);
}

char *Jstrnchr(const char *p, char c, size_t buf_size)
{
	if (!p)
		return (NULL);

	while (buf_size-- > 0)
	{
		if (*p == c)
			return ((char *)p);
		p++;
	}
	return (NULL);
}

size_t Jwcslen(const wchar_t *s)
{
	const wchar_t *p;

	p = s;
	while (*p)
		p++;

	return p - s;
}

int Jstricmp(const char *pStr1, size_t Str1_size, const char *pStr2, size_t Str2_size)
{
	const char *Str1_end = pStr1 + Str1_size;
	const char *Str2_end = pStr2 + Str2_size;
	char c1, c2;
	int v;

	do
	{
		c1 = *pStr1++;
		c2 = *pStr2++;
		// The casts are necessary when pStr1 is shorter & char is signed
		v = (unsigned int)tolower(c1) - (unsigned int)tolower(c2);
	} while (v == 0 && c1 != '\0' && pStr1 < Str1_end && c2 != '\0' && pStr2 < Str2_end);

	return v;
}

int Jstrcmp(const char *str1, const char *str2, size_t str1_size, size_t str2_size)
{
	size_t str1_len = Jstrnlen(str1, str1_size);
	size_t str2_len = Jstrnlen(str2, str2_size);

	if (str1_len != str2_len)
		return -1;
	else
	{
		return strncmp(str1, str2, str1_len);
	}
}

#define Jstrcpy(x, y) Jstrncpy(x, y, sizeof(x))
char *Jstrncpy(char *strDest, const char *strSource, size_t count)
{
	unsigned int i;
	if (count <= 0)
		return strDest;
	if (!strSource)
		return strDest;
	size_t CpyCount = count - 1;
	for (i = 0; i < CpyCount; i++)
	{
		if (strSource[i] != 0)
			strDest[i] = strSource[i];
		else
			break;
	}
	strDest[(i < CpyCount) ? i : CpyCount] = 0;

	//	Return=strncpy(strDest,strSource,count-1);
	//	strDest[count-1]=0;
	return strDest;
}

char *Jstrncpy_s(char *s1, const char *s2, size_t n)
{
	char *s = s1;
	while (n > 0 && *s2 != '\0')
	{
		*s++ = *s2++;
		--n;
	}
	while (n > 0)
	{
		*s++ = '\0';
		--n;
	}
	return s1;
}

char *Jstrncpy2(char *strDest, const char *strSource, size_t count, size_t strDest_size)
{
	if (strDest_size <= 0 || count <= 0)
		return strDest;
	if (!strSource || !strDest)
		return NULL;
	size_t CpyCount = MIN(count, strDest_size - 1); //(count<strDest_size - 1) ? count : strDest_size - 1;
													/*	for (i=0; i<CpyCount; i++)
	{
		if(strSource[i]!=0)	strDest[i]=strSource[i];
		else break;
	}
*/
	strncpy(strDest, strSource, CpyCount);
	strDest[CpyCount] = 0;

	return strDest;
}

char *Jstrncat(char *strDest, const char *strSource, size_t count)
{
	size_t i, len, CatCount;

	strDest[count - 1] = 0;
	CatCount = count - 1;
	for (i = 0, len = Jstrlen(strDest); i + len < CatCount && strSource[i] != 0; i++)
		strDest[i + len] = strSource[i];
	strDest[(i + len < CatCount) ? i + len : CatCount] = 0;
	//	Return=strncat(strDest,strSource,Len);
	return strDest;
}

/*char *Jstrncat(char *strDest, const char *strSource, size_t count)
{
	int		Len;
	char	*Return;

	strDest[count-1]=0;
	Len=count-strlen(strDest)-1;
	Return=strncat(strDest,strSource,Len);
	return Return;
}*/

char *Jstrncat2(char *strDest, const char *strSource, size_t count, size_t strDest_size)
{
	size_t i, len, CatCount;

	strDest[strDest_size - 1] = 0;
	len = Jstrlen(strDest);
	CatCount = (count + len < strDest_size - 1) ? count + len : strDest_size - 1;
	for (i = 0; i + len < CatCount && strSource[i] != 0; i++)
		strDest[i + len] = strSource[i];
	strDest[(i + len < CatCount) ? i + len : CatCount] = 0;
	//	Return=strncat(strDest,strSource,Len);
	return strDest;
}

// #ifdef	_MSC_VER
char *Jsprintf(char *String, size_t Str_size, const char *format, ...)
{
	va_list arglist;
	int Len;

	va_start(arglist, format);
	if (format != NULL)
	{ // Combine String
		Len = _vsntprintf(String, Str_size, format, arglist);
		String[Str_size - 1] = 0;
		va_end(arglist);
		// Put String to Component
		/*if (Len<0)	return NULL; else */ return String;
	}
	else
	{
		va_end(arglist);
		return NULL;
	}
}

int Jsnprintf(char *String, size_t Str_size, const char *format, ...)
{
	va_list arglist;
	int Len = 0;

	va_start(arglist, format);
	if (format != NULL)
	{ // Combine String
		Len = _vsntprintf(String, Str_size, format, arglist);
		String[Str_size - 1] = 0;
		va_end(arglist);
		return Len < 0 ? Str_size - 1 : Len;
	}
	else
	{
		va_end(arglist);
		return 0;
	}
}
// #endif

char *Jstrftime(char *strDest, size_t maxsize, const char *format, const timeb *tp)
{
	// %f: millitm
	// ex: ------------
	// char		Buf[100];
	// timeb	Time;
	// tm		*p;
	// ftime(&Time);
	// p=localtime(&Time.time);
	// Jstrftime(Buf,sizeof(Buf),"%Y/%m/%d %H:%M:%S.%f",&Time);
	// Jstrftime(Buf,sizeof(Buf),"%Y/%m/%d %H:%M:%S.%f"); // Get current time
	// ex: ------------
	// char *wday[]={"Sun","Mon","Tue","Wed","Thu","Fri","Sat"};
	// tm: printf("%d/%d/%d %s %d:%d:%d",p->tm_year+1900,p->tm_mon+1,p->tm_mday,wday[p->tm_wday],p->tm_hour,p->tm_min,p->tm_sec);
	const size_t Buf_SIZE = 1024;
	int Len;
	size_t format_Len, Buf_Len;
	unsigned int i, j, k;
	char Buf[Buf_SIZE], Buf_milli[10];
	const char *Rformat = format;
	struct tm *p;
	bool millitm_Flag = false;

	if (tp == NULL)
	{
		timeb tp2;
		ftime(&tp2);
		p = localtime(&tp2.time);
		tp = &tp2;
	}
	if ((format_Len = strlen(format)) < Buf_SIZE)
	{
		Buf[0] = '\0';
		Buf_Len = format_Len;
		Len = _snprintf(Buf_milli, sizeof(Buf_milli), "%03d", tp->millitm);
		if (Len >= 0 && Len <= 3)
		{
			for (i = j = k = 0; i < format_Len && Buf_Len < (Buf_SIZE - 3); i++)
			{
				if (format[i] == '%' && format[i + 1] == 'f')
				{
					millitm_Flag = true;
					strncat(Buf, &format[j], i - j);
					Buf[(k += i - j)] = '\0';
					strcat(Buf, Buf_milli);
					k += Len;
					Buf_Len += Len - 2;
					j = i + 2;
				}
			}
			if (i >= format_Len && millitm_Flag)
			{
				strcat(Buf, &format[j]);
				Rformat = Buf;
			}
		}
	}
	p = localtime(&tp->time);
	strftime(strDest, maxsize, Rformat, p);
	return strDest;
}

std::string *Jstringftime(std::string &strDest, const char *format, const timeb *tp)
{
	// %f: millitm
	// ex: ------------
	// char		Buf[100];
	// timeb	Time;
	// tm		*p;
	// ftime(&Time);
	// p=localtime(&Time.time);
	// Jstrftime(Buf,sizeof(Buf),"%Y/%m/%d %H:%M:%S.%f",&Time);
	// Jstrftime(Buf,sizeof(Buf),"%Y/%m/%d %H:%M:%S.%f"); // Get current time
	// ex: ------------
	// char *wday[]={"Sun","Mon","Tue","Wed","Thu","Fri","Sat"};
	// tm: printf("%d/%d/%d %s %d:%d:%d",p->tm_year+1900,p->tm_mon+1,p->tm_mday,wday[p->tm_wday],p->tm_hour,p->tm_min,p->tm_sec);
	const size_t Buf_SIZE = 1024;
	int Len;
	size_t format_Len, Buf_Len;
	unsigned int i, j, k;
	char Buf[Buf_SIZE], Buf_milli[10];
	const char *Rformat = format;
	struct tm *p;
	bool millitm_Flag = false;

	if (tp == NULL)
	{
		timeb tp2;
		ftime(&tp2);
		p = localtime(&tp2.time);
		tp = &tp2;
	}
	if ((format_Len = strlen(format)) < Buf_SIZE)
	{
		Buf[0] = '\0';
		Buf_Len = format_Len;
		Len = _snprintf(Buf_milli, sizeof(Buf_milli), "%03d", tp->millitm);
		if (Len >= 0 && Len <= 3)
		{
			for (i = j = k = 0; i < format_Len && Buf_Len < (Buf_SIZE - 3); i++)
			{
				if (format[i] == '%' && format[i + 1] == 'f')
				{
					millitm_Flag = true;
					strncat(Buf, &format[j], i - j);
					Buf[(k += i - j)] = '\0';
					strcat(Buf, Buf_milli);
					k += Len;
					Buf_Len += Len - 2;
					j = i + 2;
				}
			}
			if (i >= format_Len && millitm_Flag)
			{
				strcat(Buf, &format[j]);
				Rformat = Buf;
			}
		}
	}
	p = localtime(&tp->time);
	strftime(Buf, sizeof(Buf), Rformat, p);
	strDest = Buf;
	return &strDest;
}

unsigned long JGetTickCount()
{
	timeval ts;

	gettimeofday(&ts, NULL);
	return (unsigned long)(ts.tv_usec / 1000) + ((unsigned long)ts.tv_sec * 1000);
}

char *Jstrntok(char *s, const char *delim, char **last)
{
	char *spanp;
	int c, sc;
	char *tok;
	if (s == NULL && (s = *last) == NULL)
		return (NULL);
	/*
	 * Skip (span) leading delimiters (s += strspn(s, delim), sort of).
	 */
cont:
	c = *s++;
	for (spanp = (char *)delim; (sc = *spanp++) != 0;)
	{
		if (c == sc)
			goto cont;
	}
	if (c == 0)
	{
		/* no non-delimiter characters */
		*last = NULL;
		return (NULL);
	}
	tok = s - 1;
	/*
	 * Scan token (scan for delimiters: s += strcspn(s, delim), sort of).
	 * Note that delim must have one NUL; we stop if we see that, too.
	 */
	for (;;)
	{
		c = *s++;
		spanp = (char *)delim;
		do
		{
			if ((sc = *spanp++) == c)
			{
				if (c == 0)
					s = NULL;
				else
					s[-1] = 0;
				*last = s;
				return (tok);
			}
		} while (sc != 0);
	}
	/* NOTREACHED */
}

char *Jstrtok(char *string, const char *control)
{
	// Token output allow empty if the string contain continuous control char, example: ";;;"
	unsigned char *str;
	const unsigned char *ctrl = (unsigned char *)control;
	unsigned char map[32];
	int count;
	static char *nextoken;

	// Clear control map
	for (count = 0; count < 32; count++)
		map[count] = 0;

	// Set bits in delimiter table
	// �ϥΦ��k�u�n32Byte���ܼơA�Y�i�N���Ҧ�ASCII���r���A�ϱo���r���u�n�X���޿�B��Y�i
	do
	{
		map[*ctrl >> 3] |= (1 << (*ctrl & 7));
	} while (*ctrl++);

	// Initialize str. If string is NULL, set str to the saved pointer (i.e., continue breaking tokens out of the string * from the last strtok call)
	if (string)
		str = (unsigned char *)string;
	else
		str = (unsigned char *)nextoken;

	// Find beginning of token (skip over leading delimiters). Note that there is no token iff this loop sets str to point to the terminal * null (*str == '\0')
	//	while ( (map[*str >> 3] & (1 << (*str & 7))) && *str )	str++;

	string = (char *)str;

	// Find the end of the token. If it is not the end of the string, put a null there.
	for (; *str; str++)
		if (map[*str >> 3] & (1 << (*str & 7)))
		{
			*str++ = '\0';
			break;
		}

	// Update nextoken (or the corresponding field in the per-thread data structure
	nextoken = (char *)str;

	// Determine if a token has been found.
	if (string == (char *)str)
		return NULL;
	else
		return string;
}

char *Jstrtok2(char *strToken, const char *strDelimit, int &Pos)
{
	// --- For strtok ---
	// for (i=0;strtok((i==0)?Buf:NULL,";");i++)
	// drawback: Can't nest strtok
	// ---For Jstrtok ---
	// Pos<0: Jstrtok==strtok
	// Pos>=0: for (SC_Pos=0;Jstrtok(Buf,";",SC_Pos);)
	// Advantage: nest strtok
	char *TokenCur, *strToken2;
	strToken2 = strToken + ((Pos >= 0 && strToken) ? Pos : 0);
	if ((TokenCur = Jstrtok(strToken2, strDelimit)) != NULL && Pos >= 0)
		Pos += (int)strlen(TokenCur) + 1;
	return TokenCur;
}

bool IsFileExist(const char *File)
{
	struct stat File_stat;

	if (stat(File, &File_stat) < 0)
		return false;
	else
		return true;
}

/*
LPHOSTENT Jgethostbyname(char *s1)
{
	return gethostbyname(s1);
}

int Get_LocalIP(char *IP,size_t IP_size)
{
	int			i=0;
#ifdef _MSC_VER
	WSADATA     WsaData;
	char		S1[256];
	LPHOSTENT   	lpHostEnt=NULL;
#else
	struct ifaddrs * ifAddrStruct=NULL;
	void * tmpAddrPtr=NULL;
#endif

#ifdef _MSC_VER
	WSAStartup(0x512,&WsaData);
	gethostname(S1,sizeof(S1)-1);
	if ((lpHostEnt=Jgethostbyname(S1))!=NULL)
	{
		for (i=0; *(lpHostEnt->h_addr_list+i)!=0; i++)
		{
			Jstrncpy(IP,inet_ntoa(*(LPIN_ADDR)*(lpHostEnt->h_addr_list+i)),IP_size);
			if (strcmp(IP,LOOPBACK_IPV4))	break;
		}
	}else { IP[0]=0; return -1;}
#else
	getifaddrs(&ifAddrStruct);
	IP[0]=0;
 	while (ifAddrStruct!=NULL)
	{
		if (ifAddrStruct->ifa_addr->sa_family==AF_INET && strcmp(ifAddrStruct->ifa_name, "lo")!=0)
		{	// check it is IP4 and not lo0 is a valid IP4 Address
			tmpAddrPtr=&((struct sockaddr_in *)ifAddrStruct->ifa_addr)->sin_addr;
			Jstrncpy(IP,inet_ntoa(*(LPIN_ADDR)tmpAddrPtr),IP_size);
			break;
		}
		else	ifAddrStruct=ifAddrStruct->ifa_next;
	}
	if (!IP[0])
	{
		Jstrncpy(IP,LOOPBACK_IPV4,sizeof(IP_size));
		freeifaddrs(ifAddrStruct);
		return -1;
	}
	freeifaddrs(ifAddrStruct);
#endif
	return 0;
}

*/
int Jgetpeername(SOCKET s, sockaddr *name, int *namelen)
{ // For Linux
#ifdef _MSC_VER
	return getpeername(s, name, namelen);
#else
	return getpeername(s, name, (socklen_t *)namelen);
#endif
}

int Jgetsockname(SOCKET s, sockaddr *name, int *namelen)
{ // For Linux
#ifdef _MSC_VER
	return getsockname(s, name, namelen);
#else
	return getsockname(s, name, (socklen_t *)namelen);
#endif
}

int Jrecvfrom(SOCKET s, char *buf, int len, int flags, sockaddr *from, int *fromlen)
{
#ifdef _MSC_VER
	return recvfrom(s, buf, len, flags, from, fromlen);
#else
	//	return recvfrom(s,buf,len,flags,from,fromlen);
	return recvfrom(s, buf, len, flags, from, (socklen_t *)fromlen);
#endif
}

int Jsendto(SOCKET s, const char *buf, int len, int flags, const struct sockaddr *to, int tolen)
{
#ifdef _MSC_VER
	return sendto(s, buf, len, flags, to, tolen);
#else
	return sendto(s, buf, len, flags, to, (socklen_t)tolen);
#endif
}

SOCKET Jaccept(SOCKET s, sockaddr *addr, int *addrlen)
{
#ifdef _MSC_VER
	return accept(s, addr, addrlen);
#else
	return accept(s, addr, (socklen_t *)addrlen);
#endif
}

int Jrecv(SOCKET s, char *buf, int len, int flags)
{
#ifdef _MSC_VER
	return recv(s, buf, len, flags);
#else
	return recv(s, buf, len, flags);
#endif
}

int Jtlv_recv(SOCKET s, char *buf, int len, int flags)
{
#ifdef _MSC_VER
	return recv(s, buf, len, flags);
#else
	//return k_tlv_recv(s,buf,len,flags);
	return recv(s, buf, len, flags);
#endif
}

int Jsend(SOCKET s, const char *buf, int len, int flags)
{
#ifdef _MSC_VER
	return send(s, buf, len, flags);
#else
	return send(s, buf, len, flags);
#endif
}

int Jtlv_send(SOCKET s, const char *buf, int len, int flags)
{
#ifdef _MSC_VER
	return send(s, buf, len, flags);
#else
	//return k_tlv_send(s,buf,len,flags);
	return send(s, buf, len, flags);
#endif
}

int Jclosesocket(SOCKET s)
{
#ifdef _MSC_VER
	return closesocket(s);
#else
	return close(s);
#endif
}

char *Jstrrev(char *string)
{
	size_t i, Len;
	char ch;

	for (i = 0, Len = strlen(string); i < int(Len / 2); i++)
	{
		ch = string[Len - i - 1];
		string[Len - i - 1] = string[i];
		string[i] = ch;
	}
	return string;
}

BOOL URL_Filter(const char *Url)
{
	char *Token, URL[255], DN_Value[10][100];
	int i, j, IP_Value[4], DN_Size, IP_Size, DN_Num;
	BOOL Filter_Flag;

	// Filter_Flag=2: Domain NAme OK
	// Filter_Flag=1: Pure IP OK
	// Filter_Flag=0: URL FALSE

	if (strcmp(Url, "") && strlen(Url) < 255)
	{
		// Initial
		Filter_Flag = 0;
		for (i = 0; i < 4; i++)
			IP_Value[i] = -1;
		DN_Size = sizeof(DN_Value) / sizeof(DN_Value[0]);
		IP_Size = sizeof(IP_Value) / sizeof(IP_Value[0]);
		memset(DN_Value, 0, sizeof(DN_Value));
		// Check URL
		Jstrncpy(URL, Url, sizeof(URL));
		for (i = 0; i < (int)strlen(URL); i++)
			if ((URL[i] < '0' || URL[i] > '9') && URL[i] != '.')
				break;
		if (i < (int)strlen(Url))
		{ // Domain Name
			for (DN_Num = j = 0; j < (int)strlen(URL); j++)
			{
				if (URL[j] == '.')
					DN_Num++;
			}
			Filter_Flag = (DN_Num >= 1) ? 2 : 0; // Filter_Flag=2 ==> Domain Name OK
		}
		else
		{ // Pure IP
			for (j = 0; (Token = strtok((j == 0) ? URL : NULL, ".")) != NULL; j++)
			{
				if (strcmp(Token, ""))
					IP_Value[j] = atoi(Token);
				else
					break;
			}
			for (j = 0; j < IP_Size; j++)
				if (IP_Value[j] < 0 || IP_Value[j] > 255)
					break;
			Filter_Flag = (j >= IP_Size) ? 1 : 0; // Filter_Flag=1 ==> Pure IP OK
		}
	}
	else
		Filter_Flag = 0;

	return Filter_Flag;
}

bool Port_Filter(unsigned short Port)
{
	bool Filter_Flag;

	// Initial
	Filter_Flag = false;
	// Check Port
	if (Port <= 0 || Port >= 65536)
		Filter_Flag = false;
	else
		Filter_Flag = true;

	return Filter_Flag;
}

#ifdef _MSC_VER
int Get_Modul_FileName(char *szFileName)
{
	szFileName[0] = 0;
	HINSTANCE hInstance = GetModuleHandle(NULL);
	GetModuleFileName(hInstance, szFileName, MAX_PATH);
	return 0;
}

int Get_0x15_0x16(char *String, int String_Size, bool Dierct_Flag)
{
	size_t i, k, Len;
	char Temp_Buf[128];

	memset(Temp_Buf, 0, sizeof(Temp_Buf));
	for (i = k = 0, Len = strlen(String); i < Len; i++)
	{
		// Transfrom
		Temp_Buf[i + k] = String[i] ^ 0x15 ^ 0x16;
		if (!Dierct_Flag)
		{
			if (Temp_Buf[i + k] == '"')
			{
				Temp_Buf[i + k] = '\\';
				Temp_Buf[i + k + 1] = '"';
				k++;
			}
			else if (Temp_Buf[i + k] == '\'')
			{
				Temp_Buf[i + k] = '\\';
				Temp_Buf[i + k + 1] = '\'';
				k++;
			}
			else if (Temp_Buf[i + k] == '\\')
			{
				Temp_Buf[i + k] = '\\';
				Temp_Buf[i + k + 1] = '\\';
				k++;
			}
		}
	}
	Jstrncpy(String, Temp_Buf, String_Size);
	return 0;
}

bool Get_Version(const char *Modul_FileName, char *Version, size_t Ver_size)
{
	DWORD dwSize, dwReserved;
	LPVOID lpBuffer;
	char *lpStr;
	UINT *str;
	UINT dwLength;
	HANDLE hdl;
	DWORD dwLangSet;
	char szText[256];
	Version[0] = 0;
	dwSize = GetFileVersionInfoSize((char *)Modul_FileName, &dwReserved);
	hdl = GetProcessHeap();
	if (!hdl)
		return false;
	lpBuffer = HeapAlloc(hdl, HEAP_ZERO_MEMORY, dwSize);
	if (lpBuffer && GetFileVersionInfo((char *)Modul_FileName, 0, dwSize, lpBuffer))
	{ // �Ѽƥi�Ѧ�Resource Version, 040404b0���ܤ����c��
		if (!::VerQueryValue(lpBuffer, "\\VarFileInfo\\Translation", (LPVOID *)&str, &dwLength))
		{
			if (lpBuffer)
				HeapFree(hdl, 0, lpBuffer);
			return FALSE;
		}
		dwLangSet = MAKELONG(HIWORD(str[0]), LOWORD(str[0]));
		_snprintf(szText, sizeof(szText), "\\StringFileInfo\\%08x\\FileVersion", dwLangSet);
		VerQueryValue(lpBuffer, szText, (LPVOID *)&lpStr, &dwLength);
		Jstrncpy(Version, lpStr, Ver_size);
	}
	if (lpBuffer)
		HeapFree(hdl, 0, lpBuffer);
	return true;
}

bool Check_ReExecute()
{
	char Exe_Name[MAX_PATH];
	HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if (hSnapShot == INVALID_HANDLE_VALUE)
		return 0;
	PROCESSENTRY32 procentry;
	procentry.dwSize = sizeof(PROCESSENTRY32);
	BOOL bFlag = Process32First(hSnapShot, &procentry);

	if ((int)strlen(AfxGetApp()->m_pszExeName) < (MAX_PATH - 5))
		Jsprintf(Exe_Name, sizeof(Exe_Name), "%s.exe", AfxGetApp()->m_pszExeName);
	else
		return false;

	int i = 0;
	while (bFlag)
	{
		if (!_stricmp(procentry.szExeFile, Exe_Name))
			i++;
		if (i > 1)
			return true;
		procentry.dwSize = sizeof(PROCESSENTRY32);
		bFlag = Process32Next(hSnapShot, &procentry);
	}

	return false;
}

#else
int GetPrivateProfileString(const char *lpAppName, const char *lpKeyName, const char *lpDefault, char *lpReturnedString, unsigned int nSize, const char *lpFileName)
{
	if (!lpAppName || !lpKeyName || !lpReturnedString || !lpFileName)
		return -1;

	FILE *fp;
	char line[256];
	char buffer[256];
	char keyname[256];
	int ch, i = 0, flag = 0;
	char *temp;

	if ((fp = fopen(lpFileName, "r")) == NULL)
		return 0;

	while (!feof(fp))
	{
		ch = fgetc(fp);
		if (ch == EOF)
			break;
		else
		{
			line[i++] = ch;
			if (ch != '\n')
				continue;
		}
		if (line[i - 2] == 0x0d)
			line[i - 2] = 0;
		else
			line[--i] = '\0';
		i = 0;
		temp = strchr(line, '=');
		if ((temp != NULL) && (flag == 1))
		{
			Jsprintf(keyname, sizeof(keyname), "%s=", lpKeyName);
			if (strstr(line, keyname) != NULL)
			{
				flag = 2; // the variable of KeyName is existed
				Jstrncpy(lpReturnedString, &temp[1], nSize);
				// fclose(fp);
				// return 1;
				break;
			}
		}
		else
		{
			Jsprintf(buffer, sizeof(buffer), "[%s]", lpAppName);
			if (!strncmp(buffer, line, Jstrnlen(buffer, sizeof(buffer))))
				flag = 1; // the AppName is existed
		}
	}
	if (!flag) // the Appname and the variable of KeyName is not existed
	{
		printf("[%s] is no setup\r\n", lpAppName);
		fclose(fp);
		// return 0;
	}
	else if (flag == 1 && !lpDefault)
	{
		printf("[%s] is not found, and is not Default value\r\n", lpAppName);
		fclose(fp);
		// return 0;
	}
	else if (flag != 2 && lpDefault)
		Jstrncpy(lpReturnedString, lpDefault, nSize);
	fclose(fp);
	return flag;
}

int GetPrivateProfileInt(const char *lpAppName, const char *lpKeyName, int nDefault, const char *lpFileName)
{
	char data[1024];
	char sDefault[32];
	Jsprintf(sDefault, sizeof(sDefault), "%d", nDefault);
	if (!GetPrivateProfileString(lpAppName, lpKeyName, sDefault, data, sizeof(data) - 1, lpFileName))
		return 0;
	return Jatoi(data, sizeof(data));
}

int CheckPrivateProfileString(std::string &content, const char *lpAppName, const char *lpKeyName, const char *lpFileName)
{
	if (!lpAppName || !lpKeyName || !lpFileName)
		return -1;

	FILE *fp;
	char line[512];
	char buf[512];
	char keyname[256];
	int ch, i = 0, line_flag = 0;
	bool pass_flag = false;
	char *temp;

	if ((fp = fopen(lpFileName, "r")) == NULL)
		return 0;

	while (fgets(line, sizeof(line), fp) != NULL)
	{
		content.append(line);
		if (pass_flag)
			continue;
		temp = strchr(line, '=');
		if ((temp != NULL) && (line_flag == 1))
		{
			Jsprintf(keyname, sizeof(keyname), "%s=", lpKeyName);
			if (strstr(line, keyname) != NULL)
			{
				line_flag = 2; // the variable of KeyName is existed
				pass_flag = true;
				;
			}
		}
		else
		{
			if (line_flag == 1)
				pass_flag = true;
			Jsprintf(keyname, sizeof(keyname), "[%s]", lpAppName);
			if (!strncmp(keyname, line, Jstrnlen(keyname, sizeof(keyname))))
				line_flag = 1; // the AppName is existed
		}
	}
	fclose(fp);
	return line_flag;
}

int WritePrivateProfileString(const char *lpAppName, const char *lpKeyName, const char *lpString, const char *lpFileName)
{
	if (!lpAppName || !lpKeyName || !lpString || !lpFileName)
		return -1;

	FILE *fp;
	char buf[256];
	int line_flag;
	std::string content;

	line_flag = CheckPrivateProfileString(content, lpAppName, lpKeyName, lpFileName);

	if ((fp = fopen(lpFileName, "w")) == NULL)
		return -2;
	if (line_flag == 0)
	{
		Jsprintf(buf, sizeof(buf), "[%s]\r\n%s=%s\r\n", lpAppName, lpKeyName, lpString);
		content.insert(content.length(), buf);
	}
	else if (line_flag == 1)
	{
		Jsprintf(buf, sizeof(buf), "[%s]", lpAppName);
		std::string::size_type found = content.find(buf);
		if (found != std::string::npos)
		{
			std::string::size_type pos = content.find("\r\n[", found + 1); // find the next other Appname section
			if (pos == std::string::npos)
				pos = content.find("\r\n\r\n", found + 1); // find the empty line in under the Appname section
			if (pos != std::string::npos)
			{
				Jsprintf(buf, sizeof(buf), "%s=%s\r\n", lpKeyName, lpString);
				content.insert(pos + 2, buf);
			}
			else
			{
				if (pos == std::string::npos)
					pos = content.find("\r\n", found + 1);
				if (pos != std::string::npos)
					Jsprintf(buf, sizeof(buf), "%s=%s\r\n", lpKeyName, lpString);
				else
					Jsprintf(buf, sizeof(buf), "\r\n%s=%s\r\n", lpKeyName, lpString);
				content.append(buf);
			}
		}
	}
	else if (line_flag == 2)
	{
		Jsprintf(buf, sizeof(buf), "[%s]", lpAppName);
		std::string::size_type found = content.find(buf);
		if (found != std::string::npos)
		{
			Jsprintf(buf, sizeof(buf), "%s=", lpKeyName);
			std::string::size_type beg = content.find(buf, found);
			if (beg != std::string::npos)
			{
				std::string::size_type end = content.find("\r\n", beg);
				if (end != std::string::npos && end > beg)
				{
					Jsprintf(buf, sizeof(buf), "%s=%s", lpKeyName, lpString);
					content.replace(beg, end - beg, buf);
				}
				else
				{
					Jsprintf(buf, sizeof(buf), "%s=%s", lpKeyName, lpString);
					content.replace(beg, content.length() - beg, buf);
				}
			}
		}
	}
	fwrite(content.c_str(), 1, content.length(), fp);
	fclose(fp);

	return 0;
}
#endif

FILE *Jfopen(const char *filename, const char *mode)
{
	if (!filename || !mode)
		return NULL;

	const char ASCII_Table[256] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	char File[_MAX_PATH] = {0};
	size_t f_len = sizeof(File), i, Len;

	for (i = 0, Len = Jstrlen(filename), Len = MIN(Len, f_len); i < Len && i < (_MAX_PATH - 1); i++)
	{
		File[i] = ASCII_Table[filename[i]];
		if (File[i] == 0)
			break;
	}
	File[i] = 0;
	return fopen(File, mode);
}

int Write_Error_Msg(const char *format, ...)
{
	char szTime[64];
	FILE *Error_Fout;
	char string[3000];
	va_list arglist;
	int Len;

	va_start(arglist, format);
	if (format != NULL)
	{ // Combine String
		Len = _vsntprintf(string, sizeof(string), format, arglist);
		string[sizeof(string) - 1] = 0;
		// Put String to Component
		if ((Error_Fout = fopen("Error_Report.txt", "ab")))
		{
			fprintf(Error_Fout, "%s ==> %s\r\n", Jstrftime(szTime, sizeof(szTime), "%Y.%m.%d %H:%M:%S"), string);
			fclose(Error_Fout);
		}
	}
	else
	{
		va_end(arglist);
		return -1;
	}
	va_end(arglist);
	return 0;
}

int Create_Directory(const char *szPath, mode_t mode)
{
	int i, DirCount;
	char *Token;
	char *p = NULL, *p1 = NULL;
#ifdef _MSC_VER
	char Buf[MAX_PATH];
	char DirBuf[100][MAX_PATH];
	const int consPath_Max = MAX_PATH;
#else
	char Buf[PATH_MAX]; //PATH_MAX=4096
	char DirBuf[100][MAX_PATH];
	const int consPath_Max = PATH_MAX;
#endif
	char outPath[512] = {0};
	Get_Clean_Filename((char *)szPath, outPath, sizeof(outPath));
	if ((int)Jstrnlen(outPath, consPath_Max + 1) > consPath_Max || !outPath[0])
		return -1;
	else
	{
		memset(DirBuf, 0, sizeof(DirBuf));
		Jstrncpy(Buf, outPath, sizeof(Buf));
		for (i = 1; (Token = Jstrntok((i == 1) ? Buf : NULL, "\\/", &p)) != NULL; i++)
		{
			Jstrncpy(DirBuf[i], Token, sizeof(DirBuf[i]));
		}
		DirCount = i;
//	if (strchr(DirBuf[i-1],'.')!=NULL)	{ Jstrncpy(DirBuf[i-1],"",sizeof(DirBuf[i-1])); DirCount--;}
#if defined(OS_LINUX)
		if (DirBuf[0][0] == 0 && outPath[0] == '/')
		{
			Jsprintf(Buf, sizeof(Buf), "/%s", DirBuf[1]);
			Jstrncpy(DirBuf[1], Buf, sizeof(DirBuf[1]));
		}
#endif
		if (DirBuf[1][1] == ':')
		{
			if ((Token = Jstrntok(DirBuf[1], ":", &p1)) != NULL)
			{
				Jstrncpy(DirBuf[0], Token, sizeof(DirBuf[0]));
				Jstrncat(DirBuf[0], ":", sizeof(DirBuf[0]));
			}
			if ((Token = Jstrntok(NULL, ":", &p1)) != NULL)
				Jstrncpy(DirBuf[1], Token, sizeof(DirBuf[1]));
			else
				Jstrncpy(DirBuf[1], "", sizeof(DirBuf[1]));
		}
		// Mkdir
		Buf[0] = 0;
		for (i = 0; i < DirCount; i++)
		{
			if (DirBuf[i][0] != 0)
			{
				Jstrncat(Buf, DirBuf[i], sizeof(Buf));
				Jstrncat(Buf, "/", sizeof(Buf));
				if (!(i == 0 && DirBuf[0][1] == ':'))
				{
#ifdef _MSC_VER
					CreateDirectory(Buf, NULL);
#else
					mkdir(Buf, mode);
#endif
				}
			}
		}
	}
	return 0;
}

int Remove_End_Char(char *Data)
{
	int i;
	for (i = 0; i < int(strlen(Data)); i++)
	{
		if (Data[i] == '\r' || Data[i] == '\n')
		{
			Data[i] = 0;
			break;
		}
	}
	return 0;
}

int Remove_FrontRear_Blank(char *Data, size_t Data_size)
{
	int i;
	for (i = 0; i < int(strlen(Data)); i++)
	{
		if (Data[i] != ' ' && Data[i] != '\t')
		{
			Jstrncpy(Data, Data + i, Data_size);
			break;
		}
	}
	if (i >= int(strlen(Data)))
		Data[0] = 0;
	for (i = int(strlen(Data)) - 1; i > 0; i--)
	{
		if (Data[i] != ' ' && Data[i] != '\t')
		{
			Data[i + 1] = 0;
			break;
		}
	}

	return 0;
}

int Get_Table(FILE *Fin, char *Data, size_t Data_size, bool &Comment_Scope_Flag)
{
	char Buf[1024], *Comment_Start, *Comment_End, *Comment_Oneline;
	int Len, Get_Data_Flag = -1;
	bool Comment_Inline_Flag;

	if (Fin != NULL)
	{
		if (fgets(Buf, sizeof(Buf), Fin) != NULL)
		{
			Remove_End_Char(Buf);
			Remove_FrontRear_Blank(Buf, sizeof(Buf));
			Comment_Inline_Flag = false;
			Get_Data_Flag = -1;
			//=== Comment Process ===
			if (Comment_Scope_Flag)
			{
				if ((Comment_End = strstr(Buf, "*/")) != NULL)
				{
					Jstrncpy(Buf, Comment_End + 2, sizeof(Buf));
					Comment_Scope_Flag = false;
					Remove_FrontRear_Blank(Buf, sizeof(Buf));
					if (int(strlen(Buf)) > 0)
						Comment_Inline_Flag = true;
					else
						Get_Data_Flag = 1;
				}
			}
			if (!Comment_Scope_Flag)
			{
				while ((Comment_Start = strstr(Buf, "/*")) != NULL)
				{
					Comment_Scope_Flag = true;
					if ((Comment_End = strstr(Buf, "*/")) != NULL)
					{
						Jstrncpy(Comment_Start, Comment_End + 2, sizeof(Buf) - (Comment_Start - Buf));
						Comment_Scope_Flag = false;
					}
					else
						*Comment_Start = 0;
					if (int(strlen(Buf)) > 0 && Comment_Scope_Flag)
						Comment_Inline_Flag = true;
				}
				if ((Comment_Oneline = strstr(Buf, "//")) != NULL)
				{
					if (!Comment_Scope_Flag)
						Comment_Scope_Flag = false;
					*Comment_Oneline = 0;
					Remove_FrontRear_Blank(Buf, sizeof(Buf));
					if (int(strlen(Buf)) > 0)
						Comment_Inline_Flag = true;
					else
					{
						Comment_Inline_Flag = false;
						Get_Data_Flag = 1;
					}
				}
			}
			//=== Comment Process End ===
			if (Get_Data_Flag == 1)
				Data[0] = 0;
			else
			{
				//if		(!Comment_Scope_Flag && Comment_Inline_Flag && Comment_Oneline!=NULL)	{ Get_Data_Flag=1; Data[0]=0;}
				if (Comment_Scope_Flag && !Comment_Inline_Flag)
				{
					Get_Data_Flag = 1;
					Data[0] = 0;
				}
				else
				{
					Remove_FrontRear_Blank(Buf, sizeof(Buf));
					// Get Empty
					if (Buf[0] == 0)
					{
						Get_Data_Flag = 0;
						Data[0] = 0;
					}
					// Get []: Get_Data_Flag=2
					else if (Buf[0] == '[' && Buf[(Len = int(strlen(Buf))) - 1] == ']')
					{
						Get_Data_Flag = 2;
						Buf[Len - 1] = 0;
						Jstrncpy(Data, &Buf[1], Data_size);
					}
					// Get Table Data: Get_Data_Flag=3
					else
					{
						Get_Data_Flag = 3;
						Jstrncpy(Data, Buf, Data_size);
					}
				}
			}
		}
		else
			Get_Data_Flag = -1;
	}
	else
		Get_Data_Flag = -1;

	return Get_Data_Flag;
}

unsigned short GetCheckSum(unsigned short *buffer, int len)
{
	unsigned long cksum = 0;

	while (len > 1)
	{
		cksum += *buffer++;
		len -= sizeof(unsigned short);
	}
	if (len)
	{
		cksum += *(unsigned char *)buffer;
	}
	cksum = (cksum >> 16) + (cksum & 0xffff);
	cksum += (cksum >> 16);
	return (unsigned short)(~cksum);
}

int Cat_StreamHeader(unsigned char *Data, size_t Data_size, int Len) //�bMRTP&TCP�Ҧ��A�e����7��byte
{
	const int Buf_size = 65536;
	const int Header_size = StreamHeaderSIZE;
	unsigned char Buf[Buf_size];
	unsigned short PK_Cksum, HD_Cksum;

	if (Data == NULL)
		return -1;
	if (Len <= 0 || Len >= (int)Buf_size || (Len + Header_size) >= (int)Data_size)
		return -1;

	// �[ Header { 0xAA, L_Len, H_Len, L_DataCksum, H_DataCksum, L_HeadCksum, H_HeadCksum }
	memcpy(Buf, Data, Len);
	PK_Cksum = GetCheckSum((unsigned short *)Buf, Len);
	Data[0] = 0xAA;
	Data[1] = Len & 0xFF;
	Data[2] = (Len >> 8) & 0xFF;
	Data[3] = PK_Cksum & 0xFF;
	Data[4] = (PK_Cksum >> 8) & 0xFF;
	HD_Cksum = GetCheckSum((unsigned short *)Data, Header_size - 2);
	Data[5] = HD_Cksum & 0xFF;
	Data[6] = (HD_Cksum >> 8) & 0xFF;
	memcpy(&Data[Header_size], Buf, Len);
	Data[Header_size + Len] = 0;
	return Len + Header_size;
}

bool IsStreamHeader(const unsigned char *data, int data_Len)
{
	if (!data)
		return false;
	unsigned short HD_Cksum;

	if (data[0] == 0xAA)
	{
		if (data_Len > StreamHeaderSIZE)
		{
			HD_Cksum = *((unsigned short *)&data[5]);
			if (GetCheckSum((unsigned short *)&data[0], StreamHeaderSIZE - 2) == HD_Cksum)
				return true;
			else
				return false;
		}
		else
			return false;
	}
	else
		return false;
}

int StoreStreamBuf(pSTREAM_BUFFER pSB, const unsigned char *data, int data_Len)
{
	if (!pSB || !data)
		return -1;
	if ((pSB->Len + data_Len < (sizeof(pSB->Buf) - 1)) && data_Len >= 0)
	{
		memcpy(&pSB->Buf[pSB->Len], data, data_Len);
		pSB->Len += data_Len;
	}
	// The size of buf is too small
	else
		return -2;

	return 0;
}

int Div_Stream(unsigned char **OutData, const unsigned char *Src, int Src_Len, int &Src_Cur, pSTREAM_BUFFER pSB)
{
	// Src_Cur must be 0 when first time
	const int Header_size = StreamHeaderSIZE;
	int PK_Len = 0, Len, UnprocLen;
	unsigned short PK_Cksum, cksum;

	if (!Src)
		return -1; // The param don't conform the specification
	if (Src_Cur < 0 || Src_Cur >= Src_Len || Src_Len < 0)
		return -1; // The param don't conform the specification

	// Process incomplete old data leave in Buf
	if (pSB && pSB->Buf && pSB->Len > 0)
	{
		if (pSB->Buf[0] == 0xAA)
		{
			UnprocLen = Src_Len - Src_Cur;
			// Supplement insufficient header data
			if (pSB->Len < Header_size)
			{
				// Store incomplete data and buffer size is still smaller than stream header size
				if (pSB->Len + UnprocLen < Header_size)
				{
					if (!StoreStreamBuf(pSB, &Src[Src_Cur], UnprocLen))
						return -1;
					else
						return -2; // The size of buf is too small
				}
				// Store incomplete data whose size is smaller than stream header size
				else
					memcpy(&pSB->Buf[pSB->Len], Src, Header_size - pSB->Len);
			}
			//
			PK_Len = (int)*((unsigned short *)&pSB->Buf[1]);
			PK_Cksum = *((unsigned short *)&pSB->Buf[3]);
			if (pSB->Len < PK_Len + Header_size && PK_Len < sizeof(pSB->Buf) - 1)
			{
				Len = Header_size + PK_Len - pSB->Len;
				// Process old data of Buf plus the new data that is also incomplete
				if (Len > Src_Len)
				{
					if (!StoreStreamBuf(pSB, Src, Src_Len))
						return -1;
					else
						return -2; // The size of buf is too small
				}
				// Process complete data and return
				else
				{
					if (!StoreStreamBuf(pSB, Src, Len))
					{
						if (GetCheckSum((unsigned short *)&pSB->Buf[Header_size], PK_Len) == PK_Cksum)
						{
							*OutData = &pSB->Buf[Header_size];
							pSB->Len = 0;
							Src_Cur += Len;
							return PK_Len;
						}
						else
							return -7; // Validate checksum of packet error
					}
					else
						return -2; // The size of buf is too small
				}
			} // else impossible
		}	  // else impossible
	}		  // else then don't process this case
	if (pSB)
		pSB->Len = 0;

	for (; Src_Cur < Src_Len; Src_Cur++)
	{
		if (Src[Src_Cur] == 0xAA)
		{
			UnprocLen = Src_Len - Src_Cur;
			if (IsStreamHeader(&Src[Src_Cur], UnprocLen))
			{
				PK_Len = (int)*((unsigned short *)&Src[Src_Cur + 1]);
				PK_Cksum = *((unsigned short *)&Src[Src_Cur + 3]);
				// Process non-last and last complete data and return
				if (Src_Cur + Header_size + PK_Len <= Src_Len)
				{
					cksum = GetCheckSum((unsigned short *)&Src[Src_Cur + Header_size], PK_Len);
					// if (!(Src_Cur+Header_size+PK_Len<Src_Len && Src[Src_Cur+Header_size+PK_Len]!=0xAA && cksum!=PK_Cksum))
					if (Src_Cur + Header_size + PK_Len == Src_Len || Src[Src_Cur + Header_size + PK_Len] == 0xAA || cksum == PK_Cksum)
					{
						*OutData = (unsigned char *)&Src[Src_Cur + Header_size];
						Src_Cur += Header_size + PK_Len;
						if (cksum == PK_Cksum)
							return PK_Len;
						else
							return -5; // The data checksum error
					}				   // else then search next '0xAA'
				}
				// Process incomplete data
				else if (pSB && pSB->Buf)
				{
					if (!StoreStreamBuf(pSB, &Src[Src_Cur], UnprocLen))
					{
						Src_Cur = Src_Len;
						return -3; // The data is not enough to output
					}
					else
						return -2; // The size of buf is too small
				}
			}
			// Process incomplete data when header is cutted
			else if (pSB && pSB->Buf)
			{
				if (!StoreStreamBuf(pSB, &Src[Src_Cur], UnprocLen))
				{
					Src_Cur = Src_Len;
					return -3; // The data is not enough to output
				}
				else
					return -2; // The size of buf is too small
			}
		} // else then search next '0xAA'
	}
	if (Src_Cur < Src_Len && Src[Src_Cur] != 0xAA)
		return -4; // The data fromat is abnormal

	if (PK_Len > 0)
		return PK_Len;
	else
		return -6; // Piece of data
}

int Txt_HSF_CP(char *Buf, int Len)
{
	int i; //,j;
	char Buf1[3000];
	char A;

	//return Len;

	if ((Len <= 0) || (Len >= 2000))
		return Len;

	// ���F�P�ª� Compatible �ӫO�d for Server
	//for (j=0;j<Len;j++) if (Buf[j]=='|') break;
	//if ( ((Buf[0]!=0x30)&&(j<=3)) || (j>=Len) || ((j<Len)&&(j>=9)) ) return Len;

	for (i = 0; i < Len; i++)
		Buf1[i] = Buf[i];
	Buf[0] = '?';
	for (i = 0; i < Len; i++)
		Buf[i + 1] = char(Buf1[i] - Buf[i]);
	for (i = 0; i < Len / 2; i++)
	{
		A = Buf[2 * i + 1];
		Buf[2 * i + 1] = Buf[2 * i + 2];
		Buf[2 * i + 2] = A;
	}
	for (i = 0; i < Len / 2; i++)
	{
		A = Buf[i + 1];
		Buf[i + 1] = Buf[Len - i];
		Buf[Len - i] = A;
	}
	for (i = 0; i < Len / 2; i++)
	{
		A = Buf[i + 1];
		Buf[i + 1] = Buf[Len / 2 + i + 1];
		Buf[Len / 2 + i + 1] = A;
	}
	return Len + 1;
}

int HSF_Txt_CP(char *Buf, int Len1)
{
	int i, Len, Copy_len;
	char Buf1[2000] = {0}, Copy[2048] = {0};
	char A;

	if ((Buf[0] != '?') || (Len1 <= 1) || (Len1 >= 1000))
		return Len1;

	Copy_len = MIN(Len1, sizeof(Copy));
	memcpy(Copy, Buf, Copy_len);

	Len = Copy_len - 1;

	for (i = 0; i < Len1; i++)
		Buf1[i] = Copy[i];
	for (i = 0; i < Len / 2; i++)
	{
		A = Buf1[i + 1];
		Buf1[i + 1] = Buf1[Len / 2 + i + 1];
		Buf1[Len / 2 + i + 1] = A;
	}
	for (i = 0; i < Len / 2; i++)
	{
		A = Buf1[i + 1];
		Buf1[i + 1] = Buf1[Len - i];
		Buf1[Len - i] = A;
	}
	for (i = 0; i < Len / 2; i++)
	{
		A = Buf1[2 * i + 1];
		Buf1[2 * i + 1] = Buf1[2 * i + 2];
		Buf1[2 * i + 2] = A;
	}
	for (i = 0; i < Len && i < sizeof(Buf1) - 1; i++)
		Copy[i] = static_cast<char>(Buf1[i + 1] + Buf1[i]);
	memcpy(Buf, Copy, Copy_len);
	return Len;
}

int JTCP_send(SOCKET s, const char *buf, int len, int flags, pSTREAM_BUFFER pSB)
{
	int DataLen, nRet, WriteLen = 0;
	char Buf[65536], *pBuf;
	const int Buf_size = sizeof(Buf);

	// Add header
	if (len + StreamHeaderSIZE < Buf_size)
		memcpy(Buf, buf, len);
	DataLen = Cat_StreamHeader((unsigned char *)Buf, sizeof(Buf), len);
	// Send data
	nRet = send(s, Buf, DataLen, flags);
	// Process send buffer
	if (pSB)
	{
		if (nRet == SOCKET_ERROR)
		{
			// WSAEWOULDBLOCK
			WriteLen = DataLen;
			pBuf = Buf;
		}
		else if (nRet >= 0 && nRet < DataLen)
		{
			WriteLen = DataLen - nRet;
			pBuf = &Buf[nRet];
		}
		if (WriteLen > 0)
		{
			if (pSB->Len + WriteLen < (int)pSB->Size)
			{
				memcpy(&pSB->Buf[pSB->Len], Buf, WriteLen);
				pSB->Len += WriteLen;
				Write_Error_Msg("%d ==>Send fail: Ret=%d Data=%d Buf=%d/%d\r\n", __LINE__, nRet, WriteLen, pSB->Len, pSB->Size);
				return -2;
			}
			else
			{
				Write_Error_Msg("%d ==>The size of buf is too small: Ret=%d Data=%d Buf=%d/%d\r\n", __LINE__, nRet, WriteLen, pSB->Len, pSB->Size);
				return -3; // The size of buf is too small
			}
		}
	}
	if (nRet > StreamHeaderSIZE)
		nRet -= StreamHeaderSIZE;
	return nRet;
}

int JTCP_send1(SOCKET s, const char *buf, int len, int flags)
{
	int DataLen;
	char Buf[65536];
	const int Buf_size = sizeof(Buf);

	if (len + StreamHeaderSIZE < Buf_size)
		memcpy(Buf, buf, len);
	DataLen = Txt_HSF_CP(Buf, len);
	DataLen = Cat_StreamHeader((unsigned char *)Buf, sizeof(Buf), DataLen);

#if JTCP_DEBUG
	// Debuf
	int Len;
	char Msg[5000], Time_Buf[100];
	sockaddr_in Addr;
	Len = sizeof(sockaddr);
	Jgetpeername(s, (sockaddr *)&Addr, &Len);
	Jsprintf(Msg, sizeof(Msg), "<Send to %s %s>\r\n%s\r\n**Length=%d\r\n**Send Length=%d\r\n\r\n\r\n", inet_ntoa(Addr.sin_addr), Jstrftime(Time_Buf, sizeof(Time_Buf), "%Y/%m/%d %H:%M:%S"), buf, len, DataLen);
#ifdef _MSC_VER
//	Write_Error_Msg(Msg);
#else
	printf(Msg);
#endif
#endif

	return Jtlv_send(s, Buf, DataLen, flags);
}

typedef void (*RECVPROC)(void *Param, SOCKET Socket, char *Out, int OutLen);
int JTCP_recv(SOCKET s, int recvflags, pSTREAM_BUFFER pSB, RECVPROC lpRecvFunc, void *arglist)
{
	int PacketCount, DataLen, DataCur = 0, OutLen;
	char Data[65536] = {0}, *Out, TxtBuf[65536] = {0};

	DataLen = recv(s, Data, sizeof(Data), recvflags);
	if (DataLen <= 0 || DataLen >= sizeof(Data) - 1)
		return -1;
	for (PacketCount = 0; (OutLen = Div_Stream((unsigned char **)&Out, (unsigned char *)Data, DataLen, DataCur, pSB)) > 0; PacketCount++)
	{
		if (OutLen >= sizeof(TxtBuf) - 1)
		{
			continue;
		}
		memcpy(TxtBuf, Out, OutLen);
		lpRecvFunc(arglist, s, TxtBuf, OutLen);
	}
	return DataLen;
}

int JTCP_recv1(SOCKET s, int recvflags, pSTREAM_BUFFER pSB, RECVPROC lpRecvFunc, void *arglist)
{
	int PacketCount, DataLen, DataCur = 0, OutLen, TxtLen;
	char Data[65536] = {0}, *Out, TxtBuf[65536] = {0};

	DataLen = Jtlv_recv(s, Data, sizeof(Data) - 1, recvflags);
	if (DataLen <= 0 || DataLen >= sizeof(Data))
		return DataLen;

#if JTCP_DEBUG
	// Debuf
	int Len, TDLen = 0;
	char Msg[5000], Time_Buf[100];
	sockaddr_in Addr;
	Len = sizeof(sockaddr);
	Jgetpeername(s, (sockaddr *)&Addr, &Len);
	Jsprintf(Msg, sizeof(Msg), "<Recv from %s %s>\r\n**Recv Length=%d\r\n", inet_ntoa(Addr.sin_addr), Jstrftime(Time_Buf, sizeof(Time_Buf), "%Y/%m/%d %H:%M:%S"), DataLen);
#ifdef _MSC_VER
//	Write_Error_Msg(Msg);
#else
	printf(Msg);
#endif
#endif

	for (PacketCount = 0; (OutLen = Div_Stream((unsigned char **)&Out, (unsigned char *)Data, DataLen, DataCur, pSB)) > 0; PacketCount++)
	{
		if ((OutLen >= sizeof(TxtBuf) - 1) || OutLen < 0)
			continue;
		memcpy(TxtBuf, Out, OutLen);
		TxtLen = HSF_Txt_CP(TxtBuf, OutLen);

#if JTCP_DEBUG
		// Debuf
		TDLen += TxtLen;
		TxtBuf[TxtLen] = 0;
		Jsprintf(Msg, sizeof(Msg), "<Recv from %s %s>\r\n**Recv Length=%d\r\n", inet_ntoa(Addr.sin_addr), Jstrftime(Time_Buf, sizeof(Time_Buf), "%Y/%m/%d %H:%M:%S"), DataLen);
#ifdef _MSC_VER
		//		Write_Error_Msg(Msg);
#else
		printf(Msg);
#endif
#endif

		lpRecvFunc(arglist, s, TxtBuf, TxtLen);
	}

#if JTCP_DEBUG
	// Debug
	if (TDLen + 8 * PacketCount == DataLen)
	{
		Jsprintf(Msg, sizeof(Msg), "**Total Out Length=%d\r\n\r\n\r\n", TDLen);
#ifdef _MSC_VER
		//		Write_Error_Msg(Msg);
#else
		printf(Msg);
#endif
	}
	else
	{
		Jsprintf(Msg, sizeof(Msg), "**Total Out Length=%d\r\nIt is Fail.\r\n\r\n", TDLen);
#ifdef _MSC_VER
		//		Write_Error_Msg(Msg);
#else
		printf(Msg);
#endif
	}
#endif

	return DataLen;
}

//======== Example =========
//-------- Callback Function ----------
//	void RecvProc(void *Param,SOCKET Socket,char *Out,int OutLen)
//	{
//		CTestDlg *lpTestDlg=(CTestDlg *)Param;
//		lpTestDlg->RecvFunc(Socket,Out,OutLen);
//	}
//
//	int CTestDlg::RecvFunc(SOCKET Socket,char *Out,int OutLen)
//	{
//		char	Display[100];
//
//		memset(Display,0,sizeof(Display));
//		memcpy(Display,Out,OutLen);
//		return 0;
//	}
//-------- Use JTCP_recv Function -------
//	STREAM_BUFFER	SB;
//	Case 1:----------
//	For First:		JTCP_recv(123,0,Init_Stream_Buffer(&SB),RecvProc,this);
//	For Otherwise:	JTCP_recv(123,0,&SB,RecvProc,this);
//	Case 2:----------
//  JTCP_recv(123,0,NULL,RecvProc,this);

typedef void (*RECVPROC1)(void *Param, SOCKET Socket, char *Out, int OutLen, sockaddr_in *lpAddr, void *lpSIP_Msg);
void TTCP_recvstream(SOCKET s, sockaddr_in *lpAddr, void *lpSIP_Msg, unsigned char *Data, int DataLen, pSTREAM_BUFFER pSB, RECVPROC1 lpRecvFunc, void *arglist)
{
	int PacketCount, OutLen, DataCur = 0;
	char *Out, TxtBuf[65535];
	for (PacketCount = 0; (OutLen = Div_Stream((unsigned char **)&Out, (unsigned char *)Data, DataLen, DataCur, pSB)) > 0; PacketCount++)
	{
		memcpy(TxtBuf, Out, OutLen);
		TxtBuf[OutLen] = 0;
		lpRecvFunc(arglist, s, TxtBuf, OutLen, lpAddr, lpSIP_Msg);
	}
}

int Jgethostbyname(char *Domain, char *ip, size_t ip_size)
{
	struct addrinfo hints, *res = NULL, *p = NULL;
	int status;

	if (!Domain || Domain[0] == 0)
		return -1;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	if ((status = getaddrinfo(Domain, NULL, &hints, &res)) != 0)
	{
		return -2;
	}

	for (p = res; p != NULL; p = p->ai_next)
	{
		void *addr;
		char *ipver;
		if (p->ai_family == AF_INET)
		{
			// IPv4
			struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
			addr = &(ipv4->sin_addr);
			ipver = (char *)"IPv4";
		}
		else
		{ // IPv6
			struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p->ai_addr;
			addr = &(ipv6->sin6_addr);
			ipver = (char *)"IPv6";
		}

		// convert the IP to a string and print it:
		inet_ntop(p->ai_family, addr, ip, ip_size);
		break;
	}

	freeaddrinfo(res);

	return 0;
}

std::string GetIpFromSockaddr(int af, const sockaddr *sa)
{
	if (!sa)
		return "";
	if (af == AF_INET)
	{
		char ip[INET_ADDRSTRLEN];
		inet_ntop(af, &((struct sockaddr_in *)sa)->sin_addr, ip, INET_ADDRSTRLEN);
		std::string strIP = ip;
		return strIP;
	}
	else
	{
		char ip[INET6_ADDRSTRLEN];
		inet_ntop(af, &((struct sockaddr_in6 *)sa)->sin6_addr, ip, INET_ADDRSTRLEN);
		std::string strIP = ip;
		return strIP;
	}
}

uint16_t GetPortFromSockaddr(int af, const sockaddr *sa)
{
	if (!sa)
		return 0;
	if (af == AF_INET)
		return ntohs(((struct sockaddr_in *)sa)->sin_port);
	else
		return ntohs(((struct sockaddr_in6 *)sa)->sin6_port);
}

//================ Jstring calss ================
Jstring &Jstring::operator=(const char *str)
{
	Jstrncpy(strbuf, str, strbuf_len);
	return assign(str);
}
Jstring &Jstring::operator=(const Jstring &str)
{
	return this->assign(str);
}
Jstring &Jstring::assign(const char *str)
{
	if (strbuf)
	{
		Jstrncpy(strbuf, str, strbuf_size);
	}
	return *this;
}
Jstring &Jstring::assign(const Jstring &str)
{
	if (this != &str) // self-assignment check expected
	{
		Jstrncpy(strbuf, str.strbuf, strbuf_len);
	}
	return *this;
}
int Jstring::clear()
{
	if (!strbuf)
		return -1;
	memset(strbuf, 0, strbuf_size);
	strbuf_len = 0;
	return 0;
}
size_t Jstring::length()
{
	return strbuf_len;
}
bool Jstring::empty()
{
	return (strbuf_len == 0);
}
const char *Jstring::c_str()
{
	return strbuf;
}
int Jstring::compare(const char *str)
{
	return strcmp(strbuf, str);
}
// int Jstring::compare(const char *str, size_t size)
// {
// 	return Jstrcmp(strbuf, str, strbuf_size, size);
// }
int Jstring::compare(const Jstring &str)
{
	return Jstrcmp(strbuf, str.strbuf, strbuf_size, str.strbuf_len);
}
size_t Jstring::find(const char *find)
{
	char *found;
	found = Jstrnstr(strbuf, find, strbuf_size);
	if (!found)
		return npos;
	else
		return (size_t)(found - strbuf);
}
size_t Jstring::find(Jstring &find)
{
	char *found;
	found = Jstrnstr(strbuf, find.c_str(), strbuf_size);
	if (!found)
		return npos;
	else
		return (size_t)(found - strbuf);
}