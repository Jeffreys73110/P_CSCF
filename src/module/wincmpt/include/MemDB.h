#ifndef __MEMXXDB_H__
#define __MEMXXDB_H__

#include "MemPool.h"

///////////////////////////////////////////////////////////////////////////////////////////////
// memory database

typedef struct
{
	// main table setting
	void* table;			// table base pointer
	ymutex* mutexArr;		// mutex array
	PoolState* statePool;	// alloc pool (array)
	unsigned int tableNum;	// maximum number of inserted data

	// field setting
	unsigned int fieldSize;	// total field size for per inserted data
	unsigned int keySize;	// size of key
	void* first_lpkey;		// first key pointer from the table
}
MemDBSet;

typedef struct
{
	FastIdx			fIdx;
	unsigned char* 	table;
	unsigned int	fieldSize;
	unsigned int	keySize;
	unsigned int	keyPos;
}
MemDB, *LMemDB;

///////////////////////////////////////////////////////////////////////////////////////////////
// API

#ifdef __cplusplus
extern "C" {
#endif

int memdb_init(LMemDB obj, MemDBSet op);
int memdb_exist(LMemDB obj, const void* key); // return (index+1):exist, <0:none-exist
int memdb_insert(LMemDB obj, const void* field); // need to call memdb_exist() to check the key
int memdb_query(LMemDB obj, const void* key, void* ptr, MDB_THREAD_SAFE_ROUTE callback);
int memdb_erase(LMemDB obj, const void* key, void* ptr, MDB_THREAD_SAFE_ROUTE callback);
int memdb_route(LMemDB obj, int idx, void* ptr, MDB_THREAD_SAFE_ROUTE callback);
int memdb_route_delete(LMemDB obj, int idx); // like memdb_erase() but use it in the route callback
void memdb_lock(LMemDB obj, int idx);
void memdb_unlock(LMemDB obj, int idx);

#ifdef __cplusplus
}
#endif

#endif // __MEMXXDB_H__
