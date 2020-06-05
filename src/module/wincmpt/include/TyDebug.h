/*****************************************************************************
 Debug Function v 1.0
 ======================================
 Last update : 2015.04.23
*****************************************************************************/

#ifndef __TYDEBUG_H__
#define __TYDEBUG_H__

// Debug session
#define DEBUG_FILE_ARRAY {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL}	// Debug FILE pointer array
#define	DEBUG_STRLEN	1024		// Debug string len
#define	DEBUG_LEVEL		0			// Debug level for show trace
#define	DEBUG_OPERATE	>=			// Debug level operate for show trace (ex. level >= DEBUG_LEVEL then show it)
#define WIN_DPRINTF		1			// Using printf to replace TRACE in Windows debug (1 or 0)

// Debug or not (default setting is control by makefile)
#define yDEBUG		(GetDebugState())

// Android log function
#define yLOG_TAG	"ytrace"
#ifdef ANDROID
#include <android/log.h>
#define yLOGV(...)	__android_log_print(ANDROID_LOG_VERBOSE, yLOG_TAG, __VA_ARGS__)
#define yLOGD(...)	__android_log_print(ANDROID_LOG_DEBUG  , yLOG_TAG, __VA_ARGS__)
#define yLOGI(...)	__android_log_print(ANDROID_LOG_INFO   , yLOG_TAG, __VA_ARGS__)
#define yLOGW(...)	__android_log_print(ANDROID_LOG_WARN   , yLOG_TAG, __VA_ARGS__)
#define yLOGE(...)	__android_log_print(ANDROID_LOG_ERROR  , yLOG_TAG, __VA_ARGS__)
#endif

// Debug prior
#define LINFO		0x01
#define LWARN		0x02
#define LERROR		0x03
#define LDEBUG		0x04
#define LVERBOSE	0x05

// Label filter
#define MASK_LVERBOSE	0x01
#define MASK_LDEBUG		0x02
#define MASK_LINFO		0x04
#define MASK_LWARN		0x08
#define MASK_LERROR		0x10

// ytrace function
#ifndef __cplusplus
#define ytrace		if (yDEBUG) printf
#else
//****************************************
#ifdef _MSC_VER
#include <atlbase.h>
#if !WIN_DPRINTF
#define ytrace		if (yDEBUG) ATLTRACE
#else
#define ytrace		if (yDEBUG) printf
#endif
#elif ANDROID
#define ytrace		if (yDEBUG) yLOGD
#else
#define ytrace		if (yDEBUG) printf
#endif
//****************************************
#endif

// Debug sprintf function
#define ysprintf	if (yDEBUG) sprintf

// Other function name for debug trace file
#define	FileTracePath	FiletracePath
#define	FileTrace		Filetrace
#define	NFileTracePath	NFiletracePath
#define	NFileTrace		NFiletrace

// Open Console window in MFC project
#ifndef _MSC_VER
#define	MfcOpenConsole()
#define MfcFreeConsole()
#else
#define MfcFreeConsole	FreeConsole
#define	MfcOpenConsole() \
if (yDEBUG&&AllocConsole()) \
{ \
  freopen("CONIN$", "r+t", stdin); \
  freopen("CONOUT$", "w+t", stdout); \
} else ATLTRACE("Failed to create the console!\r\n")
#endif

// Information route
typedef struct
{
	const char *tag;	// tag
	const char *txt;	// text string contain local time & threading ID
	const char *str;	// text string
	const char *stime;	// local time string
	unsigned int utime;	// standard unix time
	unsigned int tid;	// threading ID
	int prior;			// prior label
}
DLogInfo;
#define DLOG_INFO_MAX_ROUTE_NUM 3

#ifdef __cplusplus
extern "C" {
#endif

// Debug switch setting
void SetDebugOutput(void (*kprint)(char*, int));
void AddDebugInfoRoute(void (*route)(DLogInfo*)); // add a debug information route
void DelDebugInfoRoute(void (*route)(DLogInfo*)); // remove the debug information route
void SetDebugTimeStringUseClocks(volatile char on);
void SetDebugQueue(char* buf, int num, int dlen);
void SetDebugLevel(volatile int level);
void SetDebugSwitch(volatile char on);
void SetDebugOperate(volatile int op); // 0:>=   1:>   2:<=   3:<   4:==   5:!=
void SetDebugMsgMask(volatile int op); // Default value  (show all message): MASK_LVERBOSE|MASK_LDEBUG|MASK_LINFO|MASK_LWARN|MASK_LERROR
void SetDebugFilter(const char* op);   // String example (show all message): "VDIWE" or "vdiwe"
void SetDebugFilterAtConsole(volatile char on); // Default is ON, means filter only controls the console showing
char GetDebugState(void);

// Android log function for compatible
#ifndef ANDROID
void yLOGV(const char *format, ...);	// verbose
void yLOGD(const char *format, ...);	// debug
void yLOGE(const char *format, ...);	// error
void yLOGI(const char *format, ...);	// info
void yLOGW(const char *format, ...);	// warn
#endif

// SDK log function with default TAG (auto change line)
void verlog_w(const char *format, ...);	 // verbose
void sdklog_w(const char *format, ...);	 // debug
void errlog_w(const char *format, ...);	 // error
void warnlog_w(const char *format, ...); // warn
void infolog_w(const char *format, ...); // info

// SDK log function with TAG (auto change line)
void verlog_t(const char *tag, const char *format, ...);  // verbose
void sdklog_t(const char *tag, const char *format, ...);  // debug
void errlog_t(const char *tag, const char *format, ...);  // error
void warnlog_t(const char *tag, const char *format, ...); // warn
void infolog_t(const char *tag, const char *format, ...); // info

// Quick define for SDK log with TAG:
// You need to a macros #define LOCAL_TAG_DEF to a string
#if	defined(_MSC_VER) && _MSC_VER<=1200
#define verlog verlog_w
#define sdklog sdklog_w
#define errlog errlog_w
#define warnlog warnlog_w
#define infolog infolog_w
#else
#define verlog(format, ...) verlog_t(LOCAL_TAG_DEF, format, ##__VA_ARGS__)
#define sdklog(format, ...) sdklog_t(LOCAL_TAG_DEF, format, ##__VA_ARGS__)
#define errlog(format, ...)	errlog_t(LOCAL_TAG_DEF, format, ##__VA_ARGS__)
#define warnlog(format, ...) warnlog_t(LOCAL_TAG_DEF, format, ##__VA_ARGS__)
#define infolog(format, ...) infolog_t(LOCAL_TAG_DEF, format, ##__VA_ARGS__)
#endif

// Break point
#define BREAK_STOP_DBG() BreakStopLog(__func__, __LINE__, 1000)
void BreakStopLog(const char *fnc_name, unsigned int line, unsigned int msec);

// empty function
void EmptyLog(const char *format, ...);

// Debug file function
void FiletracePath(const char* path);
void Filetrace(const char* str, ...);
void NFiletracePath(unsigned int i, const char* path);
void NFiletrace(unsigned int i, const char* str, ...);

// Debug level trace (old API)
void LevelTrace(int level, const char *format, ...);
void LevelPTrace(int level, int prior, const char *format, ...);

#ifdef __cplusplus
}
#endif

#endif
