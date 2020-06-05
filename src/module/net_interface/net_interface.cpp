#include "net_interface.h"

CNetIF *CNetIF::m_instance = NULL;
CNetIF *CNetIF::GetInstance()
{
	CRASH_DEBUG_COMMAND;
	if (!m_instance)
	{
		if (!(m_instance = new CNetIF()))
		{
			DTrace_FatalError("[CNetIF] get_instance fail, m_instance = 0x%x\n", m_instance);
		}
	}
	RETURN m_instance;
}
int CNetIF::ReleaseInstance(CNetIF **ins)
{
	CRASH_DEBUG_COMMAND;
	if (!*ins)
		delete[] * ins;
	*ins = NULL;
	RETURN 0;
}
// void CNetIF::SocketModule_Callbcak_Function(MIXSOCKET wParam, long lParam, void* lpObj)
// {
// 	// m_instance->OnWindowProc(0, wParam, lParam, lpObj);
// }
// void CNetIF::MixTimer_Callbcak_Function(MIXTIMER *wParam, void* lpObj)
// {
// 	((CNetIF *)lpObj)->OnTimer(wParam);
// }

/////////////////////////////////////////////////////////////////////////////
CNetIF::CNetIF()
{
	CRASH_DEBUG_COMMAND;
	NetObjectInit();
	TimerObjectInit();
	// m_WebMixSocket = NULL_MIXSOCKET;
	// callbackFun = NULL;
	// callbackObject = NULL;

	// //--- init web
	// m_WebConnectCount=0;
	// for (int i = 0; i < WEB_CONNECT_SIZE; i++)
	// {
	// 	MutexInit(&m_WebConnectInfo[i].Mutex);
	// 	InitMixTimer(m_MixTimerCoreObjectId, &m_WebConnectInfo[i].MixTimer, this, MixTimer_Callbcak_Function);
	// }

	// //--- unit test
	// UnitTest();

	CRASH_DEBUG_COMMAND;
}

CNetIF::~CNetIF()
{
	CRASH_DEBUG_COMMAND;
	// NetCleanMixSocket(m_WebMixSocket);
	// m_WebMixSocket = NULL_MIXSOCKET;
	CRASH_DEBUG_COMMAND;
}

// void CNetIF::UnitTest()
// {
// 	// #define	VALIDATE_GetEventTypeByPath(a, b, c, d) \
// 	// 	DTrace_TestMsg("[\"%s\"] GetEventTypeByPath=%d (Correct=%d), evt_type=%s vs (Correct: %s) ## %d@%s\n", (a==c && b.compare(d)==0)? COLOR_FG(FG_Green, "Correct"):COLOR_FG(FG_Red, "Wrong"), a, c, b.c_str(), d, __LINE__, __FILE__)
// 	// #define	VALIDATE_GetEventTypeByPacket(a, b, c, d) \
// 	// 	DTrace_TestMsg("[\"%s\"] GetEventTypeByPacket=%d (Correct=%d), evt_type=%s vs (Correct: %s) ## %d@%s\n", (a==c && b.compare(d)==0)? COLOR_FG(FG_Green, "Correct"):COLOR_FG(FG_Red, "Wrong"), a, c, b.c_str(), d, __LINE__, __FILE__)

// 	// CRASH_DEBUG_COMMAND;
// 	// m_UnitTestFlag = true;

// 	// //--- GetEventTypeByPath
// 	// std::string test_event_type;
// 	// int ret;
// 	// ret = GetEventTypeByPath("/img/123.html?evt_type=abc", test_event_type);
// 	// VALIDATE_GetEventTypeByPath(ret, test_event_type, 3, "abc"); test_event_type.clear();
// 	// ret = GetEventTypeByPath("/img/123.html?evt_type=abc456&tt=123", test_event_type);
// 	// VALIDATE_GetEventTypeByPath(ret, test_event_type, 6, "abc456"); test_event_type.clear();
// 	// ret = GetEventTypeByPath("/img/123.html?evt_type=", test_event_type);
// 	// VALIDATE_GetEventTypeByPath(ret, test_event_type, 0, ""); test_event_type.clear();
// 	// ret = GetEventTypeByPath("/img/123.html?evt_type= ", test_event_type);
// 	// VALIDATE_GetEventTypeByPath(ret, test_event_type, 0, ""); test_event_type.clear();
// 	// ret = GetEventTypeByPath("/img/123.html?evt_type=123;", test_event_type);
// 	// VALIDATE_GetEventTypeByPath(ret, test_event_type, 4, "123;"); test_event_type.clear();
// 	// ret = GetEventTypeByPath("GET /img/123.html?type=def HTTP/1.1\r\nHost: 127.0.0.1\r\nCache-Control: no-cache\r\n", test_event_type);
// 	// VALIDATE_GetEventTypeByPath(ret, test_event_type, -2, ""); test_event_type.clear();

// 	// //--- GetEventTypeByPacket
// 	// ret = GetEventTypeByPacket("GET /img/123.html?evt_type=abc HTTP/1.1\r\nHost: 127.0.0.1\r\nCache-Control: no-cache\r\n", test_event_type);
// 	// VALIDATE_GetEventTypeByPacket(ret, test_event_type, 3, "abc"); test_event_type.clear();
// 	// ret = GetEventTypeByPacket("GET /img/123.html?evt_type=abc456&tt=123 HTTP/1.1\r\nHost: 127.0.0.1\r\nCache-Control: no-cache\r\n", test_event_type);
// 	// VALIDATE_GetEventTypeByPacket(ret, test_event_type, 6, "abc456"); test_event_type.clear();
// 	// ret = GetEventTypeByPacket("GET /img/123.html?evt_type= HTTP/1.1\r\nHost: 127.0.0.1\r\nCache-Control: no-cache\r\n", test_event_type);
// 	// VALIDATE_GetEventTypeByPacket(ret, test_event_type, 0, ""); test_event_type.clear();
// 	// ret = GetEventTypeByPacket("/img/123.html?evt_type=abc HTTP/1.1\r\nHost: 127.0.0.1\r\nCache-Control: no-cache\r\n", test_event_type);
// 	// VALIDATE_GetEventTypeByPacket(ret, test_event_type, -2, ""); test_event_type.clear();
// 	// ret = GetEventTypeByPacket("GET /img/123.html?evt_type=abcd\r\nHost: 127.0.0.1\r\nCache-Control: no-cache\r\n", test_event_type);
// 	// VALIDATE_GetEventTypeByPacket(ret, test_event_type, -3, ""); test_event_type.clear();
// 	// ret = GetEventTypeByPacket("GET /img/123.html?type=def HTTP/1.1\r\nHost: 127.0.0.1\r\nCache-Control: no-cache\r\n", test_event_type);
// 	// VALIDATE_GetEventTypeByPacket(ret, test_event_type, -4, ""); test_event_type.clear();
// 	// ret = GetEventTypeByPacket("GET /img/123.html?evt_type=efg:HTTP/1.1\r\n", test_event_type);
// 	// VALIDATE_GetEventTypeByPacket(ret, test_event_type, -5, ""); test_event_type.clear();

// 	m_UnitTestFlag = false;
// 	RETURN;
// }
// /////////////////////////////////////////////////////////////////////////////
// // CNetIF message handlers
// void CNetIF::SetDlgCallback(SERVERDLG_CALLBACK lpFunction, void *lpObject)
// {
// 	CRASH_DEBUG_COMMAND;
// 	callbackFun = lpFunction;
// 	callbackObject = lpObject;
// 	CRASH_DEBUG_COMMAND;
// }

// void CNetIF::GetDataFromCallback(std::string message, std::string& data_str)
// {
// 	CRASH_DEBUG_COMMAND;

// 	if (callbackFun != NULL)
// 		callbackFun(callbackObject, message, data_str);
// 	CRASH_DEBUG_COMMAND;
// }

// int CNetIF::StartDlgServer(unsigned short Port)
// {
// 	CRASH_DEBUG_COMMAND;

// 	RETURN StartTcpServer(m_WebMixSocket, Port, SERVER_DLG_EVENT);
// }

// // void CNetIF::Send_200OK_Xml(WEB_CONNECT* web_conn, std::string path)
// // {
// // 	CRASH_DEBUG_COMMAND;
// // 	if (!web_conn)
// // 	{
// // 		DTrace_Error("Send_200OK_Xml, invalid parameters\n");
// // 		RETURN;
// // 	}

// // 	int ret;
// // 	int fileLength = 0;
// // 	FILE *fio;
// // 	char filename[256] = "html";
// // 	char content_type[64] = "text/html; charset=UTF-8";

// // 	// get path
// // 	if (!path.compare("/"))	Jstrncat(filename, "/ServerDialog.html", sizeof(filename));
// // 	else	Jstrncat(filename, path.c_str(), sizeof(filename));
// // 	// send files
// // 	switch (GetContentType(path))
// // 	{
// // 	default:
// // 	case HCT_Html:
// // 		Jstrncpy(content_type, "text/html; charset=UTF-8", sizeof(content_type));
// // 		break;
// // 	case HCT_Css:
// // 		Jstrncpy(content_type, "text/css; charset=UTF-8", sizeof(content_type));
// // 		break;
// // 	case HCT_Js:
// // 		Jstrncpy(content_type, "application/javascript; charset=UTF-8", sizeof(content_type));
// // 		break;
// // 	}

// // 	if (fio = fopen(filename, "rb"))
// // 	{
// // 		fseek(fio, 0, SEEK_END);
// // 		fileLength = ftell(fio);
// // 		rewind(fio);

// // 		DTrace_CommonMsg("web file = %s\n", filename, __LINE__, __FILE__);

// // 		web_conn->SendBuf.wsabuf.len = Jsnprintf(web_conn->SendBuf.wsabuf.buf, web_conn->SendBuf.size,
// // 			"HTTP/1.1 200 OK\r\n"
// // 			"Content-Length: %d\r\n"
// // 			"Keep-Alive: timeout=5, max=100\r\n"
// // 			"Connection: Keep-Alive\r\n"
// // 			"Content-Type: %s"
// // 			"\r\n\r\n"
// // 			, fileLength, content_type);

// // 		if ((ret = NetSend(web_conn->mixsocket, web_conn->SendBuf.wsabuf, NULL))<0)
// // 		{
// // 			DTrace_Error("Send_200OK_Xml fail, mixsocket=%d, error = %d, sys_err=%s\r\n", web_conn->mixsocket, ret, GetNetCPIErrorMsg(ret));
// // 		}

// // 		while (1)
// // 		{
// // 			web_conn->SendBuf.empty();
// // 			web_conn->SendBuf.wsabuf.len = fread(web_conn->SendBuf.wsabuf.buf, 1, web_conn->SendBuf.size, fio);

// // 			if ((ret = NetSend(web_conn->mixsocket, web_conn->SendBuf.wsabuf, NULL))<0)
// // 			{
// // 				DTrace_Error("Send_200OK_Xml fail, mixsocket=%d, error = %d, sys_err=%s\r\n", web_conn->mixsocket, ret, GetNetCPIErrorMsg(ret));
// // 				break;
// // 			}

// // 			if (feof(fio)) break;
// // 		}
// // 		fclose(fio);
// // 	}
// // 	else
// // 	{
// // 		SendErrorCode(web_conn, HttpStatus::Code::NotFound);
// // 	}
// // 	CRASH_DEBUG_COMMAND;
// // }

// // void CNetIF::Send_200OK_Query(WEB_CONNECT* web_conn, std::string jasonCallback, std::string data_str)
// // {
// // 	CRASH_DEBUG_COMMAND;
// // 	int ret, ctx_len;
// // 	char messageBuf[2048];

// // 	ctx_len = data_str.length() + jasonCallback.length() + Jstrlen("()");

// // 	// DTrace_TestMsg("Send_200OK_Query, data_str=%s\n", data_str.c_str());

// // 	web_conn->SendBuf.wsabuf.len = snprintf(web_conn->SendBuf.wsabuf.buf, web_conn->SendBuf.size,
// // 		"HTTP/1.1 200 OK\r\n"
// // 		"Content-Length: %d\r\n"
// // 		"Keep-Alive: timeout=5, max=100\r\n"
// // 		"Connection: Keep-Alive\r\n"
// // 		"Content-Type: text/html\r\n\r\n"
// // 		"%s(%s)"
// // 		, ctx_len
// // 		, jasonCallback.c_str()
// // 		, data_str.c_str());

// // 	DTrace_CommonMsg("Send_200OK_Query, (len=%d) SendBuf = \n%s\n\n", web_conn->SendBuf.wsabuf.len, web_conn->SendBuf.wsabuf.buf);

// // 	if ((ret = NetSend(web_conn->mixsocket, web_conn->SendBuf.wsabuf, NULL))<0)
// // 	{
// // 		DTrace_Error("Send_200OK_Query fail, mixsocket=%d, error = %d, sys_err=%s\r\n", web_conn->mixsocket, ret, GetNetCPIErrorMsg(ret));
// // 	}
// // 	CRASH_DEBUG_COMMAND;
// // }

// // int CNetIF::SendErrorCode(WEB_CONNECT* web_conn, HttpStatus::Code code)
// // {
// // 	CRASH_DEBUG_COMMAND;
// // 	if (!web_conn)
// // 	{
// // 		DTrace_Error("Send_200OK_Xml, invalid parameters\n");
// // 		RETURN -1;
// // 	}
// // 	// if (code<100 && code>=600)
// // 	// {
// // 	// 	DTrace_Error("SendErrorCode, invalid parameters\n");
// // 	// 	RETURN -1;
// // 	// }

// // 	int ret;
// // 	int file_len = 0;
// // 	FILE *fio;

// // 	//--- send 404 code
// // 	if (code == HttpStatus::Code::NotFound)
// // 	{
// // 		if (fio = fopen("html/code/error_404.html", "rb"))
// // 		{
// // 			fseek(fio, 0, SEEK_END);
// // 			file_len = ftell(fio);
// // 			rewind(fio);

// // 			web_conn->SendBuf.wsabuf.len = Jsnprintf(web_conn->SendBuf.wsabuf.buf, web_conn->SendBuf.size,
// // 				"HTTP/1.1 200 OK\r\n"
// // 				"Content-Length: %d\r\n"
// // 				"Keep-Alive: timeout=5, max=100\r\n"
// // 				"Connection: Keep-Alive\r\n"
// // 				"Content-Type: text/html; charset=UTF-8"
// // 				"\r\n\r\n"
// // 				, file_len);

// // 			if ((ret = NetSend(web_conn->mixsocket, web_conn->SendBuf.wsabuf, NULL))<0)
// // 			{
// // 				DTrace_Error("SendErrorCode fail, mixsocket=%d, error = %d, sys_err=%s\r\n", web_conn->mixsocket, ret, GetNetCPIErrorMsg(ret));
// // 			}

// // 			while (1)
// // 			{
// // 				web_conn->SendBuf.empty();
// // 				web_conn->SendBuf.wsabuf.len = fread(web_conn->SendBuf.wsabuf.buf, 1, web_conn->SendBuf.size, fio);

// // 				if ((ret = NetSend(web_conn->mixsocket, web_conn->SendBuf.wsabuf, NULL))<0)
// // 				{
// // 					DTrace_Error("SendErrorCode fail, mixsocket=%d, error = %d, sys_err=%s\r\n", web_conn->mixsocket, ret, GetNetCPIErrorMsg(ret));
// // 					break;
// // 				}

// // 				if (feof(fio)) break;
// // 			}
// // 			fclose(fio);
// // 			RETURN 0;
// // 		}
// // 	}

// // 	//--- send error code
// // 	char	body[128];
// // 	Jsprintf(body, sizeof(body), "<h1>%d</h1><h2>%s</h2>", toInt(code), HttpStatus::reasonPhrase(code).c_str());
// // 	file_len = Jstrnlen(body, sizeof(body));

// // 	// DTrace_TestMsg("SendErrorCode, code=%d, file_len=%d, body=%s\n", toInt(code), file_len, body);

// // 	web_conn->SendBuf.wsabuf.len = Jsnprintf(web_conn->SendBuf.wsabuf.buf, web_conn->SendBuf.size,
// // 		"HTTP/1.1 200 OK\r\n"
// // 		"Content-Length: %d\r\n"
// // 		"Keep-Alive: timeout=5, max=100\r\n"
// // 		"Connection: Keep-Alive\r\n"
// // 		"Content-Type: text/html; charset=UTF-8"
// // 		"\r\n\r\n"
// // 		"%s"
// // 		, file_len,
// // 		body
// // 	);
// // 	if ((ret = NetSend(web_conn->mixsocket, web_conn->SendBuf.wsabuf, NULL))<0)
// // 	{
// // 		DTrace_Error("SendErrorCode fail, mixsocket=%d, error = %d, sys_err=%s\r\n", web_conn->mixsocket, ret, GetNetCPIErrorMsg(ret));
// // 	}

// // 	RETURN 0;
// // }

// // int CNetIF::AssignFreeWebInfo()
// // {
// // 	CRASH_DEBUG_COMMAND;
// // 	for(int i=0; i<WEB_CONNECT_SIZE; i++)
// // 	{
// // 		if(!m_WebConnectInfo[i].flag)
// // 		{
// // 			m_WebConnectInfo[i].index=i;
// // 			m_WebConnectInfo[i].flag=true;
// // 			m_WebConnectCount++;
// // 			DTrace_CommonMsg("web connect number=%d, %s\r\n", m_WebConnectCount, COLOR_FG(FG_Cyan, "line=%d", i));
// // 			RETURN i;
// // 		}
// // 	}
// // 	DTrace_Error("AssignFreeWebInfo, m_WebConnectCount=%d\n", m_WebConnectCount);
// // 	RETURN -1;
// // }

// // int CNetIF::ClearWebinfo(WEB_CONNECT* pWeb_conn)
// // {
// // 	CRASH_DEBUG_COMMAND;
// // 	if (!pWeb_conn)
// // 	{
// // 		DTrace_Error("ClearWebinfo, invalid parameters\n");
// // 		RETURN -1;
// // 	}

// // 	DTrace_CommonMsg("ClearWebinfo, web connect number=%d, %s\r\n", m_WebConnectCount, COLOR_FG(FG_Cyan, "line=%d", pWeb_conn->index));
// // 	MutexLock(&pWeb_conn->Mutex);
// // 	if (m_WebConnectCount>0) m_WebConnectCount--;
// // 	NetCleanMixSocket(pWeb_conn->mixsocket);
// // 	pWeb_conn->reset();
// // 	KillMixTimer(m_MixTimerCoreObjectId, &pWeb_conn->MixTimer);
// // 	MutexUnlock(&pWeb_conn->Mutex);
// // 	RETURN 0;
// // }

// // int CNetIF::OnWindowProc(unsigned int message, MIXSOCKET wParam, unsigned int lParam, void *lpobj)
// // {
// // 	CRASH_DEBUG_COMMAND;

// // 	char *buf;
// // 	int recv_len, ret;
// // 	int line;
// // 	int subsock;
// // 	char IP[100];
// // 	struct sockaddr_mix addrin;
// // 	WEB_CONNECT *pWeb_conn = NULL;
// // 	std::string jasonCallback;
// // 	std::string recv_data;

// // 	if (message == 0)
// // 	{
// // 		switch(lParam)
// // 		{
// // 		case TFD_ACCEPT:
// // 			DTrace_CommonMsg("%s, wParam=%d, lParam=%d, lpobj=0x%x\n", COLOR_FG(FG_Cyan, "TFD_ACCEPT"), wParam, lParam, lpobj);
// // 			line=AssignFreeWebInfo();
// // 			if(line<0)
// // 			{
// // 				if ((ret = NetAccept(wParam, &subsock, &addrin))<0)
// // 				{
// // 					DTrace_Error("NetAccept fail when line<0, error = %d, sys_err=%s\r\n", ret, GetNetCPIErrorMsg(ret));
// // 				}
// // 				closesocket(subsock);
// // 				break;
// // 			}
// // 			pWeb_conn = &m_WebConnectInfo[line];
// // 			if ((ret = NetAccept(wParam, &subsock, &pWeb_conn->addr))<0)
// // 			{
// // 				DTrace_Error("NetAccept fail, error = %d, sys_err=%s\r\n", ret, GetNetCPIErrorMsg(ret));
// // 			}
// // 			if (pWeb_conn->mixsocket != NULL_MIXSOCKET)	ret = -1;
//			// else	NetJoinMixSocket(m_NetcpiId, &pWeb_conn->mixsocket, subsock, pWeb_conn->RecvBuf.wsabuf, &m_WebConnectInfo[line], SocketModule_Callbcak_Function);
// // 			if (ret<0)
// // 			{
// // 				DTrace_Error("NetJoinMixSocket fail, error = %d, sys_err=%s\r\n", ret, GetNetCPIErrorMsg(ret));
// // 				closesocket(subsock);
// // 			}
// // 			NetResignInEvent(wParam);
// // 			SetMixTimer(m_MixTimerCoreObjectId, &pWeb_conn->MixTimer, WEB_UI_RELOAD_TIME*2, SERVER_UI_TIMER_EVENT + (line << 8));
// // 			break;

// // 		case TFD_READ:
// // 			DTrace_CommonMsg("%s, wParam=%d, lParam=%d, lpobj=0x%x\n", COLOR_FG(FG_Cyan, "TFD_READ"), wParam, lParam, lpobj);
// // 			pWeb_conn = (WEB_CONNECT*)lpobj;
// // 			if ((ret = NetRecv(wParam, &buf, &recv_len, &addrin))<0)
// // 			{
// // 				DTrace_Error("NetRecv fail, mixsocket=%d, error = %d, sys_err=%s\r\n", wParam, ret, GetNetCPIErrorMsg(ret));
// // 			}
// // 			else	recv_data.assign(buf, recv_len);
// // 			DTrace_TestMsg("READ from %s:%d , (len=%d) RecvBuf = \n%s\n\n", NetMakeAddrStr(addrin, IP), ntohs(addrin.sin_port), recv_data.length(), recv_data.c_str());
// // 			if(recv_len>0)
// // 			{
// // 				std::string data_str;
// // 				if(!strncmp(recv_data.c_str(), "GET", 3))
// // 				{
// // 					std::string http_path, event_type;
// // 					GetHttpPath(recv_data.c_str(), http_path);
// // 					DTrace_TestMsg("Get_Http_Type, path=%s\n", http_path.c_str());
// // 					switch (Get_Http_Type(http_path))
// // 					{
// // 						case HGT_XML:
// // 							Send_200OK_Xml(pWeb_conn, http_path);
// // 							DTrace_TestMsg("Get_Http_Type, type=HGT_XML\n");
// // 							break;
// // 						case HGT_Server_INFO:
// // 							Get_JasonCallback(recv_data.c_str(), &jasonCallback);
// // 							GetEventTypeByPath(http_path, event_type);
// // 							GetDataFromCallback(event_type, data_str);
// // 							Send_200OK_Query(pWeb_conn, jasonCallback, data_str);
// // 							DTrace_TestMsg("Get_Http_Type, type=HGT_Server_INFO\n");
// // 							break;
// // 						case HGT_UNKNOW:
// // 						default:
// // 							SendErrorCode(pWeb_conn, HttpStatus::Code::NotFound);
// // 							DTrace_TestMsg("Get_Http_Type, type=HGT_UNKNOW\n");
// // 							break;
// // 					}

// // 				}
// // 			}
// // 			NetResignInEvent(wParam);
// // 			SetMixTimer(m_MixTimerCoreObjectId, &pWeb_conn->MixTimer, WEB_UI_RELOAD_TIME*2, SERVER_UI_TIMER_EVENT + (pWeb_conn->index << 8));
// // 			break;

// // 		case TFD_CLOSE:
// // 			DTrace_CommonMsg("%s, wParam=%d, lParam=%d, lpobj=0x%x\n", COLOR_FG(FG_Cyan, "TFD_CLOSE"), wParam, lParam, lpobj);
// // 			pWeb_conn = (WEB_CONNECT*)lpobj;
// // 			ClearWebinfo(pWeb_conn);
// // 			break;
// // 		}
// // 	}
// // 	RETURN 0;
// // }

// // HTTP_GET_TYPE CNetIF::Get_Http_Type(std::string path)
// // {
// // 	CRASH_DEBUG_COMMAND;
// // 	if (path.find("jsoncallback=") != std::string::npos)
// // 	{
// // 		if (path.find("req_type=server_info") != std::string::npos)	{ RETURN HGT_Server_INFO; }
// // 		else	{ RETURN HGT_UNKNOW; }
// // 	}
// // 	else
// // 	{
// // 		if (!path.compare("/"))	{ RETURN HGT_XML; }
// // 		else
// // 		{
// // 			switch (GetContentType(path))
// // 			{
// // 			case HCT_Html:
// // 			case HCT_Css:
// // 			case HCT_Js:
// // 				RETURN HGT_XML;
// // 			default:
// // 				RETURN HGT_UNKNOW;
// // 			}
// // 		}
// // 	}
// // }

// // int CNetIF::GetEventTypeByPath(std::string path, std::string& event_type)
// // {
// // 	CRASH_DEBUG_COMMAND;
// // 	std::size_t beg, end;
// // 	const char szEvtType[]="evt_type=";
// // 	const int szEvtType_len = Jstrlen(szEvtType);

// // 	event_type.clear();

// // 	//--- extract event type
// // 	beg = path.find(szEvtType);
// // 	if (beg == std::string::npos)
// // 	{
// // 		DTrace_Error("GetEventTypeByPath, http path is not with evt_type parameter\n");
// // 		RETURN -2;
// // 	}
// // 	end = path.find('&', beg+szEvtType_len);
// // 	if (end == std::string::npos)	end = path.find(' ', beg+szEvtType_len);
// // 	if (end == std::string::npos)	end = path.length();
// // 	if (end<beg)
// // 	{
// // 		DTrace_FatalError("GetEventTypeByPath, system abnormal\n");
// // 		RETURN -3;
// // 	}
// // 	else	event_type = path.substr(beg+szEvtType_len, end-(beg+szEvtType_len));

// // 	RETURN event_type.length();
// // }

// // int CNetIF::GetEventTypeByPacket(std::string packet, std::string& event_type)
// // {
// // 	CRASH_DEBUG_COMMAND;
// // 	std::size_t http, beg, end;
// // 	const char szGET[]="GET";
// // 	const int szGET_len = Jstrlen(szGET);
// // 	const char szHTTP[]="HTTP/";
// // 	const int szHTTP_len = Jstrlen(szHTTP);
// // 	const char szEvtType[]="evt_type=";
// // 	const int szEvtType_len = Jstrlen(szEvtType);

// // 	event_type.clear();
// // 	//--- check HTTP format
// // 	if (packet.compare(0, szGET_len, szGET)!=0)
// // 	{
// // 		DTrace_Error("GetEventTypeByPacket, http method is not GET\n");
// // 		RETURN -2;
// // 	}

// // 	http = packet.find(szHTTP, szGET_len);
// // 	if (http==std::string::npos)
// // 	{
// // 		DTrace_Error("GetEventTypeByPacket, http format of packet is not correct\n");
// // 		RETURN -3;
// // 	}

// // 	//--- extract event type
// // 	beg = packet.find(szEvtType);
// // 	if (beg == std::string::npos)
// // 	{
// // 		DTrace_Error("GetEventTypeByPacket, http packet is not with evt_type parameter\n");
// // 		RETURN -4;
// // 	}
// // 	end = packet.find('&', beg+szEvtType_len);
// // 	if (end == std::string::npos)	end = packet.find(' ', beg+szEvtType_len);
// // 	if (end == std::string::npos || end>=http)
// // 	{
// // 		DTrace_Error("GetEventTypeByPacket, http packet is invalid\n");
// // 		RETURN -5;
// // 	}
// // 	else	event_type = packet.substr(beg+szEvtType_len, end-(beg+szEvtType_len));

// // 	RETURN event_type.length();
// // }

// // int CNetIF::GetHttpPath(const char *packet, std::string& path)
// // {
// // 	CRASH_DEBUG_COMMAND;
// // 	if (!packet)
// // 	{
// // 		DTrace_Error("GetHttpPath, invalid parameters\n");
// // 		RETURN -1;
// // 	}

// // 	std::string pkg = packet;
// // 	std::size_t http, beg, end;
// // 	const char szGET[]="GET";
// // 	const int szGET_len = Jstrlen(szGET);
// // 	const char szHTTP[]="HTTP/";
// // 	const int szHTTP_len = Jstrlen(szHTTP);

// // 	path.clear();

// // 	//--- check HTTP format
// // 	if (pkg.compare(0, szGET_len, szGET)!=0)
// // 	{
// // 		DTrace_Error("GetHttpPath, http method is not GET\n");
// // 		RETURN -2;
// // 	}

// // 	http = pkg.find(szHTTP, szGET_len);
// // 	if (http==std::string::npos)
// // 	{
// // 		DTrace_Error("GetHttpPath, http format of packet is not correct\n");
// // 		RETURN -3;
// // 	}

// // 	//--- extract path
// // 	beg = pkg.find("/", szGET_len);
// // 	if (beg==std::string::npos)
// // 	{
// // 		DTrace_Error("GetHttpPath, path is not found\n");
// // 		RETURN -4;
// // 	}
// // 	end = pkg.find(" ", beg+1);
// // 	if (end==std::string::npos)
// // 	{
// // 		DTrace_Error("GetHttpPath, path is not found\n");
// // 		RETURN -5;
// // 	}
// // 	if (end>=http)
// // 	{
// // 		DTrace_Error("GetHttpPath, path is not found\n");
// // 		RETURN -6;
// // 	}
// // 	else	path = pkg.substr(beg, end-beg);

// // 	RETURN path.length();

// // }

// // HTTP_CONTENT_TYPE CNetIF::GetContentType(std::string path)
// // {
// // 	CRASH_DEBUG_COMMAND;
// // 	const char szHtml[]="html", szCss[]="css", szJs[]="js";
// // 	const int szHtml_len=Jstrlen(szHtml), szCss_len=Jstrlen(szCss), szJs_len=Jstrlen(szJs);

// // 	std::size_t dot = path.find_last_of(".");
// // 	if (dot == std::string::npos)	{ RETURN HCT_Other; }
// // 	else if (path.compare(dot+1, szHtml_len, szHtml) == 0)	{ RETURN HCT_Html; }
// // 	else if (path.compare(dot+1, szCss_len, szCss) == 0)	{ RETURN HCT_Css; }
// // 	else if (path.compare(dot+1, szJs_len, szJs) == 0)	{ RETURN HCT_Js; }
// // 	else	{ RETURN HCT_Other; }

// // }

// // int CNetIF::Get_JasonCallback(const char *packet, std::string *jasonCallback)
// // {
// // 	CRASH_DEBUG_COMMAND
// // 	std::string http_packet_request;
// // 	int json_callback_pos = 0;
// // 	int json_callback_line_pos = 0;

// // 	http_packet_request = packet;
// // 	json_callback_pos = (int)http_packet_request.find("jsoncallback=", 0);
// // 	json_callback_pos += (int)strlen("jsoncallback=");

// // 	if (http_packet_request.find("&", json_callback_pos) != std::string::npos)
// // 		json_callback_line_pos = (int)http_packet_request.find("&", json_callback_pos);
// // 	else if (http_packet_request.find(" ", json_callback_pos) != std::string::npos)
// // 		json_callback_line_pos = (int)http_packet_request.find(" ", json_callback_pos);
// // 	else if (http_packet_request.find("\n", json_callback_pos) != std::string::npos)
// // 		json_callback_line_pos = (int)http_packet_request.find("\n", json_callback_pos);
// // 	else	{ RETURN -1; }

// // 	if (json_callback_line_pos > json_callback_pos)
// // 		*jasonCallback = http_packet_request.substr(json_callback_pos, json_callback_line_pos - json_callback_pos);
// // 	else	{ RETURN -1; }

// // 	RETURN 0;
// // }
// // void CNetIF::OnTimer(MIXTIMER *wParam)
// // {
// // 	CRASH_DEBUG_COMMAND;
// // 	int nIDEvent = GetMixTimerID(m_MixTimerCoreObjectId, wParam);
// // 	int Event=nIDEvent&0xff;
// // 	int line=(nIDEvent>>8);

// // 	if (Event==SERVER_UI_TIMER_EVENT)
// // 	{
// // 		WEB_CONNECT* pWeb_conn = &m_WebConnectInfo[line];
// // 		ClearWebinfo(pWeb_conn);
// // 	}
// // 	CRASH_DEBUG_COMMAND;
// // }

int CNetIF::NetObjectInit()
{
	CRASH_DEBUG_COMMAND;
	// TODO: Add initialization here
	int i, ret, lname;
	// int	tsize = getcpunum();
	int tsize = 1;

	// network setup
	ret = NetStartup();
	if (ret < 0)
	{
		DTrace_FatalError("NetObjectInit -- NetStartup() failed (%d) !!\r\n", ret);
		RETURN ret;
	}

	m_NetcpiId = NetCreate(tsize, tsize, 10, 10, 3, 3, 1);
	if (m_NetcpiId < 0)
	{
		// DTRACE(DTL_FatalError,"NetObjectInit -- NetCreate() failed (%d) !!\r\n", m_NetcpiId);
		RETURN ret;
	}
	lname = NetModuleName(m_NetcpiId);
	// DTrace_CommonMsg("NetObjectInit -- NetCreate() ID=%d (%s)!!\r\n", m_NetcpiId, GET_NET_MODULE_STR(lname));

	RETURN 0;
}
// void CNetIF::SetDlgCallback(SERVERDLG_CALLBACK lpFunction, void *lpObject)
// {
// 	CRASH_DEBUG_COMMAND;
// 	callbackFun = lpFunction;
// 	callbackObject = lpObject;
// 	CRASH_DEBUG_COMMAND;
// }

int CNetIF::StartNcServer(NetConnect_t *nc, int af_inet, int socket_type, const char *Server_IP, WORD ServerPort, MIXS_CALLBACK lpFunction, void *lpObject, uint32_t Event)
{
	CRASH_DEBUG_COMMAND;
	if (!nc)
	{
		DTrace_FatalError("StartTcpServer -- invalid paramters\r\n");
		RETURN - 10001;
	}
	if (nc->is_used())
	{
		DTrace_FatalError("StartTcpServer -- nc is used\r\n");
		RETURN - 10002;
	}
	int ret;

	nc->lpObject = lpObject;
	nc->lpFunction = lpFunction;
	nc->Event = Event;
	nc->af_inet = af_inet;
	nc->socket_type = socket_type;
	nc->role = 0;

	ret = NetCreateSocket(m_NetcpiId, &nc->mixsocket, nc->socket_type, nc->af_inet, nc, lpFunction);
	if (ret < 0)
	{
		// DTrace_FatalError("StartTcpServer -- Create socket fail, ServerPort=%d, error = %d, sys_err=%s\r\n", ServerPort, ret, GetNetCPIErrorMsg(ret));
		nc->reset();
		RETURN ret;
	}
	if (socket_type == SOCK_DGRAM)
	{	
		nc->alloc();
		ret = NetStartUServer(nc->mixsocket, nc->RecvBuf.wsabuf, Server_IP, ServerPort);
	}
	else if (socket_type == SOCK_STREAM)	ret = NetStartTServer(nc->mixsocket, Server_IP, ServerPort);
	if (ret < 0)
	{
		// DTrace_FatalError("StartTcpServer -- Start Start TCP server fail, ServerPort=%d, error = %d, sys_err=%s\r\n", ServerPort, ret, GetNetCPIErrorMsg(ret));
		NetNcClean(nc);
		RETURN ret;
	}
	NetGetSockName(nc->mixsocket, &nc->Laddr);
	RETURN 0;
}

int CNetIF::StartNcClient(NetConnect_t *nc, int af_inet, int socket_type, WORD ServerPort, const char *RIP, unsigned short RPort, bool tcpkeepalive, sockaddr_mix *lpRaddr, MIXS_CALLBACK lpFunction, void *lpObject, uint32_t Event)
{
	CRASH_DEBUG_COMMAND;
	if (!nc)
	{
		DTrace_FatalError("StartTcpClient -- invalid paramters\r\n");
		RETURN - 10001;
	}
	if (nc->is_used())
	{
		DTrace_FatalError("StartTcpClient -- nc is used\r\n");
		RETURN - 10002;
	}
	int ret;
	sockaddr_mix Raddr;

	nc->lpObject = lpObject;
	nc->lpFunction = lpFunction;
	nc->Event = Event;
	nc->af_inet = af_inet;
	nc->socket_type = socket_type;
	nc->role = 1;
	nc->tcpkeepalive = tcpkeepalive;

	ret = NetCreateSocket(m_NetcpiId, &nc->mixsocket, socket_type, af_inet, nc, lpFunction);
	if (ret < 0)
	{
		// DTrace_FatalError("StartTcpClient -- Create socket fail, ServerPort=%d, RIP=%s, RPort=%d, error = %d, sys_err=%s\r\n", ServerPort, RIP, RPort, ret, GetNetCPIErrorMsg(ret));
		nc->reset();
		RETURN ret;
	}
	if (tcpkeepalive)
		SetTcpSocketKeepAlive(NetGetSocketValue(nc->mixsocket), 5, 1);
	nc->alloc();
	ret = NetConnect(nc->mixsocket, nc->RecvBuf.wsabuf, NULL, ServerPort, RIP, RPort, &Raddr);
	if (ret < 0)
	{
		// DTrace_FatalError("StartTcpClient -- Start TCP client fail, ServerPort=%d, RIP=%s, RPort=%d, error = %d, sys_err=%s\r\n", ServerPort, RIP, RPort, ret, GetNetCPIErrorMsg(ret));
		NetNcClean(nc);
		RETURN ret;
	}
	NetGetSockName(nc->mixsocket, &nc->Laddr);
	
	RETURN 0;
}

int CNetIF::StartDestClient(DEST_DATA *lpDest, int af_inet, int socket_type, CRYPTO_TYPE Crypto_Type, WORD ServerPort, const char *RIP, unsigned short RPort, bool tcpkeepalive, MIXS_CALLBACK lpFunction, void *lpObject, uint32_t Event)
{
	CRASH_DEBUG_COMMAND;
	if (!lpDest || &RIP || lpFunction)
	{
		DTrace_FatalError("StartTcpClient -- invalid paramters\r\n");
		RETURN - 10001;
	}
	if (lpDest->pNc->is_used())
	{
		DTrace_FatalError("StartTcpClient -- nc is used\r\n");
		RETURN - 10002;
	}
	int ret;
	sockaddr_mix Raddr;

	// Nc
	lpDest->pNc->lpObject = lpObject;
	lpDest->pNc->lpFunction = lpFunction;
	lpDest->pNc->Event = Event;
	lpDest->pNc->af_inet = af_inet;
	lpDest->pNc->socket_type = socket_type;
	lpDest->pNc->role = 1;
	lpDest->pNc->tcpkeepalive = tcpkeepalive;
	// Dest
	Jstrncpy(lpDest->Real_IP, RIP, sizeof(lpDest->Real_IP));
	Jstrncpy(lpDest->Virtual_IP, RIP, sizeof(lpDest->Virtual_IP));
	lpDest->Port = RPort;
	lpDest->Crypto_Type = Crypto_Type;
	

	ret = NetCreateSocket(m_NetcpiId, &lpDest->pNc->mixsocket, socket_type, af_inet, lpDest->pNc, lpFunction);
	if (ret < 0)
	{
		// DTrace_FatalError("StartTcpClient -- Create socket fail, ServerPort=%d, RIP=%s, RPort=%d, error = %d, sys_err=%s\r\n", ServerPort, RIP, RPort, ret, GetNetCPIErrorMsg(ret));
		lpDest->reset();
		RETURN ret;
	}
	if (tcpkeepalive)
		SetTcpSocketKeepAlive(NetGetSocketValue(lpDest->pNc->mixsocket), 5, 1);
	lpDest->pNc->alloc();
	ret = NetConnect(lpDest->pNc->mixsocket, lpDest->pNc->RecvBuf.wsabuf, NULL, ServerPort, RIP, RPort, &lpDest->Raddr);
	if (ret < 0)
	{
		// DTrace_FatalError("StartTcpClient -- Start TCP client fail, ServerPort=%d, RIP=%s, RPort=%d, error = %d, sys_err=%s\r\n", ServerPort, RIP, RPort, ret, GetNetCPIErrorMsg(ret));
		NetNcClean(lpDest->pNc);
		lpDest->reset();
		RETURN ret;
	}
	NetGetSockName(lpDest->pNc->mixsocket, &lpDest->pNc->Laddr);
	
	RETURN 0;
}

int CNetIF::NetNcAccept(const NetConnect_t *nc, NetConnect_t *subnc, struct sockaddr_mix *addr)
{
	CRASH_DEBUG_COMMAND;
	if (!nc || !subnc)
	{
		DTrace_FatalError("NetNcAccept -- invalid paramters\r\n");
		RETURN - 10001;
	}
	if (!nc->is_used())
	{
		DTrace_FatalError("NetNcAccept -- nc is empty\r\n");
		RETURN - 10002;
	}
	if (subnc->is_used())
	{
		DTrace_FatalError("NetNcAccept -- subnc is used\r\n");
		RETURN - 10003;
	}
	int ret;
	int sockfd;

	subnc->af_inet = nc->af_inet;
	subnc->socket_type = nc->socket_type;
	subnc->role = nc->role;
	subnc->tcpkeepalive = nc->tcpkeepalive;
	subnc->lpObject = nc->lpObject;
	subnc->lpFunction = nc->lpFunction;
	subnc->Event = nc->Event;

	ret = NetAccept(nc->mixsocket, &sockfd, addr);
	if (ret < 0)
	{
		// DTrace_FatalError("NetNcAccept -- NetAccept failed, error = %d, sys_err=%s\r\n", ret, GetNetCPIErrorMsg(ret));
		subnc->reset();
		RETURN ret;
	}
	if (subnc->tcpkeepalive)
		SetTcpSocketKeepAlive(NetGetSocketValue(subnc->mixsocket), 5, 1);
	subnc->alloc();
	ret = NetJoinMixSocket(m_NetcpiId, &subnc->mixsocket, sockfd, subnc->RecvBuf.wsabuf, subnc->lpObject, subnc->lpFunction);
	if (ret < 0)
	{
		// DTrace_FatalError("NetNcAccept -- NetJoinMixSocket failed, error = %d, sys_err=%s\r\n", ret, GetNetCPIErrorMsg(ret));
		NetNcClean(subnc);
		RETURN ret;
	}
	RETURN 0;
}

int CNetIF::NetNcRecv(NetConnect_t *nc, char **lpbuf, int *len, struct sockaddr_mix *addr /*=NULL*/)
{
	CRASH_DEBUG_COMMAND;
	if (!nc)
	{
		RETURN - 1;
	}
	RETURN NetRecv(nc->mixsocket, lpbuf, len, addr);
}

int CNetIF::NetNcSend(NetConnect_t *nc, const char *lpbuf, size_t len, const struct sockaddr_mix *to /*=NULL*/)
{
	CRASH_DEBUG_COMMAND;
	if (!nc)
	{
		RETURN - 1;
	}
	if (lpbuf)
	{
		if (len <= 0)
		{
			RETURN - 2;
		}
		nc->SendBuf.copy_buf(lpbuf, len);
	}
	RETURN NetSend(nc->mixsocket, nc->SendBuf.wsabuf, to);
}

int CNetIF::NetNcResignInEvent(NetConnect_t *nc)
{
	CRASH_DEBUG_COMMAND;
	if (!nc)
	{
		RETURN - 1;
	}
	RETURN NetResignInEvent(nc->mixsocket);
}

int CNetIF::NetNcClean(NetConnect_t *nc)
{
	CRASH_DEBUG_COMMAND;
	if (!nc)
	{
		RETURN - 1;
	}
	int ret = NetCleanMixSocket(nc->mixsocket);
	nc->dealloc();
	nc->reset();
	RETURN ret;
}

int CNetIF::NetNcPostSockEvent(NetConnect_t *nc)
{
	CRASH_DEBUG_COMMAND;
	if (!nc)
	{
		RETURN - 1;
	}
	RETURN NetPostMixSockEvent(nc->mixsocket);
}

//-----------------------------------------
int CNetIF::TimerObjectInit()
{
	CRASH_DEBUG_COMMAND;
	int ret;

	m_MixTimerCoreObjectId = -1;
	MixTimerLibInit();
	if ((m_MixTimerCoreObjectId = CreateMixTimerCore(10, 1)) < 0)
	{
		DTrace_FatalError("TimerObjectInit -- CreateMixTimerCore failed\n");
		RETURN - 1;
	}
	RETURN 0;
}

int CNetIF::InitTimer(LPMIXTIMER mixTimer, void *lpObj, MIXT_CALLBACK callback)
{
	CRASH_DEBUG_COMMAND;
	if (m_MixTimerCoreObjectId < 0)
	{
		DTrace_FatalError("InitMixTimer -- Invalid MixTimerCoreObjectId\n");
		RETURN - 1;
	}
	InitMixTimer(m_MixTimerCoreObjectId, mixTimer, lpObj, callback);
	RETURN 0;
}

int CNetIF::SetTimer(LPMIXTIMER mixTimer, int timeout, unsigned int nID)
{
	CRASH_DEBUG_COMMAND;
	if (m_MixTimerCoreObjectId < 0)
	{
		DTrace_FatalError("SetMixTimer -- Invalid MixTimerCoreObjectId\n");
		RETURN - 1;
	}
	SetMixTimer(m_MixTimerCoreObjectId, mixTimer, timeout, nID);
	RETURN 0;
}

int CNetIF::KillTimer(LPMIXTIMER mixTimer)
{
	CRASH_DEBUG_COMMAND;
	if (m_MixTimerCoreObjectId < 0)
	{
		DTrace_FatalError("SetMixTimer -- Invalid MixTimerCoreObjectId\n");
		RETURN - 1;
	}
	KillMixTimer(m_MixTimerCoreObjectId, mixTimer);
	RETURN 0;
}

unsigned int CNetIF::GetTimerID(LPMIXTIMER mixTimer)
{
	CRASH_DEBUG_COMMAND;
	if (m_MixTimerCoreObjectId < 0)
	{
		DTrace_FatalError("SetMixTimer -- Invalid MixTimerCoreObjectId\n");
		RETURN - 1;
	}
	RETURN GetMixTimerID(m_MixTimerCoreObjectId, mixTimer);
}