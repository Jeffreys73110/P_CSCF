#ifndef BUILD__WIN_COMPILER_TEMP_RBTREE_H_
#define BUILD__WIN_COMPILER_TEMP_RBTREE_H_

#include "MemPool.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Core

typedef struct RBTreeNode
{
    unsigned char		color;
    unsigned int		idx;
    unsigned int		key[4];
    struct RBTreeNode*	left;
    struct RBTreeNode*	right;
    struct RBTreeNode*	parent;
}
RBTNode, *RBTree;

typedef struct rb_root
{

	RBTNode*		node;		// root
	RBTNode*		nodePool;	// node pool (array)
	ymutex			RBmutex;
	FastIdx			m_allocator;
	unsigned char* 	table;
	unsigned int	fieldSize;	// contain RBTNode & key field
	unsigned int	keySize;
	unsigned int	keyPos;
}
RBRoot, *LRBRoot;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Setting parameter

typedef struct
{
	void* table;			// table base pool array
	ymutex* mutexArr;		// mutex pool array
	RBTNode* nodeArr;		// rbtree node pool array
	PoolState* stateArr;	// allocator pool array
}
RBRootPool;

typedef struct
{
	RBRootPool pool;
	unsigned int tableNum;	// maximum number of inserted data
	unsigned int fieldSize;	// total field size for per inserted data (contain key field)
	unsigned int keySize;	// size of key
	void* first_lpkey;		// first key pointer from the table
}
RBRootSet;

typedef struct
{
	int*			idxArr;
	char			allocate;
	unsigned int	ArrNum;
	unsigned int	nCnt;
}
RBIter;

typedef enum
{
	RBT_PREORDER  = 0,
	RBT_INORDER   = 1,
	RBT_POSTORDER = 2
}
BRTItStyle;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// API

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************************
 * function: rbtree_pool_malloc()
 * description:
 *      auto create some memory pools by allocating memory
 * parameters:
 *      pool      (IN/OUT):   pool object pointer
 *      num       (IN)    :   maximum number of element (insertable data)
 *      fieldSize (IN)    :   total field size for per inserted data (contain key field)
 * return:
 *      If success, it will return 0, or return -1 for error
 ***********************************************************************/
int rbtree_pool_malloc(RBRootPool* pool, unsigned int num, unsigned int fieldSize);

/***********************************************************************
 * function: rbtree_pool_free()
 * description:
 *      auto free allocated memory pools by calling free()
 * parameters:
 *      pool      (IN/OUT):   pool object pointer
 * return:
 *      If success, it will return 0, or return -1 for error
 ***********************************************************************/
int rbtree_pool_free(RBRootPool* pool);

/***********************************************************************
 * function: rbtree_init()
 * description:
 *      initialize the red black tree
 * parameters:
 *      obj (IN/OUT) :   red black tree object pointer
 *      op      (IN) :   parameter set
 * return:
 *      If success, it will return 0, or return -1 for error
 ***********************************************************************/
int rbtree_init(LRBRoot obj, RBRootSet op);

/***********************************************************************
 * function: rbtree_deinit()
 * description:
 *      de-initialize the red black tree
 * parameters:
 *      op      (IN) :   parameter set
 * note:
 *      This API will not help you to release allocated pool.
 ***********************************************************************/
void rbtree_deinit(LRBRoot obj);

/***********************************************************************
 * function: rbtree_exist()
 * description:
 *      detect the key value is existed in the tree or not
 * parameters:
 *      obj (IN/OUT) :   red-black tree object pointer
 *      key     (IN) :   a key you want to search
 * return:
 *      If success, it will return an element index of pool,
 *      or return a negative value
 * note:
 *      The input parameter key pointer is copied not referenced style
 ***********************************************************************/
int rbtree_exist(LRBRoot obj, const void* key);

/***********************************************************************
 * function: rbtree_insert()
 * description:
 *      insert data into the red-black tree
 * parameters:
 *      obj (IN/OUT) :   red black tree object pointer
 *      field   (IN) :   a field (contain key) you want to insert
 * return:
 *      If success, it will return an element index of pool,
 *      or return a negative value
 * note:
 *      The input parameter field pointer is copied not referenced style
 ***********************************************************************/
int rbtree_insert(LRBRoot obj, const void* field);

/***********************************************************************
 * function: rbtree_route()
 * description:
 *      go into a data element threading safe route by a specified index
 * parameters:
 *      obj  (IN/OUT) :   red black tree object pointer
 *      idx      (IN) :   specified index you want to call by threading safe
 *      ptr      (IN) :   a pointer will used in callback
 *      callback (IN) :   threading safe route
 * return:
 *      If success, it will return a value base on callback's return value,
 *      or return a negative value
 ***********************************************************************/
int rbtree_route(LRBRoot obj, int idx, void* ptr, MDB_THREAD_SAFE_ROUTE callback);

/***********************************************************************
 * function: rbtree_delete()
 * description:
 *      delete a data element by a specified index
 * parameters:
 *      obj  (IN/OUT) :   red black tree object pointer
 *      idx      (IN) :   specified index you want to delete
 *      ptr      (IN) :   a pointer will used in callback
 *      callback (IN) :   threading safe route (can be NULL)
 * return:
 *      If success, it will return the specified element index,
 *      or return a negative value
 ***********************************************************************/
int rbtree_delete(LRBRoot obj, int idx, void* ptr, MDB_THREAD_SAFE_ROUTE callback);

/***********************************************************************
 * function: rbtree_query()
 * description:
 *      query a data element by a specified key
 * parameters:
 *      obj  (IN/OUT) :   red black tree object pointer
 *      key      (IN) :   specified key you want to search
 *      ptr      (IN) :   a pointer will used in callback
 *      callback (IN) :   threading safe route
 * return:
 *      If success, it will return a value base on callback's return value,
 *      or return a negative value
 * note:
 *      The input parameter field pointer is copied not referenced style
 ***********************************************************************/
int rbtree_query(LRBRoot obj, const void* key, void* ptr, MDB_THREAD_SAFE_ROUTE callback);

/***********************************************************************
 * function: rbtree_erase()
 * description:
 *      delete a data element by a specified key
 * parameters:
 *      obj  (IN/OUT) :   red black tree object pointer
 *      key      (IN) :   specified key you want to delete
 *      ptr      (IN) :   a pointer will used in callback
 *      callback (IN) :   threading safe route (can be NULL)
 * return:
 *      If success, it will return the deleted element's index,
 *      or return a negative value
 * note:
 *      The input parameter field pointer is copied not referenced style
 ***********************************************************************/
int rbtree_erase(LRBRoot obj, const void* key, void* ptr, MDB_THREAD_SAFE_ROUTE callback);

/***********************************************************************
 * function: rbtree_iterator()
 * description:
 *      prepare to traverse/iterate the red black tree
 * parameters:
 *      obj  (IN/OUT) :   red black tree object pointer
 *      it   (IN/OUT) :   iterator object pointer
 *      style    (IN) :   traverse/iterate method
 * return:
 *      If success, it will return 0, or return -1 for error
 * note:
 *      Before calling this API, you need to initialize the iterator.
 *      Please reference: rbiterator_malloc() or rbiterator_melink()
 ***********************************************************************/
int rbtree_iterator(LRBRoot obj, RBIter* it, BRTItStyle style);

/***********************************************************************
 * function: rbtree_count()
 * description:
 *      get a number of inserted element in the tree
 * parameters:
 *      obj  (IN/OUT) :   red black tree object pointer
 * return:
 *      the number of element in the tree
 ***********************************************************************/
unsigned int rbtree_count(LRBRoot obj);

/***********************************************************************
 * function: rbtree_poolsize()
 * description:
 *      get the maximum pool size
 * parameters:
 *      obj  (IN/OUT) :   red black tree object pointer
 * return:
 *      maximum pool size
 ***********************************************************************/
unsigned int rbtree_poolsize(LRBRoot obj);

/***********************************************************************
 * function: rbiterator_malloc()
 * description:
 *      initialize the iterator by allocating memory
 * parameters:
 *      it   (IN/OUT) :   iterator object pointer
 *      num      (IN) :   maximum queue size
 * return:
 *      If success, it will return 0, or return -1 for error
 * note:
 *      The queue size should be equal to the number of element in the tree
 *      So it can be obtained from rbtree_count() or use maximum pool size
 ***********************************************************************/
int rbiterator_malloc(RBIter* it, unsigned int num);

/***********************************************************************
 * function: rbiterator_melink()
 * description:
 *      initialize the iterator by linking a memory block
 * parameters:
 *      it   (IN/OUT) :   iterator object pointer
 *      num      (IN) :   maximum queue size
 *      queue    (IN) :   queue pointer
 * return:
 *      If success, it will return 0, or return -1 for error
 * note:
 *      The queue size should be equal to maximum pool size
 ***********************************************************************/
int rbiterator_melink(RBIter* it, unsigned int num, int* queue);

/***********************************************************************
 * function: rbiterator_free()
 * description:
 *      free iterator allocated memory
 * parameters:
 *      it   (IN/OUT) :   iterator object pointer
 * return:
 *      If success, it will return 0, or return -1 for error
 * note:
 *      It should be called,
 *      if you use rbiterator_malloc() to initialize the iterator
 ***********************************************************************/
int rbiterator_free(RBIter* it);

/***********************************************************************
 * function: rbiterator_begin()
 * description:
 *      get the first data element index
 * parameters:
 *      it   (IN/OUT) :   iterator object pointer
 * return:
 *      data element index
 ***********************************************************************/
int rbiterator_begin(RBIter* it);

/***********************************************************************
 * function: rbiterator_next()
 * description:
 *      get/goto the next data element index
 * parameters:
 *      it   (IN/OUT) :   iterator object pointer
 * return:
 *      a data element index
 *      if iterator already traverses all data,
 *      it'll return an invalid index
 ***********************************************************************/
int rbiterator_next(RBIter* it);

/***********************************************************************
 * function: rbiterator_begin()
 * description:
 *      get an invalid index
 * parameters:
 *      it   (IN/OUT) :   iterator object pointer
 * return:
 *      always return 0xFFFFFFFF
 * e.g.:
 * RBRoot myDB;
 * RBIter it;
 * int n;
 * ...
 * rbiterator_malloc(&it, rbtree_count(&myDB));
 * rbtree_iterator(&myDB, &it, RBT_INORDER);
 * for (n=rbiterator_begin(&it); n!=rbiterator_end(&it); n=rbiterator_next(&it))
 * {
 * 		rbtree_route(&myDB, n, NULL, callback);
 * 		...
 * }
 * rbiterator_free(&it);
 ***********************************************************************/
int rbiterator_end(RBIter* it);

#ifdef __cplusplus
}
#endif

#endif
