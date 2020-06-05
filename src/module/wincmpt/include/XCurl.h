#ifndef __X_CURL_H__
#define __X_CURL_H__

typedef enum
{
	EXD_NORMAL=0,	// -d
	EXD_URLENCODE=1	// --data-urlencode
}
xCURL_DSTYLE;
typedef struct
{
	xCURL_DSTYLE stl;
	const char *key; // key (can be null)
	const char *val; // value (can't be null)
	const char *con; // a string between key and value
	
	// flags
	unsigned char quot; // set true for using single quotation (it's useful for JSON data)
	unsigned char r_2;  // reserved flags
	unsigned char r_3;  // reserved flags
	unsigned char r_4;  // reserved flags
}
xCURL_DPARAM;
typedef struct
{
	const char *key; // key (can't be null)
	const char *val; // value (can't be null)
}
xCURL_HPARAM;
typedef struct
{
	const char *url;
	unsigned char use_k_flag; // 0: no -k;		1: use -k
	unsigned char https_flag; // 0: use HTTP;	1: use HTTPS;
	unsigned char array_alloc; // allocate flags
	unsigned char r_4; // reserved flags
	
	unsigned int H_param_num; // -H array num
	unsigned int d_param_num; // -d array num
	xCURL_HPARAM *H_param; // -H array
	xCURL_DPARAM *d_param; // -d array
	
	const char *o_param; // -o [output path]
	const char *X_param; // -X [POST, GET, PUT, DELETE]

	unsigned int H_cur; // for adding API
	unsigned int d_cur; // for adding API
}
xCURL;

/***********************************************************************
* callback: xCURL_ROUTE()
* description:
*      own cURL blocking implement
* parameters:
*      obj      (IN) :   xCURL object
*      out     (OUT) :   output result string
*      size     (IN) :   output buffer size
*      ptr      (IN) :   user pointer
* return:
*      0 for success, or <0 for failure
***********************************************************************/
typedef int (*xCURL_ROUTE)(xCURL* obj, char* out, int size, void* ptr);

//////////////////////////////////////////////////////////////////////////////////////////
// API

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************************
 * function: xCUrl_Init()
 * description:
 *      xCURL initialization
 * parameters:
 *      obj    	(IN) :   xCURL object
 *      H_array (IN) :   memory block for -H
 *      d_array (IN) :   memory block for -d
 *      H_num   (IN) :   array num for -H
 *      d_num   (IN) :   array num for -d
 * return:
 *      0 for success, or <0 for failure
 ***********************************************************************/
int xCUrl_Init(xCURL* obj, xCURL_HPARAM* H_array, xCURL_DPARAM* d_array, int H_num, int d_num);

/***********************************************************************
 * function: xCUrl_Free()
 * description:
 *      xCURL free
 * parameters:
 *      obj    	(IN) :   xCURL object
 * return:
 *      0 for success, or <0 for failure
 ***********************************************************************/
void xCUrl_Free(xCURL* obj);

/***********************************************************************
 * function: xCUrl_SetBasicInfo()
 * description:
 *      set cURL basic information
 * parameters:
 *      obj    	(IN) :   xCURL object
 *      url     (IN) :   URL
 *      X_param (IN) :   -X [POST, GET, PUT, DELETE...]
 *      k_flag  (IN) :   0: no -k;      1: use -k
 *      ssl_flag(IN) :   0: use HTTP;   1: use HTTPS;
 ***********************************************************************/
void xCUrl_SetBasicInfo(xCURL* obj, const char *url, const char *X_param, unsigned char ssl_flag, unsigned char k_flag);

/***********************************************************************
 * function: xCUrl_AddHParam()
 * description:
 *      Add cURL -H parameters
 * parameters:
 *      obj    	(IN) :   xCURL object
 *      key    	(IN) :   key string
 *      val    	(IN) :   value string
 * return:
 *      0 for success, or <0 for failure
 ***********************************************************************/
int xCUrl_AddHParam(xCURL* obj, const char *key, const char *val);

/***********************************************************************
 * function: xCUrl_AddDParam()
 * description:
 *      Add cURL -d parameters (style: xxx=yyy)
 * parameters:
 *      obj    	(IN) :   xCURL object
 *      key    	(IN) :   key string
 *      val    	(IN) :   value string
 * return:
 *      0 for success, or <0 for failure
 ***********************************************************************/
int xCUrl_AddDParam(xCURL* obj, const char *key, const char *val);

/***********************************************************************
 * function: xCUrl_UrlEncode()
 * description:
 *      URL encoding
 * parameters:
 *      in     	(IN) :   input string
 *      out     (OUT):   output URL encoding string
 *      size    (IN) :   output buffer size
 * return:
 *      Output URL encoding string length
 * note:
 *      When out is NULL & size is 0,
 *      this API will calculate encoding URL string length
 ***********************************************************************/
int xCUrl_UrlEncode(const char *in, char *out, int size);

/***********************************************************************
 * function: xCUrl_GenerateCmd()
 * description:
 *      Generate cURL command string from xCURL
 * parameters:
 *      obj    	(IN) :   xCURL object pointer
 *      out     (OUT):   output cURL command string, can be NULL here
 *      size    (IN) :   output buffer size
 * return:
 *      cURL string length
 ***********************************************************************/
int xCUrl_GenerateCmd(xCURL* obj, char* out, int size);

/***********************************************************************
 * function: xCUrl_GenerateQueryStr()
 * description:
 *      Generate cURL command string from xCURL
 * parameters:
 *      obj    	(IN) :   xCURL object pointer
 *      out     (OUT):   string including path and query string to identify a resource
 *      size    (IN) :   output buffer size
 * return:
 *      cURL string length
 ***********************************************************************/
int xCUrl_GenerateQueryStr(xCURL* obj, char* out, int size);

/***********************************************************************
 * function: xCUrl_ExecuteCmd() [BLOCKING API]
 * description:
 *      execute cURL command
 * parameters:
 *      cmd    	(IN) :   cURL command string
 *      out     (OUT):   output result string
 *      size    (IN) :   output buffer size
 * return:
 *      0 for success, or <0 for failure
 ***********************************************************************/
int xCUrl_ExecuteCmd(const char* cmd, char* out, int size);

/***********************************************************************
 * function: xCUrl_ExecuteCurl() [BLOCKING API]
 * description:
 *      execute cURL command
 * parameters:
 *      obj    	(IN) :   xCURL object pointer
 *      out     (OUT):   output result string
 *      size    (IN) :   output buffer size
 *      route   (IN) :   own cURL blocking implement route
 *      ptr     (IN) :   user pointer for own cURL blocking implement
 * return:
 *      0 for success, or <0 for failure
 * note:
 *      set route to NULL to use shell command...
 ***********************************************************************/
int xCUrl_ExecuteCurl(xCURL* obj, char* out, int size, xCURL_ROUTE route, void* ptr);

///////////////////////////////////////////////////////////////////////////////////
// external different cURL blocking implement

#if defined(_CURL_USE_HTTPC)
#define XCURL_ROUTE_NAME curl_httpc
#else
#define NULL_XCURL_ROUTE_NAME
#define XCURL_ROUTE_NAME curl_null
#endif

// Unified API name
#define __XCURL_BASE_CAT(x, y)	x##y
#define XCURL_DEFINE_CAT(x, y)	__XCURL_BASE_CAT(x, y)
#define xCUrl_RouteHandler		XCURL_DEFINE_CAT(XCURL_ROUTE_NAME, _handler)
int xCUrl_RouteHandler(xCURL* curl, char* out, int size, void* ptr);

#ifdef __cplusplus
}
#endif

#endif // __EAC_CURL_H__
