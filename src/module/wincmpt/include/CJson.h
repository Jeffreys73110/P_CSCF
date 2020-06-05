#ifndef CJSON_H__DFCF3E64_94A2_4593_9BC1_FE53DAD2A8B0__INCLUDED_
#define CJSON_H__DFCF3E64_94A2_4593_9BC1_FE53DAD2A8B0__INCLUDED_

typedef enum
{
	CJS_NULL=0,
	CJS_STRING=1,
	CJS_ARRAY=2,
	CJS_BOOL=3,
	CJS_UINT=4,
	CJS_INT=5,
	CJS_OBJ=6,
	CJS_DOUBLE=7,
	CJS_ANY=8,
}
CJSON_TYPE;

// ---------- JSON Creator ----------
typedef struct
{
	const char* key; // key
	CJSON_TYPE type; // value type
	union // value (integer)
	{
		int sint;
		unsigned int uint;
		unsigned int vbool;
	};
	union
	{
		const char* str;
		char* n_str;
		void* json; // CJSON
	};
	union
	{
		double vdouble; // double
		unsigned int str_len; // string length (parser)
		const char* jstr; // JSON string pointer
	};
	unsigned int key_len; // key length (parser)
}
CJSON_ELEMENT;
typedef struct
{
	unsigned int flag; // 32 bits flags
	unsigned int cur; // for AddElement()
	unsigned int num; // the number of element
	CJSON_ELEMENT* data; // data array pointer
}
CJSON;

// ---------- JSON Parser ----------
typedef struct
{
	unsigned int objID;
	unsigned int element_num;
	unsigned int element_cnt;
	unsigned int cjson_num;
	unsigned int cjson_cnt;
	CJSON_ELEMENT* epool;
	CJSON* jpool;
}
CJSON_PARSER;
typedef struct
{
	const char* key;
	unsigned int cur;
}
CJSON_PGET;
typedef void* CJSON_SYTANX;

// for CJSON_Syntax output in string pointer style size setting
#ifndef JSTR_POINTER
#define JSTR_POINTER 0xFFFFFFFF
#endif

////////////////////////////////////////////////////////////////////////////////
// API

#ifdef __cplusplus
extern "C" {
#endif

// JSON Creator
void CJSON_Init(CJSON* json, CJSON_ELEMENT* pool, unsigned int elemt_num); // static memory style
int CJSON_Init2(CJSON* json, unsigned int elemt_num); // memory allocate style
void CJSON_AddElement1(CJSON* json, const char* key, const char* str); // add a string to json
void CJSON_AddElement2(CJSON* json, const char* key, CJSON* ptr, char is_array); // add an array or JSON to json
void CJSON_AddElement3(CJSON* json, const char* key, int value, CJSON_TYPE vtype); // add UINT, INT, or BOOL to json
void CJSON_AddElement4(CJSON* json, const char* key, double value); // add double to json
unsigned int CJSON_CreateString(CJSON* json, char* buf, unsigned int size);
void CJSON_Free(CJSON* json);

// JSON Parser
int CJSON_FindObjEndPos(const char* jstr);
unsigned int CJSON_JLen(const char* jstr);
unsigned int CJSON_JSLen(const char* jstr);
int CJSON_ParserInit(CJSON_PARSER* parser, CJSON* jpool, CJSON_ELEMENT* epool, unsigned int cjson_num, unsigned int element_num);
int CJSON_Parser(CJSON_PARSER* parser, const char* jstr, int flags);
int CJSON_ParserGet(CJSON* json, CJSON_PGET* layers, unsigned int layer_num, CJSON** out, CJSON_TYPE type); // return the cur of elements
void CJSON_SetPGET1(CJSON_PGET* pget, const char* key);
void CJSON_SetPGET2(CJSON_PGET* pget, unsigned int cur);
void CJSON_SetPGET3(CJSON_PGET* pget, const char* key, unsigned int klen);

// JSON Sytanx Parser
int CJSON_SyntaxInit(CJSON_SYTANX* obj, unsigned int obj_size, const char* json);
int CJSON_Syntax(CJSON_SYTANX obj, void* out, unsigned int osize, unsigned int layer_num, ...);
int CJSON_SyntaxFree(CJSON_SYTANX obj);

#ifdef __cplusplus
}
#endif

#endif // CJSON_H__DFCF3E64_94A2_4593_9BC1_FE53DAD2A8B0__INCLUDED_
