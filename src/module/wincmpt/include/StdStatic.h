#ifndef _STD_STATIC_H_
#define _STD_STATIC_H_

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/***********************************************************************
 * bit array declare example:
 *
 * bitsarray m_bool[BITS_ARRAY_SIZE(75)];  // unsigned char m_bool[10];
 ***********************************************************************/
#define BITS_ARRAY_SIZE(x)	(x)/8+1

typedef unsigned char	bitsarray;

#ifdef __cplusplus
extern "C" {
#endif
void SetBit(bitsarray* ptr, unsigned int n, unsigned char vbool);
void SetBitsByStr(bitsarray* ptr, unsigned int n, const char* str);
unsigned char GetBit(bitsarray* ptr, unsigned int n);
void GetBitsIntoStr(bitsarray* ptr, unsigned int n, unsigned int bitlen, char* str);
#ifdef __cplusplus
}
#endif

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/***********************************************************************
 * NEW_CLASS_POOL_DECLARE:
 * pool name: a variable for check free memory space,
 * object name: variable name,
 * c-class: data-type,
 * array size
 ***********************************************************************/
#define NEW_CLASS_POOL_DECLARE(poolname, objname, cclass, arraysize) \
unsigned char poolname[arraysize]; \
cclass objname[arraysize];

/***********************************************************************
 * NEW_CLASS_PARAM_SET:
 * map   table name,
 * key   variable name,
 * value variable name
 ***********************************************************************/
#define NEW_CLASS_PARAM_SET(poolname, objname)	poolname, objname, sizeof(objname)/sizeof(objname[0]), sizeof(objname[0])
#define NEW_CLASS_PARAM							unsigned char*, void*, unsigned int, unsigned int

typedef struct _CLASS_POOL_STRUCT
{
	unsigned char*	useing;
	unsigned char*	values;
	unsigned int	vsize;
	unsigned int	arraysize;
}
CLASS_POOL, *LPCLASS_POOL;

#ifdef __cplusplus
extern "C" {
#endif
void	Cclass_pool(LPCLASS_POOL obj, NEW_CLASS_PARAM);
void*	Cclass_new(LPCLASS_POOL obj);
void	Cclass_delete(LPCLASS_POOL obj, void* ptr);
#ifdef __cplusplus
}
#endif

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/***********************************************************************
 * STD_MAP_DECLARE:
 * map   table variable name,
 * key   variable name,
 * value variable name,
 * key   data type,
 * value data-type,
 * array size
 ***********************************************************************/
#define STD_MAP_DECLARE(tname, kname, vname, ktype, vtype, arraysize) \
bitsarray	tname[BITS_ARRAY_SIZE(arraysize)]; \
ktype		kname[arraysize]; \
vtype		vname[arraysize];

/***********************************************************************
 * STD_MAP_PARAM_SET:
 * map   table name,
 * key   variable name,
 * value variable name
 ***********************************************************************/
#define STD_MAP_PARAM_SET(tname, kname, vname)	tname,kname,vname,sizeof(kname)/sizeof(kname[0]),sizeof(kname[0]),sizeof(vname[0])
#define STD_MAP_PARAM							bitsarray*,void*,void*,unsigned int,unsigned int,unsigned int

typedef struct _STD_MAP_STRUCT
{
	unsigned int	total;
	unsigned int	unit_num;
	unsigned int	key_size;
	unsigned int	value_size;

	bitsarray		*useing;
	unsigned char	*key;
	unsigned char	*values;
}
STD_MAP, *LPSTD_MAP;

typedef struct _STD_MAP_ITER_STRUCT
{
	// public
	void			*key;
	void			*values;
	// protected
	unsigned int	unit_count;
	unsigned int	now_pos;
	unsigned char	end_pos;
}
STD_MAP_ITER, *LPSTD_MAP_ITER;

/***********************************************************************
 * example:
 *
 * STD_MAP myMap;
 * STD_MAP_DECLARE(g_table, g_studentID, g_age, int, int, 100);
 *
 * void main()
 * {
 *		STD_Map_Init(&myMap, STD_MAP_PARAM_SET(g_table, g_studentID, g_age));
 *		STD_Map_Insert(&myMap, 102319010, 25);
 *		STD_Map_Insert(&myMap, 103319011, 24);
 *		STD_Map_Insert(&myMap, 104319012, 23);
 *
 *		STD_MAP_ITER iter;
 *		for (STD_Map_Rbegin(&myMap, &iter); !STD_Map_Rend(&myMap, &iter); STD_Map_Rnext(&myMap, &iter))
 *		{
 *			int *eachAge = (int*)iter.values;
 *			int	*eachID = (int*)iter.key;
 *		}
 *
 *		int searchID=103319011;
 *		if (STD_Map_Find(&myMap, &searchID, &iter)==0)
 *		{
 *			int *eachAge = (int*)iter.values;	// get age
 *			STD_Map_Erase(&myMap, &iter);		// remove
 *		}
 * }
 ***********************************************************************/

#ifdef __cplusplus
extern "C" {
#endif
void			STD_Map_Init(LPSTD_MAP obj, STD_MAP_PARAM);
unsigned int	STD_Map_Size(LPSTD_MAP obj);
unsigned char	STD_Map_Exist(LPSTD_MAP obj, const void* key);	// return bool style
int				STD_Map_Find(LPSTD_MAP obj, const void* key, LPSTD_MAP_ITER iter);
int				STD_Map_Insert(LPSTD_MAP obj, const void* key, const void* values);
void			STD_Map_Erase(LPSTD_MAP obj, const LPSTD_MAP_ITER iter);
void			STD_Map_Clear(LPSTD_MAP obj);
void			STD_Map_Rbegin(LPSTD_MAP obj, LPSTD_MAP_ITER iter);
void			STD_Map_Rnext(LPSTD_MAP obj, LPSTD_MAP_ITER iter);
unsigned char	STD_Map_Rend(LPSTD_MAP obj, LPSTD_MAP_ITER iter);	// return bool style
#ifdef __cplusplus
}
#endif

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/***********************************************************************
 * STD_LIST_DECLARE:
 * list  variable name,
 * value data-type,
 * array size
 ***********************************************************************/
#define STD_LIST_DECLARE(name, type, arraysize)		type name[arraysize];

/***********************************************************************
 * STD_LIST_PARAM_SET:
 * list  variable name,
 ***********************************************************************/
#define STD_LIST_PARAM_SET(name)	name,sizeof(name)/sizeof(name[0]),sizeof(name[0])
#define STD_LIST_PARAM				void*,unsigned int,unsigned int

typedef struct _STD_LIST_STRUCT
{
	unsigned int	total;
	unsigned int	unit_num;
	unsigned int	value_size;
	
	unsigned int	head_pos;
	unsigned int	tail_pos;
	unsigned char	*values;
}
STD_LIST, *LPSTD_LIST;

#ifdef __cplusplus
extern "C" {
#endif
void			STD_List_Init(LPSTD_LIST obj, STD_LIST_PARAM);
unsigned int	STD_List_Size(LPSTD_LIST obj);
void*			STD_List_Front(LPSTD_LIST obj);
void*			STD_List_Back(LPSTD_LIST obj);
void			STD_List_Pop_Front(LPSTD_LIST obj);
void			STD_List_Pop_Back(LPSTD_LIST obj);
int				STD_List_Push_Front(LPSTD_LIST obj, void* values);
int				STD_List_Push_Back(LPSTD_LIST obj, void* values);
void*			STD_List_Begin(LPSTD_LIST obj, unsigned int index);
void*			STD_List_Reverse(LPSTD_LIST obj, unsigned int index);
void			STD_List_Clear(LPSTD_LIST obj);
#ifdef __cplusplus
}
#endif

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/***********************************************************************
 * STD_VECTOR
 ***********************************************************************/

// vector template
#define STD_VECTOR_DECLARE		STD_LIST_DECLARE
#define STD_VECTOR_PARAM_SET	STD_LIST_PARAM_SET
#define STD_VECTOR_PARAM		STD_LIST_PARAM

// vector struct
#define STD_VECTOR				STD_LIST
#define LPSTD_VECTOR			LPSTD_LIST

// vector function
#define STD_Vector_Init			STD_List_Init
#define STD_Vector_Size			STD_List_Size
#define	STD_Vector_Back			STD_List_Back
#define	STD_Vector_Pop_Back		STD_List_Pop_Back
#define	STD_Vector_Push_Back	STD_List_Push_Back
#define STD_Vector_Begin		STD_List_Begin
#define STD_Vector_Clear		STD_List_Clear

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif
