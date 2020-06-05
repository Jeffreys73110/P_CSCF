#ifndef FUNCTIONS_DEF
#define FUNCTIONS_DEF
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#ifdef _MSC_VER
#include "../stdafx.h"
//===== Import Lib =====
#pragma comment(lib, "Version.lib") // For GetFileVersionInfoSize
#include "Tlhelp32.h"				// For CreateToolhelp32Snapshot
#include <limits.h>					// PATH_MAX
#include <direct.h>					// For mkdir
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#else
#include <ctype.h>
#include <string.h> // For strncpy
#include <stdlib.h> // For atoi
#include <stdio.h>	// For File
#include <stdarg.h> // For va_start
#include <limits.h>
#include <netdb.h> //hostent
#include <ifaddrs.h>
#include <arpa/inet.h>	// inet_ntoa
#include <netinet/in.h> // inet_ntoa
#include <sys/socket.h> // inet_ntoa
#include <unistd.h>		// close socket...
#include <sys/time.h>	// gettimeofday
#include <netinet/tcp.h>

typedef int BOOL;
typedef int SOCKET;
typedef unsigned int UINT;
// #define	Jsprintf		snprintf
#define _snprintf snprintf
#define _vsntprintf vsnprintf
#define MAX_PATH PATH_MAX
#define _MAX_PATH PATH_MAX
// #define MIN(a, b)		std::min(a,b)
// #define max(a, b)		std::max(a,b)
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define SOCKET_ERROR (-1)

typedef struct hostent HOSTENT;
typedef struct hostent *LPHOSTENT;
typedef struct in_addr *LPIN_ADDR;

#endif

#include <random>
// #include "StdHeader.h"

#include <time.h>	   // For localtime and tm
#include <sys/timeb.h> // For ftime and timeb
#include <sys/stat.h>  // For stat

#define LOOPBACK_IPV4 "127.0.0.1"
#define F_BOOL(x) ((x) ? true : false)

#define MAX_LENGTH 255
#define URL_LENGTH 100
#define IP_LENGTH 64
#define PORT_LENGTH 16
#define MAC_LENGTH 16
#define STREAM_BUFFER_SIZE 23552 //23K
// Define
#define atob(x) (atoi(x) ? true : false)

#define JTCP_DEBUG FALSE
#define PrintBinary_ENABLE true

typedef struct _STREAM_BUFFER
{
	unsigned char Buf[STREAM_BUFFER_SIZE];
	size_t Size;
	int Len;
	// Initial
	_STREAM_BUFFER()
	{
		reset();
	}
	void reset()
	{
		Buf[0] = 0;
		Size = STREAM_BUFFER_SIZE;
		Len = 0;
	}
} STREAM_BUFFER, *pSTREAM_BUFFER;

//pSTREAM_BUFFER Set_Stream_Buffer(pSTREAM_BUFFER pSB,unsigned char *Buf,size_t Buf_size)
//{
//	if (!pSB)	return NULL;
//	pSB->Buf=Buf;
//	pSB->Size=Buf_size;
//	pSB->Len=0;
//	return pSB;
//}

#define StreamHeaderSIZE 7

class Jstring
{
public:
	Jstring()
	{
		memset(this, 0, sizeof(Jstring));
		// strbuf_size = 1024;
		// alloc(strbuf_size);
		strbuf = charArray;
		strbuf_size = charArray_size;
	}
	// Jstring(size_t s)
	// {
	// 	memset(this, 0, sizeof(Jstring));
	// 	strbuf_size = s;
	// 	alloc(s);
	// }
	~Jstring()
	{
		// dealloc();
	}

private:
	int alloc(size_t s)
	{
		if (s <= 0 || s > 65536)
			return -1;
		if (strbuf)
			return -2;
		strbuf = new char[s];
		clear();
		return 0;
	}
	void dealloc()
	{
		if (strbuf)
			delete[] strbuf;
	}

public:
	const static size_t npos = ~1;

// private:
	char *strbuf;
	size_t strbuf_size;
	size_t strbuf_len;

private:
	const static size_t charArray_size = 64;
	char charArray[charArray_size];

public:
	Jstring &operator=(const char *str);
	Jstring &operator=(const Jstring &str);
	Jstring &assign(const char *str);
	Jstring &assign(const Jstring &str);
	int clear();
	size_t length();
	bool empty();
	const char *c_str();
	int compare(const char *str);
	// int compare(const char *str, size_t size);
	int compare(const Jstring &str);
	size_t find(const char *find);
	size_t find(Jstring &find);
};


//----- functions -----
bool replace_all_newline(std::string &str, const std::string &from, const std::string &to, int search_posi);
size_t Jstrlen(const char *string);
int Jstrnlen(const char *string, int buf_size);
#define Jatoi(str)	_Jatoi(str, sizeof(str))
int _Jatoi(const char *str, size_t str_size);
char *Jitoa(int d, char *buf, int base);
// int Jstrntoi(const char *str, char **ptr, int base, size_t str_size);
char Filename_Mapper(char name);
bool Get_Clean_Filename(char *inName, char *outName, int outNameSize);
// void Jsrand(unsigned int seed);
int Jrand(int min, int max);
void GenerateHashCode(char *hash, int hash_len);
char *Jstrnstr(const char *s, const char *find, size_t buf_size);
char *Jstrnchr(const char *p, char c, size_t buf_size);
size_t Jwcslen(const wchar_t *s);
int Jstricmp(const char *pStr1, size_t Str1_size, const char *pStr2, size_t Str2_size);
int Jstrcmp(const char *str1, const char *str2, size_t str1_size, size_t str2_size);
#define Jstrcpy(x, y) Jstrncpy(x, y, sizeof(x))
char *Jstrncpy(char *strDest, const char *strSource, size_t count);
char *Jstrncpy_s(char *s1, const char *s2, size_t n);
char *Jstrncpy2(char *strDest, const char *strSource, size_t count, size_t strDest_size);
char *Jstrncat(char *strDest, const char *strSource, size_t count);
// char *Jstrncat(char *strDest, const char *strSource, size_t count);
char *Jstrncat2(char *strDest, const char *strSource, size_t count, size_t strDest_size);
char *Jsprintf(char *String, size_t Str_size, const char *format, ...);
int Jsnprintf(char *String, size_t Str_size, const char *format, ...);
char *Jstrftime(char *strDest, size_t maxsize, const char *format, const timeb *tp = NULL);
std::string &Jstringftime(std::string &strDest, const char *format, const timeb *tp);
unsigned long JGetTickCount();
char *Jstrntok(char *s, const char *delim, char **last);
char *Jstrtok(char *string, const char *control);
char *Jstrtok2(char *strToken, const char *strDelimit, int &Pos);
bool IsFileExist(const char *File);
// LPHOSTENT Jgethostbyname(char *s1);
// int Get_LocalIP(char *IP,size_t IP_size);

int Jgetpeername(SOCKET s, sockaddr *name, int *namelen);
int Jgetsockname(SOCKET s, sockaddr *name, int *namelen);
int Jrecvfrom(SOCKET s, char *buf, int len, int flags, sockaddr *from, int *fromlen);
int Jsendto(SOCKET s, const char *buf, int len, int flags, const struct sockaddr *to, int tolen);
SOCKET Jaccept(SOCKET s, sockaddr *addr, int *addrlen);
int Jrecv(SOCKET s, char *buf, int len, int flags);
int Jtlv_recv(SOCKET s, char *buf, int len, int flags);
int Jsend(SOCKET s, const char *buf, int len, int flags);
int Jtlv_send(SOCKET s, const char *buf, int len, int flags);
int Jclosesocket(SOCKET s);
char *Jstrrev(char *string);
BOOL URL_Filter(const char *Url);
bool Port_Filter(unsigned short Port);
#ifdef _MSC_VER
int Get_Modul_FileName(char *szFileName);
int Get_0x15_0x16(char *String, int String_Size, bool Dierct_Flag);
bool Get_Version(const char *Modul_FileName, char *Version, size_t Ver_size);
bool Check_ReExecute();
#else
int GetPrivateProfileString(const char *lpAppName, const char *lpKeyName, const char *lpDefault, char *lpReturnedString, unsigned int nSize, const char *lpFileName);
int GetPrivateProfileInt(const char *lpAppName, const char *lpKeyName, int nDefault, const char *lpFileName);
int CheckPrivateProfileString(std::string &content, const char *lpAppName, const char *lpKeyName, const char *lpFileName);
int WritePrivateProfileString(const char *lpAppName, const char *lpKeyName, const char *lpString, const char *lpFileName);
#endif
FILE *Jfopen(const char *filename, const char *mode);

//------------------------------------------------------------------------------
int Write_Error_Msg(const char *format, ...);
int Create_Directory(const char *szPath, mode_t mode = 0755);
int Remove_End_Char(char *Data);
int Remove_FrontRear_Blank(char *Data, size_t Data_size);
int Get_Table(FILE *Fin, char *Data, size_t Data_size, bool &Comment_Scope_Flag);
unsigned short GetCheckSum(unsigned short *buffer, int len);
int Cat_StreamHeader(unsigned char *Data, size_t Data_size, int Len); //�bMRTP&TCP�Ҧ��A�e����7��byte
bool IsStreamHeader(const unsigned char *data, int data_Len);
int StoreStreamBuf(pSTREAM_BUFFER pSB, const unsigned char *data, int data_Len);
int Div_Stream(unsigned char **OutData, const unsigned char *Src, int Src_Len, int &Src_Cur, pSTREAM_BUFFER pSB);
int Txt_HSF_CP(char *Buf, int Len);
int HSF_Txt_CP(char *Buf, int Len1);
int JTCP_send(SOCKET s, const char *buf, int len, int flags, pSTREAM_BUFFER pSB = NULL);
int JTCP_send1(SOCKET s, const char *buf, int len, int flags);
typedef void (*RECVPROC)(void *Param, SOCKET Socket, char *Out, int OutLen);
int JTCP_recv(SOCKET s, int recvflags, pSTREAM_BUFFER pSB, RECVPROC lpRecvFunc, void *arglist);
int JTCP_recv1(SOCKET s, int recvflags, pSTREAM_BUFFER pSB, RECVPROC lpRecvFunc, void *arglist);
typedef void (*RECVPROC1)(void *Param, SOCKET Socket, char *Out, int OutLen, sockaddr_in *lpAddr, void *lpSIP_Msg);
void TTCP_recvstream(SOCKET s, sockaddr_in *lpAddr, void *lpSIP_Msg, unsigned char *Data, int DataLen, pSTREAM_BUFFER pSB, RECVPROC1 lpRecvFunc, void *arglist);
int Jgethostbyname(char *Domain, char *ip, size_t ip_size);

std::string GetIpFromSockaddr(int af, const sockaddr *sa);
uint16_t GetPortFromSockaddr(int af, const sockaddr *sa);

#define JTransfer_BinaryToHexStr(msg, len) string_JTransfer_BinaryToHexStr(msg, len).c_str()
std::string string_JTransfer_BinaryToHexStr(const char *msg, size_t len);
#define JTransfer_HexNumStrToHexBinary(hex_num_str, len)	string_JTransfer_HexNumStrToHexBinary(hex_num_str, len).c_str()
std::string string_JTransfer_HexNumStrToHexBinary(const char *hex_num_str, size_t len);
double CalClockTime(bool start_flag);
int PrintBinary(const char *msg, int len);
int MyPrintf(const char *fmt, ...);
char *MakeTcpFlagStr(tcphdr *tcp);

#endif //For FUNCTIONS_DEF
