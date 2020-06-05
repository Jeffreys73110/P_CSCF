#if !defined(AFX_MYDB_H__A922803C_F75E_45DF_92B3_F3AE40369383__INCLUDED_)
#define AFX_MYDB_H__A922803C_F75E_45DF_92B3_F3AE40369383__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyDB.h : header file
//
#define DB_SQL_LEN						4096
#define RET_CHAR_MAX					512
#define DSNCOLLECTION_BUFLEN			128
#define TIME_LEN						30

#define SQL_ERR_NO_DATA					1			//無資料可以被取回
#define SQL_EXE_SUCCESS					0			//執行SQL成功
#define SQL_ERR_UNKNOW					-1			//不知所錯
#define SQL_ERR_OPEN					-2			//無法開啟資料庫
#define SQL_ERR_INPUTDATA				-3			//輸入資料為空值
#define SQL_ERR_NOT_EXIST				-4			//資料庫無資料
#define SQL_ERR_DELETE					-5			//無法成功執行刪除資料的動作
#define SQL_ERR_ADDNEW					-6			//無法成功執行新增資料的動作
#define SQL_ERR_EDIT					-7			//無法成功執行修改資料的動作
#define SQL_ERR_ACTIVE					-8			//操作MS SQL錯誤
#define SQL_ERR_EXIST					-9			//資料已經存在
#define SQL_ERR_TYPE_ERR				-10			//型態錯誤
#define SQL_ERR_BIND_DATA				-11			//BIND資料連結錯誤
#define SQL_ERR_GET_DATA				-12			//取得資料錯誤
#define SQL_ERR_EXECUTE					-13			//執行SQL語法錯誤
#define SQL_ERR_PREPARE					-14			//執行SQL語法Prepayer時錯誤
#define SQL_ERR_ALLOW_HANDLE			-15			//執行SQLAllocHandle有錯誤
#define SQL_ERR_NULL_HSTMT				-16			//無資料庫連結資料
#define SQL_ERR_RET_ATTR_NO				-17			//無法取得回傳欄位數
#define SQL_ERR_ATTRIBUTE_NAME			-18			//無法取得欄位名稱
#define SQL_ERR_EDIT_NAME				-19			//無法修改資料表名稱
#define SQL_ERR_GETDATA					-20			//無法修改資料表名稱

#define SQL_ERR_CD_NOBALANCE			-100		//SQL錯誤 沒有餘額
#define SQL_ERR_CD_UN_PAY				-101		//SQL錯誤 未付清
#define SQL_ERR_CD_OVER_CREDITLINE		-102		//SQL錯誤 超過信用額度
#define SQL_ERR_CD_UNREGISTED			-103		//SQL錯誤 未註冊
#define SQL_ERR_CD_OVERDUE				-104		//SQL錯誤 超過使用期限
#define SQL_ERR_CD_UNKNOW_TYPE			-105		//SQL錯誤 無法判斷使用者使用類型。

#define ERROR_DB_0001		"無法開啟%s資料表，請查明。(odbc-return:%s)"
#define ERROR_DB_0002		"無法開啟%s資料表，請查明。"
#define ERROR_DB_0003		"輸入的鍵值[%s]為空值。"
#define ERROR_DB_0004		"無法收尋%s資料表，請查明。(odbc-return:%s)"
#define ERROR_DB_0005		"資料表中無法收尋到資料，請查明。"
#define ERROR_DB_0006		"無法刪除%s資料表的資料，請查明。(odbc-return:%s)"
#define ERROR_DB_0007		"無法新增資料至%s資料表，請查明。(odbc-return:%s)"
#define ERROR_DB_0008		"資料表中已存在欲新增的資料，請查明。"
#define ERROR_DB_0009		"無法修改%s資料表資料，請查明。(odbc-return:%s)"
#define ERROR_DB_0010		"無法判斷使用者使用類型。"

#define WARING_DB_00001		"無資料可以被取回。"

#define ERROR_DB_0100		"無法連結資料庫。[func]:%s->%s"
#define ERROR_DB_0101		"無連結資料庫。[func]:%s->%s"
#define ERROR_DB_0102		"無法取得回傳欄位數。[func]:%s->%s"
#define ERROR_DB_0103		"無法取得欄位名稱。[func]:%s->%s"
#define ERROR_DB_0104		"無法綁訂回傳變數。[func]:%s->%s"
#define ERROR_DB_0105		"執行資料庫操作發生錯誤。(odbc-return:%s) [func]:%s->%s"
#define ERROR_DB_0106		"SQL語法錯誤。[func]:%s->%s"
#define ERROR_DB_0107		"執行SQL-Prepare錯誤。[func]:%s->%s"
#define ERROR_DB_0108		"資料庫無符合的資料。[func]:%s->%s"
#define ERROR_DB_0109		"執行SQLAllocHandle有錯誤。[func]:%s->%s"	
#define ERROR_DB_0110		"非預期錯誤。[func]:%s->%s"	
#define ERROR_DB_0111		"無法取得下一筆資料。[func]:%s->%s"
#define ERROR_DB_0112		"無法取得欄位資料。[func]:%s->%s"

static long timestr2long(const char *sztime)
{
	CTime tProc;
	char szbuf[6][10];
	char szTimebuf[TIME_LEN];
	int  ilen = strlen(sztime);

	if(ilen > TIME_LEN) return -1;

	sprintf(szTimebuf,"%s",sztime);
	for(int i=0,j=0,k=0; i<=ilen ; i++)
	{
		if(szTimebuf[i]=='/' || szTimebuf[i]=='_' || szTimebuf[i]==':' || szTimebuf[i]=='-' || 
		   szTimebuf[i]==' ' ||i==(ilen))
		{
			szbuf[k][j] = 0;
			j = 0;
			if(strlen(szbuf[k])==0) 
			{
				return -1;
			}
			k += 1;
		}else
		{
			if(isdigit(szTimebuf[i])==0) return -1;
			szbuf[k][j] = szTimebuf[i];
			j += 1;
		}		
	}
	if(k<6)
	{
		for(;k<6;k++) strcpy(szbuf[k],"0");
	}
	tProc = CTime(atoi(szbuf[0]),atoi(szbuf[1]),atoi(szbuf[2]),atoi(szbuf[3]),atoi(szbuf[4]),atoi(szbuf[5]));
	
	return tProc.GetTime();
};

typedef struct _result_data
{
	char	strData[250];
	long	iRetLeng;
} RESULT_DATA, *lpRESULT_DATA;

enum DNS_TYPE {MSSQL_DNS, ACCESS_DNS, EXCEL_DNS};
/////////////////////////////////////////////////////////////////////////////
// CMyDB window
#include "afxdb.h"
#include "odbcinst.h"
#include "sql.h"

//==================ADO==================//
//#include <afxdisp.h>
//#include "icrsint.h"   //Include support for VC++ Extensions

class CMyDB : public CWnd
{
// Construction
public:
	CMyDB();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyDB)
	//}}AFX_VIRTUAL

// Implementation
public:
	//==================DNS==================//
	bool dnsCheck(const char *szDns);				//檢查ODBC DNS是否存在

	int dnsAddAccessOdbc(const char *szDns,			/*新增的DNS名稱*/
						 HWND hwndParent);			/*呼叫程式的Hwnd*/
													//新增ACCESS的ODBC DNS名稱

	int dnsAddMSSQLOdbc(const char *szDns,			/*新增的DNS名稱*/
						const char *szServer,		/*SQL-Server名稱*/
						const char *szDBName,		/*資料庫名稱*/
						HWND hwndParent);			/*呼叫程式的Hwnd*/
													//
	//=======================================//

	//===============CDatabase===============//
	//取得SQL回傳的錯誤描述
	char * dbGetDiagState(); 

	//綁定BOOL回傳值
	int dbBindBoolRetData(SQLHSTMT *m_hstmt,		/*增料庫資料回傳指標*/
						  int icol, 				/*第幾欄*/
						  BOOL *bdata);				/*綁定的變數*/
	//綁定BOOL回傳值
	int dbBindBoolRetData(int icol,  				/*第幾欄*/
						  BOOL *bdata,				/*綁定的變數*/
						  char *szCallingFunc);		/*呼叫的程式名稱*/

	//綁定CHAR回傳值
	int dbBindStrRetData(int icol, 
						 char*szdata, 
						 int idatalen, 
						 char *szCallingFunc);
	
	//綁定CHAR回傳值
	int dbBindStrRetData(SQLHSTMT *m_hstmt, 
						 int icol, 
						 char *szdata, 
						 int idatalen);
	
	//綁定BOUBLE回傳值
	int dbBindDoubleRetData(int icol, 
							double *dbdata, 
							char *szCallingFunc);
	
	//綁定BOUBLE回傳值
	int dbBindDoubleRetData(SQLHSTMT *m_hstmt, 
							int icol, 
							double *dbdata);
	
	//綁定FLOAT回傳值
	int dbBindFloatRetData(int icol, 
						   float *fdata, 
						   char *szCallingFunc);
	
	//綁定FLOAT回傳值
	int dbBindFloatRetData(SQLHSTMT *m_hstmt, 
						   int icol, 
						   float *fdata);
	
	//綁定INT回傳值
	int dbBindIntRetData(int icol, 
						 int *idata, 
						 char *szCallingFunc);
	
	//綁定INT回傳值
	int dbBindIntRetData(SQLHSTMT *m_hstmt, 
						 int icol, 
						 int *idata);
	
	//綁定LONG回傳值
	int dbBindLongRetData(int icol, 
						  long *lndata, 
						  char *szCallingFunc);
	
	//綁定LONG回傳值
	int dbBindLongRetData(SQLHSTMT *m_hstmt, 
						  int icol, 
						  long *lndata);
	
	//清除SQL語法
	int ClearSqlBuf();
	
	//取得時間回傳值
	int dbGetStr2TimeRetData(int icol, 
							 CTime &tdbRet, 
							 char *szCallingFunc);
	
	//取得BOOL回傳值
	int dbGetBoolRetData(SQLHSTMT *m_hstmt, 
						 int icol, 
						 BOOL *bdata);
	
	//取得BOOL回傳值
	int dbGetBoolRetData(int icol, 
						 BOOL *bdata, 
						 char *szCallingFunc);
	
	//取得CHAR回傳值
	int dbGetStrRetData(SQLHSTMT *m_hstmt, 
						int icol, 
						char *szdata, 
						int idatalen);
	
	//取得CHAR回傳值
	int dbGetStrRetData(int icol, 
						char*szdata, 
						int idatalen, 
						char *szCallingFunc);
	
	//取得DOUBLE回傳值
	int dbGetDoubleRetData(int icol, 
						   double *dbdata, 
						   char *szCallingFunc);
	
	//取得BOUBLE回傳值
	int dbGetDoubleRetData(SQLHSTMT *m_hstmt, 
						int icol, 
						double *dbdata);
	
	//取得FOLAT回傳值
	int dbGetFloatRetData(SQLHSTMT *m_hstmt, 
						  int icol, 
						  float *fdata);
	
	//取得FLOAT回傳值
	int dbGetFloatRetData(int icol, 
						  float *fdata, 
						  char *szCallingFunc);
	
	//取得INT回傳值
	int dbGetIntRetData(SQLHSTMT *m_hstmt, 
						int icol, 
						int *idata);
	
	//取得INT回傳值
	int dbGetIntRetData(int icol, 
						int *idata, 
						char *szCallingFunc);
	
	//取得LONG回傳值
	int dbGetLongRetData(SQLHSTMT *m_hstmt, 
						 int icol, 
						 long *lndata);
	
	//取得LONG回傳值
	int dbGetLongRetData(int icol, 
						 long *lndata, 
						 char *szCallingFunc);
	
	//取得錯誤描述
	int dbGetErrMsg(int iErrMsg, 
					char *szCallingFunc, 
					char *szErrFunc, 
					const char *szdbErrMsg=NULL, 
					bool IsGetDiag = false);
	
	//執行SQL並取回資料
	int DirectExecuteWithGetData(char *szSQL, 
								 char *szCallingFunc);
	
	//執行SQL並取回資料
	int dbSelectExecuteWithGetData(char *szSQL, 
								   char *szCallingFunc);
	
	//測試資料庫
	int dbTestRequery(const char *szDefaultSQL, 
					  char *szCallingFunc);
	
	//綁定BOOL回傳值
	int dbGetNextData(char *szCallingFunc);
	
	//綁定回傳值
	int dbBindQueryData(char *szCallingFunc);

	//CDataBase 連線是否建立
	int dbIsOpen();									

	//結束與資料庫連線
	int dbDisconnect(char *szCallingFunc);			
	
	//連結資料庫
	int dbConnect(const char *szOdbcDns,			/*ODBC DNS 名稱*/
				  char *szUId,						/*使用者帳號*/
				  char *szPwd,						/*使用者密碼*/
				  bool bReadOnly,					/*Read-Only*/
				  char *szCallingFunc);				/*呼叫的函示*/
													

	//檢查資料表名稱是否存在
	int dbCheckTableName(const char *szTableName,	/*資料表名稱*/
					   char *szCallingFunc);		/*呼叫的函示*/
													

	//執行SELECT語法(PERBULID)
	int dbSelectExecuteSQL(LPCTSTR lpszSQL,			/*執行的SQL語法*/
						   char *szCallingFunc);	/*呼叫的函示*/

	//直接執行SELECT語法
	int dbExecuteDirectSQL(LPCTSTR lpszSQL,			/*執行的SQL語法*/
						   char *szCallingFunc);	/*呼叫的函示*/

	//=======================================//

	//===============共用函數================//
	char * GetErrorData();//取得錯誤訊息
	//=======================================//
	
	
	//==================DNS==================//
	// Constants used.
	static const unsigned char s_SYSTEM;
	static const unsigned char s_USER;

	// Get the first DSN. ucType should be s_SYSTEM, 
	// s_USER or a combination of both (s_SYSTEM|s_USER)
	// which is default.
	// Returns empty strings upon failure.
	CString dnsGetFirstName(unsigned char ucType = (s_SYSTEM | s_USER));
	
	// Get the next DSN in the enumeration sequence.
	// Returns empty strings upon failure.
	CString dnsGetNextName();

	// Retrieve the las error message (a formated string)
	CString GetErrorMessage() { return m_sErrorMsg; }

	// Retrieve the last result code from a SQL operation.
	SQLRETURN GetLastSQLResult() { return m_iSQLResult; }

	//收尋ODBC DNS
	int dnsSearch(CStringArray &zsaDns,				/*DNS的容器*/
				  bool m_bSystem,					/*收尋系統的DNS*/
				  bool m_bUser);					/*收尋使用者的DNS*/
													
	//關閉收尋DNS連結
	int dnsCloseCollection();
	
	//取得收尋DNS連結		
	void dnsGetCollection();						

	//=======================================//

	//===============CDatabase===============//
	//建立LS Log記錄檔
	int dbCreateLsLogTable(const char *szODBC_DNS,	/*ODBC名稱*/
						   const char *szTableName,	/*資料表名稱*/
						   SQLHDBC *m_clthdbc,		/*資料庫連結指標*/
						   SQLHSTMT *m_clthstmt,	/*增料庫資料回傳指標*/
						   char *szCallingFunc);	/*呼叫的函示*/
													
	//建立Access LS Log記錄檔
	int dbCreateAccessTable(const char *szODBC_DNS,	/*ODBC名稱*/
						    const char *szTableName,/*資料表名稱*/
						    SQLHDBC *m_clthdbc,		/*資料庫連結指標*/
						    SQLHSTMT *m_clthstmt,	/*增料庫資料回傳指標*/
						    char *szCallingFunc);	/*呼叫的函示*/
													
						  
	//結束與資料庫的連結
	int dbDisconnect(SQLHDBC  *m_hdbc,				/*資料庫連結指標*/
					 SQLHSTMT *m_hstmt,				/*資料庫資料回傳指標*/
					 SQLHENV  *m_henv,				/*資料庫環境指標*/
					 char *szCallingFunc);			/*呼叫的函示*/
													
	//直接執行SQL語法
	int dbExecuteDirectSQL(LPCTSTR lpszSQL,			/*執行的SQL語法*/
						   SQLHDBC *m_hdbc,			/*資料庫連結指標*/
						   SQLHSTMT *m_hstmt,		/*資料庫資料回傳指標*/
						   char *szCallingFunc);	/*呼叫的函示*/
													
	//修改資料表名稱
	int dbEditTableName(SQLHDBC *m_hdbc,			/*資料庫連結指標*/
					    SQLHSTMT *m_hstmt,			/*資料庫資料回傳指標*/
					    const char *szOrigName,		/*原資料表名稱*/
					    const char *szNewName,		/*新資料表名稱*/
					    char *szCallingFunc);		/*呼叫的函示*/
													
	//針對SELECT語法的執行程式
	int dbSelectExecuteSQL(LPCTSTR lpszSQL,			/*執行的SQL語法*/
						   SQLHDBC *m_hdbc,			/*資料庫連結指標*/
						   SQLHSTMT *m_hstmt,		/*資料庫資料回傳指標*/
						   char *szCallingFunc);	/*呼叫的函示*/
												
	//清除回傳的資料
	int dbClearResultData();						

	//取得執行的回傳欄位名稱
	int dbGetDBAttributeName(SQLHSTMT *m_hstmt,		/*資料庫連結指標*/
						     SQLSMALLINT *iNumCols,	/*回傳的欄位數*/
						     char *szCallingFunc);	/*呼叫的函示*/
													
	//連接資料庫
	int dbConnect(LPCTSTR lpszDSN,					/*執行的SQL語法*/	
				  SQLHDBC  *m_hdbc,					/*資料庫連結指標*/
				  SQLHENV  *m_henv,					/*資料庫環境指標*/
				  char *szUId,						/*使用者帳號*/
				  char *szPwd,						/*使用者密碼*/
				  bool bReadOnly,					/*Read-Only*/
				  char *szCallingFunc);				/*呼叫的函示*/
													
	//取得下一筆資料
	int dbGetNextData(SQLHSTMT *m_hstmt,			/*資料庫連結指標*/
					  char *szCallingFunc);			/*呼叫的函示*/

	//連結回傳資料
	int dbBindQueryData(SQLHSTMT *m_hstmt,			/*資料庫連結指標*/
					    char *szCallingFunc);		/*呼叫的函示*/

	//取得回傳筆數
	int SQLRowAffectedCount(SQLINTEGER *RowCountPtr);/*影響筆數*/
	//=======================================//

	//CDatabase
	BOOL		IsConnect;
	CDatabase	db;						//Database Class
	SQLHENV     m_henv;					//Env Handle from SQLAllocEnv()
	SQLHDBC		m_hdbc;				//Connection handle
	SQLHDBC		m_backhdbc;				//Connection handle
	SQLHSTMT	m_hstmt;				//Statement handle
	char		szSQL[DB_SQL_LEN];			//SQL 
	char		szErrorData[500];		//Error Data Buffer
	char		szDiagMsg[SQL_MAX_MESSAGE_LENGTH+1]; //錯誤訊息
	char		szDefTableName[256];	//Default Table Name after Create Table
	bool		bIsDealSpace;			//是否分配空間
	RESULT_DATA	szReData[70];			//The return value after execute sql
	SQLINTEGER  sintret;
	char 		szErrState[7];

	//錯誤回傳程式
//	int PostErrMsg(const char *szErrMsg, const char *szFunc, int Type);
//	ERRMSG_DATA ErrMsgData;
//	HWND_INFO	ErrMsgHI;					//ERR Msg的聯絡事件

	virtual ~CMyDB();

	// Generated message map functions
protected:
	int dbGetDiag(SQLSMALLINT HandleType, SQLHSTMT *m_hstmt, char *szErrDiagMsg,int iDiagLen);
	//{{AFX_MSG(CMyDB)
	SQLHENV m_hSQLEnvironment;
	CString m_sErrorMsg;
	SQLRETURN m_iSQLResult;

	// Helper function.
	SQLRETURN dnsCallSQLDataSources(CString& sName, SQLUSMALLINT direction);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYDB_H__A922803C_F75E_45DF_92B3_F3AE40369383__INCLUDED_)
