// SIP_Server.cpp : implementation file
//

// #include "stdafx.h"
// #include "CallServer.h"
#include "SIP_Server.h"

// #include "Security.cpp"
// #include "module/Crypt/crypt.c"
#include "User.cpp"

// #ifdef _DEBUG
// #define new DEBUG_NEW
// #undef THIS_FILE
// static char THIS_FILE[] = __FILE__;
// #endif

/////////////////////////////////////////////////////////////////////////////
// CSIP_Server dialog

char Status_Msg[10][20] = {"", "Informational", "Success", "Redirection", "Client Error", "Server Error", "Global Failure", "unKnown Error", "", ""};
char SIP_Event_Table[65][40] =
	{
		// Informational
		"100 Trying", "180 Ringing", "181 Call Is Being Forwarded", "182 Queued", "183 Session Progress",
		// Success
		"200 OK", "202 Accepted",
		// Redirection
		"300 Multiple Choices", "301 Moved Permanently", "302 Moved Temporarily", "305 Use Proxy", "380 Alternative Service",
		// Client-Error
		"400 Bad Request", "401 Unauthorized", "402 Payment Required", "403 Forbidden", "404 Not Found", "405 Method Not Allowed",
		"406 Not Acceptable", "407 Proxy Authentication Required", "408 Request Timeout", "409 Conflict", "410 Gone",
		"411 Length required", "412 Conditional Request Failed", "413 Request Entity Too Large", "414 Request-URI Too Large", "415 Unsupported Media Type",
		"416 Unsupported URI Scheme", "417 Unknown Resource-Priority", "420 Bad Extension", "421 Extension Required", "422 Session Interval Too Small",
		"423 Interval Too Brief", "429 Provide Referrer Identity", "480 Temporarily Not Available", "481 Call Leg/Transaction Does Not Exist", "482 Loop Detected",
		"483 Too Many Hops", "484 Address Incomplete", "485 Ambiguous", "486 Busy Here", "487 Request Terminated",
		"488 Not Acceptable Here", "489 Bad Event", "491 Request Pending", "493 Undecipherable", "494 Security Agreement Required",
		// Server-Error
		"500 Internal Server Error", "501 Not Implemented", "502 Bad Gateway", "503 Service Unavailable", "504 Server Time-out",
		"505 SIP Version not supported", "513 Message Too Large", "580 Precondition Failed",
		// Global-Failure
		"600 Busy Everywhere", "603 Decline", "604 Does not exist anywhere", "606 Not Acceptable", ""};

// ===========================================================================================
// ===========================================================================================

CSIP_Server::CSIP_Server()
{
	//{{AFX_DATA_INIT(CSIP_Server)
	m_IP = "";
	m_Port = "8808";
	m_NTUT_IP = "";
	m_NTUT_Port = "";
	m_Outbound_PhoneNo = "";
	m_Log_File = "";
	m_Number_Search = "";
	//}}AFX_DATA_INIT
	// Create(IDD_SIP_SERVER);
	// ShowWindow(SW_HIDE);
	//SetWindowText("Dimeter SIP Server V1.0");
}

CSIP_Server::~CSIP_Server()
{
	Clean_All_Command_Data();
	//
	Clean_All_SubServer_Data();
	//
	Clean_All_SubServer_Mapping();
	//
	Clean_All_OnLine_Data();
	//
	Clean_All_Invite_Data();
	//
	if (NTUT_GW_Dest.is_used())
	{
		NTUT_GW_Dest.clean_nc();
		NTUT_GW_Dest.reset();
	}
	//
	SIP_Msg.FromDest.clean_nc();
	SIP_Msg.ToDest.clean_nc();
	SIP_Msg.reset();
	SIP_Msg2.FromDest.clean_nc();
	SIP_Msg2.ToDest.clean_nc();
	SIP_Msg2.reset();
}

// void CSIP_Server::DoDataExchange(CDataExchange* pDX)
// {
// 	CDialog::DoDataExchange(pDX);
// 	//{{AFX_DATA_MAP(CSIP_Server)
// 	DDX_Control(pDX, IDC_TAB1, m_Tab1);
// 	DDX_Control(pDX, IDC_LIST1, m_List1);
// 	DDX_Text(pDX, IDC_EDIT1, m_IP);
// 	DDX_Text(pDX, IDC_EDIT2, m_Port);
// 	DDX_Text(pDX, IDC_EDIT3, m_NTUT_IP);
// 	DDX_Text(pDX, IDC_EDIT4, m_NTUT_Port);
// 	DDX_Text(pDX, IDC_EDIT5, m_Outbound_PhoneNo);
// 	DDX_Text(pDX, IDC_EDIT6, m_Log_File);
// 	DDX_Text(pDX, IDC_EDIT7, m_Number_Search);
// 	//}}AFX_DATA_MAP
// }

// BEGIN_MESSAGE_MAP(CSIP_Server, CDialog)
// 	//{{AFX_MSG_MAP(CSIP_Server)
// 	ON_WM_TIMER()
// 	ON_EN_CHANGE(IDC_EDIT1, OnChangeEdit1)
// 	ON_EN_CHANGE(IDC_EDIT2, OnChangeEdit2)
// 	ON_EN_CHANGE(IDC_EDIT3, OnChangeEdit3)
// 	ON_EN_CHANGE(IDC_EDIT4, OnChangeEdit4)
// 	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, OnSelchangeTab1)
// 	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
// 	ON_EN_CHANGE(IDC_EDIT5, OnChangeEdit5)
// 	ON_EN_CHANGE(IDC_EDIT6, OnChangeEdit6)
// 	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
// 	ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
// 	ON_EN_CHANGE(IDC_EDIT7, OnChangeEdit7)
// 	ON_BN_CLICKED(IDC_BUTTON5, OnBrowse)
// 	ON_BN_CLICKED(IDC_BUTTON7, OnAccess_DB)
// 	ON_MESSAGE(SIP_SERVER_EVENT,OnSIP_Server)
// 	ON_MESSAGE(RTP_RELAY_EVENT,OnRTP_Relay)
// 	ON_MESSAGE(MONITOR_EVENT,OnMonitor)
// 	ON_BN_CLICKED(IDC_BUTTON4, OnServer_Setup)
// 	//}}AFX_MSG_MAP
// END_MESSAGE_MAP()

void CSIP_Server::SocketModule_Callbcak_Function(MIXSOCKET wParam, long lParam, void *lpObj)
{
	if (!lpObj)
	{
		DTrace_FatalError("invalid parameters, %d @ %s\n", __LINE__, __FILE__);
		RETURN;
	}
	NetConnect_t *pObj = (NetConnect_t *)lpObj;
	CSIP_Server *pSip = (CSIP_Server *)pObj->lpObject;
	if (!pSip)
	{
		DTrace_FatalError("invalid parameters, %d @ %s\n", __LINE__, __FILE__);
		RETURN;
	}

	switch (pObj->Event)
	{
	case SIP_SERVER_EVENT:
		pSip->OnSIP_Server((NetConnect_t *)lpObj, lParam);
		break;
	case RTP_RELAY_EVENT:
		pSip->OnRTP_Relay((NetConnect_t *)lpObj, lParam);
		break;
		// case MONITOR_EVENT:
		// 	pSip->OnMonitor(wParam, lParam);
		// break;
	}
}

void CSIP_Server::MixTimer_Callbcak_Function(MIXTIMER *wParam, void *lpObj)
{
	((CSIP_Server *)lpObj)->OnTimer(wParam);
}

/////////////////////////////////////////////////////////////////////////////
// CSIP_Server message handlers
// BOOL CSIP_Server::OnInitDialog()
// {
// 	CDialog::OnInitDialog();
// 	return TRUE;  // return TRUE unless you set the focus to a control
// 	              // EXCEPTION: OCX Property Pages should return FALSE
// }

int CSIP_Server::Start()
{
	CRASH_DEBUG_COMMAND;
	int i, j, k;
	char S1[100];
	// TC_ITEM	tcItem;

	// Initial Variable
	Write_Flag = FALSE;
	// GetDlgItem(IDC_BUTTON1)->SetWindowText("Enable Log File");
	Relay_100_Flag = 0;
	sprintf(S1, "Relay Level %d", Relay_100_Flag);
	// GetDlgItem(IDC_BUTTON3)->SetWindowText(S1);
	RTP_Relay_IP[0] = RTP_Relay_Port[0] = 0;
	Outbound_PhoneNo[0] = Log_File[0] = 0;
	// memset(&NTUT_GW_Addr, 0, sizeof(NTUT_GW_Addr));
	// NTUT_GW_IP[0] = NTUT_GW_Port[0] = 0;
	Access_CDMS_Flag = FALSE;
	m_lpNetIF = NULL;

	// Set lpSIP_Event_Table
	lpSIP_Event_Table[0] = 0;
	Get_Response_Code(NULL, 0);
	// Get Current Directory
	// GetCurrentDirectory(_MAX_PATH,Current_Directory);
	getcwd(Current_Directory, PATH_MAX);

	// Get Local_IP
	// WSADATA Data;
	// WSAStartup(0x202,&Data);
	hostent *lpHostEnt;
	gethostname(S1, sizeof(S1));
	if ((lpHostEnt = gethostbyname(S1)) != NULL)
	{
		for (k = 0; *(lpHostEnt->h_addr_list + k + 1) != 0; k++)
			;
		strcpy(Local_IP, inet_ntoa(*(LPIN_ADDR) * (lpHostEnt->h_addr_list + k)));
	}
	else
		Local_IP[0] = 0;

	// Get Initial Param
	Update_SIP_Server_Param((char *)"GET");

	// Start UDP Server
	Server_Port_Num = 0;
	int Port[] = SIP_SERVER_PORTS;
	m_lpNetIF = CNetIF::GetInstance();
	if (m_lpNetIF)
	{
		for (j = 0; j < SERVER_PORT_NUM; j++)
		{
			if ((SIP_Server_Port[j] = Port[j]) == 0)
				break;
			if (m_lpNetIF->StartNcServer(&SIP_Server_Nc[j], AF_INET, SOCK_DGRAM, SIP_Server_Port[j], SocketModule_Callbcak_Function, this, SIP_SERVER_EVENT) < 0)
				DTrace_Error("Start UDP(%d) Server Err\n", SIP_Server_Port[j]);
			else
				DTrace_TestMsg("Start UDP(%d) Server OK\n", SIP_Server_Port[j]);
			Server_Port_Num++;
		}
	}
	else
		DTrace_FatalError("CNetIF instance is invalid\n");

	// // Initial Param
	// UpdateData(TRUE);
	// sprintf(RTP_Relay_IP,"%s",m_IP);
	// sprintf(RTP_Relay_Port,"%s",m_Port);
	// sprintf(NTUT_GW_IP,"%s",m_NTUT_IP);
	// sprintf(NTUT_GW_Port,"%s",m_NTUT_Port);
	// sprintf(Outbound_PhoneNo,"%s",m_Outbound_PhoneNo);
	// sprintf(Log_File,"%s",m_Log_File);
	// UpdateData(FALSE);

	//Initial Data
	Init_Command();
	// Init_Monitor();
	Init_OnLine();
	Init_Invite();
	Init_SubServer_Mapping();
	Init_SubServer_Data();
	Init_CDMS_Data();

	// // Initial CDMS/Location Pointer
	// if ((lpSIP_Server_Setup=new CSIP_Server_Setup(this))!=NULL)	{ }

	// // Initial UI Menu
	// tcItem.mask = TCIF_TEXT; strcpy(S1,"OnLine"); tcItem.pszText = S1; tcItem.cchTextMax = strlen(S1); m_Tab1.InsertItem(0,&tcItem);
	// tcItem.mask = TCIF_TEXT; strcpy(S1,"Monitoring"); tcItem.pszText = S1; tcItem.cchTextMax = strlen(S1); m_Tab1.InsertItem(1,&tcItem);
	// tcItem.mask = TCIF_TEXT; strcpy(S1,"User"); tcItem.pszText = S1; tcItem.cchTextMax = strlen(S1); m_Tab1.InsertItem(2,&tcItem);
	// tcItem.mask = TCIF_TEXT; strcpy(S1,"REGISTER"); tcItem.pszText = S1; tcItem.cchTextMax = strlen(S1); m_Tab1.InsertItem(3,&tcItem);
	// tcItem.mask = TCIF_TEXT; strcpy(S1,"INVITE"); tcItem.pszText = S1; tcItem.cchTextMax = strlen(S1); m_Tab1.InsertItem(4,&tcItem);

	// Initial and STart SIP Server
	strcpy(&SIP_COMMAND[0][0], "REGISTER");
	strcpy(&SIP_COMMAND[1][0], "INVITE");
	strcpy(&SIP_COMMAND[2][0], "CANCEL");
	strcpy(&SIP_COMMAND[3][0], "ACK");
	strcpy(&SIP_COMMAND[4][0], "BYE");
	strcpy(&SIP_COMMAND[5][0], "OPTIONS");
	strcpy(&SIP_COMMAND[6][0], "REFER");
	strcpy(&SIP_COMMAND[7][0], "NOTIFY");
	strcpy(&SIP_COMMAND[8][0], "PRACK");
	strcpy(&SIP_COMMAND[9][0], "SIP/2.0");

	// Reload CDMS Data
	Set_User_Accont();


	RETURN 0;
}

void CSIP_Server::OnSIP_Server(NetConnect_t *wParam, long lParam)
{
	CRASH_DEBUG_COMMAND;
	int len, ret, i;
	sockaddr_mix Addr;
	char S1[SIP_MSG_LEGNTH + 10];
	char *buf;
	CRYPTO_TYPE Crypto_Type;

	switch (lParam)
	{
	case TFD_READ:
		// 1.讀取 SIP Message
		// sock_len = sizeof(sockaddr);
		// ret=recvfrom(wParam,S1,sizeof(S1)-1,0,(sockaddr *)&Addr,&Len); if (ret<=0) break; else S1[ret]=0;

		//
		ret = m_lpNetIF->NetNcRecv(wParam, &buf, &len, &Addr);

		if (ret < 0)
			break;
		else if (ret > SIP_MSG_LEGNTH)
			break;
		else if (ret < 10)
		{
			m_lpNetIF->NetNcSend(wParam, buf, len, &Addr);
			m_lpNetIF->NetNcResignInEvent(wParam);
			break;
		}
		else
		{
			memcpy(S1, buf, len);
			S1[len] = 0;
		}

		// Check Cryption
		// if		(check_sippacket(S1,ret)==1)	{ sip_decrypt(S1,ret,S1); S1[ret-4]=0; Crypto_Type=CT_SIPCrypto;}
		// else if (walkersun_check_sippacket(S1,ret)==1)	{ walkersun_sip_decrypt(S1,ret,S1); Crypto_Type=CT_Walkersun1;}
		// else
		Crypto_Type = CT_NoCrypto;

		// 3.分析 SIP Message
		if ((i = SIP_Message_Analyzer(wParam, &Addr, S1, &SIP_Msg, Crypto_Type)) == 0)
		{
			Write_Log("recvfrom", S1, &Addr, &SIP_Msg);
			if (!strcmp(SIP_Msg.Command, "REGISTER"))
				Process_REGISTER(&SIP_Msg);
			else if (!strcmp(SIP_Msg.Command, "ACK"))
				Process_ACK(&SIP_Msg);
			else if (!strcmp(SIP_Msg.Command, "INVITE"))
				Process_Proxy_Auth(&SIP_Msg);
			else if (!strcmp(SIP_Msg.Command, "CANCEL"))
				Process_CANCEL(&SIP_Msg);
			else if (!strcmp(SIP_Msg.Command, "PRACK"))
				Process_PRACK(&SIP_Msg);
			else if (!strcmp(SIP_Msg.Command, "BYE"))
				Process_BYE(&SIP_Msg);
			else if (!strcmp(SIP_Msg.Command, "REFER"))
				Process_REFER(&SIP_Msg);
			else if (!strcmp(SIP_Msg.Command, "NOTIFY"))
				Process_NOTIFY(&SIP_Msg);
			else if ((SIP_Msg.Command[0] >= '0') && (SIP_Msg.Command[0] <= '9'))
				Process_STATUS(&SIP_Msg);
		}
		else if (i < 0) // i=Error_Code
		{
			Write_Log("recvfrom", S1, &Addr, &SIP_Msg);
			Process_SIP_Error(&SIP_Msg, i);
		}
		m_lpNetIF->NetNcResignInEvent(wParam);
		SIP_Msg.FromDest.clean_nc();
		SIP_Msg.ToDest.clean_nc();
		break;
	}
	RETURN;
}

// -------------------------------------------------
void CSIP_Server::Process_SIP_Error(SIP_MSG *lpSIP_Msg, int Error_Code)
{
	char Status_Code[10];

	sprintf(Status_Code, "%d", Error_Code);
	Send_SIP_Command(Status_Code, lpSIP_Msg, NOCHANGE_VIA, FROM);
	Invite_Proc(TO, lpSIP_Msg, SIP_ERROR, Status_Code);
}

void CSIP_Server::Process_CANCEL(SIP_MSG *lpSIP_Msg)
{
	int Cur;

	// 1. Duplicated "CANCEL"
	if ((Cur = Check_Command(lpSIP_Msg)) >= 0)
		Send_SIP_Command(lpSIP_Msg->Command, lpSIP_Msg, ADD_VIA, TO);

	// 2. CANCEL Request: Forwarding
	else if (lpSIP_Msg->ToDest.is_used() && ((Cur = Save_Command(lpSIP_Msg)) >= 0))
	{ // Check if INVITE Process Exist
		Send_SIP_Command(lpSIP_Msg->Command, lpSIP_Msg, ADD_VIA, TO);
		m_lpNetIF->SetTimer(&Command_Data[Cur].MixTimer, TWO_SECOND, TIMERID_COMMAND + Cur);
		Invite_Proc(FROM, lpSIP_Msg, SIP_READY);
		Command_Data[Cur].State = SIP_CANCEL;
		Command_Data[Cur].Timer_Counter = 0;
	}

	// 3. To: OffLine or Command Buffer Full
	else
	{
		Send_SIP_Command("503", lpSIP_Msg, NOCHANGE_VIA, FROM);
	} // Via OK
}

void CSIP_Server::Process_PRACK(SIP_MSG *lpSIP_Msg)
{
	int Cur;

	// 1. Duplicated "PRACK"
	if ((Cur = Check_Command(lpSIP_Msg)) >= 0)
		Send_SIP_Command(lpSIP_Msg->Command, lpSIP_Msg, ADD_VIA, TO);

	// 2. PRACK Request: Forwarding
	else if (lpSIP_Msg->ToDest.is_used() && ((Cur = Save_Command(lpSIP_Msg)) >= 0))
	{ // Check if INVITE Process Exist
		Send_SIP_Command(lpSIP_Msg->Command, lpSIP_Msg, ADD_VIA, TO);
		//SetTimer(TIMERID_COMMAND+Cur,TWO_SECOND,NULL);
		//Invite_Proc(FROM, lpSIP_Msg, SIP_READY);
		//Command_Data[Cur].State=SIP_PRACK; Command_Data[Cur].Timer_Counter=0;
	}

	// 3. To: OffLine or Command Buffer Full
	else
	{
		Send_SIP_Command("503", lpSIP_Msg, NOCHANGE_VIA, FROM);
	} // Via OK
}

// ----------------------------------------------
void CSIP_Server::Process_BYE(SIP_MSG *lpSIP_Msg)
{
	int Cur111, Cur;
	SIP_MSG SIP_Data;
	char S1[100];

	// 1. Duplicated "BYE"
	if ((Cur = Check_Command(lpSIP_Msg)) >= 0)
		Send_SIP_Command(lpSIP_Msg->Command, lpSIP_Msg, ADD_VIA, TO);

	// 2. BYE Request: Forwarding
	else if (lpSIP_Msg->ToDest.is_used() && ((Cur = Save_Command(lpSIP_Msg)) >= 0))
	{ // Check if INVITE Process Exist
		memcpy(&SIP_Data, lpSIP_Msg, sizeof(SIP_MSG));
		strcpy(SIP_Data.Command, "INVITE");
		if ((Cur111 = Check_Command(&SIP_Data)) >= 0)
		{
			if (Command_Data[Cur111].State != SIP_READY)
				Clear_Command(Cur111);
		}
		else
		{
			strcpy(S1, SIP_Data.From_Name);
			strcpy(SIP_Data.From_Name, SIP_Data.To_Name);
			strcpy(SIP_Data.To_Name, S1);
			if ((Cur111 = Check_Command(&SIP_Data)) >= 0)
			{
				if (Command_Data[Cur111].State != SIP_READY)
					Clear_Command(Cur111);
			}
		}

		Send_SIP_Command(lpSIP_Msg->Command, lpSIP_Msg, ADD_VIA, TO);
		m_lpNetIF->SetTimer(&Command_Data[Cur].MixTimer, TWO_SECOND, TIMERID_COMMAND + Cur);
		Command_Data[Cur].State = SIP_BYE;
		Command_Data[Cur].Timer_Counter = 0;
		Invite_Proc(FROM, lpSIP_Msg, SIP_READY);
	}

	// 3. To: OffLine or Command Buffer Full
	else
	{
		Send_SIP_Command("503", lpSIP_Msg, NOCHANGE_VIA, FROM);
	} // Via OK
}

// ----------------------------------------------
void CSIP_Server::Process_REFER(SIP_MSG *lpSIP_Msg)
{
	int Cur;

	// 1. Duplicated "REFER"
	if ((Cur = Check_Command(lpSIP_Msg)) >= 0)
		Send_SIP_Command(lpSIP_Msg->Command, lpSIP_Msg, ADD_VIA, TO);

	// 2. REFER Request: Forwarding
	else if (lpSIP_Msg->ToDest.is_used() && ((Cur = Save_Command(lpSIP_Msg)) >= 0))
	{ // Check if INVITE Process Exist
		Send_SIP_Command(lpSIP_Msg->Command, lpSIP_Msg, ADD_VIA, TO);
		m_lpNetIF->SetTimer(&Command_Data[Cur].MixTimer, TWO_SECOND, TIMERID_COMMAND + Cur);
		Command_Data[Cur].State = SIP_REFER;
		Command_Data[Cur].Timer_Counter = 0;
	}

	// 3. To: OffLine or Command Buffer Full
	else
	{
		Send_SIP_Command("503", lpSIP_Msg, NOCHANGE_VIA, FROM);
	} // Via OK
}

void CSIP_Server::Process_NOTIFY(SIP_MSG *lpSIP_Msg)
{
	int Cur;

	Modify_NOTIFY(lpSIP_Msg);
	// 1. Duplicated "NOTIFY"
	if ((Cur = Check_Command(lpSIP_Msg)) >= 0)
		Send_SIP_Command(lpSIP_Msg->Command, lpSIP_Msg, ADD_VIA, TO);

	// 2. NOTIFY Request: Forwarding
	else if (lpSIP_Msg->ToDest.is_used() && ((Cur = Save_Command(lpSIP_Msg)) >= 0))
	{ // Check if INVITE Process Exist
		Send_SIP_Command(lpSIP_Msg->Command, lpSIP_Msg, ADD_VIA, TO);
		m_lpNetIF->SetTimer(&Command_Data[Cur].MixTimer, TWO_SECOND, TIMERID_COMMAND + Cur);
		Command_Data[Cur].State = SIP_NOTIFY;
		Command_Data[Cur].Timer_Counter = 0;
	}

	// 3. To: OffLine or Command Buffer Full
	else
	{
		Send_SIP_Command("503", lpSIP_Msg, NOCHANGE_VIA, FROM);
	} // Via OK
}

// ---------------------------------------------------
void CSIP_Server::Process_REGISTER(SIP_MSG *lpSIP_Msg)
{
	CRASH_DEBUG_COMMAND;
	int Online_Cur, Command_Cur, Step;
	bool Vary_Flag; //是否變換位置
	SIP_ONLINE_DATA PriOnline_Data;
	bool Reg_NoAuth_Flag, Reg_ANonce_Flag, First_Flag;

	Modify_Expire_Check = TRUE;
	if (Reg_Auth_Level == 4)
	{
		Reg_Auth = TRUE;
		Reg_VAddr_Check = FALSE;
		Reg_CNonce_Check = TRUE;
	}
	else if (Reg_Auth_Level == 3)
	{
		Reg_Auth = TRUE;
		Reg_VAddr_Check = FALSE;
		Reg_CNonce_Check = FALSE;
	}
	else if (Reg_Auth_Level == 2)
	{
		Reg_Auth = TRUE;
		Reg_VAddr_Check = TRUE;
		Reg_CNonce_Check = TRUE;
	}
	else if (Reg_Auth_Level == 1)
	{
		Reg_Auth = TRUE;
		Reg_VAddr_Check = TRUE;
		Reg_CNonce_Check = FALSE;
	}
	else if (Reg_Auth_Level == 0)
	{
		Reg_Auth = FALSE;
		Reg_VAddr_Check = FALSE;
		Reg_CNonce_Check = FALSE;
	}

	if ((Online_Cur = Check_OnLine(lpSIP_Msg->From_Name)) >= 0)
	{
		First_Flag = FALSE;
		Vary_Flag = Addrcmp(&lpSIP_Msg->FromDest.Raddr, &SIP_OnLine_Data[Online_Cur].Dest.Raddr);
		memcpy(&PriOnline_Data, &SIP_OnLine_Data[Online_Cur], sizeof(SIP_ONLINE_DATA));
	}
	else
	{
		Vary_Flag = TRUE;
		// 1. Save OnLine OK
		if ((Online_Cur = Save_OnLine(lpSIP_Msg, FROM)) < 0)
		{ // Server's Resource Fail
			if (Online_Cur == -1)
				Send_Auth_Msg("503", lpSIP_Msg, NOCHANGE_VIA, FROM); // Via OK
			// Account don't conform norms
			else if (Online_Cur < 0)
				Send_Auth_Msg("403", lpSIP_Msg, NOCHANGE_VIA, FROM); // Via OK
			RETURN;
		}
		else
			First_Flag = TRUE;
	}
	if (Online_Cur >= 0)
	{
		Check_MD5(lpSIP_Msg, Online_Cur, TRUE); //Access_CDMS_Flag
		// 帳號錯誤, 啥事都不做
		if (SIP_OnLine_Data[Online_Cur].CDMS_OK != 1)
		{
			Clear_OnLine(lpSIP_Msg);
			RETURN;
		}
		if (Reg_Auth_Level == 0)
			SIP_OnLine_Data[Online_Cur].MD5_OK = TRUE;
		if (Proxy_Auth_Level == 0 || (Proxy_Auth_Level == 1 && SIP_OnLine_Data[Online_Cur].MD5_OK))
			SIP_OnLine_Data[Online_Cur].Proxy_MD5_OK = TRUE;
		// No Authencation filter
		Reg_NoAuth_Flag =
			!Reg_Auth ||																										 //不認證
			Reg_Auth && (Reg_VAddr_Check && !Vary_Flag) ||																		 // 若在Addr沒變免認證模式中，且Addr沒變則不認證
			Reg_Auth && !Reg_CNonce_Check && (strlen(lpSIP_Msg->Authorization_Nonce) > 0 && SIP_OnLine_Data[Online_Cur].MD5_OK); //Pass with MD5 OK
		Reg_ANonce_Flag = Reg_Auth && Reg_CNonce_Check && (Reg_VAddr_Check && Vary_Flag);
	}

	if ((Command_Cur = Check_Command(lpSIP_Msg)) >= 0 && Command_Data[Command_Cur].State == SIP_REG)
	{
		if (!SIP_OnLine_Data[Online_Cur].MD5_OK)
		{
			CRASH_DEBUG_COMMAND;
			// SIP_REG -> SIP_READY: MD5 驗證失敗
			if (SIP_OnLine_Data[Online_Cur].Auth_OK == TRUE) //Auth_OK=TRUE indicate with Auth. header and Disget no lose
			{
				Send_Auth_Msg("403", lpSIP_Msg, NOCHANGE_VIA, FROM);
				Clear_Command(Command_Cur);
			} // Via OK
			// SIP_REG -> SIP_REG: REGISTER(Duplicate)
			else
			{
				CRASH_DEBUG_COMMAND;
				m_lpNetIF->SetTimer(&Command_Data[Command_Cur].MixTimer, TWO_SECOND, TIMERID_COMMAND + Command_Cur);
				if (Reg_ANonce_Flag)
					Proc_Auth(lpSIP_Msg, Command_Cur, Online_Cur, "401"); // Reassign new Nonce
				Send_Auth_Msg(Command_Data[Command_Cur].Response_SIP_Msg.Command, &Command_Data[Command_Cur].Response_SIP_Msg, NOCHANGE_VIA, FROM);
			}
		}
		// SIP_REG -> SIP_RELAY: 帳號存在, MD5 驗證 OK
		else
		{ // Modify Expires
			if (!First_Flag && Modify_Expire_Check)
			{
				CRASH_DEBUG_COMMAND;
				SIP_OnLine_Data[Online_Cur].Expires_Real = PriOnline_Data.Expires_Counter; // 紀錄實際之 Expires 值
				if (Progressive_Expires_Check)
				{
					if (atoi(lpSIP_Msg->Expires) != 0)
					{
						if (Vary_Flag)
							Step = (SIP_OnLine_Data[Online_Cur].Expires_OK) ? atoi(lpSIP_Msg->Expires) - 5 : atoi(lpSIP_Msg->Expires) / 2;
						else
							Step = atoi(lpSIP_Msg->Expires) + 5;
						SIP_OnLine_Data[Online_Cur].Expires_OK = TRUE;
					}
					else
						Step = 0;
					Modify_Expires(lpSIP_Msg, Step);
				}
				else if (Locking_Expires > 0)
					Modify_Expires(lpSIP_Msg, Locking_Expires);
			}
			CRASH_DEBUG_COMMAND;
			Save_OnLine(lpSIP_Msg, FROM); //Already Online
			m_lpNetIF->KillTimer(&Command_Data[Command_Cur].MixTimer);
			Send_Auth_Msg("200", lpSIP_Msg, NOCHANGE_VIA, FROM); // Via OK
			Clear_Command(Command_Cur);
		}
	}

	// New Command but Command Buffer Full
	else if ((Command_Cur = Save_Command(lpSIP_Msg)) == -1)
	{
		Send_Auth_Msg("503", lpSIP_Msg, NOCHANGE_VIA, FROM);
		Clear_OnLine(lpSIP_Msg);
	}

	// Register without Authenticate Digest or First Authenticate
	else
	{
		// SIP_READY -> SIP_REG: Must authencation
		if (!Reg_NoAuth_Flag)
		{
			CRASH_DEBUG_COMMAND;
			m_lpNetIF->SetTimer(&Command_Data[Command_Cur].MixTimer, TWO_SECOND, TIMERID_COMMAND + Command_Cur);
			// MD5 未驗證, 要求驗證(初次註冊 或 前次sockaddr改變)
			Proc_Auth(lpSIP_Msg, Command_Cur, Online_Cur, "401");
			Send_Auth_Msg(Command_Data[Command_Cur].Response_SIP_Msg.Command, &Command_Data[Command_Cur].Response_SIP_Msg, NOCHANGE_VIA, FROM);
			Command_Data[Command_Cur].State = SIP_REG;
		}
		// SIP_READY -> SIP_READY: Account Already online and MD5_OK and Addr no Vary and Pri-MD5_OK when Auth_Flag=TRUE, or Auth_Flag=FALSE
		else
		{ // Modify Expires
			if (!First_Flag && Modify_Expire_Check)
			{
				SIP_OnLine_Data[Online_Cur].Expires_Real = PriOnline_Data.Expires_Counter; // 紀錄實際之 Expires 值
				if (Progressive_Expires_Check)
				{
					if (atoi(lpSIP_Msg->Expires) != 0)
					{
						if (Vary_Flag)
							Step = (SIP_OnLine_Data[Online_Cur].Expires_OK) ? atoi(lpSIP_Msg->Expires) - 5 : atoi(lpSIP_Msg->Expires) / 2;
						else
							Step = atoi(lpSIP_Msg->Expires) + 5;
						SIP_OnLine_Data[Online_Cur].Expires_OK = TRUE;
					}
					else
						Step = 0;
					Modify_Expires(lpSIP_Msg, Step);
				}
				else if (Locking_Expires > 0)
					Modify_Expires(lpSIP_Msg, Locking_Expires);
			}
			CRASH_DEBUG_COMMAND;
			Save_OnLine(lpSIP_Msg, FROM);						 //Already Online
			Send_Auth_Msg("200", lpSIP_Msg, NOCHANGE_VIA, FROM); // Via OK
			Clear_Command(Command_Cur);
		}
	}
	RETURN;
}

void CSIP_Server::Process_Proxy_Auth(SIP_MSG *lpSIP_Msg)
{
	CRASH_DEBUG_COMMAND;
	int j, k, Online_Cur, Command_Cur, Invite_Cur;
	bool Vary_Flag; //是否變換位置
	bool Proxy_NoAuth_Flag, Proxy_Except_Flag, Proxy_ANonce_Flag;
	SIP_ONLINE_DATA PriOnline_Data;

	// Set Param
	if (Proxy_Auth_Level == 3)
	{
		Proxy_Auth = TRUE;
		Proxy_RMD5_Check = FALSE;
		Proxy_CNonce_Check = TRUE;
	}
	else if (Proxy_Auth_Level == 2)
	{
		Proxy_Auth = TRUE;
		Proxy_RMD5_Check = FALSE;
		Proxy_CNonce_Check = FALSE;
	}
	else if (Proxy_Auth_Level == 1)
	{
		Proxy_Auth = TRUE;
		Proxy_RMD5_Check = TRUE;
		Proxy_CNonce_Check = FALSE;
	}
	else if (Proxy_Auth_Level == 0)
	{
		Proxy_Auth = FALSE;
		Proxy_RMD5_Check = FALSE;
		Proxy_CNonce_Check = FALSE;
	}

	if ((Online_Cur = Check_OnLine(lpSIP_Msg->From_Name)) >= 0)
	{
		Vary_Flag = Addrcmp(&lpSIP_Msg->FromDest.Raddr, &SIP_OnLine_Data[Online_Cur].Dest.Raddr);
		memcpy(&PriOnline_Data, &SIP_OnLine_Data[Online_Cur], sizeof(SIP_ONLINE_DATA));
	}
	else
	{
		Vary_Flag = TRUE;
		// Save OnLine OK
		if ((Online_Cur = Save_OnLine(lpSIP_Msg, FROM)) < 0)
		{ // Server's Resource Fail
			if (Online_Cur == -1)
				Send_Auth_Msg("503", lpSIP_Msg, NOCHANGE_VIA, FROM); // Via OK
			// Account don't conform norms
			else if (Online_Cur < 0)
				Send_Auth_Msg("403", lpSIP_Msg, NOCHANGE_VIA, FROM); // Via OK
			RETURN;
		}
	}
	if (Online_Cur >= 0)
	{
		Check_MD5(lpSIP_Msg, Online_Cur, FALSE);
		if (Proxy_Auth_Level == 0)
			SIP_OnLine_Data[Online_Cur].Proxy_MD5_OK = TRUE;
		// Exception IP List,  No Authencation
		//	if (!strcmp(lpSIP_Msg->FR_IP, "140.124.40.116"))	Proxy_Except_Flag = TRUE;
		//	else Proxy_Except_Flag = FALSE;
		Proxy_Except_Flag = FALSE;
		// No Authencation filter
		Proxy_NoAuth_Flag =
			!Proxy_Auth ||																							//不認證
			Proxy_Except_Flag ||																					//例外清單
			Proxy_Auth && (Proxy_RMD5_Check && SIP_OnLine_Data[Online_Cur].MD5_OK) ||								//當Reg認證OK則不做Proxy認證
			Proxy_Auth && (strlen(lpSIP_Msg->Authorization_Nonce) > 0 && SIP_OnLine_Data[Online_Cur].Proxy_MD5_OK); //Pass with Proxy Auth OK
		Proxy_ANonce_Flag = Proxy_Auth && !Proxy_RMD5_Check && Proxy_CNonce_Check && Vary_Flag;
		DTrace_CommonMsg("Online_Cur>=0, =%d\r\n", Online_Cur);
	}

	// Command Already On-Process
	if ((Command_Cur = Check_Command(lpSIP_Msg)) >= 0 && Command_Data[Command_Cur].State == SIP_PROXY_AUTH)
	{ // SIP_REG -> SIP_REG: REGISTER(Duplicate)
		m_lpNetIF->SetTimer(&Command_Data[Command_Cur].MixTimer, TWO_SECOND, TIMERID_COMMAND + Command_Cur);
		if (Proxy_ANonce_Flag)
			Proc_Auth(lpSIP_Msg, Command_Cur, Online_Cur, "407"); // Reassign new Nonce
		Send_Auth_Msg(Command_Data[Command_Cur].Response_SIP_Msg.Command, &Command_Data[Command_Cur].Response_SIP_Msg, NOCHANGE_VIA, FROM);
		DTrace_CommonMsg("Command_Cur>=0,  =%d\r\n", Command_Cur);
	}

	// SIP_READY -> SIP_READY
	else if (Proxy_NoAuth_Flag)
	{
		Save_OnLine(lpSIP_Msg, FROM); //Already Online
		if (((j = Get_Position(lpSIP_Msg->Cmd_Str, "Proxy-Authorization:")) > 0) && !(lpSIP_Msg->Command[0] >= 0x30 && lpSIP_Msg->Command[0] <= 0x39))
		{
			if ((k = Get_NextLine(&lpSIP_Msg->Cmd_Str[j])) > 0)
			{
				k += j;
				Deletion(lpSIP_Msg->Cmd_Str, j, k - 1);
			}
		} // Remove Proxy-Authorization
		// SendMessage(INVITE_EVENT, (WPARAM)lpSIP_Msg, (LPARAM)NULL);
		Process_INVITE(lpSIP_Msg);
	}

	// New Command but Command Buffer Full
	else if ((Command_Cur = Save_Command(lpSIP_Msg)) == -1)
		Send_Auth_Msg("503", lpSIP_Msg, NOCHANGE_VIA, FROM);

	// SIP_READY/SIP_PROXY_AUTH -> SIP_PROXY_AUTH: Check Proxy_Authorization
	else if (!Proxy_NoAuth_Flag)
	{ // Restore Partial Dial-No. function for Auth.
		if ((Invite_Cur = Check_Invite(lpSIP_Msg)) >= 0 && strlen(SIP_Invite_Data[Invite_Cur].Partial_PhoneNo) > 0)
		{
			strcpy(lpSIP_Msg->To_Name, SIP_Invite_Data[Invite_Cur].Partial_PhoneNo);
			strcpy(lpSIP_Msg->Command_Name, SIP_Invite_Data[Invite_Cur].Partial_PhoneNo);
			Edit_Name(lpSIP_Msg->Cmd_Str, "INVITE", lpSIP_Msg->To_Name);
			Edit_Name(lpSIP_Msg->Cmd_Str, "To:", SIP_Invite_Data[Invite_Cur].Partial_PhoneNo);
		}
		// Process Auth
		m_lpNetIF->SetTimer(&Command_Data[Command_Cur].MixTimer, TWO_SECOND, TIMERID_COMMAND + Command_Cur);
		Proc_Auth(lpSIP_Msg, Command_Cur, Online_Cur, "407");
		Send_Auth_Msg(Command_Data[Command_Cur].Response_SIP_Msg.Command, &Command_Data[Command_Cur].Response_SIP_Msg, NOCHANGE_VIA, FROM);
		Command_Data[Command_Cur].State = SIP_PROXY_AUTH;
		Command_Data[Command_Cur].Timer_Counter = 0;
		DTrace_CommonMsg("Proxy_Authorization\r\n");
	}
	RETURN;
}

int CSIP_Server::Send_Auth_Msg(const char *Command, SIP_MSG *lpSIP_Data, int Via_Flag, int Send_Flag)
{
	CRASH_DEBUG_COMMAND;
	int i, k;
	char S1[SIP_MSG_LEGNTH], S2[1000];
	bool Quintum_Flag;

	Quintum_Flag = FALSE;
	strcpy(S1, lpSIP_Data->Cmd_Str);

	// Send "100 Tring" before "401" or "200" for Unintum UA
	if (strstr(lpSIP_Data->User_Agent, "Quintum") != NULL)
	{
		Quintum_Flag = TRUE;
		Send_SIP_Command("100", lpSIP_Data, NOCHANGE_VIA, FROM);
	}

	// WWW-Authenticate:
	if (!strcmp(Command, "401"))
	{
		if ((i = Get_Position(S1, "WWW-Authenticate:")) > 0)
		{
			k = Get_NextLine(&S1[i]) + i;
			Deletion(S1, i, k - 1);
		}
		else if ((i = Get_Position(S1, "Contact:")) < 0)
		{
			if ((i = Get_Position(S1, "Via:")) < 0)
			{
				if ((i = Get_Position(S1, "Expires:")) < 0)
					i = Get_Position(S1, "CSeq:");
			}
		}
		if (i > 0)
		{
			if (Quintum_Flag)
			{
				sprintf(S2, "WWW-Authenticate: Digest realm=\"%s\", nonce=\"%s\"\r\n",
						lpSIP_Data->WWW_Authenticate_Realm, lpSIP_Data->WWW_Authenticate_Nonce);
			}
			else
			{
				sprintf(S2, "WWW-Authenticate: Digest realm=\"%s\", nonce=\"%s\", algorithm=\"%s\"\r\n",
						lpSIP_Data->WWW_Authenticate_Realm, lpSIP_Data->WWW_Authenticate_Nonce, lpSIP_Data->WWW_Authenticate_Algorithm);
			}
			Insertion(S1, i, S2);
		}
	}
	// WWW-Authenticate:
	else if (!strcmp(Command, "407"))
	{
		if ((i = Get_Position(S1, "Proxy-Authenticate:")) > 0)
		{
			k = Get_NextLine(&S1[i]) + i;
			Deletion(S1, i, k - 1);
		}
		else if ((i = Get_Position(S1, "Contact:")) < 0)
		{
			if ((i = Get_Position(S1, "Via:")) < 0)
			{
				if ((i = Get_Position(S1, "Expires:")) < 0)
					i = Get_Position(S1, "CSeq:");
			}
		}
		if (i > 0)
		{
			if (Quintum_Flag)
			{
				sprintf(S2, "Proxy-Authenticate: Digest realm=\"%s\", nonce=\"%s\"\r\n",
						lpSIP_Data->Proxy_Authenticate_Realm, lpSIP_Data->Proxy_Authenticate_Nonce);
			}
			else
			{
				sprintf(S2, "Proxy-Authenticate: Digest realm=\"%s\", nonce=\"%s\", algorithm=\"%s\"\r\n",
						lpSIP_Data->Proxy_Authenticate_Realm, lpSIP_Data->Proxy_Authenticate_Nonce, lpSIP_Data->Proxy_Authenticate_Algorithm);
			}
			Insertion(S1, i, S2);
		}
	}

	// Send SIP Command
	strcpy(lpSIP_Data->Cmd_Str, S1);
	Send_SIP_Command(Command, lpSIP_Data, NOCHANGE_VIA, FROM);

	RETURN 0;
}

// 此副程式有問題需修改
void CSIP_Server::Process_ACK(SIP_MSG *lpSIP_Msg)
{
	int Cur111;
	SIP_MSG SIP_Data;

	// Check if INVITE Process Exist
	memcpy(&SIP_Data, lpSIP_Msg, sizeof(SIP_MSG));
	strcpy(SIP_Data.Command, "INVITE");
	if ((Cur111 = Check_Command(&SIP_Data)) >= 0)
	{
		if (Command_Data[Cur111].State == SIP_200)
		{
			if (Command_Data[Cur111].Disconnect_Flag == FALSE)
				Send_SIP_Command(lpSIP_Msg->Command, lpSIP_Msg, ADD_VIA, TO);
			Command_Data[Cur111].State = SIP_ACK;
			Command_Data[Cur111].Timer_Counter = 0;
			Invite_Proc(FROM, lpSIP_Msg, SIP_200);
		}
		else if (Command_Data[Cur111].State == SIP_ACK)
		{
			if (Command_Data[Cur111].Disconnect_Flag == FALSE)
				Send_SIP_Command(lpSIP_Msg->Command, lpSIP_Msg, ADD_VIA, TO);
		}
		else if (Command_Data[Cur111].State == SIP_PROXY_AUTH)
		{
			m_lpNetIF->KillTimer(&Command_Data[Cur111].MixTimer);
			Clear_Command(Cur111);
		}
	}
}

void CSIP_Server::Process_INVITE(SIP_MSG *lpSIP_Msg)
{
	int i, Command_Cur;

	DTrace_CommonMsg("INVITE----------------------------\r\n");
	// 1. To_Error: "To" OffLine, Check UnME User, If OnLine, Send I0|PhoneNo|CallerID|.., 通知前來註冊
	if (!lpSIP_Msg->ToDest.is_used())
		Send_SIP_Command("404", lpSIP_Msg, NOCHANGE_VIA, FROM);

	// 2. Command Already On-Process
	else if ((Command_Cur = Check_Command(lpSIP_Msg)) >= 0)
	{
		// 2.1 INVITE(Duplicate): Waiting Relay, NOP, but Update SIP_Msg
		if ((Command_Data[Command_Cur].State == SIP_RELAY_CONNECT) || (Command_Data[Command_Cur].State == SIP_RELAY_READ))
		{
		}
		// 2.2 INVITE(Duplicate): UpdateCommand & Re-Send
		else if (Command_Data[Command_Cur].State == SIP_INVITE)
		{
			Send_SIP_Command(Command_Data[Command_Cur].SIP_Msg.Command, &Command_Data[Command_Cur].SIP_Msg, ADD_VIA, TO);
		}
		// 2.3 send 100/180/183
		else if ((Command_Data[Command_Cur].State == SIP_100) || (Command_Data[Command_Cur].State == SIP_200))
		{
			Send_SIP_Command(Command_Data[Command_Cur].Response_SIP_Msg.Command, &Command_Data[Command_Cur].Response_SIP_Msg, DELETE_VIA, FROM);
		} //ADD_VIA,FROM); }
		// 2.4 Clear Old Command(需考慮Relay之清除) and Save New Command(需考慮新Relay之Set)
		else if (Command_Data[Command_Cur].State == SIP_ACK)
		{
			Clear_Command(Command_Cur);
			// PostMessage(INVITE_EVENT,(WPARAM)lpSIP_Msg,(LPARAM)NULL);
			WindowProc(INVITE_EVENT, (WPARAM)lpSIP_Msg, (LPARAM)NULL);
		}
	}

	// 3. New Command but Command Buffer Full
	else if ((Command_Cur = Save_Command(lpSIP_Msg)) == -1)
	{
		Send_SIP_Command("503", lpSIP_Msg, NOCHANGE_VIA, FROM);
	} // Via OK

	// 4. New Command(RELAY): SIP_READY -> SIP_RELAY_CONNECT
	else if ((lpSIP_Msg->RTP_Relay_Flag) &&
			 ((i = m_lpNetIF->StartDestClient(&Command_Data[Command_Cur].Relay_TCP_Dest, AF_INET, SOCK_STREAM, CT_SIPCrypto, 0, lpSIP_Msg->Relay_TCP_IP, atoi(lpSIP_Msg->Relay_TCP_Port), false, SocketModule_Callbcak_Function, this, RTP_RELAY_EVENT)) > 0))
	{
		m_lpNetIF->SetTimer(&Command_Data[Command_Cur].MixTimer, TWO_SECOND, TIMERID_COMMAND + Command_Cur);
		Command_Data[Command_Cur].State = SIP_RELAY_CONNECT;
		Command_Data[Command_Cur].Timer_Counter = 0;
	}

	// 5. New Command(!Relay|Relay Fail): SIP_READY -> SIP_INVITE
	else
	{
		if (lpSIP_Msg->RTP_Relay_Flag)
		{
			lpSIP_Msg->RTP_Relay_Flag = FALSE;
			Command_Data[Command_Cur].SIP_Msg.RTP_Relay_Flag = FALSE;
		} // 強迫取消 Relay
		Send_SIP_Command(lpSIP_Msg->Command, lpSIP_Msg, ADD_VIA, TO);
		m_lpNetIF->SetTimer(&Command_Data[Command_Cur].MixTimer, TWO_SECOND, TIMERID_COMMAND + Command_Cur);
		// Command_Data[Command_Cur].Timer_Counter=0; has been set at Item 5(Save_Command)
		Command_Data[Command_Cur].State = SIP_INVITE;
		Command_Data[Command_Cur].Timer_Counter = 0;
	}
}

void CSIP_Server::Process_STATUS(SIP_MSG *lpSIP_Msg)
{
	CRASH_DEBUG_COMMAND;
	int Cur111, Online_Cur;

	// 1. Response associated with Command
	if ((Cur111 = Check_Command(lpSIP_Msg)) >= 0)
	{
		strcpy(lpSIP_Msg->ViaBuf, Command_Data[Cur111].SIP_Msg.ViaBuf); // 讓Delete_Via時 復原舊Via資料
		if (((!strcmp(lpSIP_Msg->Command, "183")) || (!strcmp(lpSIP_Msg->Command, "200"))) && (strlen(lpSIP_Msg->SDP) > 0) && (Command_Data[Cur111].SIP_Msg.RTP_Relay_Flag))
		{
			Modify_SDP(lpSIP_Msg, Command_Data[Cur111].Relay_UDP_IP, Command_Data[Cur111].Relay_UDP_FPort, Command_Data[Cur111].Relay_UDP_TPort);
			if (Command_Data[Cur111].SIP_Msg.RL1_Flag == TRUE)
			{
				Send_Relay_Command("0RL1", lpSIP_Msg, &Command_Data[Cur111]);
				Command_Data[Cur111].SIP_Msg.RL1_Flag = FALSE;
			}
		}
		Update_Command_By_Response(Cur111, lpSIP_Msg);

		// For INVITE Command
		if (Command_Data[Cur111].State == SIP_INVITE)
		{
			Send_SIP_Command(lpSIP_Msg->Command, lpSIP_Msg, DELETE_VIA, FROM); // Via OK
			if (lpSIP_Msg->Command[0] == '1')
			{
				Command_Data[Cur111].State = SIP_100;
				Command_Data[Cur111].Timer_Counter = 0;
			}
			else if ((lpSIP_Msg->Command[0] == '2') || (lpSIP_Msg->Command[0] == '3') || (lpSIP_Msg->Command[0] == '4'))
			{
				Command_Data[Cur111].State = SIP_200;
				Command_Data[Cur111].Timer_Counter = 0;
				Invite_Proc(FROM, lpSIP_Msg, SIP_INVITE);
				//if (lpSIP_Msg->Command[0]=='2')
				{
					memcpy(lpSIP_Msg, &Command_Data[Cur111].SIP_Msg, sizeof(SIP_Msg));
					Modify_CSeq(lpSIP_Msg->Cmd_Str, "ACK");
					Send_SIP_Command("ACK", lpSIP_Msg, ADD_VIA, TO); // 加此行避免ACK不發或發錯
					Invite_Proc(TO, lpSIP_Msg, SIP_INVITE, "ACK");	 //當Invite_Proc(To, .)時, 最後一個Command才需要加
				}
			}
		}
		else if (Command_Data[Cur111].State == SIP_100)
		{
			if (lpSIP_Msg->Command[0] == '1')
			{
				Send_SIP_Command(lpSIP_Msg->Command, lpSIP_Msg, DELETE_VIA, FROM); // Via OK
			}
			if ((lpSIP_Msg->Command[0] == '2') || lpSIP_Msg->Command[0] == '3' || (lpSIP_Msg->Command[0] == '4'))
			{
				Send_SIP_Command(lpSIP_Msg->Command, lpSIP_Msg, DELETE_VIA, FROM); // Via OK
				Invite_Proc(FROM, lpSIP_Msg, SIP_100);
				memcpy(lpSIP_Msg, &Command_Data[Cur111].SIP_Msg, sizeof(SIP_Msg));
				Modify_CSeq(Command_Data[Cur111].SIP_Msg.Cmd_Str, "ACK");
				Send_SIP_Command("ACK", &Command_Data[Cur111].SIP_Msg, ADD_VIA, TO); // 加此行避免ACK不發或發錯
				Command_Data[Cur111].State = SIP_200;
				Command_Data[Cur111].Timer_Counter = 0;
			}
		}
		else if (Command_Data[Cur111].State == SIP_ABANDON)
		{
			if (!strcmp(lpSIP_Msg->Command, "487"))
			{
				if (Command_Data[Cur111].Abandon_Flag == 1)
				{
					Send_SIP_Command("486", lpSIP_Msg, DELETE_VIA, FROM);
					Command_Data[Cur111].State = SIP_200;
					Command_Data[Cur111].Timer_Counter = 0;
					Invite_Proc(FROM, lpSIP_Msg, SIP_ABANDON);
				}
				else if (Command_Data[Cur111].Abandon_Flag == 2)
				{
					Modify_CSeq(Command_Data[Cur111].SIP_Msg.Cmd_Str, "ACK");
					Send_SIP_Command("ACK", &Command_Data[Cur111].SIP_Msg, ADD_VIA, TO);
					Command_Data[Cur111].State = SIP_ACK;
					Command_Data[Cur111].Timer_Counter = 0;
					//在Abandon_Flag=2時不帶入Invite_Proc(.), 因此通電話位結束，將導到GW
				}
				else if (Command_Data[Cur111].Abandon_Flag == 3)
				{
					sprintf(lpSIP_Msg->Contact_Name, "002886%s", &lpSIP_Msg->To_Name[1]);
					strcpy(lpSIP_Msg->Contact_URL, Local_IP);
					if ((Online_Cur = Check_OnLine(lpSIP_Msg->From_Name, &lpSIP_Msg->FromDest.Raddr)) >= 0)
						sprintf(lpSIP_Msg->Contact_Port, "%d", SIP_OnLine_Data[Online_Cur].Server_Port);
					Delete_OneLine(lpSIP_Msg->Cmd_Str, "Contact:");
					Send_SIP_Command("302", lpSIP_Msg, DELETE_VIA, FROM);
					Invite_Proc(FROM, lpSIP_Msg, SIP_ABANDON);
					Command_Data[Cur111].State = SIP_200;
					Command_Data[Cur111].Timer_Counter = 0;
				}
			}
		}
		else if (Command_Data[Cur111].State == SIP_200)
		{
			if ((lpSIP_Msg->Command[0] == '2') || lpSIP_Msg->Command[0] == '3' || (lpSIP_Msg->Command[0] == '4'))
			{
				if (Command_Data[Cur111].Abandon_Flag && !strcmp(lpSIP_Msg->Command, "487"))
					Send_SIP_Command("486", lpSIP_Msg, DELETE_VIA, FROM); // Via OK
				else
					Send_SIP_Command(lpSIP_Msg->Command, lpSIP_Msg, DELETE_VIA, FROM); // Via OK
			}
		}
		else if ((lpSIP_Msg->Command[0] == '2' || lpSIP_Msg->Command[0] == '4') &&
				 (Command_Data[Cur111].State == SIP_BYE || Command_Data[Cur111].State == SIP_CANCEL || Command_Data[Cur111].State == SIP_REFER || Command_Data[Cur111].State == SIP_NOTIFY))
		{
			m_lpNetIF->KillTimer(&Command_Data[Cur111].MixTimer);
			Send_SIP_Command(lpSIP_Msg->Command, lpSIP_Msg, DELETE_VIA, FROM); // Via OK
			if (Command_Data[Cur111].State == SIP_BYE || Command_Data[Cur111].State == SIP_CANCEL)
				Invite_Proc(FROM, lpSIP_Msg, Command_Data[Cur111].State);
			Clear_Command(Cur111);
		}
		else if (Command_Data[Cur111].State == SIP_ACK)
		{
			if ((lpSIP_Msg->Command[0] == '2') || lpSIP_Msg->Command[0] == '3' || (lpSIP_Msg->Command[0] == '4'))
			{
				Modify_CSeq(Command_Data[Cur111].SIP_Msg.Cmd_Str, "ACK");
				Send_SIP_Command("ACK", &Command_Data[Cur111].SIP_Msg, ADD_VIA, TO);
			}
		}
	}
	RETURN;
}

LRESULT CSIP_Server::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// 重新處理 INVITE Command
	if (message == INVITE_EVENT)
	{
		Process_INVITE((SIP_MSG *)wParam);
	}

	// return CDialog::WindowProc(message, wParam, lParam);
}

void CSIP_Server::OnTimer(MIXTIMER *wParam)
{
	CRASH_DEBUG_COMMAND;
	if (!m_lpNetIF)
	{
		DTrace_FatalError("CNetIF instance is invalid");
		RETURN;
	}
	UINT nIDEvent = m_lpNetIF->GetTimerID(wParam);
	int Cur, i, j, Invite_Cur, Online_Cur, SubServer_Cur;
	char S1[200];
	SIP_MSG Temp_SIP_Data;
	DEST_DATA Connect_Data;
	const char szTest[] = "test";

	// 處理 OnLine Data
	if ((nIDEvent == TIMERID_EXPIRES) && (SIP_OnLine_Num > 0))
	{
		for (i = 0, Cur = 0; Cur < SIP_ONLINE_SIZE; Cur++)
		{
			if (SIP_OnLine_Data[Cur].Flag)
			{
				if (SIP_OnLine_Num == 0)
				{
					Cur = SIP_ONLINE_SIZE;
					break;
				}
				if (SIP_OnLine_Data[Cur].Expires_Counter >= SIP_OnLine_Data[Cur].Expires_Limit)
				{ // 判斷Socket是否與SIP_Server_Socket相同，不是則關閉
					for (j = 0; j < Server_Port_Num; j++)
						if (SIP_OnLine_Data[Cur].Dest.pNc->mixsocket == SIP_Server_Nc[j].mixsocket)
							break;
					//if ((SIP_OnLine_Data[Cur].PhoneNo[0]=='0') && j>=Server_Port_Num)	Close_Socket(SIP_OnLine_Data[Cur].Socket, m_hWnd); // PC to Phone(Start_TCP_Client)
					Clear_OnLine_By_Cur(Cur);
				}
				else
					SIP_OnLine_Data[Cur].Expires_Counter++;
				// 定時向 Client 發 Packet, 以保持 NAT Channel Activity
				if ((SIP_OnLine_Data[Cur].Expires_Counter % UDP_ALIVE_TIME) == 0)
				{
					m_lpNetIF->NetNcSend(SIP_OnLine_Data[Cur].Dest.pNc, szTest, strlen(szTest), &SIP_OnLine_Data[Cur].Dest.Raddr);
				}
				i++;
				if (i >= SIP_OnLine_Num)
				{
					Cur = SIP_ONLINE_SIZE;
					break;
				}
			}
		}
		DTrace_CommonMsg("UAC=%d Relay=%d Com.Queue=%d Invite=%d\n", SIP_OnLine_Num, 0, Command_Queue_Num, SIP_Invite_Num);
		// SetWindowText(S1);
	}

	// 處理 Command Queue
	else if ((nIDEvent >= TIMERID_COMMAND) && (nIDEvent < (TIMERID_COMMAND + COMMAND_QUEUE_SIZE)))
	{
		Cur = nIDEvent - TIMERID_COMMAND;
		// TIMEOUT: SIP_REG/SIP_PROXY_AUTH -> SIP_READY
		if (Command_Data[Cur].State == SIP_REG || Command_Data[Cur].State == SIP_PROXY_AUTH)
		{
			Command_Data[Cur].Timer_Counter += TWO_SECOND;
			if (Command_Data[Cur].Timer_Counter < TCP_TIMEOUT_DUR)
			{
				if (Command_Data[Cur].State == SIP_PROXY_AUTH || ((Online_Cur = Check_OnLine(Command_Data[Cur].SIP_Msg.From_Name)) >= 0 && !SIP_OnLine_Data[Online_Cur].Auth_OK))
				{
					Send_SIP_Command(Command_Data[Cur].Response_SIP_Msg.Command, &Command_Data[Cur].Response_SIP_Msg, NOCHANGE_VIA, FROM);
				}
			}
			else
			{
				m_lpNetIF->KillTimer(wParam);
				Clear_Command(Cur);
			}
		}

		// TIMEOUT: SIP_BYE/CANCEL/REFER/NOTIFY -> SIP_READY
		else if ((Command_Data[Cur].State == SIP_BYE) || (Command_Data[Cur].State == SIP_CANCEL) || (Command_Data[Cur].State == SIP_REFER) || (Command_Data[Cur].State == SIP_NOTIFY))
		{
			Command_Data[Cur].Timer_Counter += TWO_SECOND;
			if (Command_Data[Cur].Timer_Counter < TCP_TIMEOUT_DUR)
			{
				Send_SIP_Command(Command_Data[Cur].SIP_Msg.Command, &Command_Data[Cur].SIP_Msg, ADD_VIA, TO);
			}
			else
			{
				m_lpNetIF->KillTimer(wParam);
				Send_SIP_Command("408", &Command_Data[Cur].SIP_Msg, NOCHANGE_VIA, FROM);
				if (Command_Data[Cur].State == SIP_BYE || Command_Data[Cur].State == SIP_CANCEL)
					Invite_Proc(EXTRA, &Command_Data[Cur].SIP_Msg, Command_Data[Cur].State);
				Clear_Command(Cur);
			}
		}

		// TIMEOUT: SIP_INVITE -> SIP_200
		else if (Command_Data[Cur].State == SIP_INVITE)
		{
			Command_Data[Cur].Timer_Counter += TWO_SECOND;
			if (Command_Data[Cur].Timer_Counter < TCP_TIMEOUT_DUR / 2)
			{
				Send_SIP_Command(Command_Data[Cur].SIP_Msg.Command, &Command_Data[Cur].SIP_Msg, ADD_VIA, TO);
			}
			else
			{
				if ((Invite_Cur = Check_Invite(&Command_Data[Cur].SIP_Msg)) >= 0 && SIP_Invite_Data[Invite_Cur].OutBound_SubServer_Flag)
				{ // SubServer Route 2nd or 3rd SubServer...
					SIP_Invite_Data[Invite_Cur].OutBound_SubServer_Flag = FALSE;
					if (OutBound_SubServer_Filter(&Command_Data[Cur].SIP_Msg, &Connect_Data))
					{
						memcpy(&SIP_Msg2, &Command_Data[Cur].SIP_Msg, sizeof(SIP_MSG));
						SIP_Msg2.ToDest.copy_from(&Connect_Data);
						// SIP_Msg2.ToNc.copy_from(&Connect_Data.Nc);
						// memcpy(&SIP_Msg2.TAddr, &Connect_Data.Addr, sizeof(SIP_Msg2.TAddr));
						// strcpy(SIP_Msg2.TV_IP, Connect_Data.V_IP);
						// strcpy(SIP_Msg2.TR_IP, Connect_Data.R_IP);
						// SIP_Msg2.To_Crypto_Type = CT_NoCrypto;
						// PostMessage(INVITE_EVENT, (WPARAM)&SIP_Msg2, (LPARAM)NULL);
						WindowProc(INVITE_EVENT, (WPARAM)&SIP_Msg2, (LPARAM)NULL);
					}
				}
				else if (Command_Data[Cur].PSTN_Flag)
				{ // Request Timoout more than 5 seconds and Forwarding to GW
					Update_Command(Cur, "302");
					strcpy(Command_Data[Cur].Response_SIP_Msg.ViaBuf, Command_Data[Cur].SIP_Msg.ViaBuf); //可以拿掉，因為在Update_Command時早已Copy過
					sprintf(Command_Data[Cur].Response_SIP_Msg.Contact_Name, "002886%s", &Command_Data[Cur].Response_SIP_Msg.To_Name[1]);
					strcpy(Command_Data[Cur].Response_SIP_Msg.Contact_URL, Local_IP);
					if ((Online_Cur = Check_OnLine(Command_Data[Cur].Response_SIP_Msg.From_Name, &Command_Data[Cur].Response_SIP_Msg.FromDest.Raddr)) >= 0)
						sprintf(Command_Data[Cur].Response_SIP_Msg.Contact_Port, "%d", SIP_OnLine_Data[Online_Cur].Server_Port);
					Delete_OneLine(Command_Data[Cur].Response_SIP_Msg.Cmd_Str, "Contact:");
					Send_SIP_Command("302", &Command_Data[Cur].Response_SIP_Msg, DELETE_VIA, FROM);
					Command_Data[Cur].State = SIP_200;
					Command_Data[Cur].Timer_Counter = 0;
					Invite_Proc(FROM, &Command_Data[Cur].Response_SIP_Msg, SIP_ABANDON);
					Command_Data[Cur].Disconnect_Flag = TRUE;
					// Clear Online for avoid forward "ACK" to Callee
					if ((Online_Cur = Check_OnLine(Command_Data[Cur].Response_SIP_Msg.To_Name, &Command_Data[Cur].Response_SIP_Msg.ToDest.Raddr)) >= 0)
					{
						SIP_OnLine_Data[Online_Cur].Proxy_MD5_OK = FALSE;
						Update_Expire(&Command_Data[Cur].Response_SIP_Msg, Online_Cur);
					}
				}
				else
				{ // Request Timeout
					Update_Command(Cur, "408");
					strcpy(Command_Data[Cur].Response_SIP_Msg.ViaBuf, Command_Data[Cur].SIP_Msg.ViaBuf); //可以拿掉，因為在Update_Command時早已Copy過
					Send_SIP_Command("408", &Command_Data[Cur].Response_SIP_Msg, DELETE_VIA, FROM);		 // Via OK
					Invite_Proc(EXTRA, &Command_Data[Cur].SIP_Msg, SIP_INVITE);
					Command_Data[Cur].State = SIP_200;
					Command_Data[Cur].Timer_Counter = 0;
				}
			}
		}

		// TIMEOUT: SIP_100 -> SIP_ABANDON
		else if (Command_Data[Cur].State == SIP_100)
		{
			Command_Data[Cur].Timer_Counter += TWO_SECOND;
			if (Command_Data[Cur].Timer_Counter < Command_Data[Cur].Abandon_Time)
			{
			}
			else
			{
				strcpy(Command_Data[Cur].Response_SIP_Msg.ViaBuf, Command_Data[Cur].SIP_Msg.ViaBuf); //可以拿掉，因為在Update_Command時早已Copy過
				// Send "CANCEL"
				memcpy(&Temp_SIP_Data, &Command_Data[Cur].SIP_Msg, sizeof(SIP_MSG)); //使用Temp_SIP是因為不讓CANCEL蓋過原本Command中的INVITE
				Modify_CSeq(Temp_SIP_Data.Cmd_Str, "CANCEL");
				Send_SIP_Command("CANCEL", &Temp_SIP_Data, ADD_VIA, TO);
				// Set Param
				Command_Data[Cur].Abandon_Flag = (Command_Data[Cur].PSTN_Flag) ? 2 : 1;
				if (Command_Data[Cur].Abandon_Flag == 2)
					Command_Data[Cur].Abandon_Flag = (Forwarding_Mode_Check) ? 2 : 3; //當設為2表示程式Forwarding GW為內部走法, 若為3表示Forwarding GW走302
				// Abandon Proc
				if (Command_Data[Cur].Abandon_Flag == 1)
					Invite_Proc(TO, &Command_Data[Cur].SIP_Msg, SIP_100, "CANCEL"); //當Invite_Proc(To, .)時, 最後一個Command才需要加
				else if (Command_Data[Cur].Abandon_Flag == 2)
				{
					if ((Invite_Cur = Check_Invite(&Command_Data[Cur].SIP_Msg)) >= 0)
						SIP_Invite_Data[Invite_Cur].Forwarding_GW_Flag = TRUE;
					if ((Online_Cur = Check_OnLine(Command_Data[Cur].SIP_Msg.From_Name)) >= 0)
					{ // Re-send Invite
						memcpy(&SIP_Msg2, &Command_Data[Cur].SIP_Msg, sizeof(SIP_MSG));
						SIP_Msg2.ToDest.copy_from(&SIP_OnLine_Data[Online_Cur].Trunk_GW);
						// SIP_Msg2.ToNc.copy_from(&SIP_OnLine_Data[Online_Cur].Trunk_GW.Nc);
						// memcpy(&SIP_Msg2.TAddr, &SIP_OnLine_Data[Online_Cur].Trunk_GW.Addr, sizeof(SIP_Msg2.TAddr));
						// strcpy(SIP_Msg2.TV_IP, SIP_OnLine_Data[Online_Cur].Trunk_GW.V_IP);
						// strcpy(SIP_Msg2.TR_IP, SIP_OnLine_Data[Online_Cur].Trunk_GW.R_IP);
						// SIP_Msg2.To_Crypto_Type = CT_NoCrypto;
						// PostMessage(INVITE_EVENT, (WPARAM)&SIP_Msg2, (LPARAM)NULL);
					}
				}
				Command_Data[Cur].State = SIP_ABANDON;
				Command_Data[Cur].Timer_Counter = 0;
				if (Command_Data[Cur].Abandon_Flag == 2)
					WindowProc(INVITE_EVENT, (WPARAM)&SIP_Msg2, (LPARAM)NULL);
			}
		}

		// TIMEOUT: SIP_ABANDON -> SIP_200
		else if (Command_Data[Cur].State == SIP_ABANDON)
		{
			Command_Data[Cur].Timer_Counter += TWO_SECOND;
			if (Command_Data[Cur].Timer_Counter < TCP_TIMEOUT_DUR)
			{
				Send_SIP_Command("CANCEL", &Command_Data[Cur].SIP_Msg, ADD_VIA, TO);
			}
			else
			{
				if (Command_Data[Cur].Abandon_Flag == 1)
				{
					Update_Command(Cur, "486");
					Send_SIP_Command("486", &Command_Data[Cur].Response_SIP_Msg, DELETE_VIA, FROM); // Via OK
					Invite_Proc(FROM, &Command_Data[Cur].Response_SIP_Msg, SIP_ABANDON);
					Command_Data[Cur].State = SIP_200;
					Command_Data[Cur].Timer_Counter = 0;
				}
				else if (Command_Data[Cur].Abandon_Flag == 2)
				{
					Modify_CSeq(Command_Data[Cur].SIP_Msg.Cmd_Str, "ACK");
					Send_SIP_Command("ACK", &Command_Data[Cur].SIP_Msg, ADD_VIA, TO);
					Command_Data[Cur].State = SIP_ACK;
					Command_Data[Cur].Timer_Counter = 0;
					//在Abandon_Flag=2時不帶入Invite_Proc(.), 因此通電話位結束，將導到GW
				}
				else if (Command_Data[Cur].Abandon_Flag == 3)
				{
					Update_Command(Cur, "302");
					sprintf(Command_Data[Cur].Response_SIP_Msg.Contact_Name, "002886%s", &Command_Data[Cur].Response_SIP_Msg.To_Name[1]);
					strcpy(Command_Data[Cur].Response_SIP_Msg.Contact_URL, Local_IP);
					if ((Online_Cur = Check_OnLine(Command_Data[Cur].Response_SIP_Msg.From_Name, &Command_Data[Cur].Response_SIP_Msg.FromDest.Raddr)) >= 0)
						sprintf(Command_Data[Cur].Response_SIP_Msg.Contact_Port, "%d", SIP_OnLine_Data[Online_Cur].Server_Port);
					Delete_OneLine(Command_Data[Cur].Response_SIP_Msg.Cmd_Str, "Contact:");
					Send_SIP_Command("302", &Command_Data[Cur].Response_SIP_Msg, DELETE_VIA, FROM);
					Invite_Proc(FROM, &Command_Data[Cur].Response_SIP_Msg, SIP_ABANDON);
					Command_Data[Cur].State = SIP_200;
					Command_Data[Cur].Timer_Counter = 0;
				}
			}
		}

		// TIMEOUT: SIP_RELAY_CONNECT/READY -> SIP_INVITE
		else if ((Command_Data[Cur].State == SIP_RELAY_CONNECT) || (Command_Data[Cur].State == SIP_RELAY_READ))
		{
			Command_Data[Cur].Timer_Counter += TWO_SECOND;
			if (Command_Data[Cur].Timer_Counter < TCP_TIMEOUT_DUR / 2) // 等 5秒,  未回答則不導通 Relay
			{														   //Send_SIP_Command("100", &Command_Data[Cur].SIP_Msg, ADD_VIA, FROM);
			}
			else
			{
				Send_SIP_Command(Command_Data[Cur].SIP_Msg.Command, &Command_Data[Cur].SIP_Msg, ADD_VIA, TO);
				Command_Data[Cur].SIP_Msg.RTP_Relay_Flag = FALSE;
				Command_Data[Cur].Relay_TCP_Dest.clean_nc();
				Command_Data[Cur].Relay_TCP_Dest.reset();
				Command_Data[Cur].State = SIP_INVITE;
				Command_Data[Cur].Timer_Counter = 0;
			}
		}

		// TIMEOUT: SIP_200 -> SIP_ACK
		else if (Command_Data[Cur].State == SIP_200)
		{
			Command_Data[Cur].Timer_Counter += TWO_SECOND;
			if (Command_Data[Cur].Timer_Counter < TCP_TIMEOUT_DUR)
			{
				strcpy(Command_Data[Cur].Response_SIP_Msg.ViaBuf, Command_Data[Cur].SIP_Msg.ViaBuf);
				if (Command_Data[Cur].Abandon_Flag != 2)
					Send_SIP_Command(Command_Data[Cur].Response_SIP_Msg.Command, &Command_Data[Cur].Response_SIP_Msg, DELETE_VIA, FROM);
			} // 有些狀況不可 Delete_Via
			else
			{
				Modify_CSeq(Command_Data[Cur].SIP_Msg.Cmd_Str, "ACK");
				Send_SIP_Command("ACK", &Command_Data[Cur].SIP_Msg, ADD_VIA, TO); // 要修改 CSeq: 12 INVITE 成為 CSeq: 12 ACK
				Command_Data[Cur].State = SIP_ACK;
				Command_Data[Cur].Timer_Counter = 0;
				Invite_Proc(TO, &Command_Data[Cur].SIP_Msg, SIP_200, "ACK"); //當Invite_Proc(To, .)時, 最後一個Command才需要加
			}
		}

		// TIMEOUT: SIP_ACK -> SIP_READY
		else if (Command_Data[Cur].State == SIP_ACK)
		{
			Command_Data[Cur].Timer_Counter += TWO_SECOND;
			if (Command_Data[Cur].Timer_Counter < TCP_TIMEOUT_DUR / 2)
			{
			}
			else
			{
				m_lpNetIF->KillTimer(wParam);
				Clear_Command(Cur);
			}
		}

		// TIMEOUT: unKnown State -> SIP_READY
		else
		{
			Clear_Command(Cur);
			m_lpNetIF->KillTimer(wParam);
		}
	}
	else if (nIDEvent >= TIMERID_INVITE && nIDEvent < (TIMERID_INVITE + SIP_INVITE_SIZE))
	{
		Invite_Cur = nIDEvent - TIMERID_INVITE;
		Clear_Invite_By_Cur(Invite_Cur);
	}
	else if (nIDEvent >= TIMERID_SUBSERVER_EVENT && nIDEvent < (TIMERID_SUBSERVER_EVENT + MAPPING_DATA_SIZE))
	{
		m_lpNetIF->KillTimer(wParam);
		SubServer_Cur = nIDEvent - TIMERID_SUBSERVER_EVENT;
		// Set SubServer Data
		Update_SubServer_Data(SubServer_Data[SubServer_Cur].Real_IP, SubServer_Data[SubServer_Cur].Port, CT_NoCrypto);
		// Clear SubServer_Data
		Clear_SubServer_Data(SubServer_Cur);
	}

	// CDialog::OnTimer(nIDEvent);
	RETURN;
}

// BOOL CSIP_Server::DestroyWindow()
// {
// 	int i, Cur;
// 	FILE *Out;

// 	if ((SIP_OnLine_Num >= 0) && ((Out = fopen(SIP_ONLINE_FILE, "wb")) != NULL))
// 	{
// 		for (i = 0, Cur = 0; Cur < SIP_ONLINE_SIZE; Cur++)
// 		{
// 			if (SIP_OnLine_Data[Cur].Flag)
// 			{
// 				fwrite(&SIP_OnLine_Data[Cur], sizeof(SIP_ONLINE_DATA), 1, Out);
// 				i++;
// 			}
// 			if (i >= SIP_OnLine_Num)
// 			{
// 				Cur = SIP_ONLINE_SIZE;
// 				break;
// 			}
// 		}
// 		fclose(Out);
// 	}
// 	// if ( (Monitor_Data_Num>=0) && ((Out=fopen(SIP_MONITOR_FILE,"wb"))!=NULL) )
// 	// {
// 	//    for (i=0,Cur=0;Cur<MONITOR_DATA_SIZE;Cur++)
// 	//    {   if (Monitor_Data[Cur].Flag) { fwrite(&Monitor_Data[Cur],sizeof(MONITOR_DATA),1,Out); i++; }
// 	//        if (i>=Monitor_Data_Num) { Cur=MONITOR_DATA_SIZE; break; }    }
// 	//    fclose(Out);
// 	// }
// 	// if (lpSIP_Server_Setup)	{ delete lpSIP_Server_Setup; lpSIP_Server_Setup=NULL; }

// 	// return CDialog::DestroyWindow();
// }

void CSIP_Server::OnRTP_Relay(NetConnect_t *wParam, LPARAM lParam)
{
	CRASH_DEBUG_COMMAND;
	int Cur, j;
	char S1[1024];
	char *buf;
	int ret, len;

	switch (lParam)
	{
	case TFD_CONNECTED:
		// FD_CONNECT: SIP_RELAY_CONNECT -> SIP_RELAY_READ
		if (((Cur = Check_Command_By_Relay_Socket(wParam->mixsocket)) >= 0) && (Command_Data[Cur].State == SIP_RELAY_CONNECT))
		{
			Send_Relay_Command("0RL", &Command_Data[Cur].SIP_Msg, &Command_Data[Cur]);
			Command_Data[Cur].State = SIP_RELAY_READ;
			Command_Data[Cur].Timer_Counter = 0;
			m_lpNetIF->NetNcResignInEvent(wParam);
		}
		else
		{
			m_lpNetIF->NetNcClean(wParam);
		}
		break;

	case TFD_CONNECT_FAIL:
		// FD_DISCONNECT: SIP_RELAY_CONNECT -> SIP_INVITE
		if (((Cur = Check_Command_By_Relay_Socket(wParam->mixsocket)) >= 0) && (Command_Data[Cur].State == SIP_RELAY_CONNECT))
		{
			Send_SIP_Command(Command_Data[Cur].SIP_Msg.Command, &Command_Data[Cur].SIP_Msg, ADD_VIA, TO); //FROM);
			Command_Data[Cur].SIP_Msg.RTP_Relay_Flag = FALSE;
			Command_Data[Cur].State = SIP_INVITE;
			Command_Data[Cur].Timer_Counter = 0;
		}
		m_lpNetIF->NetNcClean(wParam);
		break;

	case TFD_READ:
		ret = m_lpNetIF->NetNcRecv(wParam, &buf, &len, NULL);
		if (ret < 0)
			break;
		else
		{
			memcpy(S1, buf, len);
			S1[len] = 0;
			j = Hsf_recv(0, S1, len, 0);
		}

		if (j <= 0)
			break;
		else
			S1[j] = 0;
		Cur = Check_Command_By_Relay_Socket(wParam->mixsocket);
		Write_Log("recv", S1, NULL, &Command_Data[Cur].SIP_Msg);
		if ((Cur >= 0) && (Command_Data[Cur].State == SIP_RELAY_READ))
		{
			Command_Process(S1, &Command[0][0], '|', COMMAND_NUM, COMMAND_LEN);
			// FD_READ(RL0): SIP_RELAY_READ -> SIP_INVITE
			if (!strcmp(&Command[0][0], "RL0"))
			{
				strcpy(Command_Data[Cur].Relay_UDP_IP, &Command[1][0]);
				strcpy(Command_Data[Cur].Relay_UDP_FPort, &Command[2][0]);
				strcpy(Command_Data[Cur].Relay_UDP_TPort, &Command[3][0]);
				Modify_SDP(&Command_Data[Cur].SIP_Msg, Command_Data[Cur].Relay_UDP_IP, Command_Data[Cur].Relay_UDP_FPort, Command_Data[Cur].Relay_UDP_TPort);
				Send_SIP_Command(Command_Data[Cur].SIP_Msg.Command, &Command_Data[Cur].SIP_Msg, ADD_VIA, TO);
				Command_Data[Cur].SIP_Msg.RL1_Flag = TRUE;
				Command_Data[Cur].State = SIP_INVITE;
				Command_Data[Cur].Timer_Counter = 0;
			}
			// FD_READ(RL1): SIP_RELAY_READ -> SIP_INVITE(No Relay)
			else
			{
				Send_SIP_Command(Command_Data[Cur].SIP_Msg.Command, &Command_Data[Cur].SIP_Msg, ADD_VIA, TO);
				Command_Data[Cur].SIP_Msg.RTP_Relay_Flag = FALSE;
				Command_Data[Cur].State = SIP_INVITE;
				Command_Data[Cur].Timer_Counter = 0;
				m_lpNetIF->NetNcClean(wParam);
			}
		}
		break;
	case TFD_WRITE:
		break;
	case TFD_CLOSE:
		// FD_CLOSE: SIP_RELAY_READ -> SIP_INVITE
		if ((Cur = Check_Command_By_Relay_Socket(wParam->mixsocket)) >= 0)
		{
			if (Command_Data[Cur].State == SIP_RELAY_READ)
			{
				Send_SIP_Command(Command_Data[Cur].SIP_Msg.Command, &Command_Data[Cur].SIP_Msg, ADD_VIA, TO);
				Command_Data[Cur].SIP_Msg.RTP_Relay_Flag = FALSE;
				Command_Data[Cur].State = SIP_INVITE;
				Command_Data[Cur].Timer_Counter = 0;
				m_lpNetIF->NetNcClean(wParam);
			}
			else if ((Command_Data[Cur].State == SIP_INVITE) || (Command_Data[Cur].State == SIP_100))
			{
				Command_Data[Cur].SIP_Msg.RL1_Flag = FALSE;
				m_lpNetIF->NetNcClean(wParam);
			}
		}
		else
			m_lpNetIF->NetNcClean(wParam);
		break;
	}
	RETURN;
}

// void CSIP_Server::OnMonitor(WPARAM wParam,LPARAM lParam)
// {
// 	CRASH_DEBUG_COMMAND;
// 	char         S1[512];
// 	int          Len,i;
// 	sockaddr_in  Addr;
// 	SOCKET       Socket_No;

//   	switch(LOWORD(lParam))
//   	{
// 		case TFD_ACCEPT:
// 			Len=sizeof(sockaddr);
// 			Socket_No=accept(wParam,(sockaddr *)&Addr,&Len);
// 			break;
// 		case TFD_READ:
// 			// 0MT | PhoneNo | Monitor_TCP_IP | Monitor_TCP_Port | Total
// 			i=Hsf_recv(wParam,S1,sizeof(S1),0); if (i>0) S1[i]=0; else break;
// 			Write_Log("recv",S1,NULL,NULL);
// 			i=Command_Process(S1,&Command[0][0],'|',COMMAND_NUM,COMMAND_LEN);
// 			// Check_Total(..)
// 			if (!strcmp(&Command[0][0],"0MT"))
// 			{  Len=sizeof(sockaddr); getpeername(wParam,(sockaddr *)&Addr,&Len);
// 				if (strcmp(&Command[2][0],inet_ntoa(Addr.sin_addr))) sprintf(S1,"%s1|Monitoring Server Can't Locate under NAT",&Command[0][1]);
// 				else			                                     sprintf(S1,"%s0|OK",&Command[0][1]);
// 				Hsf_send(wParam,S1,strlen(S1),0);
// 				Write_Log("send",S1,NULL,NULL);
// 				Insert_Monitor(&Command[1][0],&Command[2][0],&Command[3][0]);
// 			}
// 			else if (!strcmp(&Command[0][0],"0MT1"))
// 			{
// 				sprintf(S1,"%s0|OK",&Command[0][1]);
// 					Write_Log("send",S1,NULL,NULL);
// 					Hsf_send(wParam,S1,strlen(S1),0);
// 					Delete_Monitor(&Command[1][0]);
// 			}
// 			break;
// 		case TFD_CLOSE:
// 			Close_Socket(wParam,m_hWnd);
// 			break;
//   	}
// 	RETURN;
// }

// void CSIP_Server::OnChangeEdit1()
// {
// 	Update_SIP_Server_Param("WRITE");
// }

// void CSIP_Server::OnChangeEdit2()
// {
// 	Update_SIP_Server_Param("WRITE");
// }

// void CSIP_Server::OnChangeEdit3()
// {
// 	char Old_GW_IP[256],Old_GW_Port[16];

// 	//NTUT_GW_IP
// 	strcpy(Old_GW_IP,NTUT_GW_IP);
// 	strcpy(Old_GW_Port,NTUT_GW_Port);
// 	Update_SIP_Server_Param("WRITE");
// 	UpdateData(TRUE); strcpy(NTUT_GW_IP,m_NTUT_IP); UpdateData(FALSE);
// 	// Set NTUT_GW Param
// 	if (NTUT_GW_Socket!=0)	{ closesocket(NTUT_GW_Socket); NTUT_GW_Socket=0;}
// 	if (URL_Filter(NTUT_GW_IP))
// 	{
// 		Start_UDP_Client(&NTUT_GW_Socket,&NTUT_GW_Addr,SIP_SERVER_PORT,atoi(NTUT_GW_Port),NTUT_GW_IP,SIP_SERVER_EVENT,m_hWnd);
// 		Update_Trunk_GW(Old_GW_IP,Old_GW_Port);
// 	}
// 	else	{ NTUT_GW_Socket=0; memset(&NTUT_GW_Addr,0,sizeof(sockaddr_in));}
// }

// void CSIP_Server::OnChangeEdit4()
// {	// NTUT_GW_Port
// 	Update_SIP_Server_Param("WRITE");
// 	// Set NTUT_GW Param
// 	if (NTUT_GW_Socket!=0)	{ closesocket(NTUT_GW_Socket); NTUT_GW_Socket=0;}
// 	if (URL_Filter(NTUT_GW_IP))
// 	{ Start_UDP_Client(&NTUT_GW_Socket,&NTUT_GW_Addr,SIP_SERVER_PORT,atoi(NTUT_GW_Port),NTUT_GW_IP,SIP_SERVER_EVENT,m_hWnd);}
// 	else	{ NTUT_GW_Socket=0; memset(&NTUT_GW_Addr,0,sizeof(sockaddr_in));}
// }

// void CSIP_Server::OnChangeEdit5()
// {
// 	Update_SIP_Server_Param("WRITE");
// }

// void CSIP_Server::OnChangeEdit6()
// {
// 	Update_SIP_Server_Param("WRITE");
// }

// void CSIP_Server::OnChangeEdit7()
// {
// 	Update_SIP_Server_Param("WRITE");
// }

// void CSIP_Server::Show_OnLine()
// {
// 	int    i,Cur,Item;
// 	char   S1[100];
// 	SIP_ONLINE_DATA	*lpOnline_Data;

// 	UpdateData(TRUE);
// 	Item=m_Tab1.GetCurSel();
// 	UpdateData(FALSE);

// 	if (Item==0)
// 	{
// 		m_List1.DeleteAllItems();
// 		if (Tab1_Item!=Item)
// 		{
// 			Tab1_Item=Item;	// Avoid repeat calculate column
// 			// Delete Column
// 			i = m_List1.GetHeaderCtrl()->GetItemCount();
// 			for (Cur=0;Cur < i;Cur++)  m_List1.DeleteColumn(0);
// 			// Insert Column
// 			m_List1.InsertColumn(0, "No.");	        m_List1.SetColumnWidth(0, 40);
// 			m_List1.InsertColumn(1, "PhoneNo");	    m_List1.SetColumnWidth(1, 110);
// 			m_List1.InsertColumn(2, "Name");	    m_List1.SetColumnWidth(2, 90);
// 			m_List1.InsertColumn(3, "Crypto");	    m_List1.SetColumnWidth(3, 90);
// 			m_List1.InsertColumn(4, "IP");  	    m_List1.SetColumnWidth(4, 210);
// 			m_List1.InsertColumn(5, "Port");  	    m_List1.SetColumnWidth(5, 100);
// 			m_List1.InsertColumn(6, "Expires");	    m_List1.SetColumnWidth(6, 160);
// 			m_List1.InsertColumn(7, "Account/ Password/ Proxy"); m_List1.SetColumnWidth(7, 100);
// 			m_List1.InsertColumn(8, "Mobile");	    m_List1.SetColumnWidth(8, 90);
// 			m_List1.SetExtendedStyle(LVS_EX_FULLROWSELECT);
// 		}
// 		for (i=0,Cur=0;i<SIP_OnLine_Num && Cur<SIP_ONLINE_SIZE;Cur++)
// 		{
// 			lpOnline_Data=&SIP_OnLine_Data[Cur];
// 			if (lpOnline_Data->Flag)
// 			{
// 				sprintf(S1,"%d",i);
// 				m_List1.InsertItem(i,S1);
// 				m_List1.SetItemText(i,1,lpOnline_Data->PhoneNo);
// 				m_List1.SetItemText(i,2,lpOnline_Data->Name);
// 				// Crypto Type
// 				if		(lpOnline_Data->Crypto_Type==CT_NoSetting)		m_List1.SetItemText(i,3,NO_Setting_TAG);
// 				else if	(lpOnline_Data->Crypto_Type==CT_NoCrypto)		m_List1.SetItemText(i,3,NO_CRYPTO_TAG);
// 				else if (lpOnline_Data->Crypto_Type==CT_SIPCrypto)		m_List1.SetItemText(i,3,SIP_CRYPTO_TAG);
// 				else if (lpOnline_Data->Crypto_Type==CT_Walkersun1)		m_List1.SetItemText(i,3,WALKERSUN_CRYPTO_TAG);
// 				else													m_List1.SetItemText(i,3,Ctypto_Error_TAG);
// 				//
// 				sprintf(S1,"%s/ %s",lpOnline_Data->R_IP,lpOnline_Data->V_IP);
// 				m_List1.SetItemText(i,4,S1);
// 				sprintf(S1,"%d/ %d",lpOnline_Data->S_Port,lpOnline_Data->Server_Port);
// 				m_List1.SetItemText(i,5,S1);
// 				sprintf(S1,"%d/ %d/ %d/ %d",lpOnline_Data->Expires_Client,lpOnline_Data->Expires_Real,lpOnline_Data->Expires_Limit,lpOnline_Data->Expires_Counter);
// 				m_List1.SetItemText(i,6,S1);
// 				if		(lpOnline_Data->CDMS_OK==1)		strcpy(S1,"OK");
// 				else if (lpOnline_Data->CDMS_OK==0)		strcpy(S1,"unCheck");
// 				else if (lpOnline_Data->CDMS_OK==-1)	strcpy(S1,"Fail");
// 				if (lpOnline_Data->MD5_OK)  strcat(S1,"/ OK"); else strcat(S1,"/ Fail");
// 				if (lpOnline_Data->Proxy_MD5_OK)  strcat(S1,"/ OK"); else strcat(S1,"/ Fail");
// 				m_List1.SetItemText(i,7,S1);
// 				m_List1.SetItemText(i,8,lpOnline_Data->Mobile);
// 				if (!strcmp(Search_PhoneNo,lpOnline_Data->PhoneNo))
// 				{ m_List1.SetFocus(); m_List1.SetItemState(i, LVIS_SELECTED, LVIS_SELECTED);}
// 				i++;
// 			}
// 		}
// 	}
// }

// void CSIP_Server::Show_Invite()
// {
// 	int		i,Cur,Item;
// 	char	S1[100];
// 	CTime	Start_Time;
// 	CTimeSpan	Elapsed_Time;

// 	UpdateData(TRUE); Item=m_Tab1.GetCurSel(); UpdateData(FALSE);
// 	if (Item==4)
// 	{
// 		m_List1.DeleteAllItems();
// 		if (Tab1_Item!=Item)
// 		{
// 			Tab1_Item=Item;	// Avoid repeat calculate column
// 			// Delete Column
// 			i = m_List1.GetHeaderCtrl()->GetItemCount();
// 			for (Cur=0;Cur < i;Cur++)  m_List1.DeleteColumn(0);
// 			// Insert Column
// 			m_List1.InsertColumn(0, "No.");					m_List1.SetColumnWidth(0, 40);
// 			m_List1.InsertColumn(1, "From_Name");			m_List1.SetColumnWidth(1, 123);
// 			m_List1.InsertColumn(2, "TO_Name");				m_List1.SetColumnWidth(2, 123);
// 			m_List1.InsertColumn(3, "State");				m_List1.SetColumnWidth(3, 100);
// 			m_List1.InsertColumn(4, "IP");					m_List1.SetColumnWidth(4, 215);
// 			m_List1.InsertColumn(5, "Elapsed_Time");		m_List1.SetColumnWidth(5, 100);
// 			m_List1.InsertColumn(6, "Sart_Time");			m_List1.SetColumnWidth(6, 100);
// 			m_List1.InsertColumn(7, "Port");				m_List1.SetColumnWidth(7, 100);
// 		}
// 		for (i=0,Cur=0;Cur<SIP_INVITE_SIZE;Cur++)
// 		{
// 			if (SIP_Invite_Num==0) { Cur=SIP_INVITE_SIZE; break; }
// 			if (SIP_Invite_Data[Cur].Flag && SIP_Invite_Data[Cur].Answer_State!=Temporary)
// 			{
// 				//----- Show Invite -----
// 				sprintf(S1,"%d",i);
// 				m_List1.InsertItem(i,S1);
// 				//From_Name
// 			   	sprintf(S1,"%s (%s)",SIP_Invite_Data[Cur].From_User_Name,SIP_Invite_Data[Cur].From_PhoneNo);
// 				m_List1.SetItemText(i,1,S1);
// 				//To_Name
// 				sprintf(S1,"%s (%s)",SIP_Invite_Data[Cur].To_User_Name,SIP_Invite_Data[Cur].To_PhoneNo);
// 				m_List1.SetItemText(i,2,S1);
// 				//From_IP->To_IP
// 				sprintf(S1,"%s->",SIP_Invite_Data[Cur].FR_IP);
// 				sprintf(S1,"%s%s",S1,SIP_Invite_Data[Cur].TR_IP);
// 				m_List1.SetItemText(i,4,S1);
// 				//State
// 				if		(SIP_Invite_Data[Cur].Answer_State==Inviting)		strcpy(S1,"Inviting...");
// 				else if (SIP_Invite_Data[Cur].Answer_State==Answer)			strcpy(S1,"Conversing...");
// 				else if (SIP_Invite_Data[Cur].Answer_State==Abandon)		strcpy(S1,"Abandons...");
// 				else if (SIP_Invite_Data[Cur].Answer_State==Busy)			strcpy(S1,"Busy...");
// 				else if (SIP_Invite_Data[Cur].Answer_State==Forwarding)		strcpy(S1,"Forwarding...");
// 				else if (SIP_Invite_Data[Cur].Answer_State==Terminating)	strcpy(S1,"Terminating...");
// 				else														strcpy(S1,"UnKnow");
// 				m_List1.SetItemText(i,3,S1);
// 				//Start_Time
// 				if (SIP_Invite_Data[Cur].Start_Time!=0)
// 				{
// 					Start_Time=SIP_Invite_Data[Cur].Start_Time;
// 					sprintf(S1,Start_Time.Format("%H:%M:%S_%Y.%m.%d"));
// 					m_List1.SetItemText(i,6,S1);
// 					//Elapsed_Time
// 					Elapsed_Time=CTime::GetCurrentTime().GetTime()-Start_Time.GetTime();
// 					sprintf(S1,"%s",Elapsed_Time.Format("%D_%H:%M:%S"));
// 					m_List1.SetItemText(i,5,S1);
// 				}
// 				else
// 				{
// 					Start_Time=SIP_Invite_Data[Cur].Invite_Time;
// 					sprintf(S1,Start_Time.Format("%H:%M:%S_%Y.%m.%d"));
// 					m_List1.SetItemText(i,6,S1);
// 				}
// 				i++; if (i>=SIP_Invite_Num) { Cur=SIP_INVITE_SIZE; break; }
// 			}
// 		}
// 	}
// }

// void CSIP_Server::Show_Monitoring()
// {	// Monitoring Statistic
// 	int		i,Cur,Item;
// 	char	S1[100];

// 	UpdateData(TRUE); Item=m_Tab1.GetCurSel(); UpdateData(FALSE);
// 	if (Item==1)
// 	{
// 		m_List1.DeleteAllItems();
// 		if (Tab1_Item!=Item)
// 		{
// 			Tab1_Item=Item;	// Avoid repeat calculate column
// 			// Delete Column
// 			i = m_List1.GetHeaderCtrl()->GetItemCount();
// 			for (Cur=0;Cur < i;Cur++)  m_List1.DeleteColumn(0);
// 			// Insert Column
// 			m_List1.InsertColumn(0, "No.");					m_List1.SetColumnWidth(0, 40);
// 			m_List1.InsertColumn(1, "Monitoring PhoneNo.");	m_List1.SetColumnWidth(1, 150);
// 			m_List1.InsertColumn(2, "Monitoring IP");		m_List1.SetColumnWidth(2, 150);
// 			m_List1.InsertColumn(3, "Monitoring Port");		m_List1.SetColumnWidth(3, 150);
// 			m_List1.InsertColumn(4, "OnLine ?");			m_List1.SetColumnWidth(4, 100);
// 		}
// 		if (Monitor_Data_Num>0)
// 		{
// 			for (i=0,Cur=0;Cur<MONITOR_DATA_SIZE;Cur++)
// 			{
// 				if (Monitor_Data_Num==0) { Cur=MONITOR_DATA_SIZE; break; }
// 				if (Monitor_Data[Cur].Flag)
// 				{
// 					sprintf(S1,"%d",i);
// 					m_List1.InsertItem(i,S1);
// 					m_List1.SetItemText(i,1,Monitor_Data[Cur].PhoneNo);
// 					m_List1.SetItemText(i,2,Monitor_Data[Cur].Monitor_TCP_IP);
// 					m_List1.SetItemText(i,3,Monitor_Data[Cur].Monitor_TCP_Port);
// 					i++; if (i>=Monitor_Data_Num) { Cur=MONITOR_DATA_SIZE; break; }
// 				}
// 			}
// 		}
// 	}
// }

int CSIP_Server::Invite_Proc(int Send_Flag, SIP_MSG *lpSIP_Msg, SIP_STATE State, const char *Command)
{ // 當Server端主動送封包時，需填入Command否則無法得知正確發送的Command
	int Invite_Cur;
	SIP_INVITE_DATA *lpInvite_Data;
	std::string timestr1, timestr2;

	if ((Invite_Cur = Check_Invite(lpSIP_Msg)) >= 0)
		lpInvite_Data = &SIP_Invite_Data[Invite_Cur];
	// Request Process
	if (Send_Flag == FROM)
	{
		// Save Invite
		if (!strcmp(lpSIP_Msg->Command, "INVITE"))
		{
			if (Invite_Cur < 0)
			{
				if ((Invite_Cur = Save_Invite(lpSIP_Msg)) >= 0)
					lpInvite_Data = &SIP_Invite_Data[Invite_Cur];
				ftime(&lpInvite_Data->Invite_Time);
				sprintf(SIP_Invite_Data[Invite_Cur].Log_File_Name, "%s\\%s to %s\\%s",
						Jstringftime(timestr1, "%Y-%m-%d\\%H clock", &lpInvite_Data->Invite_Time)->c_str(),
						lpInvite_Data->From_PhoneNo,
						lpInvite_Data->To_PhoneNo,
						Jstringftime(timestr2, "%H-%M-%S", &lpInvite_Data->Invite_Time)->c_str());
				lpInvite_Data->Answer_State = Inviting;
			}
		}
		else if (Invite_Cur >= 0)
		{
			// Start count time
			if (!strcmp(lpSIP_Msg->Command, "ACK"))
			{
				if ((lpInvite_Data->Answer_State == Busy || lpInvite_Data->Answer_State == Forwarding) && State == SIP_200)
					Clear_Invite(lpSIP_Msg);
				else if (lpInvite_Data->Answer_State == Inviting)
				{
					ftime(&lpInvite_Data->Start_Time);
					lpInvite_Data->Answer_State = Answer;
				}
			}
			// Close Invite & End count time
			else if (!strcmp(lpSIP_Msg->Command, "BYE") && lpInvite_Data->Answer_State == Answer)
			{
				ftime(&lpInvite_Data->End_Time);
				lpInvite_Data->Answer_State = Terminating;
				Write_Invite_Log(lpInvite_Data, Answer);
			}
			else if (!strcmp(lpSIP_Msg->Command, "CANCEL") && lpInvite_Data->Answer_State == Inviting)
			{
				lpInvite_Data->Answer_State = Abandon;
				Write_Invite_Log(lpInvite_Data, Abandon);
			}
			// Response Process
			else
			{
				if (lpInvite_Data->Answer_State == Inviting)
				{
					if (!strcmp(lpSIP_Msg->Command, "200") && (State == SIP_100 || State == SIP_INVITE))
					{
						ftime(&lpInvite_Data->Start_Time);
						lpInvite_Data->Answer_State = Answer;
					}
					else if (!strcmp(lpSIP_Msg->Command, "302"))
					{
						lpInvite_Data->Answer_State = Forwarding;
						Write_Invite_Log(lpInvite_Data, Forwarding);
					}
					else if (lpSIP_Msg->Command[0] == '4')
					{
						if (!strcmp(lpSIP_Msg->Command, "486"))
						{
							Write_Invite_Log(lpInvite_Data, Busy);
							lpInvite_Data->Answer_State = Busy;
						}
						else
						{
							Write_Invite_Log(lpInvite_Data, Issue);
							Clear_Invite(lpSIP_Msg);
						}
					}
				}
				else if ((lpInvite_Data->Answer_State == Terminating || lpInvite_Data->Answer_State == Abandon))
				{
					if (!strcmp(lpSIP_Msg->Command, "200") && (State == SIP_BYE || State == SIP_CANCEL))
						Clear_Invite(lpSIP_Msg);
					else if (!strcmp(lpSIP_Msg->Command, "481"))
						Clear_Invite(lpSIP_Msg);
				}
			}
		}
	}
	else if ((Send_Flag == TO) && Invite_Cur >= 0 && Command != NULL) // Server發送封包
	{																  // 用於當超過30秒Callee沒接電話,Server將會中止這通會話

		// Request Process
		if (!strcmp(Command, "CANCEL") && lpInvite_Data->Answer_State == Inviting)
		{
			Write_Invite_Log(lpInvite_Data, Abandon);
			Clear_Invite(lpSIP_Msg);
		}
		else if (!strcmp(Command, "ACK"))
		{
			if (lpInvite_Data->Answer_State == Inviting)
			{
				ftime(&lpInvite_Data->Start_Time);
				lpInvite_Data->Answer_State = Answer;
			}
			else if (lpInvite_Data->Answer_State == Busy || lpInvite_Data->Answer_State == Forwarding)
				Clear_Invite(lpSIP_Msg);
		}
		// Response Process
		else
		{
			if (lpInvite_Data->Answer_State == Inviting && State == SIP_ERROR)
			{ // SIP_ERROR狀態當Server主動發送封包時出現
				if (Command[0] == '4')
				{
					Write_Invite_Log(lpInvite_Data, Issue);
					Clear_Invite(lpSIP_Msg);
				}
			}
		}
	}
	// TIMEOUT
	else if (Send_Flag == EXTRA && Invite_Cur >= 0)
	{
		if (lpInvite_Data->Answer_State == Inviting && State == SIP_INVITE)
		{
			Write_Invite_Log(lpInvite_Data, Issue);
			Clear_Invite(lpSIP_Msg);
		}
		else if (lpInvite_Data->Answer_State == Terminating || lpInvite_Data->Answer_State == Abandon)
		{
			if (State == SIP_BYE || State == SIP_CANCEL)
				Clear_Invite(lpSIP_Msg);
		}
	}
	// Show_Invite();
	//	Access_OnLine_State(lpSIP_Msg);

	return 0;
}

// void CSIP_Server::OnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult)
// {
// 	int Item;

// 	if ( (Item=m_Tab1.GetCurSel()) <0)	Item=0;
// 	// OnLine Statistic
// 	if (Item==0)  Show_OnLine();
// 	else if (Item==1)  Show_Monitoring();
// 	else if (Item==4)  Show_Invite();

// 	*pResult = 0;
// }

// void CSIP_Server::OnButton1()
// {
//    if (Write_Flag) {  Write_Flag=FALSE; GetDlgItem(IDC_BUTTON1)->SetWindowText("Enable Log File"); }
//    else            {  Write_Flag=TRUE;  GetDlgItem(IDC_BUTTON1)->SetWindowText("Disable Log File"); }

// }

// void CSIP_Server::OnButton2()
// {
// int Cur;
//     if (Monitor_Data_Num>0)
// 	{  for (Cur=0;Cur<MONITOR_DATA_SIZE;Cur++) if (Monitor_Data[Cur].Flag) Monitor_Data[Cur].Flag=FALSE;
// 	   Monitor_Data_Num=0;	}
// }

// void CSIP_Server::OnButton3()
// {
// 	char S1[100];
//     Relay_100_Flag++; Relay_100_Flag%=4; sprintf(S1,"Relay Level %d",Relay_100_Flag);
// 	if      (Relay_100_Flag==0) SetWindowText("Relay(Low):    PC to PC(Auto)");
// 	else if (Relay_100_Flag==1) SetWindowText("Relay(Middle): PC to PC");
// 	else if (Relay_100_Flag==2) SetWindowText("Relay(High):   PC to PC + Called Party");
// 	else if (Relay_100_Flag==3) SetWindowText("Relay(100%):   PC to PC + Called Party + PC to Phone");
// 	GetDlgItem(IDC_BUTTON3)->SetWindowText(S1);
// 	Update_SIP_Server_Param("WRITE");
// }

// void CSIP_Server::OnBrowse()
// {
// 	OPENFILENAME ofn;  // older version
// 	char	szFileName[_MAX_PATH + _MAX_FNAME + _MAX_EXT],	szTitleName[_MAX_FNAME + _MAX_EXT];

// 	memset(&ofn, 0, sizeof(ofn));
// 	memset(&szFileName,0,sizeof(szFileName));
// 	memset(&szTitleName,0,sizeof(szTitleName));

// 	ofn.lStructSize			= sizeof (OPENFILENAME) ;
// 	ofn.hwndOwner			= m_hWnd;
// 	ofn.lpstrFilter			= "Text files {*.txt}\0*.txt\0\0";
// 	ofn.lpstrCustomFilter	= NULL;
// 	ofn.nMaxCustFilter		= 0;
// 	ofn.nFilterIndex		= 0;
// 	ofn.lpstrFile			= szFileName ;
// 	ofn.nMaxFile			= _MAX_PATH;
// 	ofn.lpstrFileTitle		= szTitleName ;
// 	ofn.nMaxFileTitle		= _MAX_FNAME + _MAX_EXT ;
// 	ofn.lpstrDefExt			= "txt" ;
// 	ofn.lpstrInitialDir		= "C:";
// 	ofn.lpstrTitle			= "";
// 	ofn.Flags				= OFN_PATHMUSTEXIST + OFN_CREATEPROMPT + OFN_EXPLORER + OFN_NOCHANGEDIR;//+ OFN_ALLOWMULTISELECT ;
// 	ofn.nFileOffset			= 0;
// 	ofn.nFileExtension		= 0;
// 	ofn.lCustData			= 0l;
// 	ofn.lpfnHook			= NULL;
// 	ofn.lpTemplateName		= NULL;

// 	if (GetOpenFileName(&ofn)==TRUE)	ShellExecute(m_hWnd,"open",szFileName,NULL,NULL,SW_SHOWNORMAL);
// 	else return;
// }

int CSIP_Server::Update_SIP_Server_Param(char *Method)
{
	char S1[100];

	if (!strcmp(Method, "GET"))
	{
		GetPrivateProfileString("Setup Info", "RTP_Relay_IP", m_IP.c_str(), RTP_Relay_IP, sizeof(RTP_Relay_IP), SERVER_INFO_PATH);
		GetPrivateProfileString("Setup Info", "RTP_Relay_Port", m_Port.c_str(), RTP_Relay_Port, sizeof(RTP_Relay_Port), SERVER_INFO_PATH);
		GetPrivateProfileString("Setup Info", "NTUT_GW_IP", m_NTUT_IP.c_str(), NTUT_GW_IP, sizeof(NTUT_GW_IP), SERVER_INFO_PATH);
		GetPrivateProfileString("Setup Info", "NTUT_GW_Port", m_NTUT_Port.c_str(), NTUT_GW_Port, sizeof(NTUT_GW_Port), SERVER_INFO_PATH);
		GetPrivateProfileString("Setup Info", "Outbound_PhoneNo", m_Outbound_PhoneNo.c_str(), Outbound_PhoneNo, sizeof(Outbound_PhoneNo), SERVER_INFO_PATH);
		GetPrivateProfileString("Setup Info", "Log_File", m_Log_File.c_str(), Log_File, sizeof(Log_File), SERVER_INFO_PATH);
		GetPrivateProfileString("Setup Info", "Search_PhoneNo", m_Number_Search.c_str(), Search_PhoneNo, sizeof(Search_PhoneNo), SERVER_INFO_PATH);
		Relay_100_Flag = GetPrivateProfileInt("Setup Info", "Relay_100_Flag", 0, SERVER_INFO_PATH);
		Reg_Auth_Level = GetPrivateProfileInt("Setup2 Info", "Reg_Auth_Level", MAX_REG_LEVEL, SERVER_INFO_PATH);
		Proxy_Auth_Level = GetPrivateProfileInt("Setup2 Info", "Proxy_Auth_Level", MAX_PROXY_LEVEL, SERVER_INFO_PATH);
		Expire_Limit_Weight = GetPrivateProfileInt("Setup2 Info", "Expire_Limit_Weight", EXPIRE_LIMIT_WEIGHT, SERVER_INFO_PATH);

		// UpdateData(TRUE);
		m_IP = RTP_Relay_IP;
		m_Port = RTP_Relay_Port;
		m_NTUT_IP = NTUT_GW_IP;
		m_NTUT_Port = NTUT_GW_Port;
		m_Outbound_PhoneNo = Outbound_PhoneNo;
		m_Log_File = Log_File;
		m_Number_Search = Search_PhoneNo;
		sprintf(S1, "Relay Level %d", Relay_100_Flag);
		// GetDlgItem(IDC_BUTTON3)->SetWindowText(S1);
		// UpdateData(FALSE);

		// Set NTUT_GW Param
		if (m_lpNetIF)
		{
			if (NTUT_GW_Dest.is_used())
			{
				NTUT_GW_Dest.clean_nc();
				NTUT_GW_Dest.reset();
			}
			if (URL_Filter(NTUT_GW_IP))
			{
				m_lpNetIF->StartDestClient(&NTUT_GW_Dest, AF_INET, SOCK_DGRAM, CT_SIPCrypto, SIP_SERVER_PORT, NTUT_GW_IP, atoi(NTUT_GW_Port), false, SocketModule_Callbcak_Function, this, SIP_SERVER_EVENT);
			}
			// else
			// {
			// 	NTUT_GW_Nc.reset();
			// 	// memset(&NTUT_GW_Addr, 0, sizeof(NTUT_GW_Addr));
			// }
		}
	}
	else if (!strcmp(Method, "WRITE"))
	{
		struct stat fattrib;
		if (stat(SETUP_FOLDER, &fattrib) < 0)
			Create_Directory(SETUP_FOLDER);
		// UpdateData(TRUE);
		strcpy(RTP_Relay_IP, m_IP.c_str());
		strcpy(RTP_Relay_Port, m_Port.c_str());
		strcpy(NTUT_GW_IP, m_NTUT_IP.c_str());
		strcpy(NTUT_GW_Port, m_NTUT_Port.c_str());
		strcpy(Outbound_PhoneNo, m_Outbound_PhoneNo.c_str());
		strcpy(Log_File, m_Log_File.c_str());
		strcpy(Search_PhoneNo, m_Number_Search.c_str());
		// UpdateData(FALSE);

		WritePrivateProfileString("Setup Info", "RTP_Relay_IP", m_IP.c_str(), SERVER_INFO_PATH);
		WritePrivateProfileString("Setup Info", "RTP_Relay_Port", m_Port.c_str(), SERVER_INFO_PATH);
		WritePrivateProfileString("Setup Info", "NTUT_GW_IP", m_NTUT_IP.c_str(), SERVER_INFO_PATH);
		WritePrivateProfileString("Setup Info", "NTUT_GW_Port", m_NTUT_Port.c_str(), SERVER_INFO_PATH);
		WritePrivateProfileString("Setup Info", "Outbound_PhoneNo", m_Outbound_PhoneNo.c_str(), SERVER_INFO_PATH);
		WritePrivateProfileString("Setup Info", "Log_File", m_Log_File.c_str(), SERVER_INFO_PATH);
		WritePrivateProfileString("Setup Info", "Search_PhoneNo", m_Number_Search.c_str(), SERVER_INFO_PATH);
		sprintf(S1, "%d", Relay_100_Flag);
		WritePrivateProfileString("Setup Info", "Relay_100_Flag", S1, SERVER_INFO_PATH);
	}
	return 0;
}

// int CSIP_Server::Update_SIP_Server_Param(char *Method)
// {
// 	int		Locking_Expires,Expire_Limit_Weight;
// 	char	S1[100];
// 	char	Log_Path[_MAX_PATH];
// 	BOOL	Invite_Log_Check,Call_Log_Check,Call_Flows_Check,Reg_Flows_Check;
// 	BOOL	Forwarding_GW_Check,Forwarding_Mode_Check;
// 	BOOL	Progressive_Expires_Check,SubServer_Mapping_Check;
// 	BOOL	AutoCompose_PW_Check,ReplaceCode_Check;

// 	if (!strcmp(Method,"GET"))
// 	{
// 		GetPrivateProfileString("Setup2 Info","Reg_Auth_Msg",m_Reg_Auth_Msg,Reg_Auth_Msg,sizeof(Reg_Auth_Msg),SERVER_INFO_PATH);
// 		GetPrivateProfileString("Setup2 Info","Proxy_Auth_Msg",m_Proxy_Auth_Msg,Proxy_Auth_Msg,sizeof(Proxy_Auth_Msg),SERVER_INFO_PATH);
// 		Reg_Auth_Level = GetPrivateProfileInt("Setup2 Info","Reg_Auth_Level",MAX_REG_LEVEL,SERVER_INFO_PATH);
// 		Proxy_Auth_Level = GetPrivateProfileInt("Setup2 Info","Proxy_Auth_Level",MAX_PROXY_LEVEL,SERVER_INFO_PATH);
// 		GetPrivateProfileString("Setup2 Info","Log_Path",m_Log_Path,Log_Path,sizeof(Log_Path),SERVER_INFO_PATH);
// 		Invite_Log_Check = GetPrivateProfileInt("Setup2 Info","Invite_Log_Check",m_Invite_Log_Check,SERVER_INFO_PATH);
// 		Call_Log_Check = GetPrivateProfileInt("Setup2 Info","Call_Log_Check",m_Call_Log_Check,SERVER_INFO_PATH);
// 		Call_Flows_Check = GetPrivateProfileInt("Setup2 Info","Call_Flows_Check",m_Call_Flows_Check,SERVER_INFO_PATH);
// 		Reg_Flows_Check = GetPrivateProfileInt("Setup2 Info","Reg_Flows_Check",m_Reg_Flows_Check,SERVER_INFO_PATH);
// 		Forwarding_GW_Check = GetPrivateProfileInt("Setup2 Info","Forwarding_GW_Check",m_Forwarding_GW_Check,SERVER_INFO_PATH);
// 		Forwarding_Mode_Check = GetPrivateProfileInt("Setup2 Info","Forwarding_Mode_Check",m_Forwarding_Mode_Check,SERVER_INFO_PATH);
// 		Progressive_Expires_Check = GetPrivateProfileInt("Setup2 Info","Progressive_Expires_Check",m_Progressive_Expires_Check,SERVER_INFO_PATH);
// 		Locking_Expires = GetPrivateProfileInt("Setup2 Info","Locking_Expires",atoi(m_Locking_Expires),SERVER_INFO_PATH);
// 		SubServer_Mapping_Check = GetPrivateProfileInt("Setup2 Info","SubServer_Mapping_Check",m_SubServer_Mapping_Check,SERVER_INFO_PATH);
// 		AutoCompose_PW_Check = GetPrivateProfileInt("Setup2 Info","AutoCompose_PW_Check",m_AutoCompose_PW_Check,SERVER_INFO_PATH);
// 		Expire_Limit_Weight = GetPrivateProfileInt("Setup2 Info","Expire_Limit_Weight",EXPIRE_LIMIT_WEIGHT,SERVER_INFO_PATH);
// 		ReplaceCode_Check = GetPrivateProfileInt("Setup2 Info","ReplaceCode_Check",m_ReplaceCode_Check,SERVER_INFO_PATH);
// 		UpdateData(TRUE);

// 		m_Reg_Auth_Msg = Reg_Auth_Msg;
// 		m_Proxy_Auth_Msg = Proxy_Auth_Msg;
// 		sprintf(S1,"Reg_Auth_Level = %d",Reg_Auth_Level); GetDlgItem(IDC_BUTTON1)->SetWindowText(S1);
// 		sprintf(S1,"Proxy_Auth Level = %d",Proxy_Auth_Level); GetDlgItem(IDC_BUTTON6)->SetWindowText(S1);
// 		m_Log_Path = Log_Path;
// 		m_Invite_Log_Check = Invite_Log_Check;
// 		m_Call_Log_Check = Call_Log_Check;
// 		m_Call_Flows_Check = Call_Flows_Check;
// 		m_Reg_Flows_Check = Reg_Flows_Check;
// 		m_Forwarding_GW_Check = Forwarding_GW_Check; GetDlgItem(IDC_CHECK9)->EnableWindow(Forwarding_GW_Check);
// 		m_Forwarding_Mode_Check = Forwarding_Mode_Check;
// 		m_Progressive_Expires_Check = Progressive_Expires_Check; GetDlgItem(IDC_EDIT5)->EnableWindow(!Progressive_Expires_Check);
// 		sprintf(S1,"%d",Locking_Expires); m_Locking_Expires = S1;
// 		m_SubServer_Mapping_Check = SubServer_Mapping_Check; GetDlgItem(IDC_BUTTON4)->EnableWindow(SubServer_Mapping_Check); GetDlgItem(IDC_BUTTON5)->EnableWindow(SubServer_Mapping_Check);
// 		m_AutoCompose_PW_Check = AutoCompose_PW_Check;
// 		sprintf(S1,"%d",Expire_Limit_Weight); m_Expire_Limit_Weight = S1;
// 		m_ReplaceCode_Check = ReplaceCode_Check; GetDlgItem(IDC_BUTTON7)->EnableWindow(ReplaceCode_Check); GetDlgItem(IDC_BUTTON8)->EnableWindow(ReplaceCode_Check);
// 		UpdateData(FALSE);

// 		if (SubServer_Mapping_Check)	OnSubServer_Table_Reload(); else lpSIP_Server->Init_SubServer_Mapping();
// 		if (ReplaceCode_Check)	OnReplace_Phone_Reload(); else lpSIP_Server->Init_Replace_Code();
// 		//--- Debug ---
// 		TRACE("Update_SIP_Server_Param(GET), Log_Path=%s\r\n",Log_Path);
// 		//-------------
// 	}
// 	else if (!strcmp(Method,"WRITE"))
// 	{
// 		if(GetFileAttributes(SETUP_FOLDER)==0xFFFFFFFF)	Create_Directory(SETUP_FOLDER);
// 		UpdateData(TRUE);
// 		strcpy(Reg_Auth_Msg,m_Reg_Auth_Msg);
// 		strcpy(Proxy_Auth_Msg,m_Proxy_Auth_Msg);
// 		strcpy(Log_Path,m_Log_Path);
// 		Invite_Log_Check = m_Invite_Log_Check;
// 		Call_Log_Check = m_Call_Log_Check;
// 		Call_Flows_Check = m_Call_Flows_Check;
// 		Reg_Flows_Check = m_Reg_Flows_Check;
// 		Forwarding_GW_Check = m_Forwarding_GW_Check;
// 		Forwarding_Mode_Check = m_Forwarding_Mode_Check;
// 		Progressive_Expires_Check = m_Progressive_Expires_Check;
// 		Locking_Expires = atoi(m_Locking_Expires);
// 		SubServer_Mapping_Check = m_SubServer_Mapping_Check;
// 		AutoCompose_PW_Check = m_AutoCompose_PW_Check;
// 		Expire_Limit_Weight = atoi(m_Expire_Limit_Weight);
// 		ReplaceCode_Check = m_ReplaceCode_Check;
// 		UpdateData(FALSE);

// 		WritePrivateProfileString("Setup2 Info","Reg_Auth_Msg",m_Reg_Auth_Msg,SERVER_INFO_PATH);
// 		WritePrivateProfileString("Setup2 Info","Proxy_Auth_Msg",m_Proxy_Auth_Msg,SERVER_INFO_PATH);
// 		sprintf(S1,"%d",Reg_Auth_Level); WritePrivateProfileString("Setup2 Info","Reg_Auth_Level",S1,SERVER_INFO_PATH);
// 		sprintf(S1,"%d",Proxy_Auth_Level); WritePrivateProfileString("Setup2 Info","Proxy_Auth_Level",S1,SERVER_INFO_PATH);
// 		WritePrivateProfileString("Setup2 Info","Log_Path",m_Log_Path,SERVER_INFO_PATH);
// 		sprintf(S1,"%d",Invite_Log_Check); WritePrivateProfileString("Setup2 Info","Invite_Log_Check",S1,SERVER_INFO_PATH);
// 		sprintf(S1,"%d",Call_Log_Check); WritePrivateProfileString("Setup2 Info","Call_Log_Check",S1,SERVER_INFO_PATH);
// 		sprintf(S1,"%d",Call_Flows_Check); WritePrivateProfileString("Setup2 Info","Call_Flows_Check",S1,SERVER_INFO_PATH);
// 		sprintf(S1,"%d",Reg_Flows_Check); WritePrivateProfileString("Setup2 Info","Reg_Flows_Check",S1,SERVER_INFO_PATH);
// 		sprintf(S1,"%d",Forwarding_GW_Check); WritePrivateProfileString("Setup2 Info","Forwarding_GW_Check",S1,SERVER_INFO_PATH);
// 		sprintf(S1,"%d",Forwarding_Mode_Check); WritePrivateProfileString("Setup2 Info","Forwarding_Mode_Check",S1,SERVER_INFO_PATH);
// 		sprintf(S1,"%d",Progressive_Expires_Check); WritePrivateProfileString("Setup2 Info","Progressive_Expires_Check",S1,SERVER_INFO_PATH);
// 		WritePrivateProfileString("Setup2 Info","Locking_Expires",m_Locking_Expires,SERVER_INFO_PATH);
// 		sprintf(S1,"%d",SubServer_Mapping_Check); WritePrivateProfileString("Setup2 Info","SubServer_Mapping_Check",S1,SERVER_INFO_PATH);
// 		sprintf(S1,"%d",AutoCompose_PW_Check); WritePrivateProfileString("Setup2 Info","AutoCompose_PW_Check",S1,SERVER_INFO_PATH);
// 		WritePrivateProfileString("Setup2 Info","Expire_Limit_Weight",m_Expire_Limit_Weight,SERVER_INFO_PATH);
// 		sprintf(S1,"%d",ReplaceCode_Check); WritePrivateProfileString("Setup2 Info","ReplaceCode_Check",S1,SERVER_INFO_PATH);
// 		//--- Debug ---
// 		TRACE("Update_SIP_Server_Param(WRITE), Log_Path=%s\r\n",Log_Path);
// 		//-------------
// 	}
// 	// Return Paramter to SIP_Server
// 	lpSIP_Server->Reg_Auth_Level = Reg_Auth_Level;
// 	lpSIP_Server->Proxy_Auth_Level = Proxy_Auth_Level;
// 	strcpy(lpSIP_Server->Log_Path,Log_Path);
// 	lpSIP_Server->Invite_Log_Check = (Invite_Log_Check)? TRUE:FALSE;
// 	lpSIP_Server->Call_Log_Check = (Call_Log_Check)? TRUE:FALSE;
// 	lpSIP_Server->Call_Flows_Check = (Call_Flows_Check)? TRUE:FALSE;
// 	lpSIP_Server->Reg_Flows_Check = (Reg_Flows_Check)? TRUE:FALSE;
// 	lpSIP_Server->Forwarding_GW_Check = (Forwarding_GW_Check)? TRUE:FALSE;
// 	lpSIP_Server->Forwarding_Mode_Check = (Forwarding_Mode_Check)? TRUE:FALSE;
// 	lpSIP_Server->Progressive_Expires_Check = (Progressive_Expires_Check)? TRUE:FALSE;
// 	lpSIP_Server->Locking_Expires = Locking_Expires;
// 	lpSIP_Server->SubServer_Mapping_Check = (SubServer_Mapping_Check)? TRUE:FALSE;
// 	lpSIP_Server->AutoCompose_PW_Check = (AutoCompose_PW_Check)? TRUE:FALSE;
// 	lpSIP_Server->Expire_Limit_Weight = Expire_Limit_Weight;
// 	lpSIP_Server->ReplaceCode_Check = (ReplaceCode_Check)? TRUE:FALSE;
// 	//--- Debug ---
// 	TRACE("Update_SIP_Server_Param(lpSOP_Server), Log_Path=%s\r\n",Log_Path);
// 	//-------------
// 	return 0;
// }

int CSIP_Server::Save_SubServer_Data(DEST_DATA *lpSubServer_Data)
{
	int SubServer_Cur;

	if ((SubServer_Cur = Check_SubServer_Data(lpSubServer_Data)) < 0)
	{
		for (SubServer_Cur = 0; SubServer_Cur < SUBSERVER_DATA_SIZE; SubServer_Cur++)
		{
			if (!SubServer_Data[SubServer_Cur].is_used())
				break;
		}
		if (SubServer_Cur < SUBSERVER_DATA_SIZE)
		{
			memcpy(&SubServer_Data[SubServer_Cur], lpSubServer_Data, sizeof(DEST_DATA));
			SubServer_Num++;
		}
		else
			SubServer_Cur = SUBSERVER_DATA_SIZE;
	}
	else
	{
		SubServer_Data[SubServer_Cur].copy_from(lpSubServer_Data);
		SubServer_Num++;
	}
	if (SubServer_Cur < SUBSERVER_DATA_SIZE)
		return SubServer_Cur;
	else
		return -1;
}

int CSIP_Server::Check_SubServer_Data(DEST_DATA *lpSubServer_Data)
{
	int i, SubServer_Cur;

	if (SubServer_Num > 0 && SubServer_Num <= SUBSERVER_DATA_SIZE)
	{
		for (i = 0, SubServer_Cur = 0; SubServer_Cur < SUBSERVER_DATA_SIZE; SubServer_Cur++)
		{
			if (SubServer_Data[SubServer_Cur].is_used() &&
				(!strcmp(SubServer_Data[SubServer_Cur].Real_IP, lpSubServer_Data->Real_IP)) && (SubServer_Data[SubServer_Cur].Port == lpSubServer_Data->Port))
				break;
			i++;
			if (i >= SubServer_Num)
			{
				SubServer_Cur = SUBSERVER_DATA_SIZE;
				break;
			}
		}
	}
	else
		SubServer_Cur = SUBSERVER_DATA_SIZE;
	if (SubServer_Cur < SUBSERVER_DATA_SIZE)
		return SubServer_Cur;
	else
		return -1;
}

int CSIP_Server::Clear_SubServer_Data(int SubServer_Cur)
{
	CRASH_DEBUG_COMMAND;
	SubServer_Data[SubServer_Cur].clean_nc();
	SubServer_Data[SubServer_Cur].reset();
	SubServer_Num--;
	RETURN 0;
}

int CSIP_Server::Clean_All_SubServer_Data()
{
	CRASH_DEBUG_COMMAND;
	for (int Cur; Cur < SUBSERVER_DATA_SIZE; Cur++)
	{
		if (SubServer_Data[Cur].is_used())
		{
			Clear_SubServer_Data(Cur);
		}
	}
	SubServer_Num = 0;
	RETURN 0;
}

// void CSIP_Server::OnAccess_DB()
// {
// 	Access_CDMS_Flag=(!Access_CDMS_Flag)? TRUE:FALSE;
// 	GetDlgItem(IDC_BUTTON7)->SetWindowText((!Access_CDMS_Flag)? "Open CMDS Access":"Close CMDS Access");
// }

// void CSIP_Server::OnServer_Setup()
// {
// 	if (!lpSIP_Server_Setup) return;
// 	if (lpSIP_Server_Setup->IsWindowVisible())	lpSIP_Server_Setup->ShowWindow(SW_HIDE);
// 	else	lpSIP_Server_Setup->ShowWindow(SW_SHOW);
// }

//====================== SIP_File ================================

int CSIP_Server::Write_Log(const char *Head, char *S1, sockaddr_mix *lpAddr, SIP_MSG *lpSIP_Msg)
{
	CRASH_DEBUG_COMMAND;
	FILE *HsfOut1;
	char IP[64], Name[200];
	WORD Port;
	std::string timestr;

	// 顯示部分訊息
#ifdef _DEBUG
	char S2[120];
	int i;
	for (i = 0; i < 70; i++)
		if ((S1[i] == 0x0a) || (S1[i] == 0x0d))
			break;
	memcpy(S2, S1, i);
	S2[i] = 0;
	if (lpSIP_Msg != NULL)
		DTrace_TestMsg("%s:  %s Expire: %s\r\n", Head, S2, lpSIP_Msg->Expires);
	else
		DTrace_TestMsg("%s:  %s\r\n", Head, S2);
#endif

	// Write call log with each session
	Write_Call_Log(Head, S1, lpSIP_Msg, lpAddr);
	//----------------------------------------------------------------------------------------------------
	if (!Write_Flag)
	{
		RETURN - 1;
	}

	CRYPTO_TYPE Crypto_Type;
	char szCrypto_Type[20];

	// 寫入 Log 檔案
	if ((strlen(Log_File) > 0) && (strlen(Log_File) < 20))
		sprintf(Name, "C:/SIP_Log_%s.txt", Log_File);
	else
		strcpy(Name, LOG_FILE_NAME);
	if (strlen(Log_File) > 0)
	{
		if ((lpSIP_Msg == NULL) /* || (lpAddr==NULL) */)
		{
			RETURN - 1;
		}
		if ((strcmp(Log_File, lpSIP_Msg->From_Name)) && (strncmp(Log_File, lpSIP_Msg->To_Name, strlen(Log_File))))
		{
			RETURN - 1;
		}
	}
	Get_Network_Mothod(Head, lpSIP_Msg, Crypto_Type);
	if ((HsfOut1 = fopen(Name, "a+")) == NULL)
	{
		RETURN - 1;
	}
	else
	{
		if (lpAddr != NULL)
		{
			Port = lpAddr->sin_port;
			Port = ((Port & 0xFF) << 8) + ((Port >> 8) & 0xFF);
			strcpy(IP, inet_ntoa(lpAddr->sin_addr));
		}
		else
		{
			Port = 0;
			IP[0] = 0;
		}
		// Crypto Type
		if (Crypto_Type == CT_NoCrypto)
			strcpy(szCrypto_Type, "NoCrypto");
		else if (Crypto_Type == CT_SIPCrypto)
			strcpy(szCrypto_Type, "SIP_Crypto");
		else if (Crypto_Type == CT_Walkersun1)
			strcpy(szCrypto_Type, "Walkersun1_Crypto");
		else
			strcpy(szCrypto_Type, "Crypto_Error");

		// Print Msg
		fprintf(HsfOut1, "         << %s %s %s:%d  %s >>\r\n", szCrypto_Type, Head, IP, Port, Jstringftime(timestr, "%Y/%m/%d %H:%M:%S.%f", NULL)->c_str());
		fprintf(HsfOut1, "%s\r\n\r\n", S1);
		fclose(HsfOut1);
	}
	RETURN 0;
}

int CSIP_Server::Get_Network_Mothod(const char *Head, const SIP_MSG *lpSIP_Msg, CRYPTO_TYPE &Crypto_Type)
{
	bool From_To_Flag, Request_Flag, Temp_Flag;

	if (lpSIP_Msg != NULL)
	{
		if (!strcmp(Head, HEAD_RECVFROM) || !strcmp(Head, HEAD_SENDTO))
		{
			From_To_Flag = (!strcmp(Head, HEAD_RECVFROM)) ? TRUE : FALSE;
			Request_Flag = (lpSIP_Msg->Command[0] < '1' || lpSIP_Msg->Command[0] > '6') ? TRUE : FALSE;
			Temp_Flag = ((Request_Flag && From_To_Flag) || (!Request_Flag && !From_To_Flag)) ? TRUE : FALSE;
			Crypto_Type = (Temp_Flag) ? lpSIP_Msg->FromDest.Crypto_Type : lpSIP_Msg->ToDest.Crypto_Type;
		}
		else
		{
			Crypto_Type = CT_NoCrypto;
		}
	}
	else
		return -1;
	return 0;
}

// -------------------------------------------------------------------------------------------
// int  CSIP_Server::Init_Monitor()
// {
// 	int		Cur,i;
// 	FILE	*In;

// 	Monitor_Data_Num=0;
// 	for (Cur=0;Cur<MONITOR_DATA_SIZE;Cur++) Monitor_Data[Cur].Flag=FALSE;
// 	if ( (i=Start_TCP_Server(&Monitor_Socket,MONITOR_PORT,MONITOR_EVENT,m_hWnd))< 0 )
// 	{  DTrace_Error("TCP Server for Monitor is Fail"); }

// 	if ((In=fopen(SIP_MONITOR_FILE,"rb"))!=NULL)
// 	{	while (!feof(In))	{ i=fread(&Monitor_Data[Monitor_Data_Num],sizeof(MONITOR_DATA),1,In); if (i==1) Monitor_Data_Num++; }
// 		fclose(In);
// 	}
// 	return 0;
// }

// int  CSIP_Server::Check_Monitor(SIP_MSG *lpSIP_Data)
// {
// int  Cur,i;
//      for (i=0,Cur=0;Cur<MONITOR_DATA_SIZE;Cur++)
// 	 {   if (Monitor_Data_Num==0) { Cur=MONITOR_DATA_SIZE; break; }
// 		 if (Monitor_Data[Cur].Flag)
// 	     {  if ( (!strcmp(Monitor_Data[Cur].PhoneNo,lpSIP_Data->From_Name)) || (!strcmp(Monitor_Data[Cur].PhoneNo,lpSIP_Data->To_Name)) ) break;
// 	        i++; if (i>=Monitor_Data_Num) { Cur=MONITOR_DATA_SIZE; break; }  }  }
// 	 if (Cur<MONITOR_DATA_SIZE) return Cur; else return -1;
// }

// int  CSIP_Server::Check_Monitor_By_PhoneNo(char *PhoneNo)
// {
// int  Cur,i;
//      for (i=0,Cur=0;Cur<MONITOR_DATA_SIZE;Cur++)
// 	 {   if (Monitor_Data_Num==0) { Cur=MONITOR_DATA_SIZE; break; }
// 		 if (Monitor_Data[Cur].Flag)
// 	     {  if (!strcmp(Monitor_Data[Cur].PhoneNo,PhoneNo)) break;
// 	        i++; if (i>=Monitor_Data_Num) { Cur=MONITOR_DATA_SIZE; break; }  }  }
// 	 if (Cur<MONITOR_DATA_SIZE) return Cur; else return -1;
// }

// int  CSIP_Server::Insert_Monitor(char *PhoneNo,char *Monitor_TCP_IP,char *Monitor_TCP_Port)
// {
// int  Cur,i;
//      for (i=0,Cur=0;Cur<MONITOR_DATA_SIZE;Cur++)
// 	 {   if (Monitor_Data_Num==0) { Cur=MONITOR_DATA_SIZE; break; }
// 	     if (Monitor_Data[Cur].Flag)
// 	     {  if (!strcmp(Monitor_Data[Cur].PhoneNo,PhoneNo)) break;
// 	        i++; if (i>=Monitor_Data_Num) { Cur=MONITOR_DATA_SIZE; break; }  }  }
// 	 // 已存在, 更新之
// 	 if (Cur<MONITOR_DATA_SIZE)
// 	 {  strcpy(Monitor_Data[Cur].Monitor_TCP_IP,Monitor_TCP_IP);
// 	    strcpy(Monitor_Data[Cur].Monitor_TCP_Port,Monitor_TCP_Port);
// 		return Cur;
// 	 }
// 	 else
// 	 {  for (Cur=0;Cur<MONITOR_DATA_SIZE;Cur++) if (!Monitor_Data[Cur].Flag) break;
// 	    if (Cur<MONITOR_DATA_SIZE)
// 		{  Monitor_Data[Cur].Flag=TRUE; Monitor_Data_Num++;
// 		   strcpy(Monitor_Data[Cur].PhoneNo,PhoneNo);
// 		   strcpy(Monitor_Data[Cur].Monitor_TCP_IP,Monitor_TCP_IP);
// 	       strcpy(Monitor_Data[Cur].Monitor_TCP_Port,Monitor_TCP_Port);
// 		   return Cur; }
// 		else return -1;
// 	 }
// }

// int  CSIP_Server::Delete_Monitor(char *PhoneNo)
// {
// int  Cur,i;
//      for (i=0,Cur=0;Cur<MONITOR_DATA_SIZE;Cur++)
// 	 {   if (Monitor_Data_Num==0) { Cur=MONITOR_DATA_SIZE; break; }
// 	     if (Monitor_Data[Cur].Flag)
// 	     {  if (!strcmp(Monitor_Data[Cur].PhoneNo,PhoneNo)) break;
// 	        i++; if (i>=Monitor_Data_Num) { Cur=MONITOR_DATA_SIZE; break; }  }  }
// 	 if (Cur<MONITOR_DATA_SIZE)
// 	 {  Monitor_Data[Cur].Flag=FALSE; Monitor_Data_Num--;
// 		return Cur; 	 }
// 	 else
// 	 {  return -1;	 }
// }

// ========================================================================
int CSIP_Server::Init_Command()
{
	int Cur;

	Command_Queue_Num = 0;
	for (Cur = 0; Cur < COMMAND_QUEUE_SIZE; Cur++)
	{
		Command_Data[Cur].reset();
		if (m_lpNetIF)
			m_lpNetIF->InitTimer(&Command_Data[Cur].MixTimer, this, MixTimer_Callbcak_Function);
	}
	return 0;
}

int CSIP_Server::Check_Command_By_Relay_Socket(MIXSOCKET Socket)
{
	int i, Cur;

	// 2. Check Exist Command
	for (i = 0, Cur = 0; Cur < COMMAND_QUEUE_SIZE; Cur++)
	{
		if (Command_Queue_Num == 0)
		{
			Cur = COMMAND_QUEUE_SIZE;
			break;
		}
		if (Command_Data[Cur].State != SIP_READY)
		{
			if (Command_Data[Cur].Relay_TCP_Dest.pNc->mixsocket == Socket)
				break;
			i++;
			if (i >= Command_Queue_Num)
			{
				Cur = COMMAND_QUEUE_SIZE;
				break;
			}
		}
	}
	// 3. Return Result
	if (Cur >= COMMAND_QUEUE_SIZE)
		return -1;
	else
		return Cur;
}
int CSIP_Server::Check_Command(SIP_MSG *lpSIP_Data)
{
	int i, Cur;
	char Command[100];

	for (i = 0, Cur = 0; Cur < COMMAND_QUEUE_SIZE; Cur++)
	{
		if (Command_Queue_Num == 0)
		{
			Cur = COMMAND_QUEUE_SIZE;
			break;
		}
		if (Command_Data[Cur].State != SIP_READY)
		{ // 1. Check Command
			if ((lpSIP_Data->Command[0] >= '1') && (lpSIP_Data->Command[0] <= '4'))
				Get_CSeq_Num(lpSIP_Data->Cmd_Str, "CSeq:", Command);
			else
				strcpy(Command, lpSIP_Data->Command);
			// 2. Check From, To
			if ((!strcmp(Command_Data[Cur].SIP_Msg.Command, Command)) &&
				(!strcmp(Command_Data[Cur].SIP_Msg.From_Name, lpSIP_Data->From_Name)) &&
				(!strcmp(Command_Data[Cur].SIP_Msg.To_Name, lpSIP_Data->To_Name)))
				break;
			// 3. Calculate i
			i++;
			if (i >= Command_Queue_Num)
			{
				Cur = COMMAND_QUEUE_SIZE;
				break;
			}
		}
	}

	if (Cur < COMMAND_QUEUE_SIZE)
		return Cur;
	else
		return -1;
}

int CSIP_Server::Save_Command(SIP_MSG *lpSIP_Data)
{
	int Command_Cur;

	for (Command_Cur = 0; Command_Cur < COMMAND_QUEUE_SIZE; Command_Cur++)
	{
		if (Command_Data[Command_Cur].State == SIP_READY)
			break;
	}
	if (Command_Cur < COMMAND_QUEUE_SIZE)
	{
		if (!strcmp(lpSIP_Data->Command, "REGISTER"))
			Command_Data[Command_Cur].State = SIP_REG;
		else if (!strcmp(lpSIP_Data->Command, "CANCEL"))
			Command_Data[Command_Cur].State = SIP_CANCEL;
		else if (!strcmp(lpSIP_Data->Command, "BYE"))
			Command_Data[Command_Cur].State = SIP_BYE;
		else if (!strcmp(lpSIP_Data->Command, "REFER"))
			Command_Data[Command_Cur].State = SIP_REFER;
		else if (!strcmp(lpSIP_Data->Command, "NOTIFY"))
			Command_Data[Command_Cur].State = SIP_NOTIFY;
		else if (!strcmp(lpSIP_Data->Command, "ACK"))
			return -1;
		else if (!strcmp(lpSIP_Data->Command, "INVITE"))
		{ //To_Name: (02~09)...
			Command_Data[Command_Cur].PSTN_Flag = (Forwarding_GW_Check && !lpSIP_Data->Forwarding_GW_Flag) && (lpSIP_Data->To_Name[0] == '0' && (lpSIP_Data->To_Name[1] >= '2' && lpSIP_Data->To_Name[1] <= '9'));
			Command_Data[Command_Cur].Abandon_Time = (Command_Data[Command_Cur].PSTN_Flag) ? Forwarding_GW_TIME : ABANDON_TIME;
		}
		memcpy(&Command_Data[Command_Cur].SIP_Msg, lpSIP_Data, sizeof(SIP_MSG));
		Command_Data[Command_Cur].Response_SIP_Msg.Command[0] = 0;
		Command_Data[Command_Cur].Timer_Counter = Command_Data[Command_Cur].Relay_Cur = 0;
		Command_Data[Command_Cur].Relay_UDP_IP[0] = Command_Data[Command_Cur].Relay_UDP_FPort[0] = Command_Data[Command_Cur].Relay_UDP_TPort[0] = 0;
		// Command_Data[Command_Cur].Relay_TCP_Socket=0;
		Command_Data[Command_Cur].Abandon_Flag = 0;
		Command_Data[Command_Cur].Disconnect_Flag = FALSE;
		Command_Queue_Num++;
		return Command_Cur;
	}
	else
		return -1;
}

int CSIP_Server::Clear_Command(int Cur)
{
	if (Command_Data[Cur].State == SIP_READY)
		return -1;
	else
	{
		if (m_lpNetIF)
			m_lpNetIF->KillTimer(&Command_Data[Cur].MixTimer);
		Command_Data[Cur].Relay_TCP_Dest.clean_nc();
		Command_Data[Cur].reset();
		Command_Queue_Num--;
		return 0;
	}
}

int CSIP_Server::Clean_All_Command_Data()
{
	for (int Cur; Cur < COMMAND_QUEUE_SIZE; Cur++)
	{
		Clear_Command(Cur);
	}
	Command_Queue_Num = 0;
}

int CSIP_Server::Update_Command(int Cur, const char *Command)
{
	int i, j;
	char Buf[500], Buf1[100], S1[SIP_MSG_LEGNTH];

	if ((Command_Data[Cur].State != SIP_RELAY_CONNECT) &&
		(Command_Data[Cur].State != SIP_RELAY_READ) &&
		(Command_Data[Cur].State != SIP_INVITE) &&
		(Command_Data[Cur].State != SIP_100) &&
		(Command_Data[Cur].State != SIP_200))
		return -1;
	else
	{
		Command_Data[Cur].State = SIP_200;
		Command_Data[Cur].Timer_Counter = 0;
		// 1. Update Command(Command 與 Cmd_Str/CSeq 需同時修改, 以免造成 Send_SIP_Command 錯誤)
		if (strlen(Command_Data[Cur].Response_SIP_Msg.Command) <= 0)
			memcpy(&Command_Data[Cur].Response_SIP_Msg, &Command_Data[Cur].SIP_Msg, sizeof(SIP_MSG));
		strcpy(Command_Data[Cur].Response_SIP_Msg.Command, Command);
		// Generate Command Line
		if ((Command[0] >= '1') && (Command[0] <= '6'))
		{
			Get_Response_Code(Buf1, atoi(Command));
			sprintf(Buf, "SIP/2.0 %s\r\n", Buf1);
		}
		else
			sprintf(Buf, "%s sip:%s@%s SIP/2.0\r\n", Command, Command_Data[Cur].Response_SIP_Msg.Command_Name, Command_Data[Cur].Response_SIP_Msg.Command_URL);
		// Replace Command line of Cmd_Str by new Command line
		strcpy(S1, Command_Data[Cur].Response_SIP_Msg.Cmd_Str);
		for (i = 0; i < (int)strlen(S1); i++)
			if ((S1[i] == 0x0a) || (S1[i] == 0x0d))
				break;
		for (j = i; j < (int)strlen(S1); j++)
			if ((S1[j] != 0x0a) && (S1[j] != 0x0d))
				break;
		strcpy(S1, Buf);
		strcat(S1, &Command_Data[Cur].Response_SIP_Msg.Cmd_Str[j]);
		strcpy(Command_Data[Cur].Response_SIP_Msg.Cmd_Str, S1);
	}
	return 0;
}
int CSIP_Server::Update_Command_By_SIP_Msg(int Cur, SIP_MSG *lpSIP_Msg)
{
	if (Command_Data[Cur].State != SIP_READY)
		memcpy(&Command_Data[Cur].SIP_Msg, lpSIP_Msg, sizeof(SIP_MSG));
	return 0;
}
int CSIP_Server::Update_Command_By_Response(int Cur, SIP_MSG *lpSIP_Msg)
{
	memcpy(&Command_Data[Cur].Response_SIP_Msg, lpSIP_Msg, sizeof(SIP_MSG));
	return 0;
}

// ===================================================================================
int CSIP_Server::Init_OnLine()
{
	int i;

	SIP_OnLine_Num = 0;
	for (i = 0; i < SIP_ONLINE_SIZE; i++)
	{
		SIP_OnLine_Data[i].reset();
	}
	if (m_lpNetIF)
	{
		m_lpNetIF->InitTimer(&ExpiresMixTimer, this, MixTimer_Callbcak_Function);
		m_lpNetIF->SetTimer(&ExpiresMixTimer, ONE_SECOND, TIMERID_EXPIRES);
	}

	return 0;
}

int CSIP_Server::Check_OnLine(char *PhoneNo, sockaddr_mix *Addr)
{
	CRASH_DEBUG_COMMAND;
	if (!PhoneNo)
	{
		DTrace_FatalError("invalid parameters, %d @ %s\n", __LINE__, __FILE__);
		RETURN - 1;
	}

	int i, Cur;
	bool LogicA, LogicB;

	// Search whether PhoneNo Exist or not ?
	if (PhoneNo[0] && SIP_OnLine_Num > 0)
	{
		for (i = 0, Cur = 0; Cur < SIP_ONLINE_SIZE; Cur++)
		{
			if (SIP_OnLine_Data[Cur].Flag)
			{
				if (!strcmp(SIP_OnLine_Data[Cur].PhoneNo, PhoneNo))
				{
					if (Addr != NULL)
					{
						LogicA = Addrcmp(Addr, &SIP_OnLine_Data[Cur].Trunk_GW.Raddr);
						LogicB = Addrcmp(&SIP_OnLine_Data[Cur].Dest.Raddr, &SIP_OnLine_Data[Cur].Trunk_GW.Raddr);
						if ((!LogicA && !LogicB) || (LogicA && LogicB))
							break;
					}
					// Logic Operator
					else if (Addrcmp(&SIP_OnLine_Data[Cur].Dest.Raddr, &SIP_OnLine_Data[Cur].Trunk_GW.Raddr))
						break;
				}
				i++;
				if (i >= SIP_OnLine_Num)
				{
					Cur = SIP_ONLINE_SIZE;
					break;
				}
			}
		}
		if (Cur >= SIP_ONLINE_SIZE)
		{
			RETURN - 4;
		}
		else
		{
			RETURN Cur;
		}
	}
	else
	{
		RETURN - 3;
	}
}

int CSIP_Server::Save_OnLine(SIP_MSG *lpData, int Flag)
{
	CRASH_DEBUG_COMMAND;
	int Cur, i;
	char SS1[SIP_MSG_LEGNTH];
	sockaddr_mix *lpAddr;

	// From: Used on Command, To: Used on Status and PC to Phone
	if (Flag == FROM)
	{
		strcpy(SS1, lpData->From_Name);
		lpAddr = &lpData->FromDest.Raddr;
	}
	else
	{
		strcpy(SS1, lpData->To_Name);
		lpAddr = &lpData->ToDest.Raddr;
	}

	// Error Checking
	if (strlen(SS1) < 1)
	{
		RETURN - 2;
	} // 號碼長度不能小於1
	else if (strlen(SS1) > 30)
	{
		RETURN - 3;
	} // 號碼長度不能大於30
	for (Cur = 0; Cur < (int)strlen(SS1); Cur++)
		if (((SS1[Cur] < '0') || (SS1[Cur] > '9')) && (SS1[Cur] != '-'))
		{
			RETURN - 4;
		}

	// 2. OffLine
	if ((Cur = Check_OnLine(SS1, lpAddr)) < 0)
	{
		if (SIP_OnLine_Num >= SIP_ONLINE_SIZE)
		{
			RETURN - 1;
		}
		for (Cur = 0; Cur < SIP_ONLINE_SIZE; Cur++)
			if (!SIP_OnLine_Data[Cur].Flag)
				break;
		if (Cur >= SIP_ONLINE_SIZE)
		{
			RETURN - 1;
		}
		else
		{
			SIP_OnLine_Num++;
			SIP_OnLine_Data[Cur].Flag = TRUE;
			SIP_OnLine_Data[Cur].Password[0] = 0;
			SIP_OnLine_Data[Cur].Mobile[0] = SIP_OnLine_Data[Cur].Name[0] = 0;
			SIP_OnLine_Data[Cur].First_Flag = TRUE;
			SIP_OnLine_Data[Cur].CDMS_OK = 0;			// Indicate if Account is vaild in SQL
			SIP_OnLine_Data[Cur].MD5_OK = FALSE;		// Indicate if MD5 Auth. is OK
			SIP_OnLine_Data[Cur].Proxy_MD5_OK = FALSE;	// Indicate if Proxy MD5 Auth. is OK
			SIP_OnLine_Data[Cur].Expires_OK = FALSE;	// Indicate if Expires_Update is increased
			SIP_OnLine_Data[Cur].Auth_OK = FALSE;		// Indicate if "Authentication: .." is presented
			SIP_OnLine_Data[Cur].Proxy_Auth_OK = FALSE; // Indicate if "Proxy_Authentication: .." is presented
			Set_OnLine(lpData, Cur, Flag);
			Update_Expire(lpData, Cur);
			// Get Server Port
			for (i = 0; i < Server_Port_Num; i++)
			{
				if (SIP_Server_Nc[i].mixsocket == lpData->FromDest.pNc->mixsocket)
				{
					SIP_OnLine_Data[Cur].Server_Port = SIP_Server_Port[i];
					break;
				}
			}
			// Access OnLine to Database
			//	Access_OnLine_State(lpData);
		}
	}
	// 3. Already OnLine
	else
	{
		SIP_OnLine_Data[Cur].First_Flag = FALSE;
		Set_OnLine(lpData, Cur, Flag);
		Update_Expire(lpData, Cur);
		// Access OnLine to Database
		//	Access_OnLine_State(lpData);
	}

	RETURN Cur;
}

// 初次 儲存 OnLine
void CSIP_Server::Set_OnLine(SIP_MSG *lpData, int Cur, int Flag)
{
	WORD Port;

	if (Flag == FROM)
	{ // 儲存 OnLine 基本資料
		SIP_OnLine_Data[Cur].Dest.copy_from(&lpData->FromDest);
		SIP_OnLine_Data[Cur].S_Port = ntohs(lpData->FromDest.Raddr.sin_port);
		// SIP_OnLine_Data[Cur].Nc.copy_from(&lpData->FromNc);
		// memcpy(&SIP_OnLine_Data[Cur].Addr, &lpData->FAddr, sizeof(sockaddr));
		// strcpy(SIP_OnLine_Data[Cur].R_IP, lpData->FR_IP);
		// strcpy(SIP_OnLine_Data[Cur].V_IP, lpData->FV_IP);
		// SIP_OnLine_Data[Cur].Crypto_Type = lpData->From_Crypto_Type;
		strcpy(SIP_OnLine_Data[Cur].PhoneNo, lpData->From_Name);
	}
	else
	{ // 被叫端未註冊過, 首次被 INVITE(PC to Phone or Co-Server), 需將之存入 OnLine Buffer
		SIP_OnLine_Data[Cur].Dest.copy_from(&lpData->ToDest);
		SIP_OnLine_Data[Cur].S_Port = ntohs(lpData->ToDest.Raddr.sin_port);

		// SIP_OnLine_Data[Cur].Nc.copy_from(&lpData->ToNc);
		// Port = lpData->TAddr.sin_port;
		// SIP_OnLine_Data[Cur].S_Port = ((Port & 0xFF) << 8) + ((Port >> 8) & 0xFF);
		// memcpy(&SIP_OnLine_Data[Cur].Addr, &lpData->TAddr, sizeof(sockaddr));
		// strcpy(SIP_OnLine_Data[Cur].R_IP, lpData->TR_IP);
		// strcpy(SIP_OnLine_Data[Cur].V_IP, lpData->TV_IP);
		// SIP_OnLine_Data[Cur].Crypto_Type = lpData->To_Crypto_Type;
		strcpy(SIP_OnLine_Data[Cur].PhoneNo, lpData->To_Name);
	}
}

int CSIP_Server::Init_CDMS_Data()
{
	memset(&CDMS_Data, 0, sizeof(CDMS_Data));
	CDMS_Data_Num = 0;
	return 0;
}

int CSIP_Server::Check_CDMS_Data(const char *Account)
{
	int i, Cur;

	if (!Account)
		return -1;

	for (i = 0, Cur = 0; Cur < CDMS_DATA_SIZE; Cur++)
	{
		if (CDMS_Data_Num == 0)
		{
			Cur = CDMS_DATA_SIZE;
			break;
		}
		if (CDMS_Data[Cur].PhoneNo[0])
		{
			// Check From, To
			if (!strcmp(CDMS_Data[Cur].PhoneNo, Account))
				break;
			i++;
			if (i >= CDMS_Data_Num)
			{
				Cur = CDMS_DATA_SIZE;
				break;
			}
		}
	}
	if (Cur < CDMS_DATA_SIZE)
		return Cur;
	else
		return -1;
	return 0;
}

int CSIP_Server::Save_CDMS_Data(const CDMS_DATA *lpCdms)
{
	int Cur;

	if (!lpCdms)
		return -1;
	for (Cur = 0; Cur < CDMS_DATA_SIZE; Cur++)
	{
		if (!CDMS_Data[Cur].PhoneNo[0])
			break;
	}
	if (Cur < CDMS_DATA_SIZE)
	{
		memcpy(&CDMS_Data[Cur], lpCdms, sizeof(CDMS_Data[Cur]));
		CDMS_Data_Num++;
		return Cur;
	}
	else
		return -1;
}

int CSIP_Server::Clear_CDMS_Data(int Cur)
{
	if (Cur < 0 || Cur >= CDMS_DATA_SIZE)
		return -1;
	if (CDMS_Data[Cur].PhoneNo[0])
	{
		memset(&CDMS_Data[Cur], 0, sizeof(CDMS_Data[Cur]));
		CDMS_Data_Num--;
	}
	return 0;
}

int CSIP_Server::Check_MD5(SIP_MSG *lpData, int Online_Cur, bool Access_Flag)
{
	int i, Cur;
	char Password[300], Response[300];

	//--- If ID unCheck, Check it and Get Password ---
	// 1. Account exist
	if (SIP_OnLine_Data[Online_Cur].CDMS_OK == 0 || Access_Flag)
	{
		if ((Cur = Check_CDMS_Data(lpData->From_Name)) >= 0)
		{
			SIP_OnLine_Data[Online_Cur].CDMS_OK = 1;
			strcpy(SIP_OnLine_Data[Online_Cur].Mobile, CDMS_Data[Cur].Mobile);
			strcpy(SIP_OnLine_Data[Online_Cur].Name, CDMS_Data[Cur].Name);
			strcpy(SIP_OnLine_Data[Online_Cur].Password, CDMS_Data[Cur].Password);
			strcpy(Password, SIP_OnLine_Data[Online_Cur].Password);
			//Trunking GW Process
			Trunk_GW_Set(&SIP_OnLine_Data[Online_Cur].Trunk_GW, CDMS_Data[Cur].TrunkGW);
		}
		// Account don't exist and account's first character isn't within '0' ~ '9'
		else
			SIP_OnLine_Data[Online_Cur].CDMS_OK = -1;
	}
	else
		strcpy(Password, SIP_OnLine_Data[Online_Cur].Password);

	// 2. Authorization Process
	if (SIP_OnLine_Data[Online_Cur].CDMS_OK == 1)
	{
		i = (!strcmp(lpData->Command, "REGISTER")) ? Get_Position(lpData->Cmd_Str, "Authorization:") : Get_Position(lpData->Cmd_Str, "Proxy-Authorization:");
		if (i >= 0)
		{
			Get_Auth(&lpData->Cmd_Str[i], "algorithm=", lpData->Authorization_Algorithm);
			Get_Auth(&lpData->Cmd_Str[i], "nonce=", lpData->Authorization_Nonce);
			Get_Auth(&lpData->Cmd_Str[i], "realm=", lpData->Authorization_Realm);
			Get_Auth(&lpData->Cmd_Str[i], "response=", lpData->Authorization_Response);
			Get_Auth(&lpData->Cmd_Str[i], "uri=", lpData->Authorization_Uri);
			Get_Auth(&lpData->Cmd_Str[i], "username=", lpData->Authorization_Username);
		}
		if ((strlen(lpData->Authorization_Nonce) > 0) &&
			(strlen(lpData->Authorization_Realm) > 0) &&
			(strlen(lpData->Authorization_Response) > 0) &&
			(strlen(lpData->Authorization_Uri) > 0) &&
			(strlen(lpData->Authorization_Username) > 0) &&
			(strlen(Password) > 0))
		{
			if (!strcmp(lpData->Command, "REGISTER"))
			{
				if (Reg_Auth_Level == 2 || Reg_Auth_Level == 4)
					strcpy(lpData->Authorization_Nonce, SIP_OnLine_Data[Online_Cur].Nonce);
				MD5(lpData->Authorization_Algorithm, lpData->Command, lpData->Authorization_Username, lpData->Authorization_Uri, Password, lpData->Authorization_Realm, lpData->Authorization_Nonce, Response);
				SIP_OnLine_Data[Online_Cur].Auth_OK = TRUE;
				// SIP_OnLine_Data[Online_Cur].MD5_OK = (!strcmp(lpData->Authorization_Response, Response)) ? TRUE : FALSE;

			}
			else
			{
				if (Proxy_Auth_Level == 3)
					strcpy(lpData->Authorization_Nonce, SIP_OnLine_Data[Online_Cur].Proxy_Nonce);
				MD5(lpData->Authorization_Algorithm, lpData->Command, lpData->Authorization_Username, lpData->Authorization_Uri, Password, lpData->Authorization_Realm, lpData->Authorization_Nonce, Response);
				SIP_OnLine_Data[Online_Cur].Proxy_Auth_OK = TRUE;
				SIP_OnLine_Data[Online_Cur].Proxy_MD5_OK = (!strcmp(lpData->Authorization_Response, Response)) ? TRUE : FALSE;
			}
		}
	}
	return 0;
}

int CSIP_Server::Modify_Expires(SIP_MSG *lpData, int Step)
{
	int i, j, k;
	char S1[100], SS1[SIP_MSG_LEGNTH];

	if (0 < Step && Step < MIN_EXPIRES)
		Step = MIN_EXPIRES;
	else if (Step > MAX_EXPIRES)
		Step = MAX_EXPIRES;
	sprintf(S1, "%d", Step);
	strcpy(lpData->Expires, S1);
	if ((i = Get_Position(lpData->Cmd_Str, "Expires:")) > 0)
	{
		for (j = i; j < (int)strlen(lpData->Cmd_Str); j++)
			if ((lpData->Cmd_Str[j] >= 0x30) && (lpData->Cmd_Str[j] <= 0x39))
				break;
		for (k = j; k < (int)strlen(lpData->Cmd_Str); k++)
			if ((lpData->Cmd_Str[k] < 0x30) || (lpData->Cmd_Str[j] > 0x39))
				break;
		strncpy(SS1, lpData->Cmd_Str, j);
		SS1[j] = 0;
		strcat(SS1, S1);
		strcat(SS1, &lpData->Cmd_Str[k]);
		strcpy(lpData->Cmd_Str, SS1);
	}
	return 0;
}

int CSIP_Server::Update_Expire(SIP_MSG *lpData, int Online_Cur)
{
	if (SIP_OnLine_Data[Online_Cur].First_Flag)
		SIP_OnLine_Data[Online_Cur].Expires_Real = 0;
	// 使用 REGISTER 註冊
	if (!strcmp(lpData->Command, "REGISTER"))
	{
		SIP_OnLine_Data[Online_Cur].Expires_Client = atoi(lpData->Expires);
		SIP_OnLine_Data[Online_Cur].Expires_Counter = 0;
		SIP_OnLine_Data[Online_Cur].Expires_Limit = EXPIRES_LIMIT;
		// Register OK: Expire_Limit = Client * Expire_Weight
		if (SIP_OnLine_Data[Online_Cur].MD5_OK)
			SIP_OnLine_Data[Online_Cur].Expires_Limit = SIP_OnLine_Data[Online_Cur].Expires_Client * Expire_Limit_Weight;
	}
	else
	{
		SIP_OnLine_Data[Online_Cur].Expires_Counter = 0;
		if (!SIP_OnLine_Data[Online_Cur].Proxy_MD5_OK)
		{
			SIP_OnLine_Data[Online_Cur].Expires_Client = 0;
			SIP_OnLine_Data[Online_Cur].Expires_Limit = EXPIRES_LIMIT;
		}
	}
	return 0;
}

int CSIP_Server::Proc_Auth(SIP_MSG *lpSIP_Msg, int Command_Cur, int Online_Cur, const char *Command)
{
	char Algorithm[100], Realm[100], Nonce[100];

	Update_Command_By_Response(Command_Cur, lpSIP_Msg);
	strcpy(Command_Data[Command_Cur].Response_SIP_Msg.Command, Command);
	// Assign Auth Param
	sprintf(Algorithm, "MD5");
	sprintf(Realm, "%s", lpSIP_Msg->FromDest.Real_IP);
	sprintf(Nonce, "%dntut%dhsf%lu", rand() ^ 0xA756, rand(), JGetTickCount());
	if (!strcmp(Command, "401"))
	{ // Set Auth param assign a Nonce
		strcpy(Command_Data[Command_Cur].Response_SIP_Msg.WWW_Authenticate_Algorithm, Algorithm);
		strcpy(Command_Data[Command_Cur].Response_SIP_Msg.WWW_Authenticate_Realm, Realm);
		strcpy(Command_Data[Command_Cur].Response_SIP_Msg.WWW_Authenticate_Nonce, Nonce);
		// Save Nonce
		strcpy(SIP_OnLine_Data[Online_Cur].Nonce, Command_Data[Command_Cur].Response_SIP_Msg.WWW_Authenticate_Nonce);
	}
	else
	{ // Set Auth param assign a Nonce
		strcpy(Command_Data[Command_Cur].Response_SIP_Msg.Proxy_Authenticate_Algorithm, Algorithm);
		strcpy(Command_Data[Command_Cur].Response_SIP_Msg.Proxy_Authenticate_Realm, Realm);
		strcpy(Command_Data[Command_Cur].Response_SIP_Msg.Proxy_Authenticate_Nonce, Nonce);
		// Save Nonce
		strcpy(SIP_OnLine_Data[Online_Cur].Proxy_Nonce, Command_Data[Command_Cur].Response_SIP_Msg.Proxy_Authenticate_Nonce);
	}
	return 0;
}

int CSIP_Server::Clear_OnLine(SIP_MSG *lpSIP_Data)
{
	CRASH_DEBUG_COMMAND;
	int Cur;

	// 1. Search PhoneNo
	Cur = Check_OnLine(lpSIP_Data->From_Name);
	// 2. Clear OnLine Data
	if (Clear_OnLine_By_Cur(Cur) >= 0)
	{
		RETURN 0;
	}
	else
	{
		RETURN - 1;
	}
}

int CSIP_Server::Clear_OnLine_By_Cur(int Cur)
{
	if (Cur >= 0)
	{
		// Clear Online_Data
		SIP_OnLine_Data[Cur].Dest.clean_nc();
		SIP_OnLine_Data[Cur].Trunk_GW.clean_nc();
		SIP_OnLine_Data[Cur].reset();
		SIP_OnLine_Num--;
		return 0;
	}
	else
		return -1;
}

int CSIP_Server::Clean_All_OnLine_Data()
{
	CRASH_DEBUG_COMMAND;
	for (int Cur=0; Cur<SIP_ONLINE_SIZE; Cur++)
	{
		if (SIP_OnLine_Data[Cur].Flag)	Clear_OnLine_By_Cur(Cur);
	}
	RETURN 0;
}


// ====================================================================================
int CSIP_Server::Extract_Param(char *S1, const char *Head, char *Param)
{
	int i, j, Beg, End;

	// 1. Search "c=IN IP4" or "m=audio" Position
	Beg = End = 0;
	for (j = 0, i = 0; i < (int)(strlen(S1) - strlen(Head)); i++)
	{
		if ((S1[i] | 0x20) == (Head[0] | 0x20))
		{
			for (j = 0; j < (int)strlen(Head); j++)
				if ((S1[i + j] | 0x20) != (Head[j] | 0x20))
					break;
		}
		if (j >= (int)strlen(Head))
			break;
	}

	// 2. Locate Beg/End of Param
	if (i < (int)(strlen(S1) - strlen(Head)))
	{
		Beg = i + (int)strlen(Head);
		for (i = Beg; i < (int)strlen(S1); i++)
			if (S1[i] != ' ')
			{
				Beg = i;
				break;
			}
		for (i = Beg; i < (int)strlen(S1); i++)
			if ((S1[i] == 0x20) || (S1[i] == 0x0a) || (S1[i] == 0x0d))
			{
				End = i - 1;
				break;
			}
	}

	// 3. Extract Param
	if (End >= Beg)
	{
		for (i = Beg; i <= End; i++)
			if (((S1[i] < '0') || (S1[i] > '9')) && (S1[i] != '.'))
				break;
		if (i <= End)
			return -1;
		else
		{
			strncpy(Param, &S1[Beg], End - Beg + 1);
			Param[End - Beg + 1] = 0;
		}
	}
	else
		return -1;

	return 0;
}

int CSIP_Server::Modify_Param(char *S1, const char *Head, char *Param)
{
	char SS1[SIP_MSG_LEGNTH];
	int i, Beg, End;

	// 1. Search "c=IN IP4" or "m=audio" Position
	Beg = End = 0;
	if ((i = Get_Position(S1, Head)) > 0)
	{
		Beg = i + (int)strlen(Head);
		for (i = Beg; i < (int)strlen(S1); i++)
			if (S1[i] != ' ')
			{
				Beg = i;
				break;
			}
		for (i = Beg; i < (int)strlen(S1); i++)
			if ((S1[i] == 0x20) || (S1[i] == 0x0a) || (S1[i] == 0x0d))
			{
				End = i - 1;
				break;
			}
	}

	// 3. Replace Param
	if (End >= Beg)
	{
		for (i = Beg; i <= End; i++)
			if (((S1[i] < '0') || (S1[i] > '9')) && (S1[i] != '.') && (S1[i] != '-'))
				break; // '-' 乃針對 Golden-Net 而設計
		if (i <= End)
			return -1;
		else
		{
			strncpy(SS1, S1, Beg);
			SS1[Beg] = 0;
			strcat(SS1, Param);
			strcat(SS1, &S1[End + 1]);
			strcpy(S1, SS1);
		}
	}
	else
		return -1;

	return 0;
}
int CSIP_Server::Modify_SDP(SIP_MSG *lpData, char *IP, char *FPort, char *TPort)
{
	int Old_Len, i, j, k;
	char S1[SIP_MSG_LEGNTH], Buf[100], Port[16];

	Old_Len = strlen(lpData->SDP);
	i = Modify_Param(lpData->SDP, " IN IP4", IP);
	i = Modify_Param(lpData->SDP, "c=IN IP4", IP);
	// 補Quintum Gate有兩個"C=IN IP4" 的問題
	if ((k = Get_Position(lpData->SDP, "c=IN IP4")) > 0)
		i = Modify_Param(&lpData->SDP[k + strlen("c=IN IP4")], "c=IN IP4", IP);
	// 支援新舊版，INVITE時TPort都不為"", 表示為新版取TPort, 其他情形皆取FPort
	if (!strcmp(lpData->Command, "INVITE") && strcmp(TPort, ""))
		strcpy(Port, TPort);
	else
		strcpy(Port, FPort);
	i = Modify_Param(lpData->SDP, "m=audio", Port);
	if (i < 0)
		i = Modify_Param(lpData->SDP, "m=image", Port);
	Old_Len -= strlen(lpData->SDP);
	i = atoi(lpData->Content_Length) - Old_Len;
	sprintf(lpData->Content_Length, "%d", i);
	sprintf(Buf, "Content-Length: %d", i);

	if ((i = Get_Position(lpData->Cmd_Str, "Content-Length:")) > 0)
	{
		for (j = i; j < (int)strlen(lpData->Cmd_Str); j++)
			if ((lpData->Cmd_Str[j] == 0x0a) || (lpData->Cmd_Str[j] == 0x0d))
				break;
		strncpy(S1, lpData->Cmd_Str, i);
		S1[i] = 0;
		strcat(S1, Buf);
		strcat(S1, &lpData->Cmd_Str[j]);
		strcpy(lpData->Cmd_Str, S1);
	}

	if ((i = Get_Position(lpData->Cmd_Str, "v=")) > 0)
	{
		strncpy(S1, lpData->Cmd_Str, i);
		S1[i] = 0;
		strcat(S1, lpData->SDP);
		strcpy(lpData->Cmd_Str, S1);
	}

	return 0;
}

int CSIP_Server::Send_SIP_Command(const char *Command, SIP_MSG *lpSIP_Data, int Via_Flag, int Send_Flag)
{
	if (!Command || !lpSIP_Data)
	{
		DTrace_FatalError("CNetIF instance is invalid");
		RETURN - 1;
	}
	if (!m_lpNetIF)
	{
		DTrace_FatalError("CNetIF instance is invalid");
		RETURN - 2;
	}

	int i, j, k, l, n, Len, Msg_Len;
	char S1[SIP_MSG_LEGNTH], Buf[512], Buf1[100], SS2[640], TPort[16], URL[256];
	// WSADATA			wsadata;
	SIP_MSG Temp_SIP_Msg;
	sockaddr_mix Server_Addr;
	CRYPTO_TYPE Crypto_Type;

	if (Send_Flag == FROM)
	{
		lpSIP_Data->pDest = &lpSIP_Data->FromDest;
		// lpSIP_Data->pNc = &lpSIP_Data->FromNc;
		// lpSIP_Data->pAddr = &lpSIP_Data->FAddr;
		// Crypto_Type = lpSIP_Data->From_Crypto_Type;
	}
	else if (Send_Flag == TO)
	{
		lpSIP_Data->pDest = &lpSIP_Data->ToDest;
		// lpSIP_Data->pNc = &lpSIP_Data->ToNc;
		// lpSIP_Data->pAddr = &lpSIP_Data->TAddr;
		// Crypto_Type = lpSIP_Data->To_Crypto_Type;
	}
	else
		lpSIP_Data->pDest = NULL;

	// 1. Modify Command Line
	if ((Command[0] >= '1') && (Command[0] <= '6'))
	{
		Get_Response_Code(Buf1, atoi(Command));
		sprintf(Buf, "SIP/2.0 %s\r\n", Buf1);
	}
	else
	{ // Get Server Port
		// WSAStartup(0x202, &wsadata);
		// Len = sizeof(Server_Addr);
		NetGetSockName(lpSIP_Data->ToDest.pNc->mixsocket, &Server_Addr);
		for (n = 0; n < Server_Port_Num; n++)
			if (SIP_Server_Port[n] == ntohs(Server_Addr.sin_port))
				break;
		sprintf(TPort, "%d", (n < Server_Port_Num) ? ntohs(Server_Addr.sin_port) : SIP_SERVER_PORT);
		// Update URL
		strcpy(URL, lpSIP_Data->Command_URL);
		if (atoi(lpSIP_Data->Command_Port) > 0)
		{ // URL 內涵port需將其去除
			for (n = 0; n < (int)strlen(lpSIP_Data->Command_URL); n++)
				if (lpSIP_Data->Command_URL[n] == ':')
				{
					strncpy(URL, lpSIP_Data->Command_URL, n - 1);
					URL[n] = 0;
					break;
				}
			Jstrncat(URL, TPort, sizeof(URL));
		}
		// Update Command Line
		if (!strcmp(lpSIP_Data->Command_Name, ""))
			sprintf(Buf, "%s sip:%s SIP/2.0\r\n", Command, URL);
		else
			sprintf(Buf, "%s sip:%s@%s SIP/2.0\r\n", Command, lpSIP_Data->Command_Name, URL);
	}
	strcpy(S1, lpSIP_Data->Cmd_Str);
	if ((j = Get_NextLine(S1)) > 0)
		strcpy(S1, Buf);
	strcat(S1, &lpSIP_Data->Cmd_Str[j]);

	// != 183/200/INVITE, 去除 SDP
	if (atoi(lpSIP_Data->Content_Length) > 0) //strlen(lpSIP_Data->SDP)>0)
	{										  // Remove SDP
		if (strcmp(Command, "183") && strcmp(Command, "200") && strcmp(Command, "INVITE"))
		{
			if ((i = Get_Position(S1, "v=")) > 0)
				S1[i] = 0;
			// Remove Content-Type:
			if ((i = Get_Position(S1, "Content-Type:")) > 0)
			{
				if ((k = Get_NextLine(&S1[i])) > 0)
				{
					k += i;
					Deletion(S1, i, k - 1);
				}
			}
			// Set Content-Length: 0
			if ((i = Get_Position(S1, "Content-Length:")) > 0)
			{
				for (j = i; j < (int)strlen(S1); j++)
					if (S1[j] == 0x20)
						break;
				for (k = j; k < (int)strlen(S1); k++)
					if ((S1[k] >= '0') && (S1[k] <= '9'))
						break;
				for (l = k; l < (int)strlen(S1); l++)
					if ((S1[l] < '0') || (S1[l] > '9'))
						break;
				Deletion(S1, k, l - 1);
				Insertion(S1, k, "0");
			}
		}
	}

	// INVITE, Modify Contact's URL for "ACK", Add Record-Route: is another solution
	if ((i = Get_Position(S1, "Contact:")) > 0)
	{
		for (j = i; j < (int)strlen(S1); j++)
			if (S1[j] == '@')
			{
				j++;
				break;
			}
		for (k = j; k < (int)strlen(S1); k++)
			if ((S1[k] == '>') || (S1[k] == 0x0a) || (S1[k] == 0x0d))
			{
				break;
			}

		Deletion(S1, j, k - 1);
		Insertion(S1, j, Local_IP);
	}

	// INVITE 內之 Auth 於 Response 時, 需去除之
	if (((i = Get_Position(S1, "Authorization:")) > 0) && (Command[0] >= 0x30) && (Command[0] <= 0x39))
	{
		if ((k = Get_NextLine(&S1[i])) > 0)
		{
			k += i;
			Deletion(S1, i, k - 1);
		}
	}
	else if (((i = Get_Position(S1, "Proxy-Authorization:")) > 0) && !(Command[0] >= 0x30 && Command[0] <= 0x39))
	{
		if ((k = Get_NextLine(&S1[i])) > 0)
		{
			k += i;
			Deletion(S1, i, k - 1);
		}
	}

	// Add or Modify Record-Route:
	if ((Command[0] >= 0x30) && (Command[0] <= 0x39))
	{
		Delete_Record_Route(S1);
	} //   不可清除 Record-Route: 以免 ACK 回錯位�

	else
	{
		Delete_Record_Route(S1); // 避免 Crystal Media 犯錯
		Add_Record_Route(S1, TPort);
	}

	// 避免Crystal Media 犯錯
	//if ((Command[0]>='0')&&(Command[0]<='9')&&(strlen(lpSIP_Data->ViaBuf)>0) ) 	{  Recover_Via(S1, lpSIP_Data->ViaBuf);	}
	// 3. Via Process
	if (Via_Flag == ADD_VIA)
	{ // if PC2Phone,  Add ;rport=Port;received=LIP after Existed Via
		Add_Via111(S1, TPort, lpSIP_Data->Cancel_Branch);
	}

	else if (Via_Flag == DELETE_VIA)
	{
		Delete_Via111(S1, lpSIP_Data->ViaBuf);
	}

	// Forward to Gateway
	if (!strcmp(Command, "302"))
	{
		if ((i = Get_Position(S1, "Contact:")) < 0)
		{
			if ((i = Get_Position(S1, "CSeq:")) > 0)
			{
				if (strcmp(lpSIP_Data->Contact_Port, ""))
					sprintf(SS2, "Contact: <sip:%s@%s:%s>\r\n", lpSIP_Data->Contact_Name, Local_IP, lpSIP_Data->Contact_Port);
				else
					sprintf(SS2, "Contact: <sip:%s@%s>\r\n", lpSIP_Data->Contact_Name, Local_IP);
				Insertion(S1, i, SS2);
			}
		}
		else
			Edit_URL(S1, "Contact:", Local_IP);
	}

	// 3. Send to Remote
	if (lpSIP_Data->pDest->is_used())
	{
		memcpy(&Temp_SIP_Msg, lpSIP_Data, sizeof(SIP_MSG));
		strcpy(Temp_SIP_Msg.Command, Command);
		Write_Log("sendto", S1, &lpSIP_Data->pDest->Raddr, &Temp_SIP_Msg);
		// Msg Crypt Process
		if (Crypto_Type == CT_SIPCrypto)
			Msg_Len = sip_encrypt(S1, (unsigned char *)S1);
		else if (Crypto_Type == CT_Walkersun1)
			Msg_Len = walkersun_sip_encrypt(S1, (unsigned char *)S1);
		else
			Msg_Len = strlen(S1);

		m_lpNetIF->NetNcSend(lpSIP_Data->pDest->pNc, S1, Msg_Len, &lpSIP_Data->pDest->Raddr);
	}
}

int CSIP_Server::Delete_Via(char *S1)
{
	int i, k;

	if ((i = Get_Position(S1, "Via:")) > 0)
	{
		if ((k = Get_NextLine(&S1[i])) > 0)
		{
			k += i;
			Deletion(S1, i, k - 1);
		}
	}

	return 0;
}

int CSIP_Server::Add_Via(char *S1)
{
	int i;
	char LPort[100], Buf[256];

	sprintf(LPort, "%d", SIP_SERVER_PORT);
	sprintf(Buf, "Via: SIP/2.0/UDP %s:%s;branch=z9hG4bK%d%d\r\n", Local_IP, LPort, rand(), rand());

	if ((i = Get_Position(S1, "Via:")) <= 0)
		i = Get_NextLine(S1);
	if (i >= 0)
		Insertion(S1, i, Buf);
	else
		return -1;
	return 0;
}

int CSIP_Server::Add_Record_Route(char *S1, char *Port)
{
	char SS2[200];
	int i;

	if ((i = Get_Position(S1, "Record-Route:")) > 0)
	{
		sprintf(SS2, "Record-Route: <sip:%s:%s;lr>\r\n", Local_IP, Port);
		Insertion(S1, i, SS2);
	}

	else if ((i = Get_Position(S1, "Contact:")) > 0)
	{
		sprintf(SS2, "Record-Route: <sip:%s:%s;lr>\r\n", Local_IP, Port);
		Insertion(S1, i, SS2);
	}

	return 0;
}

int CSIP_Server::Delete_Record_Route(char *S1)
{
	int i, k;

	if ((i = Get_Position(S1, "Record-Route:")) > 0)
	{
		if ((k = Get_NextLine(&S1[i])) > 0)
		{
			k += i;
			Deletion(S1, i, k - 1);
		}
	}
	return 0;
}

// -----------------------------------------------------------------
// ----------   SIP Message Process Function -----------------------
// -----------------------------------------------------------------
int CSIP_Server::SIP_Message_Analyzer(NetConnect_t *nc, sockaddr_mix *lpAddr, char *S1, SIP_MSG *Data, CRYPTO_TYPE Crypto_Type)
{
	CRASH_DEBUG_COMMAND;
	int i, Cur, Cmd = -1, Error_Code;
	char S2[SIP_MSG_LEGNTH];

	// 0. Initialize
	Data->reset();
	strcpy(Data->Cmd_Str, S1);
	// memset(Data, 0, sizeof(SIP_MSG));
	// Data->ViaBuf[0] = 0;
	// Data->Command[0] = Data->Command_Msg[0] = 0;
	// Data->Via_Received[0] = Data->SDP[0] = Data->Expires[0] = 0;
	// Data->Contact_URL[0] = 0;
	// Data->FromDest.reset();
	// Data->ToDest.reset();
	// Data->FR_IP[0] = Data->FV_IP[0] = Data->TR_IP[0] = Data->TV_IP[0] = 0;
	// Data->WWW_Authenticate_Algorithm[0] = Data->WWW_Authenticate_Nonce[0] = Data->WWW_Authenticate_Realm[0] = 0;
	// Data->Authorization_Algorithm[0] = Data->Authorization_Nonce[0] = Data->Authorization_Realm[0] = Data->Authorization_Response[0] = Data->Authorization_Uri[0] = Data->Authorization_Username[0] = 0;
	// Data->Authorization_Flag = FALSE;

	CRASH_DEBUG_COMMAND;
	// 1. Get SIP_Command
	for (i = 0; i < SIP_COMMAND_NO; i++)
	{
		if (!strncmp(&SIP_COMMAND[i][0], S1, strlen(&SIP_COMMAND[i][0])))
		{
			Cmd = i;
			break;
		}
	}

	// 2. Get SIP Parameter
	if (Cmd < SIP_COMMAND_NO)
	{ // 2.1 Get Response_Code/Request Command
		if (Cmd == SIP_COMMAND_NO - 1)
		{
			sscanf(S1, "%s %s %s", S2, Data->Command, S2);
			Data->ViaBuf[0] = 0;
		}
		else
		{
			strcpy(Data->Command, &SIP_COMMAND[i][0]);
			Get_Via(S1, Data->ViaBuf);
		}

		// 2.2 Get From/To/Call-ID/Contact
		Cur = Filter_String(S1, Data->Command, Data->Command_Name, Data->Command_URL, Data->Command_Port, Data->Command_Tag, Data->Command_Branch, Data->Command_expires, &Data->Command_Flag);		//,  &Name_Locate[0]);
		Cur = Filter_String(S1, "From:", Data->From_Name, Data->From_URL, Data->From_Port, Data->From_Tag, Data->From_Branch, Data->From_expires, &Data->From_Flag);								//,     &Name_Locate[0]);
		Cur = Filter_String(S1, "To:", Data->To_Name, Data->To_URL, Data->To_Port, Data->To_Tag, Data->To_Branch, Data->To_expires, &Data->To_Flag);												//  ,     &Name_Locate[2]);
		Cur = Filter_String(S1, "Via:", Data->Via_Name, Data->Via_URL, Data->Via_Port, Data->Via_Tag, Data->Via_Branch, Data->Via_expires, &Data->Via_Flag);										// ,     &Name_Locate[4]);
		Cur = Filter_String(S1, "Contact:", Data->Contact_Name, Data->Contact_URL, Data->Contact_Port, Data->Contact_Tag, Data->Contact_Branch, Data->Contact_expires, &Data->Contact_Flag);		// , &Name_Locate[0]);
		Cur = Filter_String(S1, "Call-ID:", Data->CallerID_Name, Data->CallerID_URL, Data->CallerID_Port, Data->CallerID_Tag, Data->CallerID_Branch, Data->CallerID_expires, &Data->CallerID_Flag); // , &Name_Locate[0]);
		Cur = Filter_String(S1, "Refer-To:", Data->ReferTo_Name, Data->ReferTo_URL, Data->ReferTo_Port, Data->ReferTo_Tag, Data->ReferTo_Branch, Data->ReferTo_expires, &Data->ReferTo_Flag);		// ,&Name_Locate[0]);

		// 2.3 CSeq/Expires/Content-Length/Max-Forwards
		Get_Field(S1, "Call-ID:", Data->CallerID);
		Get_Field(S1, "Expires:", Data->Expires);
		if (atoi(Data->Expires) == 0 && strcmp(Data->Expires, "0"))
			strcpy(Data->Expires, Data->Contact_expires); // in some situations, there is no Expries header, however the expiires is set within Contact header
		Get_Field(S1, "Content-Length:", Data->Content_Length);
		if (atoi(Data->Content_Length) > 0)
			Get_SDP(S1, Data->SDP);
		Get_Field(S1, "User-Agent:", Data->User_Agent);

		// Debug用
		if (!strcmp(Data->Command, "200"))
			i = i;

		// 2.4 Socket/Addr
		if (Cmd != (SIP_COMMAND_NO - 1))
		{
			CRASH_DEBUG_COMMAND;
			Data->FromDest.pack(nc, lpAddr, NULL, Data->Via_URL, Crypto_Type);
			CRASH_DEBUG_COMMAND;
			// Data->FromNc.copy_from(nc);
			// memcpy(&Data->FAddr, lpAddr, sizeof(Data->FAddr));
			// strcpy(Data->FV_IP, Data->Via_URL);
			// strcpy(Data->FR_IP, inet_ntoa(lpAddr->sin_addr));
			// Data->From_Crypto_Type = Crypto_Type;

			//	Check SIP Syntax
			if ((Error_Code = SIP_Syntax_Filter(Data)) < 0)
			{
				RETURN Error_Code;
			}
			//Partial dial-No.
			Transform_PhoneNo(Data, TRUE);
			//Save_OnLine(Data, FROM);
			if (strcmp(&SIP_COMMAND[Cmd][0], "REGISTER"))
				Check_Called_Party(Data);
			// if (!strcmp(&SIP_COMMAND[Cmd][0], "INVITE"))  Check_Relay_Monitor(Data); // 監聽或啟動 RTP_Relay
			Transform_PhoneNo(Data, FALSE);
		}
		else
		{
			Data->ToDest.pack(nc, lpAddr, NULL, Data->Contact_URL, Crypto_Type);
			// Data->ToNc.copy_from(nc);
			// memcpy(&Data->TAddr, lpAddr, sizeof(Data->TAddr));
			// strcpy(Data->TV_IP, Data->Contact_URL);
			// strcpy(Data->TR_IP, inet_ntoa(lpAddr->sin_addr));
			// Data->To_Crypto_Type = Crypto_Type;

			//Partial dial-No.
			Transform_PhoneNo(Data, TRUE);
			//Save_OnLine(Data, TO);
			Check_Calling_Party(Data);
			Transform_PhoneNo(Data, FALSE);
		}

		RETURN 0;
	}
	else
	{
		RETURN - 1;
	}
}

int CSIP_Server::Transform_PhoneNo(SIP_MSG *lpSIP_Msg, bool Before_Flag)
{
	int Invite_Cur;

	// Save Invite
	if (!strcmp(lpSIP_Msg->Command, "INVITE"))
	{
		if ((Invite_Cur = Check_Invite(lpSIP_Msg)) < 0)
			Invite_Proc(FROM, lpSIP_Msg, SIP_READY);
		else if (!Before_Flag)
			Save_Invite(lpSIP_Msg); // Set Param (TR_IP, TV_IP...)
	}
	// Transform: 回傳值為0表示Transform PhoneNo,-1則否
	if (ReplaceCode_Transform_PhoneNo(lpSIP_Msg, Before_Flag) >= 0)
	{
	}
	//	if (Partial_Transform_PhoneNo(lpSIP_Msg,Before_Flag)>=0)		{}
	if (InBound_Transform_PhoneNo(lpSIP_Msg, Before_Flag) >= 0)
	{
	}
	else if (OutBound_Transform_PhoneNo(lpSIP_Msg, Before_Flag) >= 0)
	{
	}

	return 0;
}

int CSIP_Server::Partial_Transform_PhoneNo(SIP_MSG *lpSIP_Msg, bool Before_Flag)
{
	char Temp_PhoneNo[50];
	int Invite_Cur;
	bool Transform_Flag;
	SIP_INVITE_DATA *lpInvite_Data;

	if ((Invite_Cur = Check_Invite(lpSIP_Msg)) >= 0)
	{
		lpInvite_Data = &SIP_Invite_Data[Invite_Cur];
		Transform_Flag = FALSE;
		// Command Flow: Caller --> Callee
		if (Partial_Dial_Filter(lpSIP_Msg))
		{
			switch (Check_Flow_State(lpSIP_Msg, TRUE)) // Recvfrom
			{
			case 0: // Request and -->
				if (Before_Flag)
				{
					strcpy(Temp_PhoneNo, lpSIP_Msg->To_Name); //先將To_Name暫存
					strcpy(lpSIP_Msg->To_Name, lpInvite_Data->Complete_PhoneNo);
					strcpy(lpSIP_Msg->Command_Name, lpInvite_Data->Complete_PhoneNo);
					Edit_Name(lpSIP_Msg->Cmd_Str, lpSIP_Msg->Command, lpSIP_Msg->To_Name);
					Edit_Name(lpSIP_Msg->Cmd_Str, "To:", lpSIP_Msg->To_Name);
					Transform_Flag = TRUE;
				}
				break;
			case 1: // Request and <--
				if (!Before_Flag)
				{
					Edit_Name(lpSIP_Msg->Cmd_Str, "From:", lpInvite_Data->Partial_PhoneNo);
					Transform_Flag = TRUE;
				}
				break;
			case 2: // Response and <--
				if (!Before_Flag)
				{
					Edit_Name(lpSIP_Msg->Cmd_Str, "To:", lpInvite_Data->Partial_PhoneNo);
					if (Get_Position(lpSIP_Msg->Cmd_Str, "Contact:") >= 0)
						Edit_Name(lpSIP_Msg->Cmd_Str, "Contact:", lpInvite_Data->Partial_PhoneNo);
					Transform_Flag = TRUE;
				}
				break;
			case 3: // Response and -->
				if (Before_Flag)
				{
					strcpy(lpSIP_Msg->From_Name, lpInvite_Data->Complete_PhoneNo);
					Edit_Name(lpSIP_Msg->Cmd_Str, "From:", lpInvite_Data->Complete_PhoneNo);
					if (Get_Position(lpSIP_Msg->Cmd_Str, "Contact:") >= 0)
						Edit_Name(lpSIP_Msg->Cmd_Str, "Contact:", lpInvite_Data->Complete_PhoneNo);
					Transform_Flag = TRUE;
				}
				break;
			}
		}
		else
			Transform_Flag = FALSE;
	}
	if (Transform_Flag == TRUE)
		return 0;
	else
		return -1;
}

bool CSIP_Server::Partial_Dial_Filter(SIP_MSG *lpSIP_Msg)
{
	bool Filter_Flag;
	char *Start_Cur, PhoneNo[50], From_Name[50];
	int Len, Invite_Cur, Type;

	if ((Invite_Cur = Check_Invite(lpSIP_Msg)) >= 0)
	{
		if (strlen(SIP_Invite_Data[Invite_Cur].Partial_PhoneNo) <= 0)
		{
			if (!strcmp(lpSIP_Msg->Command, "INVITE"))
			{
				// Partial Dial-No.
				if (strlen(lpSIP_Msg->From_Name) >= 9 &&												  //最短為9碼區
					(strlen(lpSIP_Msg->To_Name) > EXTENSION_NO_LENGTH && strlen(lpSIP_Msg->To_Name) < 9)) //若EXTENSION_NO_LENGTH=4,則To_Name至少5碼
				{
					// Get From Name
					if ((Start_Cur = strchr(lpSIP_Msg->From_Name, '-')) != NULL)
					{
						strncpy(From_Name, lpSIP_Msg->From_Name, Start_Cur - lpSIP_Msg->From_Name);
						From_Name[Start_Cur - lpSIP_Msg->From_Name] = 0;
					}
					else
						strcpy(From_Name, lpSIP_Msg->From_Name);

					// From_Name屬於10碼區: (02),(04); To_Name 5~8碼
					if ((From_Name[0] == '0' && (From_Name[1] == '2' || From_Name[1] == '4')) && strlen(From_Name) == 10 && strlen(lpSIP_Msg->To_Name) <= 8)
						Type = 1;
					// From_Name屬於9碼區: (03), (05), (06), (07), (08); To_Name 5~7碼
					else if ((From_Name[0] == '0' && (From_Name[1] >= '1' && From_Name[1] <= '8')) && strlen(From_Name) == 9 && strlen(lpSIP_Msg->To_Name) <= 7)
						Type = 2;
					// From_Name屬於大哥大號碼: 09XX...; To_Name 5~8碼
					else if ((From_Name[0] == '0' && From_Name[1] == '9') && strlen(From_Name) == 10 && strlen(lpSIP_Msg->To_Name) <= 8)
						Type = 3;
					// From_Name為8610, 8660, etc..開頭; To_Name 5~8碼
					else if ((From_Name[0] >= '1' && From_Name[0] <= '9') && strlen(From_Name) == 10 && strlen(lpSIP_Msg->To_Name) <= 8)
						Type = 4;
					// 其他
					else
						Type = 9;
					if (Type != 9 && Start_Cur != NULL)
						Type += 4;

					switch (Type)
					{
					case 1:
					case 2:
					case 3:
					case 4:
						Filter_Flag = TRUE;
						// Get Compele PhoneNo
						Len = strlen(From_Name) - strlen(lpSIP_Msg->To_Name);
						strncpy(PhoneNo, From_Name, Len);
						PhoneNo[Len] = 0;
						strcat(PhoneNo, lpSIP_Msg->To_Name);
						break;
					case 5:
					case 6:
					case 7:
					case 8:
						Filter_Flag = TRUE;
						// 若To_Name是四碼則表示撥分機，未來可擴通至5碼
						if (strlen(lpSIP_Msg->To_Name) == 4)
							Start_Cur = &lpSIP_Msg->From_Name[strlen(lpSIP_Msg->From_Name)];
						Len = (Start_Cur - lpSIP_Msg->From_Name) - strlen(lpSIP_Msg->To_Name);
						strncpy(PhoneNo, lpSIP_Msg->From_Name, Len);
						PhoneNo[Len] = 0;
						strcat(PhoneNo, lpSIP_Msg->To_Name);
						break;
					case 9:
						Filter_Flag = FALSE;
						break;
					}
				}
				// Complete Dial-No.
				else
					Filter_Flag = FALSE;

				if (Filter_Flag == TRUE)
				{ // Save Partial_PhoneNo
					strcpy(SIP_Invite_Data[Invite_Cur].Partial_PhoneNo, lpSIP_Msg->To_Name);
					strcpy(SIP_Invite_Data[Invite_Cur].Complete_PhoneNo, PhoneNo);
				}
			}
			else
				Filter_Flag = FALSE;
		}
		else
			Filter_Flag = TRUE;
	}
	else
		Filter_Flag = FALSE;

	return Filter_Flag;
}

int CSIP_Server::OutBound_Transform_PhoneNo(SIP_MSG *lpSIP_Msg, bool Before_Flag)
{
	char Buf[100];
	int i, Cur, Invite_Cur;
	bool Transform_Flag;
	SIP_INVITE_DATA *lpInvite_Data;

	if (strlen(Outbound_PhoneNo) <= 0)
		return -1;

	if ((Invite_Cur = Check_Invite(lpSIP_Msg)) >= 0)
	{
		lpInvite_Data = &SIP_Invite_Data[Invite_Cur];
		Transform_Flag = FALSE;
		// Command Flow: Caller --> Callee
		if (OutBound_Filter(lpSIP_Msg))
		{
			switch (Check_Flow_State(lpSIP_Msg, TRUE)) // Recvfrom
			{
			case 0: // Request and -->
				if (!Before_Flag)
				{ // 補上總機號碼 0227712171-2178
					Add_Name(lpSIP_Msg->Cmd_Str, "Contact:", Outbound_PhoneNo);
					Add_Name(lpSIP_Msg->Cmd_Str, "From:", Outbound_PhoneNo);
					Transform_Flag = TRUE;
				}
				break;
			case 1: // Request and <--
				if (Before_Flag)
				{ // 去除To_Name之總機號碼
					for (i = 0; i < (int)strlen(lpSIP_Msg->To_Name); i++)
						if (lpSIP_Msg->To_Name[i] == '-')
							break;
					if (i < (int)strlen(lpSIP_Msg->To_Name))
					{
						strcpy(Buf, &lpSIP_Msg->To_Name[i + 1]);
						Delete_Name(lpSIP_Msg->Cmd_Str, lpSIP_Msg->Command, Outbound_PhoneNo);
						Delete_Name(lpSIP_Msg->Cmd_Str, "To:", Outbound_PhoneNo);
						strcpy(lpSIP_Msg->Command_Name, Buf);
						strcpy(lpSIP_Msg->To_Name, Buf);
					}
					Transform_Flag = TRUE;
				}
				break;
			case 2: // Response and <--
				if (Before_Flag)
				{ // PC to Phone: 去除 總機號碼 0227712171-2178
					for (i = 0; i < (int)strlen(lpSIP_Msg->From_Name); i++)
						if (lpSIP_Msg->From_Name[i] == '-')
							break;
					if (i < (int)strlen(lpSIP_Msg->From_Name))
					{
						Delete_Name(lpSIP_Msg->Cmd_Str, "Contact:", Outbound_PhoneNo);
						Delete_Name(lpSIP_Msg->Cmd_Str, "From:", Outbound_PhoneNo);
						Cur = Filter_String(lpSIP_Msg->Cmd_Str, "From:", lpSIP_Msg->From_Name, lpSIP_Msg->From_URL, lpSIP_Msg->From_Port, lpSIP_Msg->From_Tag, lpSIP_Msg->From_Branch, lpSIP_Msg->From_expires, &lpSIP_Msg->From_Flag); //,    &Name_Locate[0]);
					}
					Transform_Flag = TRUE;
				}
				break;
			case 3: // Response and -->
				if (!Before_Flag)
				{ // 補上總機號碼 0227712171-2178
					Add_Name(lpSIP_Msg->Cmd_Str, "Contact:", Outbound_PhoneNo);
					Add_Name(lpSIP_Msg->Cmd_Str, "To:", Outbound_PhoneNo);
					Transform_Flag = TRUE;
				}
				break;
			}
		}
		else
			Transform_Flag = FALSE;
	}
	if (Transform_Flag == TRUE)
		return 0;
	else
		return -1;
}

bool CSIP_Server::OutBound_Filter(SIP_MSG *lpSIP_Msg)
{
	bool Filter_Flag;
	int Invite_Cur;

	if (strlen(Outbound_PhoneNo) <= 0)
		return FALSE;

	if ((Invite_Cur = Check_Invite(lpSIP_Msg)) >= 0)
	{
		if (!SIP_Invite_Data[Invite_Cur].OutBound_Flag)
		{
			if (!strcmp(lpSIP_Msg->Command, "INVITE"))
			{
				if (((lpSIP_Msg->To_Name[0] == '1' && strlen(lpSIP_Msg->To_Name) == 3) ||				   // 3碼1開頭的號碼
					 (lpSIP_Msg->To_Name[0] != '0' && strlen(lpSIP_Msg->To_Name) > EXTENSION_NO_LENGTH) || // 5碼以上不為0開頭的號碼
					 (lpSIP_Msg->To_Name[0] == '0' && lpSIP_Msg->To_Name[1] == '0') ||					   // 國際碼
					 (lpSIP_Msg->To_Name[0] == '0' && lpSIP_Msg->To_Name[1] != '0')						   // 0開頭的號碼
					 ) &&
					strlen(lpSIP_Msg->From_Name) <= EXTENSION_NO_LENGTH)
				{
					SIP_Invite_Data[Invite_Cur].OutBound_Flag = TRUE;
					Filter_Flag = TRUE;
				}
				else
					Filter_Flag = FALSE;
			}
			else
				Filter_Flag = FALSE;
		}
		else
			Filter_Flag = TRUE;
	}
	else
		Filter_Flag = FALSE;

	return Filter_Flag;
}

int CSIP_Server::InBound_Transform_PhoneNo(SIP_MSG *lpSIP_Msg, bool Before_Flag)
{
	int i, Cur, Invite_Cur;
	bool Transform_Flag;
	SIP_INVITE_DATA *lpInvite_Data;

	if (strlen(Outbound_PhoneNo) <= 0)
		return -1;

	if ((Invite_Cur = Check_Invite(lpSIP_Msg)) >= 0)
	{
		lpInvite_Data = &SIP_Invite_Data[Invite_Cur];
		Transform_Flag = FALSE;
		// Command Flow: Caller --> Callee
		if (InBound_Filter(lpSIP_Msg))
		{
			switch (Check_Flow_State(lpSIP_Msg, TRUE)) // Recvfrom
			{
			case 0: // Request and -->
				if (Before_Flag)
				{ // 去除To_Name之總機號碼
					for (i = 0; i < (int)strlen(lpSIP_Msg->To_Name); i++)
						if (lpSIP_Msg->To_Name[i] == '-')
							break;
					if (i < (int)strlen(lpSIP_Msg->To_Name))
					{
						Delete_Name(lpSIP_Msg->Cmd_Str, lpSIP_Msg->Command, Outbound_PhoneNo);
						Delete_Name(lpSIP_Msg->Cmd_Str, "To:", Outbound_PhoneNo);
						Cur = Filter_String(lpSIP_Msg->Cmd_Str, "To:", lpSIP_Msg->To_Name, lpSIP_Msg->To_URL, lpSIP_Msg->To_Port, lpSIP_Msg->To_Tag, lpSIP_Msg->To_Branch, lpSIP_Msg->To_expires, &lpSIP_Msg->To_Flag); //,    &Name_Locate[0]);
						strcpy(lpSIP_Msg->Command_Name, lpSIP_Msg->To_Name);
					}
					Transform_Flag = TRUE;
				}
				break;
			case 1: // Request and <--
				if (!Before_Flag)
				{ // 補上總機號碼
					Add_Name(lpSIP_Msg->Cmd_Str, "From:", Outbound_PhoneNo);
					Transform_Flag = TRUE;
				}
				break;
			case 2: // Response and <--
				if (!Before_Flag)
				{ // 補上總機號碼
					Add_Name(lpSIP_Msg->Cmd_Str, "To:", Outbound_PhoneNo);
					if (Get_Position(lpSIP_Msg->Cmd_Str, "Contact:") > 0)
						Add_Name(lpSIP_Msg->Cmd_Str, "Contact:", Outbound_PhoneNo);
					Transform_Flag = TRUE;
				}
				break;
			case 3: // Response and -->
				if (Before_Flag)
				{ // 去除總機號碼
					for (i = 0; i < (int)strlen(lpSIP_Msg->From_Name); i++)
						if (lpSIP_Msg->From_Name[i] == '-')
							break;
					if ((strlen(Outbound_PhoneNo) > 0) && (i < (int)strlen(lpSIP_Msg->From_Name)))
					{
						Delete_Name(lpSIP_Msg->Cmd_Str, "Contact:", Outbound_PhoneNo);
						Delete_Name(lpSIP_Msg->Cmd_Str, "From:", Outbound_PhoneNo);
						Cur = Filter_String(lpSIP_Msg->Cmd_Str, "From:", lpSIP_Msg->From_Name, lpSIP_Msg->From_URL, lpSIP_Msg->From_Port, lpSIP_Msg->From_Tag, lpSIP_Msg->From_Branch, lpSIP_Msg->From_expires, &lpSIP_Msg->From_Flag); //,&Name_Locate[0]);
					}
					Transform_Flag = TRUE;
				}
				break;
			}
		}
		else
			Transform_Flag = FALSE;
	}
	if (Transform_Flag == TRUE)
		return 0;
	else
		return -1;
}

bool CSIP_Server::InBound_Filter(SIP_MSG *lpSIP_Msg)
{
	bool Filter_Flag;
	int Invite_Cur;
	char Buf[128];

	if (strlen(Outbound_PhoneNo) <= 0)
		return FALSE;

	if ((Invite_Cur = Check_Invite(lpSIP_Msg)) >= 0)
	{
		if (!SIP_Invite_Data[Invite_Cur].InBound_Flag)
		{
			if (!strcmp(lpSIP_Msg->Command, "INVITE"))
			{
				sprintf(Buf, "%s-", Outbound_PhoneNo);
				if (((lpSIP_Msg->From_Name[0] == '1' && strlen(lpSIP_Msg->From_Name) == 3) ||				   // 3碼1開頭的號碼
					 (lpSIP_Msg->From_Name[0] != '0' && strlen(lpSIP_Msg->From_Name) > EXTENSION_NO_LENGTH) || // 5碼以上不為0開頭的號碼
					 (lpSIP_Msg->From_Name[0] == '0' && lpSIP_Msg->From_Name[1] == '0') ||					   // 國際碼
					 (lpSIP_Msg->From_Name[0] == '0' && lpSIP_Msg->From_Name[1] != '0')						   // 0開頭的號碼
					 ) &&
					!strncmp(lpSIP_Msg->To_Name, Buf, strlen(Buf))) // To_Name前頭為"0227712171-"
				{
					SIP_Invite_Data[Invite_Cur].InBound_Flag = TRUE;
					Filter_Flag = TRUE;
				}
				else
					Filter_Flag = FALSE;
			}
			else
				Filter_Flag = FALSE;
		}
		else
			Filter_Flag = TRUE;
	}
	else
		Filter_Flag = FALSE;

	return Filter_Flag;
}

int CSIP_Server::ReplaceCode_Transform_PhoneNo(SIP_MSG *lpSIP_Msg, bool Before_Flag)
{
	int Invite_Cur;
	bool Transform_Flag;
	SIP_INVITE_DATA *lpInvite_Data;

	if (!ReplaceCode_Check)
		return -1;

	if ((Invite_Cur = Check_Invite(lpSIP_Msg)) >= 0)
	{
		lpInvite_Data = &SIP_Invite_Data[Invite_Cur];
		Transform_Flag = FALSE;
		// Command Flow: Caller --> Callee
		if (ReplaceCode_Filter(lpSIP_Msg))
		{
			switch (Check_Flow_State(lpSIP_Msg, TRUE)) // Recvfrom
			{
			case 0: // Request and -->
				if (Before_Flag)
				{
					Edit_Name(lpSIP_Msg->Cmd_Str, lpSIP_Msg->Command, lpInvite_Data->Replaced_PhoneNo);
					Edit_Name(lpSIP_Msg->Cmd_Str, "To:", lpInvite_Data->Replaced_PhoneNo);
					strcpy(lpSIP_Msg->Command_Name, lpInvite_Data->Replaced_PhoneNo);
					strcpy(lpSIP_Msg->To_Name, lpInvite_Data->Replaced_PhoneNo);
					Transform_Flag = TRUE;
				}
				break;
			case 1: // Request and <--
				if (!Before_Flag)
				{
					Edit_Name(lpSIP_Msg->Cmd_Str, "From:", lpInvite_Data->Replace_Code);
					Transform_Flag = TRUE;
				}
				break;
			case 2: // Response and <--
				if (!Before_Flag)
				{ // 補上總機號碼
					Edit_Name(lpSIP_Msg->Cmd_Str, "To:", lpInvite_Data->Replace_Code);
					if (Get_Position(lpSIP_Msg->Cmd_Str, "Contact:") > 0)
						Edit_Name(lpSIP_Msg->Cmd_Str, "Contact:", lpInvite_Data->Replace_Code);
					Transform_Flag = TRUE;
				}
				break;
			case 3: // Response and -->
				if (Before_Flag)
				{ // 去除總機號碼
					Edit_Name(lpSIP_Msg->Cmd_Str, "From:", lpInvite_Data->Replaced_PhoneNo);
					Transform_Flag = TRUE;
				}
				break;
			}
		}
		else
			Transform_Flag = FALSE;
	}
	if (Transform_Flag == TRUE)
		return 0;
	else
		return -1;
}

bool CSIP_Server::ReplaceCode_Filter(SIP_MSG *lpSIP_Msg)
{
	bool Filter_Flag;
	int i, Invite_Cur;

	if (!ReplaceCode_Check)
		return FALSE;

	if ((Invite_Cur = Check_Invite(lpSIP_Msg)) >= 0)
	{
		if (!SIP_Invite_Data[Invite_Cur].ReplaceCode_Flag)
		{
			if (!strcmp(lpSIP_Msg->Command, "INVITE"))
			{
				// Check From_Name in Table
				for (i = 0; i < ReplaceCode_Num; i++)
				{
					if (!strcmp(ReplaceCode_Table[i][0], lpSIP_Msg->To_Name))
						break;
				}
				if (i < ReplaceCode_Num)
				{ // Check To_Name in Table
					SIP_Invite_Data[Invite_Cur].ReplaceCode_Flag = TRUE;
					Filter_Flag = TRUE;
					// Save Replace_PhoneNo
					strcpy(SIP_Invite_Data[Invite_Cur].Replace_Code, ReplaceCode_Table[i][0]);
					strcpy(SIP_Invite_Data[Invite_Cur].Replaced_PhoneNo, ReplaceCode_Table[i][1]);
				}
				else
					Filter_Flag = FALSE;
			}
			else
				Filter_Flag = FALSE;
		}
		else
			Filter_Flag = TRUE;
	}
	else
		Filter_Flag = FALSE;

	return Filter_Flag;
}

bool CSIP_Server::OutBound_SubServer_Filter(SIP_MSG *lpSIP_Msg, DEST_DATA *lpSubServer_Data)
{
	bool Filter_Flag;
	int Mapping_Cur, Invite_Cur, Flow_State;

	if ((Invite_Cur = Check_Invite(lpSIP_Msg)) >= 0)
	{
		if (!SIP_Invite_Data[Invite_Cur].OutBound_SubServer_Flag)
		{
			if (!strcmp(lpSIP_Msg->Command, "INVITE"))
			{
				if ((Mapping_Cur = Check_SubServer_Mapping(lpSIP_Msg->To_Name)) >= 0 &&
					Mapping_Data[Mapping_Cur].SubServer_Data[SIP_Invite_Data[Invite_Cur].SubServer_Count].is_used())
				{
					SIP_Invite_Data[Invite_Cur].Dest_Data.copy_from(&Mapping_Data[Mapping_Cur].SubServer_Data[SIP_Invite_Data[Invite_Cur].SubServer_Count]);
					SIP_Invite_Data[Invite_Cur].SubServer_Count++;
					Filter_Flag = TRUE;
					SIP_Invite_Data[Invite_Cur].OutBound_SubServer_Flag = TRUE;
				}
				else
					Filter_Flag = FALSE;
			}
			else
				Filter_Flag = FALSE;
		}
		else
			Filter_Flag = TRUE;
	}
	else
		Filter_Flag = FALSE;

	if (Filter_Flag)
	{
		if ((Flow_State = Check_Flow_State(lpSIP_Msg, TRUE)) == 0)
		{ // Request and -->
			lpSubServer_Data->copy_from(&SIP_Invite_Data[Invite_Cur].Dest_Data);
		}
		else if (Flow_State == 1)
		{ // Request and <--
			lpSubServer_Data->copy_from(SIP_Invite_Data[Invite_Cur].pFromDest);
			// lpSubServer_Data->Flag = TRUE;
			// lpSubServer_Data->Nc.copy_from(SIP_Invite_Data[Invite_Cur].pFromNc);
			// memcpy(&lpSubServer_Data->Addr, &SIP_Invite_Data[Invite_Cur].FAddr, sizeof(sockaddr_in));
			// strcpy(lpSubServer_Data->R_IP, SIP_Invite_Data[Invite_Cur].FR_IP);
			// strcpy(lpSubServer_Data->V_IP, SIP_Invite_Data[Invite_Cur].FV_IP);
			// lpSubServer_Data->Port = ntohs(SIP_Invite_Data[Invite_Cur].FAddr.sin_port);
			// lpSubServer_Data->Crypto_Type = SIP_Invite_Data[Invite_Cur].From_Crypto_Type;
		}
	}
	return Filter_Flag;
}

int CSIP_Server::Check_Calling_Party(SIP_MSG *Data)
{
	CRASH_DEBUG_COMMAND;
	int i, Command_Cur, Online_Cur;
	SIP_MSG SIP_Data;

	// REFER: Save Online for ReferTo_Name
	if (!strcmp(Data->Command, "202") && ((Command_Cur = Check_Command(Data)) >= 0 && Command_Data[Command_Cur].State == SIP_REFER))
	{
		if (Check_OnLine(Command_Data[Command_Cur].SIP_Msg.ReferTo_Name) < 0)
		{
			memcpy(&SIP_Data, &Command_Data[Command_Cur].SIP_Msg, sizeof(SIP_MSG));
			strcpy(SIP_Data.From_Name, SIP_Data.To_Name);
			strcpy(SIP_Data.To_Name, SIP_Data.ReferTo_Name);
			m_lpNetIF->StartDestClient(&SIP_Data.ToDest, AF_INET, SOCK_DGRAM, CT_SIPCrypto, 0, SIP_Data.ReferTo_URL, atoi(SIP_Data.ReferTo_Port), false, SocketModule_Callbcak_Function, this, SIP_SERVER_EVENT); // 取得Refer_IP&Port的sockaddr_in
			// strcpy(SIP_Data.TR_IP, SIP_Data.ReferTo_URL);
			// strcpy(SIP_Data.TV_IP, SIP_Data.ReferTo_URL);
			// SIP_Data.To_Crypto_Type = CT_NoCrypto;
			Save_OnLine(&SIP_Data, TO);
		}
	}

	if ((Command_Cur = Check_Command(Data)) >= 0)
	{
		if ((Online_Cur = Check_OnLine(Data->From_Name, &Command_Data[Command_Cur].SIP_Msg.FromDest.Raddr)) >= 0)
		{
			Data->FromDest.copy_from(&SIP_OnLine_Data[Online_Cur].Dest);
			// Data->FromNc.copy_from(&SIP_OnLine_Data[Online_Cur].Nc);
			// memcpy(&Data->FAddr, &SIP_OnLine_Data[Online_Cur].Addr, sizeof(Data->FAddr));
			// strcpy(Data->FV_IP, SIP_OnLine_Data[Online_Cur].V_IP);
			// strcpy(Data->FR_IP, SIP_OnLine_Data[Online_Cur].R_IP);
			// Data->From_Crypto_Type = SIP_OnLine_Data[Online_Cur].Crypto_Type;
			// 2.5 Get SDP
			if ((atoi(Data->Content_Length) > 0) && (strlen(Data->SDP) > 0))
			{ //Get_SDP(S1, Data->SDP);
				Data->RTP_Relay_Flag = TRUE;
				i = Extract_Param(Data->SDP, "c=IN IP4", Data->SDP_IP);
				i = Extract_Param(Data->SDP, "m=audio", Data->SDP_Port);
				if (i < 0)
					i = Extract_Param(Data->SDP, "m=image", Data->SDP_Port);
			}
		}
		else
		{
			// Data->FV_IP[0] = Data->FR_IP[0] = 0;
			Data->FromDest.reset();
		}
	}

	RETURN 0;
}
int CSIP_Server::Check_Called_Party(SIP_MSG *Data)
{
	CRASH_DEBUG_COMMAND;
	int Online_TCur, Invite_Cur;
	DEST_DATA GateWay_Data;
	DEST_DATA Connect_Data;

	if ((Invite_Cur = Check_Invite(Data)) >= 0)
	{
		// Set Trunk_GW Data
		GateWay_Data.copy_from(&NTUT_GW_Dest);
		// GateWay_Data.Flag = TRUE;
		// GateWay_Data.Nc.copy_from(&NTUT_GW_Nc);
		// memcpy(&GateWay_Data.Addr, &NTUT_GW_Addr, sizeof(GateWay_Data.Addr));
		// strcpy(GateWay_Data.R_IP, NTUT_GW_IP);
		// GateWay_Data.Port = Jatoi(NTUT_GW_Port, sizeof(NTUT_GW_Port));
		// GateWay_Data.Crypto_Type = NTUT_GW_Crypto_Type;
		// Set Forwarding_GW_Flag
		Data->Forwarding_GW_Flag = strcmp(Data->FromDest.Real_IP, GateWay_Data.Real_IP) &&				   // Condition 1: 若是從GW送過來的封包將走PC to PC
								   (SIP_Invite_Data[Invite_Cur].Forwarding_GW_Flag ||					   // Condition 2-1: 當Invite_Data.Forwarding_GW_Flag為TRUE，表示為內部走法，當ACK BYE CANCEL等Request來時將會走PC to PC
									(Data->To_Name[0] == '0' && !strcmp(Data->From_Name, Data->To_Name))); // Condition 2-2: 當From_Name=To_Name且Source_IP!=GW_IP, 則不走PC to PC
		// Set Cancel_Branch and ACK Branch
		strcpy(Data->Cancel_Branch, SIP_Invite_Data[Invite_Cur].Cancel_Branch);
	}

	// PC to PC(Sub-Server)
	if (OutBound_SubServer_Filter(Data, &Connect_Data))
	{
		Data->ToDest.copy_from(&Connect_Data);
		// Data->ToNc.copy_from(&Connect_Data.Nc);
		// memcpy(&Data->TAddr, &Connect_Data.Addr, sizeof(Data->TAddr));
		// strcpy(Data->TV_IP, Connect_Data.V_IP);
		// strcpy(Data->TR_IP, Connect_Data.R_IP);
		// Data->To_Crypto_Type = CT_NoCrypto;
		Save_OnLine(Data, TO);
	}

	// 1. PC to PC(Local+SIP-OnLine)
	else if ((Online_TCur = Check_OnLine(Data->To_Name)) >= 0 && !Data->Forwarding_GW_Flag)
	{
		Data->ToDest.copy_from(&SIP_OnLine_Data[Online_TCur].Dest);
		// Data->ToNc.copy_from(&SIP_OnLine_Data[Online_TCur].Nc);
		// memcpy(&Data->TAddr, &SIP_OnLine_Data[Online_TCur].Addr, sizeof(Data->TAddr));
		// strcpy(Data->TV_IP, SIP_OnLine_Data[Online_TCur].V_IP);
		// strcpy(Data->TR_IP, SIP_OnLine_Data[Online_TCur].R_IP);
		// Data->To_Crypto_Type = SIP_OnLine_Data[Online_TCur].Crypto_Type;
		if (Check_OnLine(Data->From_Name) < 0)
			Save_OnLine(Data, FROM);
	}

	// 1.5 Re-Save_Online for Recv Bye form Callee
	else if ((Invite_Cur >= 0 && Online_TCur < 0 && !strcmp(Data->Command, "BYE")) &&
			 !Addrcmp(&Data->FromDest.Raddr, &SIP_Invite_Data[Invite_Cur].pToDest->Raddr))
	{
		Data->ToDest.copy_from(SIP_Invite_Data[Invite_Cur].pFromDest);
		// Data->ToNc.copy_from(SIP_Invite_Data[Invite_Cur].pFromNc);
		// memcpy(&Data->TAddr, &SIP_Invite_Data[Invite_Cur].FAddr, sizeof(Data->TAddr));
		// strcpy(Data->TR_IP, SIP_Invite_Data[Invite_Cur].FR_IP);
		// strcpy(Data->TV_IP, SIP_Invite_Data[Invite_Cur].FV_IP);
		// Data->To_Crypto_Type = SIP_Invite_Data[Invite_Cur].From_Crypto_Type;
		Save_OnLine(Data, TO);
	}

	// 2. PC to GW
	else if (GateWay_Data.Real_IP &&
			 ((!strncmp(Data->To_Name, "110", 1) && strlen(Data->To_Name) == 3) ||
			  (Data->To_Name[0] == '0' && Data->To_Name[1] == '0') ||
			  (Data->To_Name[0] == '0' && Data->To_Name[1] != '0') ||
			  (Data->To_Name[0] != '0' && strlen(Data->To_Name) > EXTENSION_NO_LENGTH && strlen(Outbound_PhoneNo) > 0)) &&
			 (strcmp(GateWay_Data.Real_IP, Local_IP) &&			   // Avoid Auto-loop
			  strcmp(GateWay_Data.Real_IP, Data->FromDest.Real_IP) // In PC to GW,  the "From:" Party can't be GW
			  ))
	{
		Data->ToDest.copy_from(&GateWay_Data);
		// Data->ToNc.copy_from(&GateWay_Data.Nc);
		// memcpy(&Data->TAddr, &GateWay_Data.Addr, sizeof(Data->TAddr));
		// strcpy(Data->TV_IP, GateWay_Data.V_IP);
		// strcpy(Data->TR_IP, GateWay_Data.R_IP);
		Save_OnLine(Data, TO);
	}

	// 5. Offline
	else
	{
		// Data->TV_IP[0] = Data->TR_IP[0] = 0;
		Data->ToDest.reset();
	}

	RETURN 0;
}

int CSIP_Server::Check_Flow_State(SIP_MSG *lpSIP_Msg, bool Transmission_Flag)
{
	if (!lpSIP_Msg)
	{
		DTrace_FatalError("invalid parameters, %d @ %s\n", __LINE__, __FILE__);
		RETURN - 1;
	}

	bool Command_Type;
	int Flow_State, Invite_Cur;
	SIP_INVITE_DATA *lpInvite_Data;

	if ((Invite_Cur = Check_Invite(lpSIP_Msg)) >= 0)
	{
		lpInvite_Data = &SIP_Invite_Data[Invite_Cur];
		// Get Command_Type: TRUE表示Request,  FALSE表示Response
		Command_Type = (lpSIP_Msg->Command[0] < '1' || lpSIP_Msg->Command[0] > '6') ? TRUE : FALSE;

		if (Transmission_Flag)
		{					  // Recvfrom
			if (Command_Type) // Request
			{				  // Determine Flow: Caller --> Callee
				if (!Addrcmp(&SIP_Invite_Data[Invite_Cur].pFromDest->Raddr, &lpSIP_Msg->FromDest.Raddr))
					Flow_State = 0;
				else // Determine Flow: Callee <-- Caller
					if (!Addrcmp(&SIP_Invite_Data[Invite_Cur].pToDest->Raddr, &lpSIP_Msg->FromDest.Raddr))
					Flow_State = 1;
			}
			else // Response
			{	 // Determine Flow: Caller <-- Callee
				if (!Addrcmp(&SIP_Invite_Data[Invite_Cur].pToDest->Raddr, &lpSIP_Msg->ToDest.Raddr))
					Flow_State = 2;
				else // Determine Flow: Callee --> Caller
					if (!Addrcmp(&SIP_Invite_Data[Invite_Cur].pFromDest->Raddr, &lpSIP_Msg->ToDest.Raddr))
					Flow_State = 3;
			}
		}
		else
		{					  // Sendto
			if (Command_Type) // Request
			{				  // Determine Flow: Caller --> Callee
				if (!Addrcmp(&lpInvite_Data->pToDest->Raddr, &lpSIP_Msg->pDest->Raddr))
					Flow_State = 4;
				else // Determine Flow: Callee <-- Caller
					if (!Addrcmp(&lpInvite_Data->pFromDest->Raddr, &lpSIP_Msg->pDest->Raddr))
					Flow_State = 5;
			}
			else // Response
			{	 // Determine Flow: Caller <-- Callee
				if (!Addrcmp(&lpInvite_Data->pFromDest->Raddr, &lpSIP_Msg->pDest->Raddr))
					Flow_State = 6;
				else // Determine Flow: Callee --> Caller
					if (!Addrcmp(&lpInvite_Data->pToDest->Raddr, &lpSIP_Msg->pDest->Raddr))
					Flow_State = 7;
			}
		}
	}
	else
		Flow_State = -1;

	return Flow_State;
}

// int CSIP_Server::Check_Relay_Monitor(SIP_MSG *Data)
// {
// 	int            i,Cur;

//     Data->RTP_Relay_Flag=Data->RL1_Flag=FALSE;

//     // 判斷是否要監聽
//     if ( (Cur=Check_Monitor(Data))>=0 )
// 	{  strcpy(Data->Relay_TCP_IP,Monitor_Data[Cur].Monitor_TCP_IP);
// 	   strcpy(Data->Relay_TCP_Port,Monitor_Data[Cur].Monitor_TCP_Port);
// 	   Data->RTP_Relay_Flag=TRUE; }

// 	// 全導Relay
// 	else if (Relay_100_Flag==3)
// 	{  Data->RTP_Relay_Flag=TRUE;
// 	   strcpy(Data->Relay_TCP_IP  ,RTP_Relay_IP);
// 	   strcpy(Data->Relay_TCP_Port,RTP_Relay_Port);	 }

// 	// SDP 之 c= and m= 若資料錯誤 將強迫導 RTP_Relay
//     else if ( ((i=Extract_Param(Data->SDP,"c=IN IP4",Data->SDP_IP))<0) ||
// 	          ((i=Extract_Param(Data->SDP,"m=audio",Data->SDP_Port))<0) )
// 	{  Data->RTP_Relay_Flag=TRUE;
// 	   strcpy(Data->Relay_TCP_IP  ,RTP_Relay_IP);
// 	   strcpy(Data->Relay_TCP_Port,RTP_Relay_Port);	}

// 	// PC to Phone 除外
// 	else if ((Relay_100_Flag==2)&&(Data->From_Name[0]!='0') && (Data->From_Name[0]!='1'))
// 	{  Data->RTP_Relay_Flag=TRUE;
// 	   strcpy(Data->Relay_TCP_IP  ,RTP_Relay_IP);
// 	   strcpy(Data->Relay_TCP_Port,RTP_Relay_Port);	 }

// 	// PC to Phone 與跨網被叫端除外
// 	else if ((Relay_100_Flag==1))
// 	{  Data->RTP_Relay_Flag=TRUE;
// 	   strcpy(Data->Relay_TCP_IP  ,RTP_Relay_IP);
// 	   strcpy(Data->Relay_TCP_Port,RTP_Relay_Port);	 }

// 	// 判斷是否導通 RTP_Relay
// 	else if ( (strcmp(Data->FR_IP,Data->TR_IP)) &&
// 	          ((strcmp(Data->FR_IP,Data->FV_IP))||(strcmp(Data->TR_IP,Data->TV_IP))) )
// 	{  Data->RTP_Relay_Flag=TRUE;
// 	   strcpy(Data->Relay_TCP_IP,RTP_Relay_IP);
// 	   strcpy(Data->Relay_TCP_Port,RTP_Relay_Port); }

// 	// 2.5 Get SDP
// 	if ( (atoi(Data->Content_Length)>0) && (Data->RTP_Relay_Flag) )
// 	{  //Get_SDP(S1,Data->SDP);
// 	   i=Extract_Param(Data->SDP,"c=IN IP4",Data->SDP_IP);
// 	   i=Extract_Param(Data->SDP,"m=audio",Data->SDP_Port);
//        if (i<0) i=Extract_Param(Data->SDP,"m=image",Data->SDP_Port);
// 	}

// 	return 0;
// }

int CSIP_Server::Filter_String(char *S1, const char *S2, char *Name, char *URL, char *Port, char *Tag, char *Branch, char *expires, BOOL *Flag) //, int *Name_Locate)
{
	int i, j, k, Beg, Mid, Mid11, Mid12, Mid2, Mid31, Mid32, End, Beg1, End1;
	char Buf[128];

	// 1. Initialize
	Flag[0] = FALSE;
	//Name_Locate[0] = Name_Locate[1] = 1;
	Beg = Mid = Mid11 = Mid12 = Mid2 = Mid31 = Mid32 = End = Name[0] = URL[0] = Port[0] = Tag[0] = Branch[0] = expires[0] = 0;
	for (i = 0; i < (int)strlen(S2); i++)
		if ((S2[i] < '0') || (S2[i] > '9'))
			break;
	if (i >= (int)strlen(S2))
		return 0; // Status No.,

	// 2. Search Beg_Point, Beg=關鍵字之後位址
	k = (int)strlen(S2);
	if (k > (int)strlen(S1))
		return 0;
	for (i = 0; i <= (int)strlen(S1) - k; i++) // 需再修正
	{
		if (S1[i] == S2[0])
		{
			for (j = 0; j < k; j++)
				if ((S1[i + j] | 0x20) != (S2[j] | 0x20))
					break; // 大小寫均考慮
			if (j >= k)
			{
				Beg = i + k;
				break;
			}
		}
	}
	if (Beg == 0)
		return 0; // 關鍵字未尋獲

	// 3. Search End_Point, End=關鍵字串句尾
	End = (int)strlen(S1) - 1;
	for (i = Beg; i < End; i++)
		if ((S1[i] == 0x0a) || (S1[i] == 0x0d))
		{
			End = i - 1;
			break;
		}

	// 4. Search Mid(@), Mid1(:), Mid, Mid2(;), Mid31(<). Mid32(>)
	if (Beg < End)
	{ // 4.1 Mid(@)
		for (i = Beg; i <= End; i++)
			if (S1[i] == '@')
			{
				Mid = i;
				break;
			}
		// 4.2 Mid1(:)
		if (Mid > 0)
		{
			for (i = Mid - 1; i >= Beg; i--)
				if (S1[i] == ':')
				{
					Mid11 = i;
					break;
				}
			for (i = Mid + 1; i <= End; i++)
				if (S1[i] == ':')
				{
					Mid12 = i;
					break;
				}
		}
		else
		{
			for (i = Beg; i <= End; i++)
				if (S1[i] == ':')
				{
					Mid11 = i;
					break;
				}
			if (Mid11 > 0)
			{
				for (i = Mid11 + 1; i <= End; i++)
					if (S1[i] == ':')
					{
						Mid12 = i;
						break;
					}
			}
		}
		// 4.3 Mid2(;)
		if (Mid12 > 0)
			Beg1 = Mid12 + 1;
		else if (Mid11 > 0)
			Beg1 = Mid11 + 1;
		else
			Beg1 = Beg;
		if ((Mid > 0) && (Mid > Beg1))
			Beg1 = Mid + 1;
		for (i = Beg1; i <= End; i++)
			if (S1[i] == ';')
			{
				Mid2 = i;
				break;
			}
		// 4.4 Mid31(<)
		for (i = Beg; i <= End; i++)
			if (S1[i] == '<')
			{
				Mid31 = i;
				break;
			}
		// 4.5 Mid32(>)
		if (Mid31 > 0)
			for (i = Mid31 + 1; i <= End; i++)
				if (S1[i] == '>')
				{
					Mid32 = i;
					break;
				}
		// 4.6 Flag
		if ((Mid31 > 0) && (Mid32 > 0))
			Flag[0] = TRUE;
	}

	// 5. Get Name
	if (Mid > 0)
	{
		for (i = Mid - 1; i >= Beg; i--)
			if ((S1[i] == ';') || (S1[i] == ':') || (S1[i] == '<') || (S1[i] == ' '))
				break;
		if ((i != (Beg - 1)) && ((Mid - i - 1) > 0) && ((Mid - i - 1) < SIP_NAME_LENGTH))
		{
			memcpy(Name, &S1[i + 1], Mid - i - 1);
			Name[Mid - i - 1] = 0;
			//Name_Locate[0]=i+1; Name_Locate[1]=Name_Locate[0]+Mid-i-2;
		}
		//if (strlen(Name)>0) Get Name1
	}

	// 6. Get URL
	Beg1 = End1 = 0;
	if (Mid > 0) // hsf@1.1.1.1 ,   @->
	{
		Beg1 = Mid + 1;
		for (i = Beg1; i <= End; i++)
			if ((S1[i] == ';') || (S1[i] == ':') || (S1[i] == '>') || (S1[i] == ' '))
			{
				End1 = i - 1;
				break;
			}
		if (i > End)
			End1 = End;
	}
	else if ((Mid11 > 0) && (Mid11 < Mid12)) // with  sip: --> 1.1.1.1 <-- :5060
	{
		Beg1 = Mid11 + 1;
		End1 = Mid12 - 1;
		if (Beg1 > End1)
		{
			Beg1 = End1 = 0;
		}
	}
	else if ((Mid11 > 0) && (Mid12 == 0)) // with sip:1.1.1.1  or 1.1.1.1:5060
	{
		if (S1[Mid11 - 1] == 'p')
		{
			Beg1 = Mid11 + 1;
			for (i = Beg1; i <= End; i++)
				if ((S1[i] == ';') || (S1[i] == ':') || (S1[i] == '>') || (S1[i] == ' '))
				{
					End1 = i - 1;
					break;
				}
			if (i > End)
				End1 = End;
		}
		else
		{
			End1 = Mid11 - 1;
			for (i = End1; i >= Beg; i--)
				if ((S1[i] == ';') || (S1[i] == ':') || (S1[i] == '<') || (S1[i] == ' '))
				{
					Beg1 = i + 1;
					break;
				}
		}
	}
	else if (Mid11 == 0 && Mid12 == 0 && Mid2 > 0) // SIP/2.0/UDP 1.1.1.1;
	{
		End1 = Mid2 - 1;
		for (i = End1; i >= Beg; i--)
			if (S1[i] == ' ')
			{
				Beg1 = i + 1;
				break;
			}
		if (i < Beg)
			Beg1 = Beg;
	}
	else // INVITE 1.1.1.1  SIP/2.0
	{
		for (i = Beg; i <= End; i++)
			if (S1[i] != ' ')
			{
				Beg1 = i;
				break;
			}
		if (Beg1 > 0)
		{
			for (i = Beg1; i <= End; i++)
				if ((S1[i] == ';') || (S1[i] == ':') || (S1[i] == '>') || (S1[i] == ' '))
				{
					End1 = i - 1;
					break;
				}
			if (i > End)
				End1 = End;
		}
	}
	if ((End1 > 0) && (End1 >= Beg1) && ((End1 - Beg1 + 1) < SIP_URL_LENGTH))
	{
		memcpy(URL, &S1[Beg1], End1 - Beg1 + 1);
		URL[End1 - Beg1 + 1] = 0;
	}

	// 7. Get Port
	Beg1 = End1 = 0;
	if ((Mid11 > 0) && (Mid11 < Mid12)) // sip:1.1.1.1:5060
	{
		Beg1 = Mid12 + 1;
		for (i = Beg1; i <= End; i++)
			if ((S1[i] == ';') || (S1[i] == ':') || (S1[i] == '>') || (S1[i] == ' '))
			{
				End1 = i - 1;
				break;
			}
		if (i > End)
			End1 = End;
	}
	else if ((Mid11 > 0) && (Mid12 == 0)) // sip:1.1.1.1  or 1.1.1.1:5060
	{
		Beg1 = Mid11 + 1;
		for (i = Beg1; i <= End; i++)
			if ((S1[i] == ';') || (S1[i] == ':') || (S1[i] == '>') || (S1[i] == ' '))
			{
				End1 = i - 1;
				break;
			}
		if (i > End)
			End1 = End;
		for (i = Beg1; i <= End1; i++)
			if ((S1[i] < '0') || (S1[i] > '9'))
				break;
		if (i < End1)
		{
			Beg1 = End1 = 0;
		}
	}

	if ((End1 > 0) && (End1 >= Beg1) && ((End1 - Beg1 + 1) < SIP_PORT_LENGTH))
	{
		memcpy(Port, &S1[Beg1], End1 - Beg1 + 1);
		Port[End1 - Beg1 + 1] = 0;
	}

	// 7. Get Tag(需考慮tag之大小寫)
	Beg1 = End1 = 0;
	strcpy(Buf, "tag=");
	if (Mid2 > 0)
	{
		for (i = Mid2 + 1; i <= (End - (int)strlen(Buf) + 1); i++)
		{
			j = 0;
			if ((S1[i] | 0x20) == (Buf[0] | 0x20))
				for (j = 0; j < (int)strlen(Buf); j++)
					if ((S1[i + j] | 0x20) != (Buf[j] | 0x20))
						break;
			if (j >= (int)strlen(Buf))
			{
				Beg1 = i + (int)strlen(Buf);
				break;
			}
		}

		if (Beg1 > 0)
		{
			for (i = Beg1; i <= End; i++)
				if ((S1[i] == ';') || (S1[i] == ' '))
				{
					End1 = i - 1;
					break;
				}
			if (i > End)
				End1 = End;
		}
		if ((End1 > 0) && (End1 >= Beg1) && ((End1 - Beg1 + 1) < SIP_TAG_LENGTH))
		{
			memcpy(Tag, &S1[Beg1], End1 - Beg1 + 1);
			Tag[End1 - Beg1 + 1] = 0;
		}
	}

	// 7. Get branch(需考慮branch之大小寫)
	Beg1 = End1 = 0;
	strcpy(Buf, "branch=");
	if (Mid2 > 0)
	{
		for (i = Mid2 + 1; i <= (End - (int)strlen(Buf) + 1); i++)
		{
			j = 0;
			if ((S1[i] | 0x20) == (Buf[0] | 0x20))
				for (j = 0; j < (int)strlen(Buf); j++)
					if ((S1[i + j] | 0x20) != (Buf[j] | 0x20))
						break;
			if (j >= (int)strlen(Buf))
			{
				Beg1 = i + (int)strlen(Buf);
				break;
			}
		}
		if (Beg1 > 0)
		{
			for (i = Beg1; i <= End; i++)
				if ((S1[i] == ';') || (S1[i] == ' '))
				{
					End1 = i - 1;
					break;
				}
			if (i > End)
				End1 = End;
		}
		if ((End1 > 0) && (End1 >= Beg1) && ((End1 - Beg1 + 1) < SIP_TAG_LENGTH))
		{
			memcpy(Branch, &S1[Beg1], End1 - Beg1 + 1);
			Branch[End1 - Beg1 + 1] = 0;
		}
	}

	// 8. Get expires
	Beg1 = End1 = 0;
	strcpy(Buf, "expires=");
	if (Mid2 > 0)
	{
		for (i = Mid2 + 1; i <= (End - (int)strlen(Buf) + 1); i++)
		{
			j = 0;
			if ((S1[i] | 0x20) == (Buf[0] | 0x20))
				for (j = 0; j < (int)strlen(Buf); j++)
					if ((S1[i + j] | 0x20) != (Buf[j] | 0x20))
						break;
			if (j >= (int)strlen(Buf))
			{
				Beg1 = i + (int)strlen(Buf);
				break;
			}
		}
		if (Beg1 > 0)
		{
			for (i = Beg1; i <= End; i++)
				if ((S1[i] == ';') || (S1[i] == ' '))
				{
					End1 = i - 1;
					break;
				}
			if (i > End)
				End1 = End;
		}
		if ((End1 > 0) && (End1 >= Beg1) && ((End1 - Beg1 + 1) < SIP_TAG_LENGTH))
		{
			memcpy(expires, &S1[Beg1], End1 - Beg1 + 1);
			expires[End1 - Beg1 + 1] = 0;
		}
	}

	if ((strlen(Name) > 0) || (strlen(URL) > 0) || (strlen(Port) > 0))
		return End + 1;
	else
		return 0;
}
int CSIP_Server::Get_Field(char *S1, const char *Head, char *Field)
{
	int i, Beg, End;

	Field[0] = Beg = End = 0;
	if ((i = Get_Position(S1, Head)) > 0)
	{
		Beg = i + (int)strlen(Head);
		for (i = Beg; i < (int)strlen(S1); i++)
			if (S1[i] != ' ')
			{
				Beg = i;
				break;
			}
		for (i = Beg; i < (int)strlen(S1); i++)
			if ((S1[i] == ' ') || (S1[i] == 0x0a) || (S1[i] == 0x0d) || (S1[i] == ';'))
			{
				End = i - 1;
				break;
			}
		if ((Beg != 0) && (Beg <= End))
			strncpy(Field, &S1[Beg], (End - Beg + 1));
		Field[End - Beg + 1] = 0;
		return 0;
	}
	else
	{
		return -1;
	}
}

int CSIP_Server::Get_CSeq_Num(char *S1, const char *Head, char *Buf)
{
	int i, j, k, Beg, End;

	Beg = End = 0;
	if ((i = Get_Position(S1, Head)) > 0)
	{
		Beg = i + (int)strlen(Head);
		for (j = Beg; j < (int)strlen(S1); j++)
			if ((S1[j] == 0x0a) || (S1[j] == 0x0d))
			{
				End = j - 1;
				break;
			}
		if (End > Beg)
		{
			for (k = End; k >= Beg; k--)
				if (S1[k] != 0x20)
				{
					End = k;
					break;
				}
			for (k = End - 1; k >= Beg; k--)
				if (S1[k] == 0x20)
				{
					Beg = k + 1;
					break;
				}
			if (Beg < End)
			{
				strncpy(Buf, &S1[Beg], (End - Beg + 1));
				Buf[End - Beg + 1] = 0;
				return 0;
			}
			else
				return -1;
		}
		else
			return -1;
	}
	else
		return -1;
}

int CSIP_Server::Modify_CSeq(char *S1, const char *Head)
{
	int i, j, k, Beg, End;
	char SS1[SIP_MSG_LEGNTH];
	Beg = End = 0;
	if ((i = Get_Position(S1, "CSeq:")) > 0)
	{
		for (j = i + 5; j < (int)strlen(S1); j++)
			if ((S1[j] == 0x0a) || (S1[j] == 0x0d))
			{
				End = j - 1;
				break;
			}
		for (k = j - 1; k >= i + 5; k--)
			if (S1[k] != 0x20)
			{
				End = k;
				break;
			}
		for (k = End - 1; k >= i + 5; k--)
			if (S1[k] == 0x20)
			{
				Beg = k + 1;
				break;
			}
		if (Beg < End)
		{
			strncpy(SS1, S1, Beg);
			SS1[Beg] = 0;
			strcat(SS1, Head);
			strcat(SS1, &S1[End + 1]);
			strcpy(S1, SS1);
			return 0;
		}
		else
			return -1;
	}
	else
		return -1;
}

int CSIP_Server::Get_Position(char *S1, const char *Head)
{
	int i, j;

	for (i = 0; i < (int)(strlen(S1) - strlen(Head)); i++)
	{
		j = 0;
		if ((S1[i] | 0x20) == (Head[0] | 0x20))
		{
			for (j = 1; j < (int)strlen(Head); j++)
				if ((S1[i + j] | 0x20) != (Head[j] | 0x20))
					break;
		}
		if (j >= (int)strlen(Head))
			break;
	}

	if (i < (int)(strlen(S1) - strlen(Head)))
		return i;
	else
		return -1;
}
int CSIP_Server::Get_NextLine(char *S1)
{
	int i;
	for (i = 0; i < (int)strlen(S1); i++)
		if ((S1[i] == 0x0a) || (S1[i] == 0x0d))
			break;
	if (i < (int)strlen(S1))
	{
		if ((S1[i] == 0x0d) && (S1[i + 1] == 0x0a))
			return i + 2;
		else
			return -1;
	}
	else
		return -1;
}

int CSIP_Server::Get_SDP(char *S1, char *SDP)
{
	int i, j;

	SDP[0] = 0;
	// 1. Search "content-length" Position
	if ((i = Get_Position(S1, "Content-Length:")) > 0)
	{ // 2. Search "v="
		if ((j = Get_Position(&S1[i], "v=")) > 0)
			strcpy(SDP, &S1[i + j]);
	}
	if (strlen(SDP) > 0)
		return 0;
	else
		return -1;
}

// **************************************************************************************************************
// **************************************************************************************************************
// Process Outbound PhoneNo.
int CSIP_Server::Add_Name(char *Cmd_Str, const char *Head, char *Outbound_PhoneNo)
{
	char SS1[SIP_MSG_LEGNTH];
	int i, j, k, Beg, End;

	Beg = End = 0;
	if ((i = Get_Position(Cmd_Str, Head)) >= 0)
	{
		for (j = i; j < (int)strlen(Cmd_Str); j++)
			if ((Cmd_Str[j] == 0x0a) || (Cmd_Str[j] == 0x0d))
				break;
		for (k = i; k < j; k++)
			if (Cmd_Str[k] == '@')
				break;
		if (k < j)
		{
			for (j = k - 1; j >= i; j--)
				if ((Cmd_Str[j] == 0x20) || (Cmd_Str[j] == ':') || (Cmd_Str[j] == '<'))
				{
					Beg = j + 1;
					break;
				}
			if (Beg > 0)
			{
				strncpy(SS1, Cmd_Str, Beg);
				SS1[Beg] = 0;
				strcat(SS1, Outbound_PhoneNo);
				strcat(SS1, "-");
				strcat(SS1, &Cmd_Str[Beg]);
				strcpy(Cmd_Str, SS1);
			}
		}
	}
	return 0;
}

int CSIP_Server::Delete_Name(char *Cmd_Str, const char *Head, char *Outbound_PhoneNo)
{
	char SS1[SIP_MSG_LEGNTH];
	int i, j, k, Beg, End;

	Beg = End = 0;
	if ((i = Get_Position(Cmd_Str, Head)) >= 0)
	{
		for (j = i; j < (int)strlen(Cmd_Str); j++)
			if ((Cmd_Str[j] == 0x0a) || (Cmd_Str[j] == 0x0d))
				break;
		for (k = i; k < j; k++)
			if (Cmd_Str[k] == '@')
				break;
		if (k < j)
		{
			for (j = k - 1; j >= i; j--)
				if ((Cmd_Str[j] == 0x20) || (Cmd_Str[j] == ':') || (Cmd_Str[j] == '<'))
				{
					Beg = j + 1;
					break;
				}
			if (Beg > 0)
				for (j = Beg; j < k; j++)
					if (Cmd_Str[j] == '-')
					{
						End = j;
						break;
					}
			if (Beg <= End)
			{
				strncpy(SS1, Cmd_Str, Beg);
				SS1[Beg] = 0;
				strcat(SS1, &Cmd_Str[End + 1]);
				strcpy(Cmd_Str, SS1);
			}
		}
	}
	return 0;
}

int CSIP_Server::Edit_Name(char *Cmd_Str, const char *Head, char *Forwarding_No)
{
	char SS1[1500];
	int i, j, k, Beg, End;

	Beg = End = 0;
	if ((i = Get_Position(Cmd_Str, Head)) >= 0)
	{
		for (j = i; j < (int)strlen(Cmd_Str); j++)
			if ((Cmd_Str[j] == 0x0a) || (Cmd_Str[j] == 0x0d))
				break;
		for (k = i; k < j; k++)
			if (Cmd_Str[k] == '@')
			{
				End = k;
				break;
			}
		if (k < j)
		{
			for (j = k - 1; j >= i; j--)
				if ((Cmd_Str[j] == 0x20) || (Cmd_Str[j] == ':') || (Cmd_Str[j] == '<'))
				{
					Beg = j + 1;
					break;
				}
			if (Beg > 0)
			{
				strncpy(SS1, Cmd_Str, Beg);
				SS1[Beg] = 0;
				strcat(SS1, Forwarding_No);
				strcat(SS1, &Cmd_Str[End]);
				strcpy(Cmd_Str, SS1);
			}
		}
	}
	return 0;
}

// **************************************************************************************************************

int CSIP_Server::Get_Auth(char *S1, const char *Head, char *Buf)
{
	int i, j, Beg, End;

	Beg = End = Buf[0] = 0;
	if ((i = Get_Position(S1, Head)) > 0)
	{
		for (j = i + strlen(Head); j < (int)strlen(S1); j++)
			if ((S1[j] != 0x20) && (S1[j] != 0x0a) && (S1[j] != 0x0d) && (S1[j] != '"'))
			{
				Beg = j;
				break;
			}
		if (Beg > 0)
			for (j = Beg + 1; j < (int)strlen(S1); j++)
				if ((S1[j] == 0x20) || (S1[j] == ',') || (S1[j] == 0x0a) || (S1[j] == 0x0d) || (S1[j] == '"'))
				{
					End = j - 1;
					break;
				}
		if ((End >= Beg) && (Beg > 0))
		{
			strncpy(Buf, &S1[Beg], (End - Beg + 1));
			Buf[End - Beg + 1] = 0;
		}
	}
	return 0;
}

int CSIP_Server::Deletion(char *S1, int Beg, int End)
{
	char SS1[SIP_MSG_LEGNTH];

	if ((End < (int)strlen(S1)) && (End >= Beg) && (Beg >= 0))
	{
		strncpy(SS1, S1, Beg);
		SS1[Beg] = 0;
		strcat(SS1, &S1[End + 1]);
		strcpy(S1, SS1);
		return 0;
	}
	else
		return -1;
}

int CSIP_Server::Insertion(char *S1, int Beg, const char *Buf)
{
	char SS1[SIP_MSG_LEGNTH];

	if ((Beg < (int)strlen(S1)) && (Beg >= 0))
	{
		strncpy(SS1, S1, Beg);
		SS1[Beg] = 0;
		strcat(SS1, Buf);
		strcat(SS1, &S1[Beg]);
		strcpy(S1, SS1);
		return 0;
	}
	else
		return -1;
}

int CSIP_Server::Remove_Via(char *S1, char *ViaBuf)
{
	int j, Via_Beg[100], Via_Num = 0, i = 0;

	while (((j = Get_Position(&S1[i], "Via:")) > 0) && (Via_Num < 99))
	{
		i += j;
		Via_Beg[Via_Num] = i;
		i += strlen("Via:");
		Via_Num++;
	}
	if (Via_Num > 1) // 至少兩個Via才處理之
	{
		if ((j = Get_NextLine(&S1[Via_Beg[Via_Num - 1]])) > 0)
		{
			j += Via_Beg[Via_Num - 1];
			strncpy(ViaBuf, &S1[Via_Beg[1]], j - Via_Beg[1]);
			ViaBuf[j - Via_Beg[1]] = 0;
			Deletion(S1, Via_Beg[1], j - 1);
		}
	}
	return 0;
}
int CSIP_Server::Recover_Via(char *S1, char *ViaBuf)
{
	int j, Via_Beg[100], Via_Num = 0, i = 0;

	while (((j = Get_Position(&S1[i], "Via:")) > 0) && (Via_Num < 99))
	{
		i += j;
		Via_Beg[Via_Num] = i;
		i += strlen("Via:");
		Via_Num++;
	}
	if (Via_Num == 1) // 剛好一個Via才處理之
	{
		if ((j = Get_NextLine(&S1[Via_Beg[Via_Num - 1]])) > 0)
		{
			j += Via_Beg[Via_Num - 1];
			Insertion(S1, j, ViaBuf);
		}
	}
	return 0;
}
int CSIP_Server::Get_Via(char *S1, char *ViaBuf)
{
	int j, Via_Beg[100], Via_Num = 0, i = 0;

	ViaBuf[0] = 0;
	while (((j = Get_Position(&S1[i], "Via:")) > 0) && (Via_Num < 99))
	{
		i += j;
		Via_Beg[Via_Num] = i;
		i += strlen("Via:");
		Via_Num++;
	}
	if (Via_Num >= 1) // 至少兩個Via才處理之
	{
		if ((j = Get_NextLine(&S1[Via_Beg[Via_Num - 1]])) > 0)
		{
			j += Via_Beg[Via_Num - 1];
			strncpy(ViaBuf, &S1[Via_Beg[0]], j - Via_Beg[0]);
			ViaBuf[j - Via_Beg[0]] = 0;
		}
	}
	return 0;
}
int CSIP_Server::Add_Via111(char *S1, char *Port, const char *Branch)
{
	int j, Via_Beg[100], Via_Num = 0, i = 0;
	char LPort[100], Buf[256];

	// Construct Via: String
	sprintf(LPort, "%s", Port);
	if (strcmp(Branch, ""))
		sprintf(Buf, "Via: SIP/2.0/UDP %s:%s;branch=%s\r\n", Local_IP, LPort, Branch);
	else
		sprintf(Buf, "Via: SIP/2.0/UDP %s:%s;branch=z9hG4bK%d%d\r\n", Local_IP, LPort, rand(), rand());

	//ViaBuf[0]=0;
	while (((j = Get_Position(&S1[i], "Via:")) > 0) && (Via_Num < 99))
	{
		i += j;
		Via_Beg[Via_Num] = i;
		i += strlen("Via:");
		Via_Num++;
	}
	if (Via_Num >= 1) // 至少兩個Via才處理之
	{
		if ((j = Get_NextLine(&S1[Via_Beg[Via_Num - 1]])) > 0)
		{
			j += Via_Beg[Via_Num - 1];
			Deletion(S1, Via_Beg[0], j - 1);
			Insertion(S1, Via_Beg[0], Buf);
		}
	}
	return 0;
}
int CSIP_Server::Delete_Via111(char *S1, char *ViaBuf)
{
	int j, Via_Beg[100], Via_Num = 0, i = 0;
	//char  Buf[256];

	//ViaBuf[0]=0;
	while (((j = Get_Position(&S1[i], "Via:")) > 0) && (Via_Num < 99))
	{
		i += j;
		Via_Beg[Via_Num] = i;
		i += strlen("Via:");
		Via_Num++;
	}
	if (Via_Num >= 1) // 至少兩個Via才處理之
	{
		if ((j = Get_NextLine(&S1[Via_Beg[Via_Num - 1]])) > 0)
		{
			j += Via_Beg[Via_Num - 1];
			Deletion(S1, Via_Beg[0], j - 1);
			Insertion(S1, Via_Beg[0], ViaBuf);
		}
	}
	return 0;
}

//=======================================================================================

int CSIP_Server::Init_Invite()
{
	int i;
	SIP_Invite_Num = 0;
	for (i = 0; i < SIP_INVITE_SIZE; i++)
	{
		SIP_Invite_Data[i].reset();
		if (m_lpNetIF)
			m_lpNetIF->InitTimer(&SIP_Invite_Data[i].MixTimer, this, MixTimer_Callbcak_Function);
	}
	return 0;
}

int CSIP_Server::Clear_Invite(SIP_MSG *lpSIP_Msg)
{
	int Invite_Cur;

	Invite_Cur = Check_Invite(lpSIP_Msg);
	return Clear_Invite_By_Cur(Invite_Cur);
}

int CSIP_Server::Clear_Invite_By_Cur(int Invite_Cur)
{
	if (Invite_Cur >= 0 && SIP_Invite_Data[Invite_Cur].Flag == TRUE)
	{
		if (SIP_Invite_Data[Invite_Cur].Answer_State != Temporary)
		{
			SIP_Invite_Data[Invite_Cur].Answer_State = Temporary;
			if (m_lpNetIF)
				m_lpNetIF->SetTimer(&SIP_Invite_Data[Invite_Cur].MixTimer, TWO_SECOND, TIMERID_INVITE + Invite_Cur);
		}
		else if (SIP_Invite_Data[Invite_Cur].Answer_State == Temporary)
		{
			if (m_lpNetIF)
				m_lpNetIF->KillTimer(&SIP_Invite_Data[Invite_Cur].MixTimer);
			// Clear Invite
			SIP_Invite_Data[Invite_Cur].Dest_Data.clean_nc();
			SIP_Invite_Data[Invite_Cur].reset();
			SIP_Invite_Num--;
		}
		return 0;
	}
	else
		return -1;
}

int CSIP_Server::Clean_All_Invite_Data()
{
	CRASH_DEBUG_COMMAND;
	for (int Cur=0; Cur<SIP_INVITE_SIZE ; Cur++)
	{
		if (SIP_Invite_Data[Cur].Flag == TRUE)
		{
			Clear_Invite_By_Cur(Cur);
		}
	}
	SIP_Invite_Num=0;
	RETURN 0;
}

int CSIP_Server::Save_Invite(SIP_MSG *lpSIP_Msg)
{
	int Invite_Cur;
	if ((Invite_Cur = Check_Invite(lpSIP_Msg)) < 0) //檢查是否存在
	{
		for (Invite_Cur = 0; Invite_Cur < SIP_INVITE_SIZE; Invite_Cur++)
			if (!SIP_Invite_Data[Invite_Cur].Flag)
				break; //找尋空位儲存
		if (Invite_Cur >= SIP_INVITE_SIZE)
			return -2; //表示沒有空位
		Set_Invite(lpSIP_Msg, Invite_Cur, TRUE);
		// Set File name
		ftime(&SIP_Invite_Data[Invite_Cur].Invite_Time);
		gettimeofday(&SIP_Invite_Data[Invite_Cur].Start_TimeDest, NULL);
		SIP_Invite_Num++;
		SIP_Invite_Data[Invite_Cur].Flag = TRUE;
	}
	else
		Set_Invite(lpSIP_Msg, Invite_Cur, FALSE);

	return Invite_Cur;
}

int CSIP_Server::Set_Invite(SIP_MSG *lpSIP_Msg, int Invite_Cur, bool First_Flag)
{
	CRASH_DEBUG_COMMAND;
	int Online_Cur;

	// Set Inivte Data
	SIP_Invite_Data[Invite_Cur].pFromDest = &lpSIP_Msg->FromDest;
	// SIP_Invite_Data[Invite_Cur].pFromNc = &lpSIP_Msg->FromNc;
	// memcpy(&SIP_Invite_Data[Invite_Cur].FAddr, &lpSIP_Msg->FAddr, sizeof(SIP_Invite_Data[Invite_Cur].FAddr));
	// strcpy(SIP_Invite_Data[Invite_Cur].FR_IP, lpSIP_Msg->FR_IP);
	// strcpy(SIP_Invite_Data[Invite_Cur].FV_IP, lpSIP_Msg->FV_IP);
	// SIP_Invite_Data[Invite_Cur].From_Crypto_Type = lpSIP_Msg->From_Crypto_Type;

	SIP_Invite_Data[Invite_Cur].pToDest = &lpSIP_Msg->ToDest;
	// SIP_Invite_Data[Invite_Cur].pToNc = &lpSIP_Msg->ToNc;
	// memcpy(&SIP_Invite_Data[Invite_Cur].TAddr, &lpSIP_Msg->TAddr, sizeof(SIP_Invite_Data[Invite_Cur].TAddr));
	// strcpy(SIP_Invite_Data[Invite_Cur].TR_IP, lpSIP_Msg->TR_IP);
	// strcpy(SIP_Invite_Data[Invite_Cur].TV_IP, lpSIP_Msg->TV_IP);
	// SIP_Invite_Data[Invite_Cur].To_Crypto_Type = lpSIP_Msg->To_Crypto_Type;

	strcpy(SIP_Invite_Data[Invite_Cur].From_PhoneNo, lpSIP_Msg->From_Name);
	strcpy(SIP_Invite_Data[Invite_Cur].To_PhoneNo, lpSIP_Msg->To_Name);
	strcpy(SIP_Invite_Data[Invite_Cur].Forwarded_PhoneNo, lpSIP_Msg->To_Name);
	strcpy(SIP_Invite_Data[Invite_Cur].CallerID_Name, lpSIP_Msg->CallerID_Name);
	strcpy(SIP_Invite_Data[Invite_Cur].CallerID, lpSIP_Msg->CallerID);
	if ((Online_Cur = Check_OnLine(lpSIP_Msg->From_Name)) >= 0)
		strcpy(SIP_Invite_Data[Invite_Cur].From_User_Name, SIP_OnLine_Data[Online_Cur].Name);
	if ((Online_Cur = Check_OnLine(lpSIP_Msg->To_Name)) >= 0)
		strcpy(SIP_Invite_Data[Invite_Cur].To_User_Name, SIP_OnLine_Data[Online_Cur].Name);
	if (First_Flag == TRUE)
	{
		// Set Cancel_Branch
		sprintf(SIP_Invite_Data[Invite_Cur].Cancel_Branch, "z9hG4bK%d%d", rand(), rand());
		strcpy(lpSIP_Msg->Cancel_Branch, SIP_Invite_Data[Invite_Cur].Cancel_Branch);
	}

	RETURN 0;
}

int CSIP_Server::Check_Invite(SIP_MSG *lpSIP_Msg)
{
	int i, Cur;
	for (i = 0, Cur = 0; Cur < SIP_INVITE_SIZE; Cur++)
	{
		if (SIP_Invite_Num == 0)
		{
			Cur = SIP_INVITE_SIZE;
			break;
		}
		if (SIP_Invite_Data[Cur].Flag == TRUE)
		{
			if (!strcmp(SIP_Invite_Data[Cur].CallerID_Name, lpSIP_Msg->CallerID_Name))
				break;
			// Calculate i
			i++;
			if (i >= SIP_Invite_Num)
			{
				Cur = SIP_INVITE_SIZE;
				break;
			}
		}
	}
	if (Cur < SIP_INVITE_SIZE)
		return Cur;
	else
		return -1;
}

int CSIP_Server::Write_Invite_Log(SIP_INVITE_DATA *lpInvite_Data, ANSWER_STATE State)
{
	FILE *fp;
	char File_Name[_MAX_PATH], S2[500], S3[200], ElapsedTime[30], StartTime[30], EndTime[30], AnswerState[30];
	int i, j, Len, Item = 0;
	timeb Elapsed_Time;
	std::string timestr;

	if (Invite_Log_Check)
	{
		if (!lpInvite_Data->Flag && lpInvite_Data->Answer_State == Temporary)
			return -1;
		File_Name[0] = S2[0] = S3[0] = ElapsedTime[0] = StartTime[0] = EndTime[0] = AnswerState[0] = '\0';

		if (Log_Path[(j = strlen(Log_Path) - 1)] == '\\' || Log_Path[j] == '/')
		{
			strcpy(File_Name, Log_Path);
			File_Name[j] = 0;
		}
		sprintf(File_Name, "%s\\%s", File_Name, Jstringftime(timestr, "%Y-%m-%d", &lpInvite_Data->Invite_Time)->c_str());
		Create_Directory(File_Name);
		sprintf(File_Name, "%s\\INVITE_Log %s.txt", File_Name, Jstringftime(timestr, "%Y-%m-%d", &lpInvite_Data->Invite_Time)->c_str());
		if ((fp = fopen(File_Name, "a+")) == NULL)
			return -1;
		else
		{
			// Find Item
			while (fgets(S3, sizeof(S3), fp) != NULL)
				;
			sscanf(S3, "%d %s", &Item, S3);
			if (Item >= 0)
				Item++;
			// First Line Text
			fseek(fp, 0, SEEK_END);
			Len = ftell(fp);
			fseek(fp, 0, SEEK_SET); //get text length
			if (Len == 0)
			{
				fprintf(fp, " Item    Date    Caller_Name (PhoneNo)-->Called_Name (PhoneNo) Answer_State Elapsed_Time Start_Time  End_Time      From_Real_IP (From_Virtual_IP)-->To_Real_IP (To_Virtual_IP)   \r\n");
				fprintf(fp, "------ --------- --------------------------------------------- ------------ ------------ ----------  --------   -----------------------------------------------------------------\r\n");
			}
			//---- Invite_Log Process ----------
			// Item
			sprintf(S2, "%d", Item);
			strncat(S2, "      ", (strlen(" Item ") - strlen(S2)) * sizeof(char));
			// Date
			sprintf(S2, "%s %s", S2, Jstringftime(timestr, "%Y/%m/%d", &lpInvite_Data->Invite_Time)->c_str());
			// Caller_Name (PhoneNo)-->
			sprintf(S3, "%s (%s)", lpInvite_Data->From_User_Name, lpInvite_Data->From_PhoneNo);
			i = strlen("Caller_Name (PhoneNo)") - strlen(S3);
			if (i > 0)
				strncat(S2, "                     ", i * sizeof(char));
			sprintf(S2, "%s%s-->", S2, S3);
			// Called_Name (PhoneNo)
			sprintf(S3, "%s (%s)", lpInvite_Data->To_User_Name, lpInvite_Data->To_PhoneNo);
			i = strlen("Called_Name (PhoneNo)") - strlen(S3) + 2;
			sprintf(S2, "%s%s ", S2, S3);
			if (i > 0)
				strncat(S2, "                     ", i * sizeof(char));
			// Answer_State
			if (State == Answer)
				strcpy(AnswerState, "Answered");
			else if (State == Abandon)
				strcpy(AnswerState, "Abandons");
			else if (State == Busy)
				strcpy(AnswerState, "Busy");
			else if (State == Forwarding)
				strcpy(AnswerState, "Forwarding");
			else if (State == Issue)
				strcpy(AnswerState, "Issue");
			else
				strcpy(AnswerState, "UnKnow");
			i = strlen("Answer_State") - strlen(AnswerState) - 2;
			sprintf(S2, "%s%s ", S2, AnswerState);
			if (i > 0)
				strncat(S2, "            ", i * sizeof(char));
			// Time Process
			if (lpInvite_Data->Start_Time.time != 0 && lpInvite_Data->End_Time.time != 0)
			{
				// Elapsed_Time
				Elapsed_Time.time = lpInvite_Data->End_Time.time - lpInvite_Data->Start_Time.time;
				if (Elapsed_Time.time > 3600)
					Jstrftime(ElapsedTime, sizeof(ElapsedTime), "%H:%M:%S", &Elapsed_Time);
				else
					Jstrftime(ElapsedTime, sizeof(ElapsedTime), "%M:%S", &Elapsed_Time);
				// Start_Time & End_Time
				Jstrftime(StartTime, sizeof(StartTime), "%H:%M:%S", &lpInvite_Data->Start_Time);
				Jstrftime(StartTime, sizeof(StartTime), "%H:%M:%S", &lpInvite_Data->End_Time);
				sprintf(S2, "%s  %s    %s   %s   ", S2, ElapsedTime, StartTime, EndTime);
			}
			else if (lpInvite_Data->Start_Time.time == 0)
			{
				sprintf(S2, "%s              %s              ", S2, Jstringftime(timestr, "%H:%M:%S", &lpInvite_Data->Invite_Time)->c_str());
			}
			else
				sprintf(S2, "%s                                    ", S2);
			// From_Real_IP (From_Virtual_IP)  -->
			sprintf(S3, "%s (%s)", lpInvite_Data->pFromDest->Real_IP, lpInvite_Data->pFromDest->Virtual_IP);
			i = strlen(" From_Real_IP (From_Virtual_IP)  ") - strlen(S3);
			if (i > 0)
				strncat(S2, "                                ", i * sizeof(char));
			sprintf(S2, "%s%s-->", S2, S3);
			// To_Real_IP (To_Virtual_IP)
			sprintf(S3, "%s (%s)", lpInvite_Data->pToDest->Real_IP, lpInvite_Data->pToDest->Virtual_IP);
			i = strlen("  End_Time To_Real_IP (To_Virtual_IP) ") - strlen(S3);
			sprintf(S2, "%s%s", S2, S3);
			if (i > 0)
				strncat(S2, "                                    ", i * sizeof(char));
			sprintf(S2, "%s\r\n", S2);
			//------------------------------------
			fprintf(fp, "%s", S2);
			fclose(fp);
		}
	}
	return 0;
}

int CSIP_Server::Modify_NOTIFY(SIP_MSG *lpSIP_Msg)
{
	char *Start_Cur;

	if ((Start_Cur = strstr(lpSIP_Msg->Cmd_Str, "\r\n\r\n")) != NULL)
		*(Start_Cur + 2) = 0;

	return 0;
}

int CSIP_Server::Write_Call_Log(const char *Head, char *S1, SIP_MSG *lpSIP_Msg, sockaddr_mix *lpAddr)
{
	CRASH_DEBUG_COMMAND;
	if (!Head || !S1)
	{
		DTrace_FatalError("invalid parameters, %d @ %s\n", __LINE__, __FILE__);
		RETURN - 1;
	}
	if (!m_lpNetIF)
	{
		DTrace_FatalError("CNetIF instance is invalid, %d @ %s\n", __LINE__, __FILE__);
		RETURN - 2;
	}

	char Answer_State_Table[10][20] = {"NoCall", "Inviting", "Abandon", "Answer", "Busy", "Forwarding", "Terminating", "Issue", "Temporary"};
	char Command_State_Table[20][20] = {"SIP_READY", "SIP_INVITE", "SIP_CANCEL", "SIP_BYE", "SIP_100", "SIP_REG", "SIP_REFER", "SIP_NOTIFY", "SIP_ERROR", "SIP_TALK", "SIP_FAIL", "SIP_RELAY_CONNECT", "SIP_RELAY_READ", "SIP_200", "SIP_ACK", "SIP_PROXY_AUTH"};
	std::string timestr;
	// timeb	Time1;
	// SYSTEMTIME	TimeDest;
	timeval TimeDest;
	timeb Elapsed_Time;
	uint32_t Crt_Hour, Crt_Min, Crt_Sec, Crt_mSec;
	char Crt_Time[50];
	FILE *HsfOut1;
	int i, j, k, Invite_Cur, Online_Cur, Cmd, Len;
	char Name[100], File_Name[_MAX_PATH], IP[100], Command[100], Command_Line[SIP_MSG_LEGNTH], Buf[100], Buf1[100], *Start_Cur;
	//	char	S2[SIP_MSG_LEGNTH], S3[SIP_MSG_LEGNTH]
	char Buf_IP[200];
	char Space_Time[100], Space_Flow[100], Dotted_line[100];
	char szCrypto_Type[20];
	WORD Port;
	bool Command_Type, SDP_Flag, Transmission_Flag;
	SIP_INVITE_DATA *lpInvite_Data;
	CRYPTO_TYPE Crypto_Type;

	if (lpSIP_Msg != NULL)
		Invite_Cur = Check_Invite(lpSIP_Msg);
	else
	{
		RETURN - 1;
	}
	gettimeofday(&TimeDest, NULL);
	// Get SIP_Command
	for (i = 0; i < SIP_COMMAND_NO; i++)
	{
		if (!strncmp(&SIP_COMMAND[i][0], S1, strlen(&SIP_COMMAND[i][0])))
		{
			Cmd = i;
			break;
		}
	}
	Command_Type = (Cmd != (SIP_COMMAND_NO - 1)) ? TRUE : FALSE; // Command_Type: TRUE表示Request,  FALSE表示Response
	strcpy(Command, (Command_Type) ? SIP_COMMAND[Cmd] : lpSIP_Msg->Command);
	// Get Command Line
	if (!strcmp(Head, "send") || !strcmp(Head, "recv"))
		sprintf(Command_Line, "%s\r\n", S1);
	else if ((i = Get_NextLine(S1)) > 0)
	{
		strncpy(Command_Line, S1, i);
		Command_Line[i] = 0;
	}

	if (Invite_Cur >= 0)
	{
		lpInvite_Data = &SIP_Invite_Data[Invite_Cur];
		//----- Write Call Log -----
		// Open Call_Log file
		if (Log_Path[(k = strlen(Log_Path) - 1)] == '\\' || Log_Path[k] == '/')
		{
			strcpy(File_Name, Log_Path);
			File_Name[k] = 0;
		}
		sprintf(File_Name, "%s\\%s", File_Name, SIP_Invite_Data[Invite_Cur].Log_File_Name);
		Create_Directory(File_Name);
		sprintf(Name, "%s\\Call_Log.txt", File_Name);
		//-------------
		Get_Network_Mothod(Head, lpSIP_Msg, Crypto_Type);
		// Crypto Type
		if (Crypto_Type == CT_NoCrypto)
			strcpy(szCrypto_Type, "NoCrypto");
		else if (Crypto_Type == CT_SIPCrypto)
			strcpy(szCrypto_Type, "SIP_Crypto");
		else if (Crypto_Type == CT_Walkersun1)
			strcpy(szCrypto_Type, "Walkersun1_Crypto");
		else
			strcpy(szCrypto_Type, "Crypto_Error");
		// Write Call Log to file
		if (Call_Log_Check)
		{
			if ((HsfOut1 = fopen(Name, "a+")) != NULL)
			{
				if (lpAddr != NULL)
				{
					strcpy(IP, inet_ntoa(lpAddr->sin_addr));
					Port = ntohs(lpAddr->sin_port);
				}
				else
				{
					IP[0] = 0;
					Port = 0;
				}
				fprintf(HsfOut1, "         << %s %s %s:%d  %s >>\r\n", szCrypto_Type, Head, IP, Port, Jstringftime(timestr, "%Y.%m.%d %H:%M:%S", NULL)->c_str());
				fprintf(HsfOut1, "%s\r\n\r\n", S1);
				fclose(HsfOut1);
			}
		}
		/*		// Call Log '\r'->'' ,'\n'->''版
		sprintf(Name, "%s\\Call_Log(0x6 0x2).txt", File_Name);
		// Write Call Log to file
		if (Call_Log_Check)
		{
			if ( (HsfOut1=fopen(Name,"a+"))!=NULL)
			{  
				if (lpAddr!=NULL)	{ strcpy(IP,inet_ntoa(lpAddr->sin_addr)); Port=ntohs(lpAddr->sin_port);}
				else { IP[0]=0; Port=0;}
				strcpy(S2,S1);	
				for (j=0;j<(int)strlen(S2);j++)
				{	
					if (S2[j]=='\r')	{ strcpy(S3,&S2[j+1]); S2[j]=0; strcat(S2,"\\r"); strcat(S2,S3);}
					else if (S2[j]=='\n')	{ strcpy(S3,&S2[j+1]); S2[j]=0; strcat(S2,"\\n\r\n"); j+=3;	strcat(S2,S3);}
				}
				fprintf(HsfOut1,"         << %s %s %s:%d  %s.%d >>\r\n",szCrypto_Type,Head,IP,Port,Time1.Format("%Y.%m.%d %H:%M:%S"),TimeDest.wMilliseconds);
				fprintf(HsfOut1,"%s\r\n\r\n",S2); 
				fclose(HsfOut1);
			}
		}
*/
		//----- Graph Call Flows with each SIP Message -----
		if (0) //Call_Flows_Check)
		{
			if (!strcmp(Head, "sendto") || !strcmp(Head, "recvfrom"))
			{
				sprintf(Name, "%s/Call_Flows.txt", File_Name);
				if ((HsfOut1 = fopen(Name, "a+")) != NULL)
				{
					strcpy(Space_Time, "             ");				  //13個' '
					strcpy(Space_Flow, "                              "); //30個' '
					strcpy(Dotted_line, "-----------------------------"); //29個'-'
					//--- Get Elapsed time
					ftime(&Elapsed_Time);
					Elapsed_Time.time = TimeDest.tv_sec - lpInvite_Data->Start_TimeDest.tv_sec;
					// Elapsed_Time = difftime(TimeDest.tv_sec, lpInvite_Data->Start_TimeDest.tv_sec);
					if (TimeDest.tv_usec < lpInvite_Data->Start_TimeDest.tv_usec)
					{
						Elapsed_Time.time--;
						Elapsed_Time.millitm = (TimeDest.tv_usec - lpInvite_Data->Start_TimeDest.tv_usec + 1000000) / 1000;
					}
					else
						Elapsed_Time.millitm = (TimeDest.tv_usec - lpInvite_Data->Start_TimeDest.tv_usec) / 1000;
					//--- Get Elapsed time String
					if (Elapsed_Time.time > 3600)
						sprintf(Crt_Time, "%s%s", Jstringftime(timestr, "%H:%M:%S.%f", &Elapsed_Time)->c_str(), Space_Time);
					else if (Elapsed_Time.time > 60)
						sprintf(Crt_Time, "%s%s", Jstringftime(timestr, "%M:%S.%f", &Elapsed_Time)->c_str(), Space_Time);
					else
						sprintf(Crt_Time, "%s%s", Jstringftime(timestr, "%s.%f", &Elapsed_Time)->c_str(), Space_Time);
					Crt_Time[strlen(Space_Time)] = 0;
					//--- Get Command
					SDP_Flag = (atoi(lpSIP_Msg->Content_Length) > 0 && (!strcmp(Command, "INVITE") || !strcmp(Command, "183") || !strcmp(Command, "200"))) ? TRUE : FALSE;
					if (Command_Type || (Start_Cur = strstr(S1, lpSIP_Msg->Command)) == NULL)
						sprintf(Buf, "   %s %s%s", Command, SDP_Flag ? "SDP" : "", Space_Flow);
					else
					{
						strcpy(Buf1, Start_Cur);
						for (j = 0; j < (int)strlen(Buf); j++)
							if (Buf1[j] == '\r' || Buf1[j] == '\n')
							{
								Buf1[j] = 0;
								break;
							}
						sprintf(Buf, "   %s %s%s", Buf1, SDP_Flag ? "SDP" : "", Space_Flow);
						Command_Type = FALSE;
					}
					Buf[30] = 0;
					//--- Graph flows ---
					// First Line Text
					fseek(HsfOut1, 0, SEEK_END);
					Len = ftell(HsfOut1);
					fseek(HsfOut1, 0, SEEK_SET); //get text length
					if (Len == 0)
					{
						fprintf(HsfOut1, "          Caller                         Server                         Callee\r\n");
						sprintf(Buf_IP, "%s:%d%s", lpInvite_Data->pFromDest->Real_IP, lpInvite_Data->pFromDest->Port, Space_Flow);
						Buf_IP[35] = 0;
						sprintf(Buf_IP, "%s%s%s", Buf_IP, Local_IP, Space_Flow);
						Buf_IP[62] = 0;
						sprintf(Buf_IP, "%s%s:%d", Buf_IP, lpInvite_Data->pToDest->Real_IP, lpInvite_Data->pToDest->Port);
						fprintf(HsfOut1, "  %s\r\n", Buf_IP);
						if (!(!strcmp(lpSIP_Msg->Command, "BYE") && !Addrcmp(&lpSIP_Msg->FromDest.Raddr, &lpInvite_Data->pToDest->Raddr)))
						{ // It indicate Call flows access in the end if Condition True, so don't handle it
							sprintf(Buf_IP, "%s:%s%s", lpSIP_Msg->FromDest.Virtual_IP, lpSIP_Msg->Via_Port, Space_Flow);
							Buf_IP[38] = 0;
							if ((Online_Cur = Check_OnLine(lpSIP_Msg->From_Name)) >= 0)
								sprintf(Buf_IP, "%s%d|", Buf_IP, SIP_OnLine_Data[Online_Cur].Server_Port);
							if ((Online_Cur = Check_OnLine(lpSIP_Msg->To_Name)) >= 0)
							{
								sprintf(Buf_IP, "%s%d%s", Buf_IP, SIP_OnLine_Data[Online_Cur].Server_Port, Space_Flow);
								Buf_IP[66] = 0;
							}
							sprintf(Buf_IP, "%s%s", Buf_IP, lpSIP_Msg->ToDest.Virtual_IP);
							fprintf(HsfOut1, "  %s\r\n", Buf_IP);
						}
					}

					if (!strcmp(Head, "recvfrom"))
						Transmission_Flag = TRUE;
					else if (!strcmp(Head, "sendto"))
						Transmission_Flag = FALSE;
					switch (Check_Flow_State(lpSIP_Msg, Transmission_Flag))
					{
					// Recvfrom and -->
					case 0:
					case 3:
						fprintf(HsfOut1, "%s|%s|%s|\r\n", Crt_Time, Buf, Space_Flow);
						fprintf(HsfOut1, "%s|%s>|%s|\r\n", Space_Time, Dotted_line, Space_Flow);
						break;
					// Recvfrom and <--
					case 1:
					case 2:
						fprintf(HsfOut1, "%s|%s|%s|\r\n", Crt_Time, Space_Flow, Buf);
						fprintf(HsfOut1, "%s|%s|<%s|\r\n", Space_Time, Space_Flow, Dotted_line);
						break;
					// Sendto and -->
					case 4:
					case 7:
						fprintf(HsfOut1, "%s|%s|%s|\r\n", Crt_Time, Space_Flow, Buf);
						fprintf(HsfOut1, "%s|%s|%s>|\r\n", Space_Time, Space_Flow, Dotted_line);
						break;
					// Sendto and <--
					case 5:
					case 6:
						fprintf(HsfOut1, "%s|%s|%s|\r\n", Crt_Time, Buf, Space_Flow);
						fprintf(HsfOut1, "%s|<%s|%s|\r\n", Space_Time, Dotted_line, Space_Flow);
						break;
					}
					fclose(HsfOut1);
				}
			}
		}
	}
	RETURN 0;
}

int CSIP_Server::Get_Response_Code(char *Response_Code, int Code)
{
	int i;
	char Buf[10];

	if (lpSIP_Event_Table[0] == NULL)
	{ // Set pSIP_Event_Table for First Time
		for (i = 0; (int)strlen(SIP_Event_Table[i]) != 0; i++)
		{
			strncpy(Buf, SIP_Event_Table[i], 3);
			Buf[3] = 0;
			lpSIP_Event_Table[atoi(Buf) - 100] = SIP_Event_Table[i];
		}
	}
	if (Response_Code == NULL)
		return -1;
	// Get Response Code
	if (100 <= Code && Code < 700)
	{
		if (lpSIP_Event_Table[Code - 100] != NULL)
			strcpy(Response_Code, lpSIP_Event_Table[Code - 100]);
		else
			sprintf(Response_Code, "%d %s", Code, Status_Msg[(int)(Code / 100)]);
	}
	else
		Response_Code[0] = 0;
	return 0;
}

int CSIP_Server::Send_Relay_Command(const char *Command, SIP_MSG *lpSIP_Msg, COMMAND_QUEUE *lpCommand)
{
	if (!Command || lpSIP_Msg)
	{
		DTrace_FatalError("Send_Relay_Command -- invalid param");
		return -1;
	}

	int Total = 0; //Get_Total(wParam,"0RL",RTP_Data[Cur].SIP_Msg.FV_IP);
	char S1[SIP_MSG_LEGNTH];
	int len;

	if ((lpSIP_Msg->RL1_Flag) && (strcmp(Command, "0RL1")))
		return -2;
	if (!lpCommand->Relay_TCP_Dest.is_used())
		return -3;
	sprintf(S1, "%s|%s|%s|%s|%s|%s|%s|%s|%s|%u", Command,
			lpSIP_Msg->From_Name,
			lpSIP_Msg->To_Name,
			lpSIP_Msg->FromDest.Real_IP,
			lpSIP_Msg->ToDest.Real_IP,
			lpSIP_Msg->FromDest.Virtual_IP,
			lpSIP_Msg->ToDest.Virtual_IP,
			lpSIP_Msg->SDP_IP,
			lpSIP_Msg->SDP_Port,
			Total);
	Write_Log("send", S1, NULL, lpSIP_Msg);
	len = Hsf_send(0, S1, strlen(S1), 0);
	m_lpNetIF->NetNcSend(lpCommand->Relay_TCP_Dest.pNc, S1, len, NULL);
	if (!strcmp(Command, "0RL1"))
	{
		lpSIP_Msg->RL1_Flag = FALSE;
		lpCommand->Relay_TCP_Dest.clean_nc();
		lpCommand->Relay_TCP_Dest.reset();
	}
	return 0;
}

bool CSIP_Server::Addrcmp(sockaddr_mix *lpAddr1, sockaddr_mix *lpAddr2)
{
	char IP1[100], IP2[100];
	WORD Port1, Port2;

	if ((lpAddr1->sin_addr.s_addr == lpAddr2->sin_addr.s_addr) && lpAddr1->sin_port == lpAddr2->sin_port)
		return false;
	else
		return true;
}

// int CSIP_Server::Create_Directory(char *szPath)
// {
// 	int		i, j, Start_Cur;
// 	char	Buf[MAX_PATH];
// 	char	DirBuf[20][100];

// 	if ((int)strlen(szPath)>MAX_PATH || !strcmp(szPath, ""))	return -1;
// 	else
// 	{
// 		memset(DirBuf, 0, sizeof(DirBuf));
// 		strcpy(Buf, szPath);
// 		for (i=j=Start_Cur=0; i<(int)strlen(Buf); i++)
// 		{
// 			if (Buf[i]=='\\' || Buf[i]=='/')
// 			{
// 				if (!(i==2 && Buf[i-1]==':'))	//若第一組有':'則跳過
// 				{
// 					strncpy(DirBuf[j], &Buf[Start_Cur], i-Start_Cur); DirBuf[j++][i-Start_Cur]='\0';
// 					Start_Cur=i+1;
// 				}
// 			}
// 			else if (i==((int)strlen(Buf)-1))
// 			{	// 當最後面沒有'/' or '\'
// 				strncpy(DirBuf[j], &Buf[Start_Cur], i-Start_Cur+1); DirBuf[j++][i-Start_Cur+1]='\0';
// 				Start_Cur=i+1;
// 			}
// 		}
// 		// Mkdir
// 		strcpy(Buf,  "");
// 		for (i=0;(int)strlen(DirBuf[i])!=0;i++)
// 		{
// 			sprintf(Buf, "%s%s\\", Buf, DirBuf[i]);
// 			mkdir(Buf, 755);
// 		}
// 	}
// 	return 0;
// }

int CSIP_Server::Delete_OneLine(char *S1, const char *Head)
{
	int i, j;

	if ((i = Get_Position(S1, Head)) > 0)
	{
		if ((j = Get_NextLine(&S1[i])) > 0)
		{
			j += i;
			Deletion(S1, i, j - 1);
		}
	}
	return 0;
}

int CSIP_Server::Init_SubServer_Mapping()
{
	Mapping_Num = 0;
	for (int i = 0; i < MAPPING_DATA_SIZE; i++)
	{
		Mapping_Data[i].reset();
	}
	return 0;
}

int CSIP_Server::Init_Replace_Code()
{
	ReplaceCode_Num = 0;
	memset(ReplaceCode_Table, 0, sizeof(ReplaceCode_Table));
	return 0;
}

int CSIP_Server::Check_SubServer_Mapping(char *Name)
{
	int i, Mapping_Cur;

	if (Mapping_Num > 0 && Mapping_Num <= MAPPING_DATA_SIZE)
	{
		for (i = 0, Mapping_Cur = 0; Mapping_Cur < MAPPING_DATA_SIZE; Mapping_Cur++)
		{
			if (Mapping_Data[Mapping_Cur].Flag)
			{
				if (!strcmp(Mapping_Data[Mapping_Cur].Name, Name))
					break;
				// Calculate i
				i++;
				if (i >= Mapping_Num)
				{
					Mapping_Cur = MAPPING_DATA_SIZE;
					break;
				}
			}
		}
	}
	else
		Mapping_Cur = MAPPING_DATA_SIZE;
	if (Mapping_Cur < MAPPING_DATA_SIZE)
		return Mapping_Cur;
	else
		return -1;
}

int CSIP_Server::Save_SubServer_Mapping(MAPPING_DATA *lpMapping_Data)
{
	int Mapping_Cur;

	if ((Mapping_Cur = Check_SubServer_Mapping(lpMapping_Data->Name)) < 0)
	{
		for (Mapping_Cur = 0; Mapping_Cur < MAPPING_DATA_SIZE; Mapping_Cur++)
		{
			if (!Mapping_Data[Mapping_Cur].Flag)
				break;
		}
		if (Mapping_Cur < MAPPING_DATA_SIZE)
		{
			memcpy(&Mapping_Data[Mapping_Cur], lpMapping_Data, sizeof(MAPPING_DATA));
			Mapping_Data[Mapping_Cur].Flag = TRUE;
			Mapping_Num++;
			return Mapping_Cur;
		}
		else
			return -1;
	}
	else
		return Mapping_Cur;
}

int CSIP_Server::Update_SubServer_Data(const char *SubServer_IP, unsigned short SubServer_Port, CRYPTO_TYPE Crypto_Type)
{
	int i, Mapping_Cur, Cur;

	if (Mapping_Num > 0 && Mapping_Num <= MAPPING_DATA_SIZE)
	{
		for (i = 0, Mapping_Cur = 0; Mapping_Cur < MAPPING_DATA_SIZE; Mapping_Cur++)
		{
			if (Mapping_Data[Mapping_Cur].Flag)
			{
				for (Cur = 0; Cur < SERVER_PORT_NUM; Cur++)
					if (Mapping_Data[Mapping_Cur].SubServer_Data[Cur].is_used())
					{
						if (!strcmp(Mapping_Data[Mapping_Cur].SubServer_Data[Cur].Real_IP, SubServer_IP) &&
							Mapping_Data[Mapping_Cur].SubServer_Data[Cur].Port == SubServer_Port)
						{
							Set_SubServer_Data(&Mapping_Data[Mapping_Cur].SubServer_Data[Cur], SubServer_IP, SubServer_Port, Crypto_Type);
						}
					}
				// Calculate i
				i++;
				if (i >= Mapping_Num)
				{
					Mapping_Cur = MAPPING_DATA_SIZE;
					break;
				}
			}
		}
	}
	else
		Mapping_Cur = MAPPING_DATA_SIZE;
	if (Mapping_Cur >= MAPPING_DATA_SIZE)
		return -1;

	return 0;
}

int CSIP_Server::Clear_SubServer_Mapping(int Mapping_Cur)
{
	CRASH_DEBUG_COMMAND;
	if (Mapping_Cur < 0 || Mapping_Cur >= MAPPING_DATA_SIZE)
	{
		RETURN - 1;
	}
	if (Mapping_Data[Mapping_Cur].Flag)
	{

		for (int i = 0; i < SERVER_PORT_NUM; i++)
		{
			if (Mapping_Data[Mapping_Cur].SubServer_Data[i].is_used())
			{
				Mapping_Data[Mapping_Cur].SubServer_Data[i].clean_nc();
			}
		}
		Mapping_Data[Mapping_Cur].reset();
		Mapping_Num--;
	}

	RETURN 0;
}

int CSIP_Server::Clean_All_SubServer_Mapping()
{
	CRASH_DEBUG_COMMAND;
	int Cur;

	for (Cur = 0; Cur < MAPPING_DATA_SIZE; Cur++)
	{
		Clear_SubServer_Mapping(Cur);
	}
	Mapping_Num = 0;
	RETURN 0;
}

int CSIP_Server::Init_SubServer_Data()
{
	SubServer_Num = 0;
	for (int i = 0; i < SUBSERVER_DATA_SIZE; i++)
		SubServer_Data[i].reset();
	return 0;
}

int CSIP_Server::Set_SubServer_Data(DEST_DATA *lpSubServer_Data, const char *SubServer_IP, unsigned short SubServer_Port, CRYPTO_TYPE Crypto_Type)
{
	int i;
	sockaddr_mix addr;

	// 直接建立連線
	for (i = 0; i < SERVER_PORT_NUM; i++)
		if (SIP_Server_Port[i] == SubServer_Port)
			break;
	if (i < SERVER_PORT_NUM)
	{ // 屬於Server_Port
		if (NetMakeAddrin(&addr, SubServer_IP, SubServer_Port) < 0)
			return INVALID_ADDR_FORM;
		lpSubServer_Data->pack(&SIP_Server_Nc[i], &addr, SubServer_IP, SubServer_IP, Crypto_Type);
		// lpSubServer_Data->Nc.copy_from(&SIP_Server_Nc[i]);
		// strcpy(lpSubServer_Data->R_IP, SubServer_IP);
		// lpSubServer_Data->Port = SubServer_Port;
		// lpSubServer_Data->Crypto_Type = Crypto_Type;
	}
	// 不屬於Server_Port
	else
	{
		Jstrncpy(lpSubServer_Data->Real_IP, SubServer_IP, sizeof(lpSubServer_Data->Real_IP));
		lpSubServer_Data->Port = SubServer_Port;
		if (m_lpNetIF)
			m_lpNetIF->StartDestClient(lpSubServer_Data, AF_INET, SOCK_DGRAM, CT_SIPCrypto, SIP_SERVER_PORT, SubServer_IP, SubServer_Port, false, SocketModule_Callbcak_Function, this, SIP_SERVER_EVENT);
	}
	return 0;
}

BOOL CSIP_Server::URL_Filter(char *Url)
{
	char *Token, URL[255], DN_Value[10][100];
	int i, j, IP_Value[4], DN_Size, IP_Size, DN_Num;
	BOOL Filter_Flag;

	// Filter_Flag=2: Domain NAme OK
	// Filter_Flag=1: Pure IP OK
	// Filter_Flag=0: URL FALSE

	if (strcmp(Url, "") && strlen(Url) < 255)
	{
		// Initial
		Filter_Flag = 0;
		for (i = 0; i < 4; i++)
			IP_Value[i] = -1;
		DN_Size = sizeof(DN_Value) / sizeof(DN_Value[0]);
		IP_Size = sizeof(IP_Value) / sizeof(IP_Value[0]);
		memset(DN_Value, 0, sizeof(DN_Value));
		// Check URL
		strcpy(URL, Url);
		for (i = 0; i < (int)strlen(URL); i++)
			if ((URL[i] < '0' || URL[i] > '9') && URL[i] != '.')
				break;
		if (i < (int)strlen(Url))
		{ // Domain Name
			for (DN_Num = j = 0; j < (int)strlen(URL); j++)
			{
				if (URL[j] == '.')
					DN_Num++;
			}
			Filter_Flag = (DN_Num >= 2) ? 2 : 0; // Filter_Flag=2 ==> Domain Name OK
		}
		else
		{ // Pure IP
			for (j = 0; (Token = strtok((j == 0) ? URL : NULL, ".")) != NULL; j++)
			{
				if (strcmp(Token, ""))
					IP_Value[j] = atoi(Token);
				else
					break;
			}
			for (j = 0; j < IP_Size; j++)
				if (IP_Value[j] < 0 || IP_Value[j] > 255)
					break;
			Filter_Flag = (j >= IP_Size) ? 1 : 0; // Filter_Flag=1 ==> Pure IP OK
		}
	}
	else
		Filter_Flag = 0;

	return Filter_Flag;
}

int CSIP_Server::SIP_Syntax_Filter(SIP_MSG *lpSIP_Msg)
{
	int Error_Code;

	Error_Code = 0;

	// Check From_Name & To_Name
	if (!strcmp(lpSIP_Msg->From_Name, "") || !strcmp(lpSIP_Msg->To_Name, ""))
		Error_Code = 400;

	return -Error_Code;
}

int CSIP_Server::Domain_Name_Analysis(char *Url, char *IP)
{
	//Method: Domain_Name_Analysis(Trunk_GW_IP,Trunk_GW_IP);
	int i, err;
	char URL[100], Error[256];
	hostent *lpHost;
	// WSADATA  wsadata;

	if (URL_Filter(Url))
	{
		strcpy(URL, Url);
		// Get IP from Domain Name
		for (i = 0; i < (int)strlen(URL); i++)
			if ((URL[i] < '0' || URL[i] > '9') && URL[i] != '.')
				break;
		if (i < (int)strlen(URL))
		{
			// 啟動Winsock
			// if ((err=WSAStartup(0x202,&wsadata))!=0)	{ WSA_Get_ErrTxt(Error); return -1; }
			if ((lpHost = gethostbyname(URL)) != NULL)
				strcpy(IP, inet_ntoa(*(LPIN_ADDR)(lpHost->h_addr)));
			else
				return -1;
		}
		else
			strcpy(IP, URL);
	}
	else
		return -1;

	return 0;
}

bool CSIP_Server::Port_Filter(char *Port)
{
	char Buf[16];
	bool Filter_Flag;

	// Initial
	Filter_Flag = FALSE;
	// Check Port
	sprintf(Buf, "%d", atoi(Port));
	if ((atoi(Port) <= 0 || atoi(Port) >= 65536) || strcmp(Buf, Port))
		Filter_Flag = FALSE;
	else
		Filter_Flag = TRUE;

	return Filter_Flag;
}

int CSIP_Server::Trunk_GW_Set(DEST_DATA *lpTrunk_GW, char *Url)
{
	char Trunk_GW_Url[100], Trunk_GW_IP[100], Trunk_GW_Port[16], *Token;
	bool DN_Flag;
	//--- Trunking GW Process ---
	Trunk_GW_Url[0] = Trunk_GW_IP[0] = Trunk_GW_Port[0] = 0;
	DN_Flag = FALSE;
	strcpy(Trunk_GW_Url, Url);
	if (strcmp(Trunk_GW_Url, ""))
	{
		if ((Token = strtok(Trunk_GW_Url, ":")) != NULL)
			strcpy(Trunk_GW_IP, Token);
		if ((Token = strtok(NULL, ":")) != NULL)
			strcpy(Trunk_GW_Port, (strcmp(Trunk_GW_Port, "")) ? Token : "5060");
		else
			strcpy(Trunk_GW_Port, "5060");
		if (Domain_Name_Analysis(Trunk_GW_IP, Trunk_GW_IP) < 0)
			DN_Flag = FALSE;
		else
			DN_Flag = TRUE;
	}
	// Set Trunk_GW_Data
	if ((URL_Filter(Trunk_GW_IP) && Port_Filter(Trunk_GW_Port)) && DN_Flag)
	{ // Build Trunking GW Connection
		// lpTrunk_GW->Flag = TRUE;
		// strcpy(lpTrunk_GW->R_IP, Trunk_GW_IP);
		// lpTrunk_GW->Port = Jatoi(Trunk_GW_Port, sizeof(Trunk_GW_Port));
		if (m_lpNetIF)
		{
			if (lpTrunk_GW->is_used())
			{
				lpTrunk_GW->clean_nc();
				lpTrunk_GW->reset();
			}
			m_lpNetIF->StartDestClient(lpTrunk_GW, AF_INET, SOCK_DGRAM, CT_SIPCrypto, SIP_SERVER_PORT, Trunk_GW_IP, atoi(Trunk_GW_Port), false, SocketModule_Callbcak_Function, this, SIP_SERVER_EVENT);
		}
	}
	else
	{ // Set NTUT_GW to Trunk_GW
		lpTrunk_GW->copy_from(&NTUT_GW_Dest);
		// lpTrunk_GW->Flag = TRUE;
		// lpTrunk_GW->Nc.copy_from(&NTUT_GW_Nc);
		// memcpy(&lpTrunk_GW->Addr, &NTUT_GW_Addr, sizeof(sockaddr_in));
		// strcpy(lpTrunk_GW->R_IP, NTUT_GW_IP);
		// lpTrunk_GW->Port = Jatoi(NTUT_GW_Port, sizeof(NTUT_GW_Port));
		// lpTrunk_GW->Crypto_Type = NTUT_GW_Crypto_Type;
	}
	return 0;
}

int CSIP_Server::Update_Trunk_GW(char *Old_GW_IP, unsigned short Old_GW_Port)
{
	int i, Online_Cur;

	for (i = 0, Online_Cur = 0; Online_Cur < SIP_ONLINE_SIZE; Online_Cur++)
	{
		if (SIP_OnLine_Data[Online_Cur].Flag)
		{
			if (SIP_OnLine_Data[Online_Cur].Trunk_GW.is_used() &&
				(!strcmp(SIP_OnLine_Data[Online_Cur].Trunk_GW.Real_IP, Old_GW_IP)) && (Old_GW_Port == SIP_OnLine_Data[Online_Cur].Trunk_GW.Port))
			{
				SIP_OnLine_Data[Online_Cur].Dest.copy_from(&NTUT_GW_Dest);
				// SIP_OnLine_Data[Online_Cur].Trunk_GW.Flag = TRUE;
				// SIP_OnLine_Data[Online_Cur].Trunk_GW.Nc.copy_from(&NTUT_GW_Nc);
				// memcpy(&SIP_OnLine_Data[Online_Cur].Trunk_GW.Addr, &NTUT_GW_Addr, sizeof(sockaddr_in));
				// strcpy(SIP_OnLine_Data[Online_Cur].Trunk_GW.R_IP, NTUT_GW_IP);
				// SIP_OnLine_Data[Online_Cur].Trunk_GW.Port = Jatoi(NTUT_GW_Port, sizeof(NTUT_GW_Port));
				// SIP_OnLine_Data[Online_Cur].Trunk_GW.Crypto_Type = NTUT_GW_Crypto_Type;
			}
			i++;
			if (i >= SIP_OnLine_Num)
			{
				Online_Cur = SIP_ONLINE_SIZE;
				break;
			}
		}
	}
	if (Online_Cur >= SIP_ONLINE_SIZE)
		return -1;
	else
		return 0;
}

int CSIP_Server::Edit_URL(char *S1, const char *Head, char *URL)
{
	int i, j, k, Beg, Mid, Mid11, Mid12, Mid2, Mid31, Mid32, End, Beg1, End1;

	// 2. Search Beg_Point, Beg=關鍵字之後位址
	k = (int)strlen(Head);
	if (k > (int)strlen(S1))
		return 0;
	for (i = 0; i <= (int)strlen(S1) - k; i++) // 需再修正
	{
		if (S1[i] == Head[0])
		{
			for (j = 0; j < k; j++)
				if ((S1[i + j] | 0x20) != (Head[j] | 0x20))
					break; // 大小寫均考慮
			if (j >= k)
			{
				Beg = i + k;
				break;
			}
		}
	}
	if (Beg == 0)
		return 0; // 關鍵字未尋獲

	// 3. Search End_Point, End=關鍵字串句尾
	End = (int)strlen(S1) - 1;
	for (i = Beg; i < End; i++)
		if ((S1[i] == 0x0a) || (S1[i] == 0x0d))
		{
			End = i - 1;
			break;
		}

	// 4. Search Mid(@), Mid1(:), Mid, Mid2(;), Mid31(<). Mid32(>)
	if (Beg < End)
	{ // 4.1 Mid(@)
		for (i = Beg; i <= End; i++)
			if (S1[i] == '@')
			{
				Mid = i;
				break;
			}
		// 4.2 Mid1(:)
		if (Mid > 0)
		{
			for (i = Mid - 1; i >= Beg; i--)
				if (S1[i] == ':')
				{
					Mid11 = i;
					break;
				}
			for (i = Mid + 1; i <= End; i++)
				if (S1[i] == ':')
				{
					Mid12 = i;
					break;
				}
		}
		else
		{
			for (i = Beg; i <= End; i++)
				if (S1[i] == ':')
				{
					Mid11 = i;
					break;
				}
			if (Mid11 > 0)
			{
				for (i = Mid11 + 1; i <= End; i++)
					if (S1[i] == ':')
					{
						Mid12 = i;
						break;
					}
			}
		}
		// 4.3 Mid2(;)
		if (Mid12 > 0)
			Beg1 = Mid12 + 1;
		else if (Mid11 > 0)
			Beg1 = Mid11 + 1;
		else
			Beg1 = Beg;
		if ((Mid > 0) && (Mid > Beg1))
			Beg1 = Mid + 1;
		for (i = Beg1; i <= End; i++)
			if (S1[i] == ';')
			{
				Mid2 = i;
				break;
			}
		// 4.4 Mid31(<)
		for (i = Beg; i <= End; i++)
			if (S1[i] == '<')
			{
				Mid31 = i;
				break;
			}
		// 4.5 Mid32(>)
		if (Mid31 > 0)
			for (i = Mid31 + 1; i <= End; i++)
				if (S1[i] == '>')
				{
					Mid32 = i;
					break;
				}
	}

	// Edit URL
	Beg1 = End1 = 0;
	if (Mid > 0) // hsf@1.1.1.1 ,  @->
	{
		Beg1 = Mid + 1;
		for (i = Beg1; i <= End; i++)
			if ((S1[i] == ';') || (S1[i] == ':') || (S1[i] == '>') || (S1[i] == ' '))
			{
				End1 = i - 1;
				break;
			}
		if (i > End)
			End1 = End;
	}
	else if ((Mid11 > 0) && (Mid11 < Mid12)) // with  sip: --> 1.1.1.1 <-- :5060
	{
		Beg1 = Mid11 + 1;
		End1 = Mid12 - 1;
		if (Beg1 > End1)
		{
			Beg1 = End1 = 0;
		}
	}
	else if ((Mid11 > 0) && (Mid12 == 0)) // with sip:1.1.1.1  or 1.1.1.1:5060
	{
		if (S1[Mid11 - 1] == 'p')
		{
			Beg1 = Mid11 + 1;
			for (i = Beg1; i <= End; i++)
				if ((S1[i] == ';') || (S1[i] == ':') || (S1[i] == '>') || (S1[i] == ' '))
				{
					End1 = i - 1;
					break;
				}
			if (i > End)
				End1 = End;
		}
		else
		{
			End1 = Mid11 - 1;
			for (i = End1; i >= Beg; i--)
				if ((S1[i] == ';') || (S1[i] == ':') || (S1[i] == '<') || (S1[i] == ' '))
				{
					Beg1 = i + 1;
					break;
				}
		}
	}
	else if (Mid11 == 0 && Mid12 == 0 && Mid2 > 0) // SIP/2.0/UDP 1.1.1.1;
	{
		End1 = Mid2 - 1;
		for (i = End1; i >= Beg; i--)
			if (S1[i] == ' ')
			{
				Beg1 = i + 1;
				break;
			}
		if (i < Beg)
			Beg1 = Beg;
	}
	else // INVITE 1.1.1.1  SIP/2.0
	{
		for (i = Beg; i <= End; i++)
			if (S1[i] != ' ')
			{
				Beg1 = i;
				break;
			}
		if (Beg1 > 0)
		{
			for (i = Beg1; i <= End; i++)
				if ((S1[i] == ';') || (S1[i] == ':') || (S1[i] == '>') || (S1[i] == ' '))
				{
					End1 = i - 1;
					break;
				}
			if (i > End)
				End1 = End;
		}
	}
	if ((End1 > 0) && (End1 >= Beg1))
		Edition(S1, Beg1, End1, URL);

	return 0;
}

int CSIP_Server::Edit_Port(char *S1, const char *Head, char *Port)
{
	int i, j, k, Beg, Mid, Mid11, Mid12, Mid2, Mid31, Mid32, End, Beg1, End1;

	// 2. Search Beg_Point, Beg=關鍵字之後位址
	k = (int)strlen(Head);
	if (k > (int)strlen(S1))
		return 0;
	for (i = 0; i <= (int)strlen(S1) - k; i++) // 需再修正
	{
		if (S1[i] == Head[0])
		{
			for (j = 0; j < k; j++)
				if ((S1[i + j] | 0x20) != (Head[j] | 0x20))
					break; // 大小寫均考慮
			if (j >= k)
			{
				Beg = i + k;
				break;
			}
		}
	}
	if (Beg == 0)
		return 0; // 關鍵字未尋獲

	// 3. Search End_Point, End=關鍵字串句尾
	End = (int)strlen(S1) - 1;
	for (i = Beg; i < End; i++)
		if ((S1[i] == 0x0a) || (S1[i] == 0x0d))
		{
			End = i - 1;
			break;
		}

	// 4. Search Mid(@), Mid1(:), Mid, Mid2(;), Mid31(<). Mid32(>)
	if (Beg < End)
	{ // 4.1 Mid(@)
		for (i = Beg; i <= End; i++)
			if (S1[i] == '@')
			{
				Mid = i;
				break;
			}
		// 4.2 Mid1(:)
		if (Mid > 0)
		{
			for (i = Mid - 1; i >= Beg; i--)
				if (S1[i] == ':')
				{
					Mid11 = i;
					break;
				}
			for (i = Mid + 1; i <= End; i++)
				if (S1[i] == ':')
				{
					Mid12 = i;
					break;
				}
		}
		else
		{
			for (i = Beg; i <= End; i++)
				if (S1[i] == ':')
				{
					Mid11 = i;
					break;
				}
			if (Mid11 > 0)
			{
				for (i = Mid11 + 1; i <= End; i++)
					if (S1[i] == ':')
					{
						Mid12 = i;
						break;
					}
			}
		}
		// 4.3 Mid2(;)
		if (Mid12 > 0)
			Beg1 = Mid12 + 1;
		else if (Mid11 > 0)
			Beg1 = Mid11 + 1;
		else
			Beg1 = Beg;
		if ((Mid > 0) && (Mid > Beg1))
			Beg1 = Mid + 1;
		for (i = Beg1; i <= End; i++)
			if (S1[i] == ';')
			{
				Mid2 = i;
				break;
			}
		// 4.4 Mid31(<)
		for (i = Beg; i <= End; i++)
			if (S1[i] == '<')
			{
				Mid31 = i;
				break;
			}
		// 4.5 Mid32(>)
		if (Mid31 > 0)
			for (i = Mid31 + 1; i <= End; i++)
				if (S1[i] == '>')
				{
					Mid32 = i;
					break;
				}
	}

	// Edit Port
	Beg1 = End1 = 0;
	if ((Mid11 > 0) && (Mid11 < Mid12)) // sip:1.1.1.1:5060
	{
		Beg1 = Mid12 + 1;
		for (i = Beg1; i <= End; i++)
			if ((S1[i] == ';') || (S1[i] == ':') || (S1[i] == '>') || (S1[i] == ' '))
			{
				End1 = i - 1;
				break;
			}
		if (i > End)
			End1 = End;
	}
	else if ((Mid11 > 0) && (Mid12 == 0)) // sip:1.1.1.1  or 1.1.1.1:5060
	{
		Beg1 = Mid11 + 1;
		for (i = Beg1; i <= End; i++)
			if ((S1[i] == ';') || (S1[i] == ':') || (S1[i] == '>') || (S1[i] == ' '))
			{
				End1 = i - 1;
				break;
			}
		if (i > End)
			End1 = End;
		for (i = Beg1; i <= End1; i++)
			if ((S1[i] < '0') || (S1[i] > '9'))
				break;
		if (i < End1)
		{
			Beg1 = End1 = 0;
		}
	}

	if ((End1 > 0) && (End1 >= Beg1))
		Edition(S1, Beg1, End1, Port);

	return 0;
}

int CSIP_Server::Edition(char *S1, int Beg, int End, char *Buf)
{
	char SS1[2000];

	if ((End < (int)strlen(S1)) && (End >= Beg) && (Beg >= 0))
	{
		strncpy(SS1, S1, Beg);
		SS1[Beg] = 0;
		strcat(SS1, Buf);
		strcat(SS1, &S1[End + 1]);
		strcpy(S1, SS1);
		return 0;
	}
	else
		return -1;
}

//===================== Socket =========================
#ifdef OS_WINDOWS
int CSIP_Server::Start_TCP_Server(SOCKET *psockfd, WORD Port, DWORD EVENT, HWND Hwnd)
{
	WSADATA wsadata;
	int err;
	struct sockaddr_in tcpserver;

	// 啟動Winsock
	if ((err = WSAStartup(0x202, &wsadata)) != 0)
	{
		WSA_Get_ErrTxt(ErrMsg);
		return -1;
	}

	// TCP socket open
	if ((*psockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		WSA_Get_ErrTxt(ErrMsg);
		return -1;
	}

	// Set Asynchronized Mode
	if ((err = WSAAsyncSelect(*psockfd, Hwnd, EVENT, FD_ACCEPT | FD_READ | FD_CLOSE)) < 0)
	{
		WSA_Get_ErrTxt(ErrMsg);
		return -1;
	}

	// Socket Naming
	tcpserver.sin_family = AF_INET;
	tcpserver.sin_port = htons(Port);
	tcpserver.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(*psockfd, (struct sockaddr *)&tcpserver, sizeof(tcpserver)) < 0)
	{
		WSA_Get_ErrTxt(ErrMsg);
		return -1;
	}

	// Wait for Client Login
	if ((err = listen(*psockfd, SOMAXCONN)) < 0)
	{
		WSA_Get_ErrTxt(ErrMsg);
		return -1;
	}

	return 0;
}
int CSIP_Server::Start_TCP_Client(SOCKET *psockfd, WORD L_Port, WORD R_Port, char *IP, DWORD EVENT, HWND Hwnd)
{
	WSADATA wsadata;
	int err, i;
	struct sockaddr_in tcpclient, Addr;
	LPHOSTENT HostIP;
	char IP11[100];

	// 啟動Winsock
	if ((err = WSAStartup(0x202, &wsadata)) != 0)
	{
		WSA_Get_ErrTxt(ErrMsg);
		return -1;
	}

	// TCP socket open
	if ((*psockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		WSA_Get_ErrTxt(ErrMsg);
		return -1;
	}

	// Set Asynchronized Mode
	if ((err = WSAAsyncSelect(*psockfd, Hwnd, EVENT, FD_CONNECT | FD_READ | FD_CLOSE /*|FD_ADDRESS_LIST_CHANGE|FD_ROUTING_INTERFACE_CHANGE*/)) < 0)
	{
		WSA_Get_ErrTxt(ErrMsg);
		return -1;
	}

	// Check IP or Domain Name,
	for (i = 0; i < (int)strlen(IP); i++)
		if (((IP[i] < '0') || (IP[i] > '9')) && (IP[i] != '.'))
			break;
	if (i < (int)strlen(IP))
	{
		if ((HostIP = gethostbyname(IP)) == NULL)
		{
			WSA_Get_ErrTxt(ErrMsg);
			return -1;
		}
		else
			strcpy(IP11, inet_ntoa(*(LPIN_ADDR)(HostIP->h_addr)));
	}
	else
		strcpy(IP11, IP);

	// Socket Naming
	tcpclient.sin_family = AF_INET;
	tcpclient.sin_port = htons(R_Port);
	tcpclient.sin_addr.s_addr = inet_addr(IP11);

	// Socket Naming
	Addr.sin_family = AF_INET;
	Addr.sin_port = htons(L_Port);
	Addr.sin_addr.s_addr = htonl(INADDR_ANY);

	// Set Local's IP/Port
	if (0) //L_Port>100)
	{
		if (bind(*psockfd, (struct sockaddr *)&Addr, sizeof(sockaddr)) < 0)
		{ //int i=WSAGetLastError();
			WSA_Get_ErrTxt(ErrMsg);
			return -1;
		}
	}
	// else
	// {  Automatically Assign Local Port }

	// Connect to Server
	if (err = connect(*psockfd, (struct sockaddr *)&tcpclient, sizeof(tcpclient)) < 0)
	{
		WSA_Get_ErrTxt(ErrMsg);
		return -1;
	}

	return 0;
}
int CSIP_Server::Start_UDP_Server(SOCKET *psockfd, WORD Port, DWORD EVENT, HWND Hwnd)
{
	WSADATA wsadata;
	int err;
	struct sockaddr_in udpserver;

	// 啟動Winsock
	if ((err = WSAStartup(0x202, &wsadata)) != 0)
	{
		WSA_Get_ErrTxt(ErrMsg);
		return -1;
	}

	// TCP socket open
	if ((*psockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		WSA_Get_ErrTxt(ErrMsg);
		return -2;
	}

	// Set Asynchronized Mode
	if ((err = WSAAsyncSelect(*psockfd, Hwnd, EVENT, FD_READ)) < 0)
	{
		WSA_Get_ErrTxt(ErrMsg);
		return -3;
	}

	// Socket Naming
	udpserver.sin_family = AF_INET;
	udpserver.sin_port = htons(Port);
	udpserver.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(*psockfd, (struct sockaddr *)&udpserver, sizeof(udpserver)) < 0)
	{
		err = WSAGetLastError();
		WSA_Get_ErrTxt(ErrMsg);
		return -4;
	}

	return 0;
}
int CSIP_Server::Start_UDP_Client(SOCKET *psockfd, sockaddr_in *udpclient, WORD L_Port, WORD R_Port, char *IP, DWORD EVENT, HWND Hwnd)
{
	WSADATA wsadata;
	int err, i;
	sockaddr_in Addr;
	LPHOSTENT HostIP;
	char IP11[100];

	// 啟動Winsock
	if ((err = WSAStartup(0x202, &wsadata)) != 0)
	{
		WSA_Get_ErrTxt(ErrMsg);
		return -1;
	}

	// TCP socket open
	if ((*psockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		WSA_Get_ErrTxt(ErrMsg);
		return -1;
	}

	// Set Asynchronized Mode
	if ((err = WSAAsyncSelect(*psockfd, Hwnd, EVENT, FD_READ)) < 0)
	{
		WSA_Get_ErrTxt(ErrMsg);
		return -1;
	}

	// Check IP or Domain Name,
	for (i = 0; i < (int)strlen(IP); i++)
		if (((IP[i] < '0') || (IP[i] > '9')) && (IP[i] != '.'))
			break;
	if (i < (int)strlen(IP))
	{
		if ((HostIP = gethostbyname(IP)) == NULL)
		{
			WSA_Get_ErrTxt(ErrMsg);
			return -1;
		}
		else
			strcpy(IP11, inet_ntoa(*(LPIN_ADDR)(HostIP->h_addr)));
	}
	else
		strcpy(IP11, IP);

	// Socket Naming
	udpclient->sin_family = AF_INET;
	udpclient->sin_port = htons(R_Port);
	udpclient->sin_addr.s_addr = inet_addr(IP11);

	// Socket Naming
	Addr.sin_family = AF_INET;
	Addr.sin_port = htons(L_Port);
	Addr.sin_addr.s_addr = htonl(INADDR_ANY);

	// Set Local's IP/Port
	if (0) //L_Port>100)
	{
		if (bind(*psockfd, (struct sockaddr *)&Addr, sizeof(sockaddr)) < 0)
		{
			WSA_Get_ErrTxt(ErrMsg);
			return -1;
		}
	}
	// else
	// {  Automatically Assign Local Port }

	return 0;
}

int CSIP_Server::Close_Socket(WPARAM wParam, HWND Hwnd)
{
	WSAAsyncSelect(wParam, Hwnd, NULL, NULL);
	closesocket(wParam);
	return 0;
}
#endif // For OS_WINDOWS
// int CSIP_Server::WSA_Get_ErrTxt(char *S1)
// {
// int Err;

//   S1[0]=0;
//   Err=WSAGetLastError();
// #ifdef	OS_WINDOWS
//   if (Err==WSAEINTR)                     strcpy(S1,"WSAEINTR");
//   else if (Err==WSAEBADF)                strcpy(S1,"WSAEBADF");
//   else if (Err==WSAEACCES)               strcpy(S1,"WSAEACCES");
//   else if (Err==WSAEFAULT)               strcpy(S1,"WSAEFAULT");
//   else if (Err==WSAEINVAL)               strcpy(S1,"WSAEINVAL");
//   else if (Err==WSAEMFILE)               strcpy(S1,"WSAEMFILE");

// /*
//  * Windows Sockets definitions of regular Berkeley error constants
//  */
//   else if (Err==WSAEWOULDBLOCK)          strcpy(S1,"WSAEWOULDBLOCK");
//   else if (Err==WSAEINPROGRESS)          strcpy(S1,"WSAEINPROGRESS");
//   else if (Err==WSAEALREADY)             strcpy(S1,"WSAEALREADY");
//   else if (Err==WSAENOTSOCK)             strcpy(S1,"WSAENOTSOCK");
//   else if (Err==WSAEDESTADDRREQ)         strcpy(S1,"WSAEDESTADDRREQ");
//   else if (Err==WSAEMSGSIZE)             strcpy(S1,"WSAEMSGSIZE");
//   else if (Err==WSAEPROTOTYPE)           strcpy(S1,"WSAEPROTOTYPE");
//   else if (Err==WSAENOPROTOOPT)          strcpy(S1,"WSAENOPROTOOPT");
//   else if (Err==WSAEPROTONOSUPPORT)      strcpy(S1,"WSAEPROTONOSUPPORT");
//   else if (Err==WSAESOCKTNOSUPPORT)      strcpy(S1,"WSAESOCKTNOSUPPORT");
//   else if (Err==WSAEOPNOTSUPP)           strcpy(S1,"WSAEOPNOTSUPP");
//   else if (Err==WSAEPFNOSUPPORT)         strcpy(S1,"WSAEPFNOSUPPORT");
//   else if (Err==WSAEAFNOSUPPORT)         strcpy(S1,"WSAEAFNOSUPPORT");
//   else if (Err==WSAEADDRINUSE)           strcpy(S1,"WSAEADDRINUSE");
//   else if (Err==WSAEADDRNOTAVAIL)        strcpy(S1,"WSAEADDRNOTAVAIL");
//   else if (Err==WSAENETDOWN)             strcpy(S1,"WSAENETDOWN");
//   else if (Err==WSAENETUNREACH)          strcpy(S1,"WSAENETUNREACH");
//   else if (Err==WSAENETRESET)            strcpy(S1,"WSAENETRESET");
//   else if (Err==WSAECONNABORTED)         strcpy(S1,"WSAECONNABORTED");
//   else if (Err==WSAECONNRESET)           strcpy(S1,"WSAECONNRESET");
//   else if (Err==WSAENOBUFS)              strcpy(S1,"WSAENOBUFS");
//   else if (Err==WSAEISCONN)              strcpy(S1,"WSAEISCONN");
//   else if (Err==WSAENOTCONN)             strcpy(S1,"WSAENOTCONN");
//   else if (Err==WSAESHUTDOWN)            strcpy(S1,"WSAESHUTDOWN");
//   else if (Err==WSAETOOMANYREFS)         strcpy(S1,"WSAETOOMANYREFS");
//   else if (Err==WSAETIMEDOUT)            strcpy(S1,"WSAETIMEDOUT");
//   else if (Err==WSAECONNREFUSED)         strcpy(S1,"WSAECONNREFUSED");
//   else if (Err==WSAELOOP)                strcpy(S1,"WSAELOOP");
//   else if (Err==WSAENAMETOOLONG)         strcpy(S1,"WSAENAMETOOLONG");
//   else if (Err==WSAEHOSTDOWN)            strcpy(S1,"WSAEHOSTDOWN");
//   else if (Err==WSAEHOSTUNREACH)         strcpy(S1,"WSAEHOSTUNREACH");
//   else if (Err==WSAENOTEMPTY)            strcpy(S1,"WSAENOTEMPTY");
//   else if (Err==WSAEPROCLIM)             strcpy(S1,"WSAEPROCLIM");
//   else if (Err==WSAEUSERS)               strcpy(S1,"WSAEUSERS");
//   else if (Err==WSAEDQUOT)               strcpy(S1,"WSAEDQUOT");
//   else if (Err==WSAESTALE)               strcpy(S1,"WSAESTALE");
//   else if (Err==WSAEREMOTE)              strcpy(S1,"WSAEREMOTE");

// /*
//  * Extended Windows Sockets error constant definitions
//  */
//   else if (Err==WSASYSNOTREADY)          strcpy(S1,"WSASYSNOTREADY");
//   else if (Err==WSAVERNOTSUPPORTED)      strcpy(S1,"WSAVERNOTSUPPORTED");
//   else if (Err==WSANOTINITIALISED)       strcpy(S1,"WSANOTINITIALISED");
//   else if (Err==WSAEDISCON)              strcpy(S1,"WSAEDISCON");
//   else if (Err==WSAENOMORE)              strcpy(S1,"WSAENOMORE");
//   else if (Err==WSAECANCELLED)           strcpy(S1,"WSAECANCELLED");
//   else if (Err==WSAEINVALIDPROCTABLE)    strcpy(S1,"WSAEINVALIDPROCTABLE");
//   else if (Err==WSAEINVALIDPROVIDER)     strcpy(S1,"WSAEINVALIDPROVIDER");
//   else if (Err==WSAEPROVIDERFAILEDINIT)  strcpy(S1,"WSAEPROVIDERFAILEDINIT");
//   else if (Err==WSASYSCALLFAILURE)       strcpy(S1,"WSASYSCALLFAILURE");
//   else if (Err==WSASERVICE_NOT_FOUND)    strcpy(S1,"WSASERVICE_NOT_FOUND");
//   else if (Err==WSATYPE_NOT_FOUND)       strcpy(S1,"WSATYPE_NOT_FOUND");
//   else if (Err==WSA_E_NO_MORE)           strcpy(S1,"WSA_E_NO_MORE");
//   else if (Err==WSA_E_CANCELLED)         strcpy(S1,"WSA_E_CANCELLED");
//   else if (Err==WSAEREFUSED)             strcpy(S1,"WSAEREFUSED");

// /*
//  * Error return codes from gethostbyname() and gethostbyaddr()
//  * (when using the resolver). Note that these errors are
//  * retrieved via WSAGetLastError() and must therefore follow
//  * the rules for avoiding clashes with error numbers from
//  * specific implementations or language run-time systems.
//  * For this reason the codes are based at WSABASEERR+1001.
//  * Note also that [WSA]NO_ADDRESS is defined only for
//  * compatibility purposes.
//  */

// //#define h_errno         WSAGetLastError()

// /* Authoritative Answer: Host not found */
//   else if (Err==WSAHOST_NOT_FOUND)       strcpy(S1,"WSAHOST_NOT_FOUND");
//   else if (Err==HOST_NOT_FOUND)          strcpy(S1,"HOST_NOT_FOUND");

// /* Non-Authoritative: Host not found, or SERVERFAIL */
//   else if (Err==WSATRY_AGAIN)            strcpy(S1,"WSATRY_AGAIN");
//   else if (Err==TRY_AGAIN)               strcpy(S1,"TRY_AGAIN");

// /* Non-recoverable errors, FORMERR, REFUSED, NOTIMP */
//   else if (Err==WSANO_RECOVERY)          strcpy(S1,"WSANO_RECOVERY");
//   else if (Err==NO_RECOVERY)             strcpy(S1,"NO_RECOVERY");

// /* Valid name, no data record of requested type */
//   else if (Err==WSANO_DATA)              strcpy(S1,"WSANO_DATA");
//   else if (Err==NO_DATA)                 strcpy(S1,"NO_DATA");

// /* no address, look for MX record */
//   else if (Err==WSANO_ADDRESS)           strcpy(S1,"WSANO_ADDRESS");
//   else if (Err==NO_ADDRESS)              strcpy(S1,"NO_ADDRESS");

// /*
//  * Define QOS related error return codes
//  *
//  */
//   else if (Err==WSA_QOS_RECEIVERS)               strcpy(S1,"WSA_QOS_RECEIVERS");
//          /* at least one Reserve has arrived */
//   else if (Err==WSA_QOS_SENDERS)                 strcpy(S1,"WSA_QOS_SENDERS");
//          /* at least one Path has arrived */
//   else if (Err==WSA_QOS_NO_SENDERS)              strcpy(S1,"WSA_QOS_NO_SENDERS");
//          /* there are no senders */
//   else if (Err==WSA_QOS_NO_RECEIVERS)            strcpy(S1,"WSA_QOS_NO_RECEIVERS");
//          /* there are no receivers */
//   else if (Err==WSA_QOS_REQUEST_CONFIRMED)       strcpy(S1,"WSA_QOS_REQUEST_CONFIRMED");
//          /* Reserve has been confirmed */
//   else if (Err==WSA_QOS_ADMISSION_FAILURE)       strcpy(S1,"WSA_QOS_ADMISSION_FAILURE");
//          /* error due to lack of resources */
//   else if (Err==WSA_QOS_POLICY_FAILURE)          strcpy(S1,"WSA_QOS_POLICY_FAILURE");
//          /* rejected for administrative reasons - bad credentials */
//   else if (Err==WSA_QOS_BAD_STYLE)               strcpy(S1,"WSA_QOS_BAD_STYLE");
//          /* unknown or conflicting style */
//   else if (Err==WSA_QOS_BAD_OBJECT)              strcpy(S1,"WSA_QOS_BAD_OBJECT");
//          /* problem with some part of the filterspec or providerspecific
//           * buffer in general */
//   else if (Err==WSA_QOS_TRAFFIC_CTRL_ERROR)      strcpy(S1,"WSA_QOS_TRAFFIC_CTRL_ERROR");
//          /* problem with some part of the flowspec */
//   else if (Err==WSA_QOS_GENERIC_ERROR)           strcpy(S1,"WSA_QOS_GENERIC_ERROR");
//          /* general error */
//   else                                           sprintf(S1,"Error Code <%d> not Define",Err);
// #endif
//   return Err;
// }

int CSIP_Server::Command_Process(char *S1, char *Command, char Flag, int Command_Num, int Command_Len)
{
	int i, j, k1, k2;
	unsigned char *US1;

	US1 = (unsigned char *)S1;
	// ==== Resoluted Command Line ====
	i = 0;
	k1 = 0;
	k2 = 0;
	j = strlen(S1);
	while ((i < j) && (k1 < Command_Num - 1) && (k2 < Command_Len - 1))
	{
		if (US1[i] == (unsigned char)Flag)
		{
			Command[k1 * Command_Len + k2] = 0;
			k1++;
			k2 = 0;
			i++;
		}
		else
		{
			if (US1[i] >= 0x80)
			{
				Command[k1 * Command_Len + k2] = US1[i];
				i++;
				k2++;
			}
			Command[k1 * Command_Len + k2] = US1[i];
			i++;
			k2++;
		}
	}
	Command[k1 * Command_Len + k2] = 0;
	for (i = k1 + 1; i < Command_Num; i++)
		Command[i * Command_Len + 0] = 0;
	if ((k1 == 0) || (k2 == (Command_Len - 1)))
		return -1;
	else
		return k1;
}

int CSIP_Server::Txt_HSF(char *Buf, int Len)
{
	int i; //,j;
	char Buf1[2000];
	char A;

	//return Len;

	if ((Len <= 0) || (Len >= 1000))
		return Len;

	// 為了與舊版 Compatible 而保留 for Server
	//for (j=0;j<Len;j++) if (Buf[j]=='|') break;
	//if ( ((Buf[0]!=0x30)&&(j<=3)) || (j>=Len) || ((j<Len)&&(j>=9)) ) return Len;

	for (i = 0; i < Len; i++)
		Buf1[i] = Buf[i];
	Buf[0] = '?';
	for (i = 0; i < Len; i++)
		Buf[i + 1] = char(Buf1[i] - Buf[i]);
	for (i = 0; i < Len / 2; i++)
	{
		A = Buf[2 * i + 1];
		Buf[2 * i + 1] = Buf[2 * i + 2];
		Buf[2 * i + 2] = A;
	}
	for (i = 0; i < Len / 2; i++)
	{
		A = Buf[i + 1];
		Buf[i + 1] = Buf[Len - i];
		Buf[Len - i] = A;
	}
	for (i = 0; i < Len / 2; i++)
	{
		A = Buf[i + 1];
		Buf[i + 1] = Buf[Len / 2 + i + 1];
		Buf[Len / 2 + i + 1] = A;
	}
	return Len + 1;
}

int CSIP_Server::HSF_Txt(char *Buf, int Len1)
{
	int i, Len;
	char Buf1[2000];
	char A;

	if ((Buf[0] != '?') || (Len1 <= 1) || (Len1 >= 1000))
		return Len1;

	Len = Len1 - 1;
	for (i = 0; i < Len1; i++)
		Buf1[i] = Buf[i];
	for (i = 0; i < Len / 2; i++)
	{
		A = Buf1[i + 1];
		Buf1[i + 1] = Buf1[Len / 2 + i + 1];
		Buf1[Len / 2 + i + 1] = A;
	}
	for (i = 0; i < Len / 2; i++)
	{
		A = Buf1[i + 1];
		Buf1[i + 1] = Buf1[Len - i];
		Buf1[Len - i] = A;
	}
	for (i = 0; i < Len / 2; i++)
	{
		A = Buf1[2 * i + 1];
		Buf1[2 * i + 1] = Buf1[2 * i + 2];
		Buf1[2 * i + 2] = A;
	}
	for (i = 0; i < Len; i++)
		Buf[i] = char(Buf1[i + 1] + Buf1[i]);

	return Len;
}

int CSIP_Server::Hsf_recv(SOCKET Socket, char *Buf, int Len, int Flag)
{
	int i;
	// int	Len11;
	// sockaddr_in Addr;

	// i = recv(Socket, Buf, Len, Flag);
	if (i >= 0)
		Buf[i] = 0;
	if ((i <= 0) || (Buf[0] != '?'))
	{
		// Len11 = sizeof(sockaddr);
		// getsockname(Socket, (sockaddr *)&Addr, &Len11);
		// Write_Log("recv", Buf, &Addr);
		// return i;
		return Len;
	}
	else
	{
		// HSF_Txt((char *)Buf, i);
		// Buf[i-1] = 0;
		HSF_Txt((char *)Buf, Len);
		Buf[Len - 1] = 0;
		// Len11 = sizeof(sockaddr);
		// getsockname(Socket, (sockaddr *)&Addr, &Len11);
		// Write_Log("recv", Buf, &Addr);
		return Len - 1;
	}
}

int CSIP_Server::Hsf_send(SOCKET Socket, char *Buf1, int Len, int Flag)
{
	char Buf[2000];
	int i, j;
	// int Len11;
	// sockaddr_in Addr;

	// if (Len>=sizeof(Buf)) return -1;
	if (Len >= sizeof(Buf) - 1)
		return -1;
	else
		memcpy(Buf, Buf1, Len);
	Buf[Len] = 0;
	// Len11 = sizeof(sockaddr);
	// getsockname(Socket, (sockaddr *)&Addr, &Len11);
	// Write_Log("send", Buf, &Addr);
	j = Txt_HSF((char *)Buf, Len);
	memcpy(Buf1, Buf, j);
	// i = send(Socket, Buf, j, Flag);

	// return i-1;
	return j;
}
