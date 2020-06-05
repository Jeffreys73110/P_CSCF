// DTrace.h

#ifndef	_DTRACE_HEADER_DEF
#define	_DTRACE_HEADER_DEF

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif


#include <string.h>
#include <string>
#include <sys/timeb.h>
#include <time.h>
#include <stdio.h>
#include <assert.h>
#include "AnsiColor.h"
#include "../Functions/Functions.h"
#include "../wincmpt/include/WinCompatibility.h"
#include "../netcpi/include/NetInterface.h"

#ifndef _MSC_VER
	#define	OutputDebugString(x)	printf("%s",x)
	#define	DeleteFile(x) 			remove(x)
	#define	_rmdir(x)				rmdir(x)
	#define	PERROR(x)				DTRACE(DTL_Error, x "%s\n", strerror(errno))
#endif

#define	DTrace_FatalError(...)	DTRACE(DTL_FatalError, __VA_ARGS__)
#define	DTrace_Error(...)		DTRACE(DTL_Error, __VA_ARGS__)
#define	DTrace_Warning(...)		DTRACE(DTL_Warning, __VA_ARGS__)
#define	DTrace_SqlQuery(...)	DTRACE(DTL_SqlQuery, __VA_ARGS__)
#define	DTrace_CommonMsg(...)	DTRACE(DTL_CommonMsg, __VA_ARGS__)
#define	DTrace_SystemMsg(...)	DTRACE(DTL_SystemMsg, __VA_ARGS__)
#define	DTrace_TestMsg(...)		DTRACE(DTL_TestMsg, __VA_ARGS__)
#define	DTrace_StateMsg(...)	DTRACE(DTL_StateMsg, __VA_ARGS__)

enum	DTRACE_LEVEL	{ DTL_AllTrace, DTL_FatalError, DTL_Error, DTL_Warning, DTL_CrashDebug, DTL_SqlQuery, DTL_CommonMsg, DTL_SystemMsg, DTL_TestMsg, DTL_StateMsg, DTL_SIZE };
const char	szDTRACE_Level[][20] = {"AllTrace", "FatalError", "Error", "Warning", "CrashDebug", "SqlQuery", "CommonMsg", "SystemMsg", "TestMsg","StateMsg"};
const ColorParam_t DTRACE_Color[] = 
{ 
	{CM_Reset, TM_Dft, FG_Dft, BG_Dft}, 		// DTL_AllTrace
	{CM_Light, TM_BlinkSlow, FG_Red, BG_Dft}, 	// DTL_FatalError
	{CM_Light, TM_Dft, FG_Red, BG_Dft}, 		// DTL_Error
	{CM_Dark, TM_Dft, FG_Yellow, BG_Dft}, 		// DTL_Warning
	{CM_Reset, TM_Dft, FG_Dft, BG_Dft}, 		// DTL_CrashDebug
	{CM_Light, TM_Dft, FG_Green, BG_Dft}, 		// DTL_SqlQuery
	{CM_Dark, TM_Dft, FG_Blue, BG_Dft}, 		// DTL_CommonMsg
	{CM_Light, TM_Dft, FG_Blue, BG_Dft}, 		// DTL_SystemMsg
	{CM_Reset, TM_Dft, FG_Blue, BG_Dft}, 		// DTL_TestMsg
	{CM_Reset, TM_Dft, FG_Dft, BG_Dft}, 		// DTL_StateMsg
};


typedef struct _DTRACE_SECTION
{
	//----- MediaControl -----
	bool	AllTraceFlag;
	bool	FatalErrorFlag;
	bool	ErrorFlag;
	bool	WarningFlag;
	bool	CrashDebugFlag;
	bool	SqlQueryFlag;
	bool	CommonMsgFlag;
	bool	SystemMsgFlag;
	bool	TestMsgFlag;
	bool	StateMsgFlag;

	_DTRACE_SECTION()	{ reset(); }
	void reset()	
	{ 
		memset(this, 0, sizeof(_DTRACE_SECTION)); 
	}

} DTRACE_SECTION;


#define		CRASH_DEBUG			TRUE
#if	CRASH_DEBUG
	#define		CRASH_DEBUG_COMMAND		DTRACE(DTL_CrashDebug, "%5d @ %s\r\n", __LINE__, __FILE__)
	#define		RETURN					DTRACE(DTL_CrashDebug, "%5d @ %s\r\n", __LINE__, __FILE__);	return
#else
	#define		CRASH_DEBUG_COMMAND
	#define		RETURN			return
#endif



// Function
char* GetFTracePath();
int SetFTracePath(char *path, int server_id);
int	SetDTraceFlag(DTRACE_LEVEL, bool flag);
int	SetFTraceFlag(DTRACE_LEVEL, bool flag);
bool	IsDTrace(DTRACE_LEVEL Level);
bool	IsFTrace(DTRACE_LEVEL Level);
int	DTRACE(const char *szFmt, ...);
int	DTRACE(DTRACE_LEVEL Level, const char *szFmt, ...);
int DTraceColor();


#endif	// _DTRACE_HEADER_DEF
