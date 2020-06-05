// DTrace.cpp
//#include "StdAfx.h"
#include "DTrace.h"
#define CLEAR_DAYs_AGO 3
// bool	DTraceFlag[DTL_SIZE] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
bool DTraceFlag[DTL_SIZE] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
bool FTraceFlag[DTL_SIZE] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
FILE *fp = NULL;
timeb file_time;
tm file_tm;
ymutex file_mutex_lock;
bool file_mutex_lock_init_flag = false;
static char ftrace_path[256] = "./";
int sid = 1;
int SetDTraceFlag(DTRACE_LEVEL Level, bool flag)
{
	DTraceFlag[Level] = flag;
	return 0;
}
int SetFTracePath(char *path, int server_id)
{
	if (!path || path[0] == 0)
	{
		RETURN - 1;
	}
	int len = strlen(path) - 1;
	Jsprintf(ftrace_path, sizeof(ftrace_path) - 1, "%s", path);
	len = len <= 0 ? 0 : len;
	if (path[len] == '\\' || path[len] == '/')
		ftrace_path[len] = 0;

	file_tm.tm_mday = 0;
	sid = server_id;
	return 0;
}
char *GetFTracePath()
{
	return ftrace_path;
}
int SetFTraceFlag(DTRACE_LEVEL Level, bool flag)
{
	FTraceFlag[Level] = flag;
	return 0;
}

bool IsDTrace(DTRACE_LEVEL Level)
{
	if (!DTraceFlag[DTL_AllTrace])
		return false;
	else
		return DTraceFlag[Level];
}

bool IsFTrace(DTRACE_LEVEL Level)
{
	if (!FTraceFlag[DTL_AllTrace])
		return false;
	else
		return FTraceFlag[Level];
}

int DTRACE(const char *szFmt, ...)
{
	if (!DTraceFlag[DTL_AllTrace])
		return 0;
	if (!DTraceFlag[DTL_TestMsg])
		return 0;

	std::string strdebug;
	va_list args;
	va_start(args, szFmt);
	char szBuffer[50000];
	size_t buf_size = sizeof(szBuffer) - 1;
	int nLen;
	nLen = _vsntprintf(szBuffer, sizeof(szBuffer), szFmt, args);
	if (nLen == -1)
		szBuffer[buf_size] = 0;
	va_end(args);
	strdebug = "[";
	strdebug += szDTRACE_Level[DTL_TestMsg];
	strdebug += "]  ";
	strdebug += szBuffer;
	OutputDebugString(strdebug.c_str());

	return 0;
}

int DTRACE(DTRACE_LEVEL Level, const char *szFmt, ...)
{
	bool DFlag = IsDTrace(Level);
	bool FFlag = IsFTrace(Level);
	char TimeBuf[1024] = {0};
	char tid[32] = {0};
	if (!DFlag && !FFlag)
		return 0;
	if (!file_mutex_lock_init_flag)
	{
		MutexInit(&file_mutex_lock);
		file_mutex_lock_init_flag = true;
	}

	std::string strdebug;
	va_list args;
	char szBuffer[50000] = {0};
	size_t buf_size = sizeof(szBuffer) - 1;
	int nLen;

	timeb current_time, three_day_time;
	tm *ptm;
	va_start(args, szFmt);
	nLen = _vsntprintf(szBuffer, sizeof(szBuffer), szFmt, args);
	va_end(args);
	if (nLen == -1)
		szBuffer[buf_size] = 0;
	_snprintf(tid, sizeof(tid), "%ld", gettid());
	strdebug = tid;
	strdebug += "[";
	// strdebug += COLOR_STR(CM_Light, TM_Dft, FG_Red, BG_Dft, szDTRACE_Level[Level]);
	// strdebug += ColorString(DEF_ColorParam(Level), szDTRACE_Level[Level]).c_str();
	strdebug += COLOR_STR(DTRACE_Color[Level], szDTRACE_Level[Level]);
	strdebug += "]  ";
	strdebug += szBuffer;
	if (DFlag)
		OutputDebugString(strdebug.c_str());
	ftime(&current_time);
	ptm = localtime(&current_time.time);
	MutexLock(&file_mutex_lock);
	if (ptm->tm_mday != file_tm.tm_mday || ptm->tm_hour != file_tm.tm_hour)
	{
		//clear log
		char TimeBuf1[64] = {0}, TimeBuf2[64] = {0};
		char file_name[256] = {0}, old_fname[256] = {0}, file_name2[256] = {0};
		ftime(&file_time);
		//save now() to file_tm
		memcpy(&file_tm, localtime(&file_time.time), sizeof(file_tm));
		int i = strlen(ftrace_path) - 1;
		i = (i < 0) ? 0 : i;
		if (ftrace_path[i] == '\\' || ftrace_path[i] == '/')
			ftrace_path[i] = 0;

		Jstrftime(TimeBuf1, sizeof(TimeBuf1), "%Y-%m-%d", &current_time);
		Jstrftime(TimeBuf2, sizeof(TimeBuf2), "%Y-%m-%d/%H clock.txt", &current_time);

		_snprintf(file_name, sizeof(file_name) - 1, "%s/FileTraceLog/%d/%s", ftrace_path, sid, TimeBuf1);
		//create file
		if (!IsFileExist(file_name))
			Create_Directory(file_name);
		//log file process
		if (fp)
		{
			if (!fclose(fp))
				fp = NULL;
		}
		if (!fp)
		{
			Jsprintf(file_name2, sizeof(file_name2) - 1, "%s/FileTraceLog/%d/%s", ftrace_path, sid, TimeBuf2);
			if (!(fp = fopen(file_name2, "ab")))
			{
				DTRACE("****************************Failed to DTRACE OPEN %s**************************\r\n", file_name2);
			}
		}

		//clear three days log;
		//get three days ago log file.;
		three_day_time.time = current_time.time - (24 * CLEAR_DAYs_AGO * 3600);
		char tmp_time[64] = {0};
		_snprintf(old_fname, sizeof(old_fname) - 1, "%s/FileTraceLog/%d/", ftrace_path, sid);
		Jstrftime(tmp_time, sizeof(tmp_time), "%Y-%m-%d/%H clock.txt", &three_day_time);
		Jstrncat(old_fname, tmp_time, sizeof(old_fname));
		DeleteFile(old_fname);
		if (ptm->tm_hour == 23)
		{
			_snprintf(old_fname, sizeof(old_fname) - 1, "%s/FileTraceLog/%d/", ftrace_path, sid);
			Jstrftime(tmp_time, sizeof(tmp_time), "%Y-%m-%d", &three_day_time);
			Jstrncat(old_fname, tmp_time, sizeof(old_fname));
			_rmdir(old_fname);
		}
	}

	if (FFlag)
	{
		if (fp)
		{
			Jstrftime(TimeBuf, sizeof(TimeBuf), "%Y/%m/%d %H:%M:%S.%f\t", &current_time);
			fwrite(TimeBuf, 1, strlen(TimeBuf), fp);
			fwrite(strdebug.c_str(), 1, strdebug.length(), fp);
			fflush(fp);
		}
	}
	MutexUnlock(&file_mutex_lock);
	if (Level == DTL_FatalError)
	{
		assert(0);
	}

	return 0;
}

int DTraceColor()
{
	printf("DTRACE Color %s\n", COLOR_STR(DTRACE_Color[DTL_AllTrace], "DTL_AllTrace"));
	printf("DTRACE Color %s\n", COLOR_STR(DTRACE_Color[DTL_FatalError], "DTL_FatalError"));
	printf("DTRACE Color %s\n", COLOR_STR(DTRACE_Color[DTL_Error], "DTL_Error"));
	printf("DTRACE Color %s\n", COLOR_STR(DTRACE_Color[DTL_Warning], "DTL_Warning"));
	printf("DTRACE Color %s\n", COLOR_STR(DTRACE_Color[DTL_CrashDebug], "DTL_CrashDebug"));
	printf("DTRACE Color %s\n", COLOR_STR(DTRACE_Color[DTL_SqlQuery], "DTL_SqlQuery"));
	printf("DTRACE Color %s\n", COLOR_STR(DTRACE_Color[DTL_CommonMsg], "DTL_CommonMsg"));
	printf("DTRACE Color %s\n", COLOR_STR(DTRACE_Color[DTL_SystemMsg], "DTL_SystemMsg"));
	printf("DTRACE Color %s\n", COLOR_STR(DTRACE_Color[DTL_TestMsg], "DTL_TestMsg"));
	printf("DTRACE Color %s\n", COLOR_STR(DTRACE_Color[DTL_StateMsg], "DTL_StateMsg"));

	return 0;
}