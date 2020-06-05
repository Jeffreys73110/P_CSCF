#ifndef __THREADING_H__
#define __THREADING_H__

// avoid conflicts for Windows define
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

// avoid LiteOS define __linux__
#ifdef __LITEOS__
#undef __linux__
#endif

// Windows threading include
#if	defined(_MSC_VER)
#include <windows.h>

// Unix base threading include
#elif defined(__linux__)||defined(__FreeBSD__)||defined(__MACH__)||defined(ANDROID)
#define STANDARD_UNIX_TRD_INCLUDE
#include <semaphore.h>
#include <pthread.h>

// Other OS threading include
#else
#ifndef UNUSUAL_OS
#define UNUSUAL_OS
#endif
#include "../unusual-os/upthread.h"

#endif

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Special structure
#if defined(_MSC_VER)
typedef struct
{
	unsigned char recursive; // 0 or 1
	CRITICAL_SECTION crit;
	HANDLE sem;
}
WinMutex;
typedef struct 
{
	unsigned int waiters_count;
	WinMutex waiters_count_lock;
	HANDLE signal_event;
	HANDLE broadcast_event;
} 
WinCond;
typedef struct
{
	HANDLE handle;
	unsigned char created;
	unsigned char detached;
	unsigned char r_flag_0;
	unsigned char r_flag_1;
	void* (*func)(void*);
	void* usr_ptr;
}
WinThreading;
#elif defined(STANDARD_UNIX_TRD_INCLUDE)
typedef struct
{
	unsigned char recursive; // 0 or 1
	pthread_mutex_t lock;
	unsigned long owner; // owner thread ID
	unsigned int cnt;
}
UnixMutex;
typedef struct
{
	pthread_cond_t cond;
	UnixMutex lock;
	unsigned int count;
}
CondSem;
#else
#endif

// Unified interface by definition
#if defined(_MSC_VER)
#define	ypthread_t		WinThreading
#define	ythred			void*
#define	ycond			WinCond
#define ymutex			WinMutex
#define ysemaphore		HANDLE
#elif defined(STANDARD_UNIX_TRD_INCLUDE)
#define	ypthread_t		pthread_t
#define	ythred			void*
#define	ycond			pthread_cond_t
#define ymutex			UnixMutex
#define ysemaphore		CondSem
#else
#define	ypthread_t		uthread_t
#define	ythred			uthred
#define	ycond			ucond_t
#define ymutex			umutex_t
#define ysemaphore		usem_t
#endif

#define MAX_SEMCONT		2147483647
#define INFINITE_CWAIT	-1

// Queue list system
#define MAX_CQL_THREADING	10
#define MAX_INIT_QUEUE_LEN	10
typedef void (*CQLIST_CALLBACK)(unsigned int queueID, unsigned int postID, void* lpValue, void* lpObj);
struct QList
{
	void*	lpValue;
	void*	lpNext;	// next QList
};
typedef struct 
{
	unsigned int	vsign;  // 20151120
	unsigned int	tnumm;
	unsigned int	queueID;

	struct QList*	vlist;		// forward pointer
	struct QList*	vpush;		// push pointer
	struct QList*	vqueueAL;	// alloc style
	struct QList	vqueue[MAX_INIT_QUEUE_LEN];
	unsigned int	QListNum;
	unsigned int	QListLen;

	ypthread_t		vthread[MAX_CQL_THREADING];
	ymutex			vlock;
	ysemaphore		vsemaphore;
	unsigned char	vloopBreaker;
	CQLIST_CALLBACK	callback;
	void*			lpObj;
} 
CQueueList;

// sleep function
#if	defined(_MSC_VER)
#define	ysleep(x)	Sleep(x)
#elif defined(STANDARD_UNIX_TRD_INCLUDE)
#define ysleep(x)	usleep(x*1000)
#else
#define ysleep(x)	uos_sleep(x)
#endif

#ifdef RENAME_MUTEX_API
#define MutexInit	_xMutexInit
#define MutexRInit	_xMutexRInit
#define MutexLock	_xMutexLock
#define MutexUnlock	_xMutexUnlock
#define MutexFree	_xMutexFree
#endif

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
extern "C" {
#endif

// Thread function
int yCreateThread(ypthread_t* output, int stack_ksize, ythred (*func)(void*), void* args);
int yCreateDThread(ypthread_t* output, int stack_ksize, ythred (*func)(void*), void* args); // detached
int yThreadJoin(ypthread_t input, void **retval);

// Mutex lock ( 'R' means Recursive )
int MutexRInit(ymutex* lplock);
int MutexInit(ymutex* lplock);
int MutexLock(ymutex* lplock);
int MutexUnlock(ymutex* lplock);
int MutexFree(ymutex* lplock);

// Semaphore
int SemaphoreInit(ysemaphore* lpsem, unsigned int intitvalue/*=0*/);
int SemaphoreWait(ysemaphore* lpsem, int msecs/*=INFINITE_CWAIT*/);
int SemaphorePost(ysemaphore* lpsem);
int SemaphoreFree(ysemaphore* lpsem);

// Condition variable
int CondInit(ycond* lpcond);
int CondWait(ycond* lpcond, ymutex* lock, int msecs/*=INFINITE_CWAIT*/);
int CondSignal(ycond* lpcond);
int CondBroadcast(ycond* lpcond);
int CondFree(ycond* cond);

// Queue list function
void QListInit(CQueueList* lp, CQLIST_CALLBACK callback, void* lpObj, unsigned int queueID, unsigned int tnum, unsigned int tsize);
void QListPost(CQueueList* lp, void* pushValue, unsigned int msize, unsigned int postID);
void QListFree(CQueueList* lp);

#ifdef __cplusplus
}
#endif

#endif // #ifndef __THREADING_H__
