#ifndef WINCMPT_SRC_INCLUDE_MEMPOOL_H_
#define WINCMPT_SRC_INCLUDE_MEMPOOL_H_

#include "Threading.h"

typedef int (*MDB_THREAD_SAFE_ROUTE)(int idx, void* pt);

///////////////////////////////////////////////////////////////////////////////////////////////
// fast alloc index

typedef struct
{
	int state;
	int allocIdx;
}
PoolState, *LPoolState;

typedef struct
{
	PoolState* Array;
	ymutex* mutexArr;
	unsigned int ArrayNum;
	unsigned int allocPos;
	unsigned int freePos;
	unsigned int count;
	int tempIdx;
	ymutex tLock;
}
FastIdx, *LFastIdx;

///////////////////////////////////////////////////////////////////////////////////////////////
// API

#ifdef __cplusplus
extern "C" {
#endif

int fastIdx_init(LFastIdx obj, PoolState* Array, ymutex* mutexArr, unsigned int ArrayNum); // mutexArr can be null (non-threading save case)
int fastIdx_init2(LFastIdx obj, PoolState* Array, ymutex* mutexArr, unsigned int ArrayNum); // all state is alloced
int fastIdx_alloc(LFastIdx obj, void* ptr, MDB_THREAD_SAFE_ROUTE callback); // return an index, or <0 for error
int fastIdx_free(LFastIdx obj, int idx, void* ptr, MDB_THREAD_SAFE_ROUTE callback); // return an index, or <0 for error
int fastIdx_route(LFastIdx obj, int idx, void* ptr, MDB_THREAD_SAFE_ROUTE stateT, MDB_THREAD_SAFE_ROUTE stateF); // stateT: if state is true:1, tateF: return value base on callback return value
int fastIdx_route_delete(LFastIdx obj, int idx); // like fastIdx_free() but use it in the route callback
void fastIdx_lock(LFastIdx obj, int idx);
void fastIdx_unlock(LFastIdx obj, int idx);
void fastIdx_deinit(LFastIdx obj);

#ifdef __cplusplus
}
#endif

#endif /* WINCMPT_SRC_INCLUDE_MEMPOOL_H_ */
