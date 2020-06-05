// MyDB.cpp : implementation file
//

#include "stdafx.h"
#include "CallServer.h"
#include "MyDB.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyDB
/*static */const unsigned char CMyDB::s_SYSTEM	= 0x01;
/*static */const unsigned char CMyDB::s_USER	= 0x02;

CMyDB::CMyDB()
{
	m_hdbc = NULL;
	m_hstmt = NULL;
	m_henv = NULL;
	memset(szReData,0,sizeof(szReData));
	memset(szErrorData,0,sizeof(szErrorData));
	memset(szDefTableName,0,sizeof(szDefTableName));
	::CoInitialize(NULL);
}

CMyDB::~CMyDB()
{
}


BEGIN_MESSAGE_MAP(CMyDB, CWnd)
	//{{AFX_MSG_MAP(CMyDB)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMyDB message handlers
//CDatabase
int CMyDB::dbConnect(LPCTSTR lpszDSN, SQLHDBC *m_hdbc, SQLHENV  *m_henv, char *szUId, char *szPwd, bool bReadOnly, char *szCallingFunc)
{
	//SQLRETURN   retcode;

	if(db.IsOpen()) return 0;
	try
	{
		if(!db.Open(lpszDSN,false,bReadOnly,"ODBC;",true))
		{
			return dbGetErrMsg(SQL_ERR_OPEN,szCallingFunc,"dbConnect()");
		};
		*m_hdbc = db.m_hdbc;
		m_backhdbc = db.m_hdbc;
		
		/*Allocate environment handle */
		/*retcode = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, m_henv);

		if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) 
		{
			//* Set the ODBC version environment attribute /
			retcode = SQLSetEnvAttr(*m_henv, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0); 

			if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) 
			{	
				//* Allocate connection handle /
				retcode = SQLAllocHandle(SQL_HANDLE_DBC, *m_henv, m_hdbc); 

				if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) 
				{
					//* Set Access Database mode in Read only/
					if(bReadOnly) SQLSetConnectAttr(*m_hdbc, SQL_ATTR_ACCESS_MODE, (void *)SQL_MODE_READ_ONLY, 0);
					else SQLSetConnectAttr(*m_hdbc, SQL_ATTR_ACCESS_MODE, (void *)SQL_MODE_READ_WRITE , 0);

					//* Set login timeout to 5 seconds. /
					SQLSetConnectAttr(*m_hdbc, SQL_LOGIN_TIMEOUT, (void *)5, 0);

					//* Connect to data source /
					retcode = SQLConnect(*m_hdbc, (SQLCHAR*) lpszDSN, SQL_NTS,
							  (SQLCHAR*) szUId, SQL_NTS,
							  (SQLCHAR*) szPwd, SQL_NTS);
					
					if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
					{					
						return dbGetErrMsg(SQL_ERR_OPEN,szCallingFunc,"dbConnect()");
					}
					IsConnect = TRUE;					
					
				}SQLFreeHandle(SQL_HANDLE_DBC, m_hdbc);
			}
		}
		SQLFreeHandle(SQL_HANDLE_ENV, m_henv);*/

	}
	catch(CDBException*  pe)
	{
		return dbGetErrMsg(SQL_ERR_ACTIVE,szCallingFunc,"dbConnect()",pe->m_strError);
	}
	return SQL_EXE_SUCCESS;
}

char * CMyDB::GetErrorData()
{
	return szErrorData;
}

int CMyDB::dbGetDBAttributeName(SQLHSTMT *m_hstmt, SQLSMALLINT *iNumCols, char *szCallingFunc)
{
	SQLRETURN m_retiCol;
	SQLSMALLINT StringLeng;//回傳值的長度
	
	try
	{
		if(m_hstmt == NULL)
		{
			return dbGetErrMsg(SQL_ERR_NULL_HSTMT,szCallingFunc,"GetDBAttributeName()");
		}
		//取得回傳的欄位數
		m_retiCol = SQLNumResultCols(*m_hstmt, iNumCols);
		if(m_retiCol==SQL_STILL_EXECUTING || m_retiCol==SQL_ERROR || m_retiCol==SQL_INVALID_HANDLE)
		{
			//無法取得回傳欄位數
			dbGetDiag(SQL_HANDLE_STMT ,m_hstmt, szDiagMsg,SQL_MAX_MESSAGE_LENGTH);
			return dbGetErrMsg(SQL_ERR_RET_ATTR_NO,szCallingFunc,"GetDBAttributeName()",szDiagMsg,true);			
		}
		for(int i=0; i<*iNumCols; i++)
		{
			//取得回傳列的欄位名稱(可以不要做)
			SQLRETURN m_retAttr = SQLColAttribute(*m_hstmt, 
												  i+1, 
												  SQL_DESC_NAME,
												  &(szReData+i)->strData, 
												  RET_CHAR_MAX, 
												  &StringLeng, 
												  0);

			if(m_retiCol==SQL_STILL_EXECUTING || m_retiCol==SQL_ERROR || m_retiCol==SQL_INVALID_HANDLE)
			{
				//"無法取得欄位名稱。
				dbGetDiag(SQL_HANDLE_STMT ,m_hstmt, szDiagMsg,SQL_MAX_MESSAGE_LENGTH);
				return dbGetErrMsg(SQL_ERR_ATTRIBUTE_NAME,szCallingFunc,"GetDBAttributeName()",szDiagMsg,true);	
			}

			//建立回傳列勾結暫存BUFFER
			SQLRETURN m_retBind = SQLBindCol(*m_hstmt,
											 i+1,
											 SQL_C_TCHAR,
											 &(szReData+i)->strData,
											 RET_CHAR_MAX,
											 &(szReData+i)->iRetLeng);

			if(m_retBind==SQL_ERROR || m_retBind==SQL_INVALID_HANDLE)
			{
				//"無法綁訂回傳變數。"
				dbGetDiag(SQL_HANDLE_STMT ,m_hstmt, szDiagMsg,SQL_MAX_MESSAGE_LENGTH);
				return dbGetErrMsg(SQL_ERR_BIND_DATA,szCallingFunc,"GetDBAttributeName()",szDiagMsg,true);				
			}
		}
	}
	catch(CDBException *pe)
	{
		//"執行資料庫發生錯誤。
		return dbGetErrMsg(SQL_ERR_ACTIVE,szCallingFunc,"GetDBAttributeName()",pe->m_strError);
	}
	return 0;
}

int CMyDB::dbClearResultData()
{
	memset(&szReData[0],0,sizeof(szReData));
	return 0;
}

int CMyDB::dbSelectExecuteSQL(LPCTSTR lpszSQL, SQLHDBC *m_hdbc, SQLHSTMT *m_hstmt, char *szCallingFunc)
{
	SQLINTEGER   lRecordsRead = 0;
	BOOL bSuccess = FALSE;
	SQLRETURN rc;

	try
	{
		dbClearResultData();
		if( *m_hstmt!=NULL )
		{		
			SQLFreeStmt(*m_hstmt, SQL_DROP );		
		}
		if (SQL_SUCCESS == (rc = ::SQLAllocHandle(SQL_HANDLE_STMT, *m_hdbc, m_hstmt)))
		{
			rc = ::SQLPrepare(*m_hstmt, (unsigned char*)lpszSQL, SQL_NTS);
			if (SQL_SUCCESS == rc || SQL_SUCCESS_WITH_INFO == rc)
			{
				rc = ::SQLExecute(*m_hstmt);
				if(SQL_NO_DATA == rc)
				{
					return dbGetErrMsg(SQL_ERR_NOT_EXIST,szCallingFunc,"dbSelectExecuteSQL");
				}
				else if(SQL_SUCCESS == rc || SQL_SUCCESS_WITH_INFO == rc)
				{
					return SQL_EXE_SUCCESS;
				}
				else 
				{
					//"SQL語法錯誤。
					dbGetDiag(SQL_HANDLE_STMT ,m_hstmt, szDiagMsg,SQL_MAX_MESSAGE_LENGTH);
					return dbGetErrMsg(SQL_ERR_EXECUTE,szCallingFunc,"SelectExecuteSQL()",szDiagMsg,true);
				}
			}
			else 
			{
				//執行SQL-Prepare錯誤。[func]:%s->%s
				dbGetDiag(SQL_HANDLE_STMT ,m_hstmt, szDiagMsg,SQL_MAX_MESSAGE_LENGTH);
				return dbGetErrMsg(SQL_ERR_PREPARE,szCallingFunc,"SelectExecuteSQL()",szDiagMsg,true);
			}					
		}
		else 
		{
			dbGetDiag(SQL_HANDLE_STMT ,m_hstmt, szDiagMsg,SQL_MAX_MESSAGE_LENGTH);
			return dbGetErrMsg(SQL_ERR_ALLOW_HANDLE,szCallingFunc,"SelectExecuteSQL()",szDiagMsg,true);			
		}			
	}catch(CDBException *pe)
	{
		return dbGetErrMsg(SQL_ERR_ACTIVE,szCallingFunc,"SelectExecuteSQL()",pe->m_strError);		
	}
	return dbGetErrMsg(SQL_ERR_UNKNOW,szCallingFunc,"SelectExecuteSQL()");	
}

int CMyDB::dbEditTableName(SQLHDBC *m_hdbc, SQLHSTMT *m_hstmt, const char *szOrigName, const char *szNewName, char *szCallingFunc)
{
	SQLRETURN rc;
	sprintf(szSQL,"EXEC sp_rename '%s', '%s'",szOrigName,szNewName);
	try
	{
		if( *m_hstmt!=NULL )
		{		
			SQLFreeStmt( *m_hstmt, SQL_DROP );		
		}

		if (SQL_SUCCESS == (rc = ::SQLAllocHandle(SQL_HANDLE_STMT, *m_hdbc, m_hstmt)))
		{
			rc = ::SQLExecDirect(*m_hstmt,(unsigned char*)szSQL,SQL_NTS);

			if (SQL_SUCCESS == rc || SQL_SUCCESS_WITH_INFO == rc)
			{
				return SQL_EXE_SUCCESS;
			}else 
			{
				dbGetDiag(SQL_HANDLE_STMT ,m_hstmt, szDiagMsg,SQL_MAX_MESSAGE_LENGTH);
				return dbGetErrMsg(SQL_ERR_EDIT_NAME,szCallingFunc,"SelectExecuteSQL()",szDiagMsg,true);	
			}
		}else
		{
			dbGetDiag(SQL_HANDLE_STMT ,m_hstmt, szDiagMsg,SQL_MAX_MESSAGE_LENGTH);
			return dbGetErrMsg(SQL_ERR_EDIT_NAME,szCallingFunc,"SelectExecuteSQL()",szDiagMsg,true);	
		}
	
	}catch(CDBException *pe)
	{
		return dbGetErrMsg(SQL_ERR_ACTIVE,szCallingFunc,"EditTableName()",pe->m_strError);
	}
	return dbGetErrMsg(SQL_ERR_UNKNOW,szCallingFunc,"EditTableName()");
}

int CMyDB::dbExecuteDirectSQL(LPCTSTR lpszSQL, SQLHDBC *m_hdbc, SQLHSTMT *m_hstmt, char *szCallingFunc)
{
	SQLINTEGER   lRecordsRead = 0;
	BOOL bSuccess = FALSE;
	SQLRETURN rc;

	try
	{
		if( *m_hstmt!=NULL )
		{		
			SQLFreeStmt( *m_hstmt, SQL_DROP );		
		}
		if (SQL_SUCCESS == (rc = ::SQLAllocHandle(SQL_HANDLE_STMT, *m_hdbc, m_hstmt)))
		{
			rc = ::SQLExecDirect(*m_hstmt,(unsigned char*)lpszSQL,SQL_NTS);

			if (SQL_SUCCESS == rc || SQL_SUCCESS_WITH_INFO == rc)
			{
				return SQL_EXE_SUCCESS;
			}else if(SQL_NO_DATA_FOUND == rc)
			{
				return dbGetErrMsg(SQL_ERR_NOT_EXIST,szCallingFunc,"dbSelectExecuteSQL");
			}
			else
			{
				//"SQL語法錯誤。
				dbGetDiag(SQL_HANDLE_STMT ,m_hstmt, szDiagMsg,SQL_MAX_MESSAGE_LENGTH);
				return dbGetErrMsg(SQL_ERR_EXECUTE,szCallingFunc,"ExecuteDirectSQL()",szDiagMsg,true);	
			}
		}
		else 
		{
			//執行SQL語法Prepayer時錯誤
			dbGetDiag(SQL_HANDLE_STMT ,m_hstmt, szDiagMsg,SQL_MAX_MESSAGE_LENGTH);
			return dbGetErrMsg(SQL_ERR_ALLOW_HANDLE,szCallingFunc,"ExecuteDirectSQL()",szDiagMsg,true);
		}
			
	}catch(CDBException *pe)
	{
		return dbGetErrMsg(SQL_ERR_ACTIVE,szCallingFunc,"ExecuteDirectSQL()",pe->m_strError);
	}
	return dbGetErrMsg(SQL_ERR_UNKNOW,szCallingFunc,"ExecuteDirectSQL()");
}

int CMyDB::dbDisconnect(SQLHDBC *m_hdbc, SQLHSTMT *m_hstmt, SQLHENV  *m_henv, char *szCallingFunc)
{
//	SQLRETURN rc;
	try
	{
		//if(IsConnect == TRUE)
		if(dbIsOpen())
		{
			IsConnect = FALSE;
			SQLFreeStmt( *m_hstmt, SQL_DROP );
			db.Close();
			/*rc = SQLDisconnect( *m_hdbc );
			if( rc== SQL_SUCCESS || rc == SQL_SUCCESS_WITH_INFO)
				rc = SQLFreeHandle( SQL_HANDLE_DBC, *m_hdbc);
			SQLFreeHandle( SQL_HANDLE_ENV, *m_henv );*/
			*m_hdbc = NULL;
			*m_henv = NULL; 
		}
		dbClearResultData();
	}catch(CDBException *pe)
	{
		return dbGetErrMsg(SQL_ERR_ACTIVE,szCallingFunc,"DisconnectDB()",pe->m_strError);
	}
	return SQL_EXE_SUCCESS;
}

int CMyDB::dbCheckTableName(const char *szTableName, char *szCallingFunc)
{
	if(szTableName != NULL)
		if(strlen(szTableName) > 0)
			return 0;

	sprintf(szErrorData,"%s [func]:[%s]->CheckTableName()","請輸入資料表名稱",szCallingFunc);
	return -1;
}


int CMyDB::dbConnect(const char *szOdbcDns, char *szUId, char *szPwd, bool bReadOnly, char *szCallingFunc)
{
	return dbConnect(szOdbcDns, &m_hdbc, &m_henv, szUId,  szPwd, bReadOnly, szCallingFunc);
}

int CMyDB::dbDisconnect(char *szCallingFunc)
{
	return dbDisconnect(&m_hdbc, &m_hstmt, &m_henv, szCallingFunc);
}

int CMyDB::dbIsOpen()
{
	return db.IsOpen();//IsConnect;
}

//DNS
void CMyDB::dnsGetCollection()
{
	m_iSQLResult = (SQL_SUCCESS);
	m_hSQLEnvironment= (SQL_NULL_HANDLE);
	// Initialize an environment handle.
	m_iSQLResult = SQLAllocHandle(SQL_HANDLE_ENV,
								  SQL_NULL_HANDLE,
								  &m_hSQLEnvironment);
	if (m_iSQLResult != SQL_SUCCESS)
	{
		m_sErrorMsg = "SQLAllocHandle failed";
		return;
	}

	// Must set the ODBC version we wish to use...
	// Otherwise you will get an error.
	m_iSQLResult = SQLSetEnvAttr(m_hSQLEnvironment,
								 SQL_ATTR_ODBC_VERSION,
								 (SQLPOINTER)SQL_OV_ODBC3,
								 0); //this parameter is ignored when setting ODBC version.
	if (m_iSQLResult != SQL_SUCCESS)
	{
		sprintf(szErrorData, "SQLSetEnvAttr failed to set ODBC version");
		return;
	}
}

int CMyDB::dnsCloseCollection()
{
	// Must release the environment handle.
	return m_iSQLResult = SQLFreeHandle(SQL_HANDLE_ENV, m_hSQLEnvironment);
}

SQLRETURN CMyDB::dnsCallSQLDataSources(CString& sName, SQLUSMALLINT direction)
{
	SQLSMALLINT dataLen1,dataLen2;
	SQLCHAR sTemp[DSNCOLLECTION_BUFLEN];
	SQLCHAR sDesc[DSNCOLLECTION_BUFLEN];

	// This call enumerates the DSNs.
	m_iSQLResult = SQLDataSources(m_hSQLEnvironment, 
								  direction,
								  sTemp,
								  DSNCOLLECTION_BUFLEN,
								  &dataLen1,
								  sDesc,
								  DSNCOLLECTION_BUFLEN,
								  &dataLen2);

	switch (m_iSQLResult)
	{
	case SQL_SUCCESS:			
	case SQL_SUCCESS_WITH_INFO:	
		sName = sTemp;
		break;
	case SQL_NO_DATA: 
		// This is not an error but rather information that there 
		// are no more DSNs to enumerate.
		m_sErrorMsg = "No more DSNs to enumerate";
		break;
	case SQL_ERROR:				
		{
			SQLCHAR sState[10];
			SQLINTEGER iNativeError;
			SQLCHAR sMsg[DSNCOLLECTION_BUFLEN];
			(void)SQLGetDiagRec(SQL_HANDLE_ENV,
								m_hSQLEnvironment,
								1,
								sState,
								&iNativeError,
								sMsg,
								DSNCOLLECTION_BUFLEN,
								&dataLen2);
			m_sErrorMsg.Format("SQL_ERROR: %s:%s",sState,sMsg);
		}
		break;
	case SQL_INVALID_HANDLE:
		m_sErrorMsg = "Invalid Handle";
		break;
	default:
		m_sErrorMsg = "Unknown ERROR";
	}

	return m_iSQLResult;
}

CString CMyDB::dnsGetFirstName(unsigned char ucType /* = (s_SYSTEM | s_USER)*/)
{
	CString sResult = "";
	if (ucType != s_SYSTEM &&
		ucType != s_USER &&
		ucType != (s_SYSTEM | s_USER))
	{
		m_sErrorMsg = "Unknown DSN type";
		m_iSQLResult = SQL_ERROR;
	}
	else
	{
		SQLUSMALLINT direction = SQL_FETCH_FIRST;
		if (ucType == s_SYSTEM)
			direction = SQL_FETCH_FIRST_SYSTEM;
		else if (ucType == s_USER)
			direction = SQL_FETCH_FIRST_USER;
		if (SQL_SUCCESS != dnsCallSQLDataSources(sResult, direction))
		{
			//TODO: maybe do something here...
			sprintf(szErrorData,"無法取得第一筆ODBC DNS資料。");
		}
	}

	return sResult;
}

CString CMyDB::dnsGetNextName()
{
	CString sResult = "";
	if (SQL_SUCCESS != dnsCallSQLDataSources(sResult, SQL_FETCH_NEXT))
	{
		//TODO: maybe do something here...
		sprintf(szErrorData,"無法取得下一筆ODBC DNS資料。");
	}

	return sResult;
}

int CMyDB::dnsSearch(CStringArray &zsaDns, bool m_bSystem, bool m_bUser)
{
	CString s;
	dnsGetCollection();
	for (bool bFirst = true; GetLastSQLResult() == SQL_SUCCESS; bFirst = false)
	{
		if (bFirst)
			s = dnsGetFirstName((m_bSystem?CMyDB::s_SYSTEM:0) |
								(m_bUser?CMyDB::s_USER:0));
		else
			s = dnsGetNextName();

		if (GetLastSQLResult() == SQL_SUCCESS) zsaDns.Add(s);
	}

	if (GetLastSQLResult() != SQL_NO_DATA) sprintf(szErrorData,"無任何ODBC DNS資料。");
	dnsCloseCollection();
	return true;
}

bool CMyDB::dnsCheck(const char *szDns)
{
	CString s;
	dnsGetCollection();	
	for (bool bFirst = true; GetLastSQLResult() == SQL_SUCCESS; bFirst = false)
	{
		if (bFirst)
			s = dnsGetFirstName((1?CMyDB::s_SYSTEM:0) |
								(1?CMyDB::s_USER:0));
		else
			s = dnsGetNextName();

		if (GetLastSQLResult() == SQL_SUCCESS) 
			if(strcmp(szDns,s)==0)
				return true;
	}
	dnsCloseCollection();
	return false;
}


//CDatabase
int CMyDB::dnsAddMSSQLOdbc(const char *szDns, const char *szServer, const char *szDBName, HWND hwndParent)
{
	char *szDriver = "SQL Server";
	char szAttributes[256];
	sprintf(szAttributes,"DSN=%s! SERVER=%s! DATABASE=%s!!", 
												  szDns,
												  szServer,
												  szDBName);
	int iAttrLen=strlen(szAttributes);
	for(int i=0;i<iAttrLen;i++) if(szAttributes[i]=='!') szAttributes[i]='\0';
	if(dnsCheck(szDns)==true)
	{
		sprintf(szErrorData,"ODBS DNS 已經存在!!!");
		if(SQLConfigDataSource(hwndParent,ODBC_CONFIG_DSN,szDriver,szAttributes)==TRUE) return 0;
		else return -1;
	}
	
	if(SQLConfigDataSource(NULL,ODBC_ADD_DSN,szDriver,szAttributes)==TRUE) return 0;

	sprintf(szErrorData,"ODBS DNS 新增失敗!!!");
	return -1;	
}

int CMyDB::dnsAddAccessOdbc(const char *szDns, HWND hwndParent)
{
	char *szDriver = "Microsoft Access Driver (*.mdb)";
	char szAttributes[256];

	//============找尋ACCESS檔案所在位置============
	POSITION pos;
	static TCHAR _szFilter[]  = "Microsoft Access Files (*.mdb)|*.mdb||";
	char szFPbuf[MAX_PATH+1];
	CFileDialog dialog(TRUE,NULL,NULL, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_EXPLORER,_szFilter,this);
	GetCurrentDirectory(MAX_PATH,szFPbuf);
	dialog.m_ofn.lpstrInitialDir = szFPbuf;
	if(dialog.DoModal()==IDOK) pos = dialog.GetStartPosition();				
	else return -1;
	//===============================================

	sprintf(szAttributes,"DSN=%s! DBQ=%s! DEFAULTDIR=%s!!", 
												  szDns,
												  dialog.GetNextPathName( pos ),
												  szFPbuf);

	int iAttrLen=strlen(szAttributes);
	for(int i=0;i<iAttrLen;i++) if(szAttributes[i]=='!') szAttributes[i]='\0';
	
	if(dnsCheck(szDns)==true)
	{
		sprintf(szErrorData,"ODBS DNS 已經存在!!!");
		if(SQLConfigDataSource(hwndParent,ODBC_CONFIG_DSN,szDriver,szAttributes)==TRUE) return 0;
		else return -1;
	}
	
	if(SQLConfigDataSource(NULL,ODBC_ADD_DSN,szDriver,szAttributes)==TRUE) return 0;
	sprintf(szErrorData,"ODBS DNS 新增失敗!!!");
	return -1;	
}

int CMyDB::SQLRowAffectedCount(SQLINTEGER *RowCountPtr)
{
	return ::SQLRowCount(m_hstmt, RowCountPtr);
}

/*int CMyDB::PostErrMsg(const char *szErrMsg, const char *szFunc, int Type)
{
	strcpy(ErrMsgData.szErrMsg,szErrMsg);
	strcpy(ErrMsgData.szErrFunc,szFunc);
	strcat(ErrMsgData.szErrFunc,"{CCDMS_EM}");
	::SendMessage(ErrMsgHI.g_Hwnd,ErrMsgHI.g_Event,(WPARAM)&ErrMsgData,Type);
	return 0;
}*/

int CMyDB::dbBindQueryData(SQLHSTMT *m_hstmt, char *szCallingFunc)
{
	SQLSMALLINT iNumCols;
	try
	{
		SQLRETURN m_retiCol = SQLNumResultCols(*m_hstmt, &iNumCols);
		if(m_retiCol==SQL_STILL_EXECUTING || m_retiCol==SQL_ERROR || m_retiCol==SQL_INVALID_HANDLE)
		{
			return dbGetErrMsg(SQL_ERR_RET_ATTR_NO,szCallingFunc,"dbBindQueryData()");
		}
		//動態分配回傳值的暫存BUFFER
		for(int i=0; i<iNumCols; i++)
		{
			//建立回傳列勾結暫存BUFFER
			SQLRETURN m_retBind = SQLBindCol(*m_hstmt,
											 i+1,
											 SQL_C_TCHAR,
											 &((szReData+i)->strData),
											 RET_CHAR_MAX,
											 &((szReData+i)->iRetLeng));

			if(m_retBind==SQL_ERROR || m_retBind==SQL_INVALID_HANDLE)
			{
				//sprintf(szErrorData,"[%s]->[BindQueryData]",szCallingFunc);
				//PostErrMsg(_T("無法綁訂回傳變數"),szErrorData,ERROR_MSG);
				return dbGetErrMsg(SQL_ERR_BIND_DATA,szCallingFunc,"dbBindQueryData()");
			}
		}
	}
	catch(CDBException *pe)
	{
		//sprintf(szErrorData,"[%s]->[BindQueryData]",szCallingFunc);
		//PostErrMsg(pe->m_strError,szErrorData,ERROR_MSG);
		return dbGetErrMsg(SQL_ERR_ACTIVE,szCallingFunc,"dbBindQueryData()",pe->m_strError);
	}
	return SQL_EXE_SUCCESS;
}

int CMyDB::dbGetNextData(SQLHSTMT *m_hstmt, char *szCallingFunc)
{
	SQLRETURN   m_retFetch;
	//取得下一列資料
	try
	{
		m_retFetch = SQLFetch(*m_hstmt);
		if(m_retFetch==SQL_ERROR || m_retFetch==SQL_INVALID_HANDLE)
		{
			dbGetDiag(SQL_HANDLE_STMT ,m_hstmt, szDiagMsg,SQL_MAX_MESSAGE_LENGTH);
			return dbGetErrMsg(SQL_ERR_GET_DATA,szCallingFunc,"dbGetNextData()");
		}else if(m_retFetch==SQL_NO_DATA)
			return dbGetErrMsg(SQL_ERR_NO_DATA,szCallingFunc,"dbGetNextData()");
	}catch(CDBException *pe)
	{
		return dbGetErrMsg(SQL_ERR_ACTIVE,szCallingFunc,"dbGetNextData()",pe->m_strError);
	}
	
	return SQL_EXE_SUCCESS;
}

int CMyDB::dbExecuteDirectSQL(LPCTSTR lpszSQL, char *szCallingFunc)
{
	return dbExecuteDirectSQL(lpszSQL, &m_hdbc, &m_hstmt, szCallingFunc);
}

int CMyDB::dbSelectExecuteSQL(LPCTSTR lpszSQL, char *szCallingFunc)
{
	return dbSelectExecuteSQL(lpszSQL, &m_hdbc, &m_hstmt, szCallingFunc);
}

int CMyDB::dbBindQueryData(char *szCallingFunc)
{
	return dbBindQueryData(&m_hstmt, szCallingFunc);
}

int CMyDB::dbGetNextData(char *szCallingFunc)
{
	return dbGetNextData(&m_hstmt, szCallingFunc);
}

int CMyDB::dbTestRequery(const char *szDefaultSQL, char *szCallingFunc)
{
	//WCH 2006-03-26
	int ireturn=0; 
	char szSQL[256];
	if(!dbIsOpen()) 
		if((ireturn=dbConnect(szDefaultSQL,NULL,NULL,true,szCallingFunc))!=SQL_EXE_SUCCESS)
			return ireturn; 
	sprintf(szSQL,"SELECT COUNT(*) AS TotalRecord FROM %s ",szDefaultSQL);
	if((ireturn=dbSelectExecuteSQL(szSQL,szCallingFunc))<SQL_EXE_SUCCESS) 
		return ireturn;
	if((ireturn=dbBindQueryData(szCallingFunc))!=SQL_EXE_SUCCESS) return ireturn;
	if((ireturn=dbGetNextData(szCallingFunc))!=SQL_EXE_SUCCESS) return ireturn;
	return atol(szReData[0].strData);
}

int CMyDB::dbSelectExecuteWithGetData(char *szSQL, char *szCallingFunc)
{
	int isqlret = 0;
	if((isqlret=dbSelectExecuteSQL(szSQL,szCallingFunc))>=0)
	{
		if((isqlret=dbBindQueryData(szCallingFunc))==0)
		{
			return isqlret=dbGetNextData(szCallingFunc); 			
		}else 
			return  isqlret;		
	}else 
		return isqlret;
	
	return dbGetErrMsg(SQL_ERR_UNKNOW,szCallingFunc,"dbSelectExecuteWithGetData()") ;	
}

int CMyDB::DirectExecuteWithGetData(char *szSQL, char *szCallingFunc) 
{
	int isqlret = 0; 
	if((isqlret=dbExecuteDirectSQL(szSQL,szCallingFunc))>=0) 
	{
		if((isqlret=dbBindQueryData(szCallingFunc))==0) 
		{
			return isqlret=dbGetNextData(szCallingFunc); 
		}else 
			return isqlret; 
	}else 
		return isqlret;
	
	return -1 ;	
}

int CMyDB::dbGetErrMsg(int iErrMsg, char *szCallingFunc, char *szErrFunc,const char *szdbErrMsg, bool IsGetDiag)
{
	switch(iErrMsg)
	{
	case SQL_ERR_NO_DATA:		//無資料可以被取回
		sprintf(szErrorData,WARING_DB_00001,szCallingFunc,szErrFunc);
		break;
	case SQL_EXE_SUCCESS:		//執行SQL成功
		break;
	case SQL_ERR_UNKNOW:		//不知所錯
		//"非預期錯誤。[func]:%s->%s"	
		sprintf(szErrorData,ERROR_DB_0110,szCallingFunc,szErrFunc);
		break;
	case SQL_ERR_OPEN:			//無法開啟資料庫
		//"無法連結資料庫。[func]:%s->%s"
		sprintf(szErrorData,ERROR_DB_0100,szCallingFunc,szErrFunc);
		break;
	case SQL_ERR_INPUTDATA:		//輸入資料為空值
		break;
	case SQL_ERR_NOT_EXIST:		//資料庫無資料
		//"資料庫無符合的資料。[func]:%s->%s"
		sprintf(szErrorData,ERROR_DB_0108,szCallingFunc,szErrFunc);
		break;
	case SQL_ERR_DELETE:		//無法成功執行刪除資料的動作
		break;
	case SQL_ERR_ADDNEW:		//無法成功執行新增資料的動作
		break;
	case SQL_ERR_EDIT:			//無法成功執行修改資料的動作
		break;
	case SQL_ERR_ACTIVE:		//操作MS-SQL錯誤
		//"執行資料庫操作發生錯誤。(odbc-return:%s) [func]:%s->%s"
		sprintf(szErrorData,ERROR_DB_0105,szdbErrMsg,szCallingFunc,szErrFunc);
		break;
	case SQL_ERR_EXIST:			//資料已經存在
		break;
	case SQL_ERR_TYPE_ERR:		//型態錯誤
		break;
	case SQL_ERR_BIND_DATA:		//BIND資料連結錯誤
		//"無法綁訂回傳變數。"
		if(IsGetDiag) sprintf(szErrorData,ERROR_DB_0104"[odbc-ret:%s]",szCallingFunc,szErrFunc,szdbErrMsg);
		else sprintf(szErrorData,ERROR_DB_0104,szCallingFunc,szErrFunc);
		break;
	case SQL_ERR_GET_DATA:		//取得資料錯誤
		//"無法取得下一筆資料。[func]:%s->%s"
		sprintf(szErrorData,ERROR_DB_0111,szCallingFunc,szErrFunc);
		break;
	case SQL_ERR_EXECUTE:		//執行SQL語法錯誤
		//"SQL語法錯誤。[func]:%s->%s"
		sprintf(szErrorData,ERROR_DB_0106,szCallingFunc,szErrFunc);
		break;
	case SQL_ERR_PREPARE:		//執行SQL語法Prepayer時錯誤
		//執行SQL-Prepare錯誤。[func]:%s->%s
		sprintf(szErrorData,ERROR_DB_0107,szCallingFunc,szErrFunc);
		break;
	case SQL_ERR_ALLOW_HANDLE:	//執行SQL語法Prepayer時錯誤
		//"執行SQLAllocHandle有錯誤。[func]:%s->%s"
		sprintf(szErrorData,ERROR_DB_0109,szCallingFunc,szErrFunc);
		break;
	case SQL_ERR_NULL_HSTMT:	//無資料庫連結資料
		//"無連結資料庫。[func]:%s->%s"	
		sprintf(szErrorData,ERROR_DB_0101,szCallingFunc,szErrFunc);
		break;
	case SQL_ERR_RET_ATTR_NO:	//無法取得回傳欄位數
		//"無法取得回傳欄位數。。[func]:%s->%s"
		if(IsGetDiag) sprintf(szErrorData,ERROR_DB_0102"[odbc-ret:%s]",szCallingFunc,szErrFunc,szdbErrMsg);
		else sprintf(szErrorData,ERROR_DB_0102,szCallingFunc,szErrFunc);
		break;
	case SQL_ERR_ATTRIBUTE_NAME://無法取得欄位名稱
		//"無法取得欄位名稱。[func]:%s->%s"
		if(IsGetDiag) sprintf(szErrorData,ERROR_DB_0103"[odbc-ret:%s]",szCallingFunc,szErrFunc,szdbErrMsg);
		else sprintf(szErrorData,ERROR_DB_0103,szCallingFunc,szErrFunc);
		break;
	case SQL_ERR_GETDATA://無法取得欄位資料
		if(IsGetDiag) sprintf(szErrorData,ERROR_DB_0112"[odbc-ret:%s]",szCallingFunc,szErrFunc,szdbErrMsg);
		else sprintf(szErrorData,ERROR_DB_0112,szCallingFunc,szErrFunc);
		break;
	default:
		break;
	}
	return iErrMsg;
}

//取得資料庫錯誤訊息
int CMyDB::dbGetDiag(SQLSMALLINT HandleType, SQLHSTMT *m_hstmt, char *szErrDiagMsg, int iDiagLen)
{
	SQLSMALLINT imsglen=0;
	SQLRETURN rc;
	SQLCHAR SqlState[7]; 
	SQLINTEGER NativeError; 
	SQLCHAR sqlszErrMsg[SQL_MAX_MESSAGE_LENGTH];
	ZeroMemory(szErrDiagMsg,iDiagLen);
	ZeroMemory(szErrState,sizeof(szErrState));
	ZeroMemory(sqlszErrMsg,SQL_MAX_MESSAGE_LENGTH);
	rc=SQLGetDiagRec(HandleType,
					 *m_hstmt,
					 1,
					 (SQLCHAR*)SqlState, 
					 &NativeError,
					 (SQLCHAR*)sqlszErrMsg,
					 sizeof(sqlszErrMsg),
					 &imsglen);
	if(rc==SQL_SUCCESS || rc==SQL_SUCCESS_WITH_INFO)
	{
		SqlState[6]='\0';
		sqlszErrMsg[SQL_MAX_MESSAGE_LENGTH]='\0';
		sprintf(szErrState,"%s",SqlState);
		sprintf(szErrDiagMsg,"eCode:%s, eNote:%s",SqlState, sqlszErrMsg);
	}
	return 0;
}

//********************************取得資料********************************//

//取得 long 類型的資料
int CMyDB::dbGetLongRetData(int icol, long *lndata, char *szCallingFunc)
{
	SQLRETURN ret;
	ret=dbGetLongRetData(&m_hstmt, icol, lndata);
	if(ret!=SQL_SUCCESS && ret!= SQL_SUCCESS_WITH_INFO)
	{
		//無法取得欄位資料
		dbGetDiag(SQL_HANDLE_STMT ,&m_hstmt, szDiagMsg,SQL_MAX_MESSAGE_LENGTH);
		return dbGetErrMsg(SQL_ERR_RET_ATTR_NO,szCallingFunc,"dbGetLongRetData",szDiagMsg,true);
	}
	return 0;
}

int CMyDB::dbGetLongRetData(SQLHSTMT *m_hstmt, int icol, long *lndata)
{
	return SQLGetData(*m_hstmt, icol, SQL_C_SLONG, lndata, 0, &sintret);
}

//取得 int 類型的資料
int CMyDB::dbGetIntRetData(int icol, int *idata, char *szCallingFunc)
{
	SQLRETURN ret;
	ret=dbGetIntRetData(&m_hstmt, icol, idata);
	if(ret!=SQL_SUCCESS && ret!= SQL_SUCCESS_WITH_INFO)
	{
		//無法取得欄位資料
		dbGetDiag(SQL_HANDLE_STMT ,&m_hstmt, szDiagMsg,SQL_MAX_MESSAGE_LENGTH);
		return dbGetErrMsg(SQL_ERR_RET_ATTR_NO,szCallingFunc,"dbGetIntRetData",szDiagMsg,true);
	}
	return 0;
}

int CMyDB::dbGetIntRetData(SQLHSTMT *m_hstmt, int icol, int *idata)
{
	return SQLGetData(*m_hstmt, icol, SQL_C_SSHORT, idata, 0, &sintret);
}


//取得 float 類型的資料
int CMyDB::dbGetFloatRetData(int icol, float *fdata, char *szCallingFunc)
{
	SQLRETURN ret;
	ret=dbGetFloatRetData(&m_hstmt, icol, fdata);
	if(ret!=SQL_SUCCESS && ret!= SQL_SUCCESS_WITH_INFO)
	{
		//無法取得欄位資料
		dbGetDiag(SQL_HANDLE_STMT ,&m_hstmt, szDiagMsg,SQL_MAX_MESSAGE_LENGTH);
		return dbGetErrMsg(SQL_ERR_RET_ATTR_NO,szCallingFunc,"dbGetFloatRetData",szDiagMsg,true);
	}
	return 0;
}

int CMyDB::dbGetFloatRetData(SQLHSTMT *m_hstmt, int icol, float *fdata)
{
	return SQLGetData(*m_hstmt, icol, SQL_C_FLOAT, fdata, 0, &sintret);
}

//取得 double 類型的資料
int CMyDB::dbGetDoubleRetData(int icol, double *dbdata, char *szCallingFunc)
{
	SQLRETURN ret;
	ret=dbGetDoubleRetData(&m_hstmt, icol, dbdata);
	if(ret!=SQL_SUCCESS && ret!= SQL_SUCCESS_WITH_INFO)
	{
		//無法取得欄位資料
		dbGetDiag(SQL_HANDLE_STMT ,&m_hstmt, szDiagMsg,SQL_MAX_MESSAGE_LENGTH);
		return dbGetErrMsg(SQL_ERR_RET_ATTR_NO,szCallingFunc,"dbGetDoubleRetData",szDiagMsg,true);
	}
	return 0;
}

int CMyDB::dbGetDoubleRetData(SQLHSTMT *m_hstmt, int icol, double *dbdata)
{
	return SQLGetData(*m_hstmt, icol, SQL_C_DOUBLE, dbdata, 0, &sintret);
}

//取得 char * 類型的資料
int CMyDB::dbGetStrRetData(int icol, char *szdata, int idatalen, char *szCallingFunc)
{
	SQLRETURN ret;
	ret=dbGetStrRetData(&m_hstmt, icol, szdata, idatalen);
	if(ret!=SQL_SUCCESS && ret!= SQL_SUCCESS_WITH_INFO)
	{
		//無法取得欄位資料
		dbGetDiag(SQL_HANDLE_STMT ,&m_hstmt, szDiagMsg,SQL_MAX_MESSAGE_LENGTH);
		return dbGetErrMsg(SQL_ERR_RET_ATTR_NO,szCallingFunc,"dbGetStrRetData",szDiagMsg,true);
	}
	return 0;
}

int CMyDB::dbGetStrRetData(SQLHSTMT *m_hstmt, int icol, char *szdata, int idatalen)
{
	return SQLGetData(*m_hstmt, icol, SQL_C_CHAR, szdata, idatalen, &sintret);
}

int CMyDB::dbGetBoolRetData(int icol, BOOL *bdata, char *szCallingFunc)
{
	SQLRETURN ret;
	ret=dbGetBoolRetData(&m_hstmt, icol, bdata);
	if(ret!=SQL_SUCCESS)
	{
		//無法取得欄位資料
		dbGetDiag(SQL_HANDLE_STMT ,&m_hstmt, szDiagMsg,SQL_MAX_MESSAGE_LENGTH);
		return dbGetErrMsg(SQL_ERR_RET_ATTR_NO,szCallingFunc,"dbBindBoolRetData",szDiagMsg,true);
	}
	return 0;
}

int CMyDB::dbGetBoolRetData(SQLHSTMT *m_hstmt, int icol, BOOL *bdata)
{
	return SQLGetData(*m_hstmt, icol, SQL_C_SSHORT, bdata, 0, &sintret);
}

int CMyDB::dbGetStr2TimeRetData(int icol, CTime &tdbRet, char *szCallingFunc)
{
	SQLRETURN ret;
	long lnrettime;
	char szsbRet[TIME_LEN];
	ret=dbGetStrRetData(&m_hstmt, icol, szsbRet, TIME_LEN);
	if(ret!=SQL_SUCCESS)
	{
		//無法取得欄位資料
		dbGetDiag(SQL_HANDLE_STMT ,&m_hstmt, szDiagMsg,SQL_MAX_MESSAGE_LENGTH);
		return dbGetErrMsg(SQL_ERR_RET_ATTR_NO,szCallingFunc,"dbBindBoolRetData",szDiagMsg,true);
	}else
	{
		if(lnrettime=timestr2long(szsbRet)<0) lnrettime=0 ;
		tdbRet=CTime(lnrettime);
	}
	return 0;
}


int CMyDB::ClearSqlBuf()
{
	ZeroMemory(szSQL,sizeof(szSQL));
	return 0;
}

//BIND DATA WITH INDEX 
//Bind long 類型的資料
int CMyDB::dbBindLongRetData(int icol, long *lndata, char *szCallingFunc)
{
	SQLRETURN ret;
	ret=dbBindLongRetData(&m_hstmt, icol, lndata);
	if(ret!=SQL_SUCCESS)
	{
		//無法取得欄位資料
		dbGetDiag(SQL_HANDLE_STMT ,&m_hstmt, szDiagMsg,SQL_MAX_MESSAGE_LENGTH);
		return dbGetErrMsg(SQL_ERR_RET_ATTR_NO,szCallingFunc,"dbBindLongRetData",szDiagMsg,true);
	}
	return 0;
}

int CMyDB::dbBindLongRetData(SQLHSTMT *m_hstmt, int icol, long *lndata)
{
	return SQLBindCol(*m_hstmt, icol, SQL_C_SLONG, lndata, 0, &sintret);
}

//取得 int 類型的資料
int CMyDB::dbBindIntRetData(int icol, int *idata, char *szCallingFunc)
{
	SQLRETURN ret;
	ret=dbBindIntRetData(&m_hstmt, icol, idata);
	if(ret!=SQL_SUCCESS)
	{
		//無法取得欄位資料
		dbGetDiag(SQL_HANDLE_STMT ,&m_hstmt, szDiagMsg,SQL_MAX_MESSAGE_LENGTH);
		return dbGetErrMsg(SQL_ERR_RET_ATTR_NO,szCallingFunc,"dbBindIntRetData",szDiagMsg,true);
	}
	return 0;
}

int CMyDB::dbBindIntRetData(SQLHSTMT *m_hstmt, int icol, int *idata)
{
	return SQLBindCol(*m_hstmt, icol, SQL_C_SSHORT, idata, 0, &sintret);
}


//取得 float 類型的資料
int CMyDB::dbBindFloatRetData(int icol, float *fdata, char *szCallingFunc)
{
	SQLRETURN ret;
	ret=dbBindFloatRetData(&m_hstmt, icol, fdata);
	if(ret!=SQL_SUCCESS)
	{
		//無法取得欄位資料
		dbGetDiag(SQL_HANDLE_STMT ,&m_hstmt, szDiagMsg,SQL_MAX_MESSAGE_LENGTH);
		return dbGetErrMsg(SQL_ERR_RET_ATTR_NO,szCallingFunc,"dbBindFloatRetData",szDiagMsg,true);
	}
	return 0;
}

int CMyDB::dbBindFloatRetData(SQLHSTMT *m_hstmt, int icol, float *fdata)
{
	return SQLBindCol(*m_hstmt, icol, SQL_C_FLOAT, fdata, 0, &sintret);
}

//取得 double 類型的資料
int CMyDB::dbBindDoubleRetData(int icol, double *dbdata, char *szCallingFunc)
{
	SQLRETURN ret;
	ret=dbBindDoubleRetData(&m_hstmt, icol, dbdata);
	if(ret!=SQL_SUCCESS)
	{
		//無法取得欄位資料
		dbGetDiag(SQL_HANDLE_STMT ,&m_hstmt, szDiagMsg,SQL_MAX_MESSAGE_LENGTH);
		return dbGetErrMsg(SQL_ERR_RET_ATTR_NO,szCallingFunc,"dbBindDoubleRetData",szDiagMsg,true);
	}
	return 0;
}

int CMyDB::dbBindDoubleRetData(SQLHSTMT *m_hstmt, int icol, double *dbdata)
{
	return SQLBindCol(*m_hstmt, icol, SQL_C_DOUBLE, dbdata, 0, &sintret);
}

//取得 char * 類型的資料
int CMyDB::dbBindStrRetData(int icol, char *szdata, int idatalen, char *szCallingFunc)
{
	SQLRETURN ret;
	ret=dbBindStrRetData(&m_hstmt, icol, szdata, idatalen);
	if(ret!=SQL_SUCCESS)
	{
		//無法取得欄位資料
		dbGetDiag(SQL_HANDLE_STMT ,&m_hstmt, szDiagMsg,SQL_MAX_MESSAGE_LENGTH);
		return dbGetErrMsg(SQL_ERR_RET_ATTR_NO,szCallingFunc,"dbBindStrRetData",szDiagMsg,true);
	}
	return 0;
}

int CMyDB::dbBindStrRetData(SQLHSTMT *m_hstmt, int icol, char *szdata, int idatalen)
{
	return SQLBindCol(*m_hstmt, icol, SQL_C_CHAR, szdata, idatalen, &sintret);
}

int CMyDB::dbBindBoolRetData(int icol, BOOL *bdata, char *szCallingFunc)
{
	SQLRETURN ret;
	ret=dbBindBoolRetData(&m_hstmt, icol, bdata);
	if(ret!=SQL_SUCCESS)
	{
		//無法取得欄位資料
		dbGetDiag(SQL_HANDLE_STMT ,&m_hstmt, szDiagMsg,SQL_MAX_MESSAGE_LENGTH);
		return dbGetErrMsg(SQL_ERR_RET_ATTR_NO,szCallingFunc,"dbBindBoolRetData",szDiagMsg,true);
	}
	return 0;
}

int CMyDB::dbBindBoolRetData(SQLHSTMT *m_hstmt, int icol, BOOL *bdata)
{
	return SQLBindCol(*m_hstmt, icol, SQL_C_SSHORT, bdata, 0, &sintret);
}

char * CMyDB::dbGetDiagState()
{
	return szErrState;
}
