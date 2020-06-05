#ifndef __BASEINCLUDE_H__
#define __BASEINCLUDE_H__

// avoid conflicts for Windows define
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

// avoid LiteOS define __linux__
#ifdef __LITEOS__
#undef __linux__
#endif

// -------------------- Windows include -------------------- //
#if	defined(_MSC_VER)

/*	wsock32.lib is old version 1.1,
 *	Ws2_32.lib is new version 2.0.
 *	use wsock32.lib & winsock.h together,
 *	use Ws2_32.lib & winsock2.h together.
 *	winsock.h and winsock2.h can not be used together,
 *	winsock.h is replaced by winsock2.h¡A
 *	winsock2.h is not a extension of winsock.h */

#pragma warning (disable:4786)
#pragma comment (lib, "Ws2_32")

// standard include
#define STANDARD_C_INCLUDE
#define ALLOC_IMPLEMENT

// special include
#include <windows.h>
#include <direct.h>
#include <sys/timeb.h>

// network include
#include <winsock2.h>
#include <ws2tcpip.h>

// VC6 addrinfo patch
#include "VC6Addrinfo.h"

// -------------------- Free BSD include -------------------- //
#elif defined(__FreeBSD__)

// standard include
#define STANDARD_C_INCLUDE
#define STANDARD_UNIX_INCLUDE
#define ALLOC_IMPLEMENT

// network include
#include <sys/types.h>
#include <net/if_dl.h>

// -------------------- OSX include -------------------- //
#elif defined(__MACH__)

// standard include
#define STANDARD_C_INCLUDE
#define STANDARD_UNIX_INCLUDE
#define ALLOC_IMPLEMENT

// special include
#include <mach/mach.h>
#include <mach/task.h>
#include <mach/clock.h>

// network include
#include <sys/types.h>
#include <net/if_dl.h>

// -------------------- Android include -------------------- //
#elif defined(__ANDROID__)

// standard include
#define STANDARD_C_INCLUDE
#define STANDARD_UNIX_INCLUDE
#define ALLOC_IMPLEMENT

// -------------------- Linux include -------------------- //
#elif defined(__linux__)

// standard include
#define STANDARD_C_INCLUDE
#define STANDARD_UNIX_INCLUDE
#define ALLOC_IMPLEMENT

// -------------------- Other OS include -------------------- //
#else

// specify a special define for unusual OS
#define UNUSUAL_OS
// unusual OS platform bridging header
#include "../unusual-os/bridging.h"
// standard library name definition
#include "../unusual-os/standardlib.h"

#endif

// -------------------- Standard C include -------------------- //
#ifdef STANDARD_C_INCLUDE
#include <time.h>
#include <errno.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#endif
// -------------------- Standard C++ include -------------------- //
#ifdef __cplusplus
#include <iostream>
#include <vector>
#include <list>
#include <map>
#endif
// -------------------- Standard Unix include -------------------- //
#ifdef STANDARD_UNIX_INCLUDE
// Unix standard
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <sys/stat.h>
// network include ( <ifaddrs.h> is not standard part )
#include <netdb.h>
#include <resolv.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
// multi threading include
#include <semaphore.h>
#include <pthread.h>
#endif

/////////////////////////////////////////////////////////////////////

#ifndef FAR
#define FAR
#endif

// command line for grep
#if	defined(_MSC_VER)
#define TM_COMMAND_GREP		"findstr"
#elif defined(STANDARD_UNIX_INCLUDE)
#define TM_COMMAND_GREP		"grep"
#else
#define TM_COMMAND_GREP		""
#endif

// different name for #define ALLOC_IMPLEMENT
#ifdef ALLOC_IMPLEMENT
#ifndef CMALLOC
#define	CMALLOC		ALLOC_IMPLEMENT
#endif
#endif

// Vector, list, map compatibility definition
#ifdef __cplusplus
#define TPair(x,y)		std::pair<x, y>
#define TMap(x,y)		std::map<x, y>
#define TVector(x)		std::vector<x>
#define TList(x)		std::list<x>
#define TItor(x)		x::iterator
#define TRitor(x)		x::reverse_iterator
#else
#define TPair(x,y)		char
#define TMap(x,y)		char
#define TVector(x)		char
#define TList(x)		char
#define TItor(x)		char
#define TRitor(x)		char
#endif

// mkdir compatibility
#if	defined(_MSC_VER)
#define ymkdir(dir)		mkdir(dir)
#elif defined(STANDARD_UNIX_INCLUDE)
#define ymkdir(dir)		mkdir(dir, 0777)
#else
#define ymkdir(dir)
#endif

// Get system information compatibility
#if	defined(_MSC_VER)
#define gettid()		GetCurrentThreadId()
#define getcpunum()		_WIN_getcpunum()
#elif defined(__MACH__)
#define gettid()		_OSX_getthreadid()
#define getcpunum()		sysconf(_SC_NPROCESSORS_ONLN)
#elif defined(__ANDROID__)
#define gettid()		syscall(__NR_gettid)
#define getcpunum()		sysconf(_SC_NPROCESSORS_ONLN)
#elif defined(__linux__)
#define gettid()		syscall(__NR_gettid)
#define getcpunum()		sysconf(_SC_NPROCESSORS_ONLN)
#else
#define gettid()		uos_get_threadID()
#define getcpunum()		1
#endif

// Special function definition
#if defined(_MSC_VER) && !defined(EFIX64) && !defined(EFI32)
#if !defined  snprintf
#define	snprintf _WIN_snprintf
#endif
#if !defined vsnprintf
#define	vsnprintf _WIN_vsnprintf
#endif
#define strcasecmp _stricmp
#define strncasecmp _strnicmp
#endif

// special function for binding FILE (RTOS) or something
#ifndef UNUSUAL_OS
#define	fbind(x) // no use now, old version....
#define	uos_socket_lock(a) 0
#define	uos_socket_unlock()
#define FILE_poollink(x, y) 0
#define uos_is_alloc_sram()	0
#define uos_is_multi_thread() 1
#define HUGE_ARRAY
#define utime_t time_t
#endif

// Type define compatibility
#include "TypeDefine.h"

// Common Structure
typedef struct 
{
	// timeb style for ftime()
	utime_t time;
	unsigned short millitm;

	// timeval style for gettimeofday()
	utime_t tv_sec;
	long tv_usec;
}
TimeB;
typedef struct 
{
	int		sys;
	TimeB	t1;
	TimeB	t2;
} 
ytime_dist;

/////////////////////////////////////////////////////////////////////
// API

#ifdef __cplusplus
extern "C" {
#endif

// test big endian problem
unsigned char is_memory_inverse(void);
#define is_big_endian	is_memory_inverse	// other name

// translate the string to positive or negative integer
int mix_atoi(const char *str);

// clock timestamp
unsigned int GetClockTimeStamp(void); // return msec

// Time B function
void ftimeB(TimeB *lp);
void GetCurrentMTime(utime_t* sec, unsigned short* msec);
void StartTimeDist(ytime_dist* lp);
utime_t FinishTimeDist(ytime_dist* lp);	// return msec

// gettimeofday() for windows
#if defined(_MSC_VER)
struct timezone
{
	int  tz_minuteswest;	// minutes W of Greenwich
	int  tz_dsttime;		// type of dst correction
};
int gettimeofday(struct timeval *tv, struct timezone *tz);
#endif

// special function
char* oxoo(char* buf, unsigned int bsize, char val);
char* zerop(char* str, unsigned int bsize, int vInt, unsigned int zeroNum);
char* dftoa(char* buf, unsigned int bsize, double val);

int Ncommand(const char *S1, char *Command, char Flag, int Command_Num, int Command_Len);
int NCommandCutter(char* str, char cut_char, char* ptr[], int ptr_num);
void NCommandRecover(char* str, char cut_char, int cut_char_num);

// popen function
void* cmd_popen(const char *command);
char* cmd_fgets(char *str, int num, void* stream);
int cmd_pclose(void* stream);

// windows function
#ifdef _MSC_VER
int _WIN_getcpunum(void);
int _WIN_snprintf(char* str, unsigned int size, const char* format, ...);
int _WIN_vsnprintf(char* str, unsigned int size, const char* format, va_list ap);
#endif

// apple function
#ifdef __MACH__
unsigned long _OSX_getthreadid(void);
#endif

#ifdef __cplusplus
}
#endif

#endif // #ifndef __BASEINCLUDE_H__
