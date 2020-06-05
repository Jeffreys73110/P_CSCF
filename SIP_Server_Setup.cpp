// SIP_Server_Setup.cpp : implementation file
//

#include "stdafx.h"
#include "CallServer.h"
#include "SIP_Server_Setup.h"
#include "CallServerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#include	"SIP_Server.h"
CSIP_Server *lpSIP_Server;
/////////////////////////////////////////////////////////////////////////////
// CSIP_Server_Setup dialog


CSIP_Server_Setup::CSIP_Server_Setup(CWnd* pParent /*=NULL*/)
	: CDialog(CSIP_Server_Setup::IDD, pParent)
{
	lpSIP_Server=(CSIP_Server*)pParent;
	//{{AFX_DATA_INIT(CSIP_Server_Setup)
	m_Proxy_Auth_Msg = _T("");
	m_Reg_Auth_Msg = _T("");
	m_Log_Path = _T(SIP_LOG_PATH);
	m_Invite_Log_Check = TRUE;
	m_Call_Log_Check = TRUE;
	m_Call_Flows_Check = TRUE;
	m_Reg_Flows_Check = TRUE;
	m_Forwarding_GW_Check = TRUE;
	m_Forwarding_Mode_Check = FALSE;
	m_Progressive_Expires_Check = TRUE;
	m_Locking_Expires = _T("120");
	m_SubServer_Mapping_Check = FALSE;
	m_SubServer_Mapping_Msg = _T("");
	m_AutoCompose_PW_Check = TRUE;
	m_Expire_Limit_Weight = _T("");
	m_ReplaceCode_Check = FALSE;
	m_ReplaceCode_Msg = _T("");
	//}}AFX_DATA_INIT
	Create(IDD_SERVER_SETUP);
	ShowWindow(SW_HIDE);
}


void CSIP_Server_Setup::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSIP_Server_Setup)
	DDX_Text(pDX, IDC_EDIT2, m_Proxy_Auth_Msg);
	DDX_Text(pDX, IDC_EDIT1, m_Reg_Auth_Msg);
	DDX_Text(pDX, IDC_EDIT4, m_Log_Path);
	DDX_Check(pDX, IDC_CHECK4, m_Invite_Log_Check);
	DDX_Check(pDX, IDC_CHECK5, m_Call_Log_Check);
	DDX_Check(pDX, IDC_CHECK6, m_Call_Flows_Check);
	DDX_Check(pDX, IDC_CHECK7, m_Reg_Flows_Check);
	DDX_Check(pDX, IDC_CHECK8, m_Forwarding_GW_Check);
	DDX_Check(pDX, IDC_CHECK9, m_Forwarding_Mode_Check);
	DDX_Check(pDX, IDC_CHECK10, m_Progressive_Expires_Check);
	DDX_Text(pDX, IDC_EDIT5, m_Locking_Expires);
	DDX_Check(pDX, IDC_CHECK11, m_SubServer_Mapping_Check);
	DDX_Text(pDX, IDC_EDIT6, m_SubServer_Mapping_Msg);
	DDX_Check(pDX, IDC_CHECK12, m_AutoCompose_PW_Check);
	DDX_Text(pDX, IDC_EDIT7, m_Expire_Limit_Weight);
	DDX_Check(pDX, IDC_CHECK13, m_ReplaceCode_Check);
	DDX_Text(pDX, IDC_EDIT13, m_ReplaceCode_Msg);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSIP_Server_Setup, CDialog)
	//{{AFX_MSG_MAP(CSIP_Server_Setup)
	ON_BN_CLICKED(IDC_BUTTON1, OnRegister_Level)
	ON_BN_CLICKED(IDC_BUTTON6, OnProxy_Auth_Level)
	ON_EN_CHANGE(IDC_EDIT4, OnLog_Path)
	ON_BN_CLICKED(IDC_CHECK4, OnInvite_Log_Check)
	ON_BN_CLICKED(IDC_CHECK5, OnCall_Log_Check)
	ON_BN_CLICKED(IDC_CHECK6, OnCall_Flows_Check)
	ON_BN_CLICKED(IDC_CHECK7, OnReg_Flows_Check)
	ON_BN_CLICKED(IDC_BUTTON2, OnAdvanced_Edit)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON3, OnBrowseForld)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_CHECK8, OnForwarding_GW)
	ON_BN_CLICKED(IDC_CHECK9, OnForwarding_Mode)
	ON_BN_CLICKED(IDC_CHECK10, OnProgressive_Expires)
	ON_EN_CHANGE(IDC_EDIT5, OnLocking_Expires)
	ON_BN_CLICKED(IDC_BUTTON4, OnEdit_Mapping_List)
	ON_BN_CLICKED(IDC_BUTTON5, OnSubServer_Table_Reload)
	ON_BN_CLICKED(IDC_CHECK12, OnAuto_Compose_PW)
	ON_EN_CHANGE(IDC_EDIT7, OnExpire_Limit_Weight)
	ON_BN_CLICKED(IDC_CHECK13, OnReplaceCode_Check)
	ON_BN_CLICKED(IDC_BUTTON7, OnReplace_Phone_Table)
	ON_BN_CLICKED(IDC_BUTTON8, OnReplace_Phone_Reload)
	ON_BN_CLICKED(IDC_CHECK11, OnSubServer_Mapping_Check)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSIP_Server_Setup message handlers

BOOL CSIP_Server_Setup::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// Initial Paramter
	Reg_Auth_Level = MAX_REG_LEVEL;
	Proxy_Auth_Level = MAX_PROXY_LEVEL;
	UpdateData(TRUE);
	m_Reg_Auth_Msg = "(High loadding) Authencate for any time, but CHECK Nonce which system lastest assign.";
	m_Proxy_Auth_Msg = "(High loadding) Authencate for any time, but CHECK Nonce which system lastest assign.";
	m_Log_Path = SIP_LOG_PATH;
	UpdateData(FALSE);
	Update_SIP_Server_Param("GET");
	// Get Current Directory
	GetCurrentDirectory(_MAX_PATH,Current_Directory);

	return TRUE;
}

void CSIP_Server_Setup::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent==ADVANCED_EDIT_TIMER)
	{
		KillTimer(nIDEvent);
		Advanced_Edit_Proc(FALSE);
	}

	CDialog::OnTimer(nIDEvent);
}

void CSIP_Server_Setup::OnRegister_Level() 
{
	char Buf[100],Buf1[200];
	
	Reg_Auth_Level++; Reg_Auth_Level%=(MAX_REG_LEVEL+1);
	sprintf(Buf,"Reg_Auth Level = %d",Reg_Auth_Level); GetDlgItem(IDC_BUTTON1)->SetWindowText(Buf);
	UpdateData(TRUE);
	if      (Reg_Auth_Level==0) sprintf(Buf1,"(Low loadding) No Authencation.");
	else if (Reg_Auth_Level==1) sprintf(Buf1,"(Middle loadding) Authencate when Addr changed, but NonCHECK Nonce which system lastest assign.");
	else if (Reg_Auth_Level==2) sprintf(Buf1,"(Middle loadding) Authencate when Addr changed, but CHECK Nonce which system lastest assign.");
	else if (Reg_Auth_Level==3) sprintf(Buf1,"(High loadding) Authencate for any time, but NonCHECK Nonce which system lastest assign.");
	else if (Reg_Auth_Level==4) sprintf(Buf1,"(High loadding) Authencate for any time, but CHECK Nonce which system lastest assign.");
	m_Reg_Auth_Msg = Buf1;
	UpdateData(FALSE);
	Update_SIP_Server_Param("WRITE");
}

void CSIP_Server_Setup::OnProxy_Auth_Level() 
{
	char Buf[100],Buf1[200];

	Proxy_Auth_Level++; Proxy_Auth_Level%=(MAX_PROXY_LEVEL+1); 
	sprintf(Buf,"Proxy_Auth Level = %d",Proxy_Auth_Level); GetDlgItem(IDC_BUTTON6)->SetWindowText(Buf);
	UpdateData(TRUE);
	if      (Proxy_Auth_Level==0) sprintf(Buf1,"(Low loadding) No Authencation.");
	else if (Proxy_Auth_Level==1) sprintf(Buf1,"(Middle loadding) Proxy Authorize when Register authencation pass.");
	else if (Proxy_Auth_Level==2) sprintf(Buf1,"(High loadding) Authencate for any time, but NonCHECK Nonce which system lastest assign.");
	else if (Proxy_Auth_Level==3) sprintf(Buf1,"(High loadding) Authencate for any time, but CHECK Nonce which system lastest assign.");
	m_Proxy_Auth_Msg = Buf1;
	UpdateData(FALSE);
	Update_SIP_Server_Param("WRITE");
}

int CSIP_Server_Setup::Advanced_Edit_Proc(bool Enable) 
{
	int		Enable_Num;
	int		nID[100]={IDC_EDIT4,IDC_CHECK4,IDC_CHECK5,IDC_CHECK6,IDC_CHECK7,IDC_BUTTON3,IDC_EDIT7,0xFFFF};
	bool	Flag[100];

	for (Enable_Num=0;Enable_Num<100;Enable_Num++)
	{
		if (nID[Enable_Num]==0xFFFF)	break;
		else
		{	// If either WindowEnabled=1 and Enable=1 or WindowEnabled=0 and Enable=0 then don't EnableWindow()
			Flag[Enable_Num] = (GetDlgItem(nID[Enable_Num])->IsWindowEnabled())? TRUE:FALSE;
			Flag[Enable_Num] = (!Flag[Enable_Num]&&Enable)||(Flag[Enable_Num]&&!Enable);	
			if (Flag[Enable_Num])	GetDlgItem(nID[Enable_Num])->EnableWindow(Enable);
		}
	}
	if (Enable)	SetTimer(ADVANCED_EDIT_TIMER,30000,NULL);
	
	return 0;
}

void CSIP_Server_Setup::OnLog_Path() 
{
	Update_SIP_Server_Param("WRITE");
	Advanced_Edit_Proc(TRUE);
}

void CSIP_Server_Setup::OnInvite_Log_Check() 
{
	Update_SIP_Server_Param("WRITE");
	Advanced_Edit_Proc(TRUE);
}

void CSIP_Server_Setup::OnCall_Log_Check() 
{
	Update_SIP_Server_Param("WRITE");
	Advanced_Edit_Proc(TRUE);
}

void CSIP_Server_Setup::OnCall_Flows_Check() 
{
	Update_SIP_Server_Param("WRITE");
	Advanced_Edit_Proc(TRUE);
}

void CSIP_Server_Setup::OnReg_Flows_Check() 
{
	Update_SIP_Server_Param("WRITE");
	Advanced_Edit_Proc(TRUE);
}

void CSIP_Server_Setup::OnAdvanced_Edit() 
{
	Advanced_Edit_Proc(TRUE);
}

int CSIP_Server_Setup::Update_SIP_Server_Param(char *Method)
{
	int		Locking_Expires,Expire_Limit_Weight;
	char	S1[100];
	char	Log_Path[_MAX_PATH];
	BOOL	Invite_Log_Check,Call_Log_Check,Call_Flows_Check,Reg_Flows_Check;
	BOOL	Forwarding_GW_Check,Forwarding_Mode_Check;
	BOOL	Progressive_Expires_Check,SubServer_Mapping_Check;
	BOOL	AutoCompose_PW_Check,ReplaceCode_Check;

	if (!strcmp(Method,"GET"))
	{
		GetPrivateProfileString("Setup2 Info","Reg_Auth_Msg",m_Reg_Auth_Msg,Reg_Auth_Msg,sizeof(Reg_Auth_Msg),SERVER_INFO_PATH);
		GetPrivateProfileString("Setup2 Info","Proxy_Auth_Msg",m_Proxy_Auth_Msg,Proxy_Auth_Msg,sizeof(Proxy_Auth_Msg),SERVER_INFO_PATH);
		Reg_Auth_Level = GetPrivateProfileInt("Setup2 Info","Reg_Auth_Level",MAX_REG_LEVEL,SERVER_INFO_PATH);
		Proxy_Auth_Level = GetPrivateProfileInt("Setup2 Info","Proxy_Auth_Level",MAX_PROXY_LEVEL,SERVER_INFO_PATH);
		GetPrivateProfileString("Setup2 Info","Log_Path",m_Log_Path,Log_Path,sizeof(Log_Path),SERVER_INFO_PATH);
		Invite_Log_Check = GetPrivateProfileInt("Setup2 Info","Invite_Log_Check",m_Invite_Log_Check,SERVER_INFO_PATH);
		Call_Log_Check = GetPrivateProfileInt("Setup2 Info","Call_Log_Check",m_Call_Log_Check,SERVER_INFO_PATH);
		Call_Flows_Check = GetPrivateProfileInt("Setup2 Info","Call_Flows_Check",m_Call_Flows_Check,SERVER_INFO_PATH);
		Reg_Flows_Check = GetPrivateProfileInt("Setup2 Info","Reg_Flows_Check",m_Reg_Flows_Check,SERVER_INFO_PATH);
		Forwarding_GW_Check = GetPrivateProfileInt("Setup2 Info","Forwarding_GW_Check",m_Forwarding_GW_Check,SERVER_INFO_PATH);
		Forwarding_Mode_Check = GetPrivateProfileInt("Setup2 Info","Forwarding_Mode_Check",m_Forwarding_Mode_Check,SERVER_INFO_PATH);
		Progressive_Expires_Check = GetPrivateProfileInt("Setup2 Info","Progressive_Expires_Check",m_Progressive_Expires_Check,SERVER_INFO_PATH);
		Locking_Expires = GetPrivateProfileInt("Setup2 Info","Locking_Expires",atoi(m_Locking_Expires),SERVER_INFO_PATH);
		SubServer_Mapping_Check = GetPrivateProfileInt("Setup2 Info","SubServer_Mapping_Check",m_SubServer_Mapping_Check,SERVER_INFO_PATH);
		AutoCompose_PW_Check = GetPrivateProfileInt("Setup2 Info","AutoCompose_PW_Check",m_AutoCompose_PW_Check,SERVER_INFO_PATH);
		Expire_Limit_Weight = GetPrivateProfileInt("Setup2 Info","Expire_Limit_Weight",EXPIRE_LIMIT_WEIGHT,SERVER_INFO_PATH);
		ReplaceCode_Check = GetPrivateProfileInt("Setup2 Info","ReplaceCode_Check",m_ReplaceCode_Check,SERVER_INFO_PATH);
		UpdateData(TRUE);

		m_Reg_Auth_Msg = Reg_Auth_Msg;
		m_Proxy_Auth_Msg = Proxy_Auth_Msg;
		sprintf(S1,"Reg_Auth_Level = %d",Reg_Auth_Level); GetDlgItem(IDC_BUTTON1)->SetWindowText(S1);
		sprintf(S1,"Proxy_Auth Level = %d",Proxy_Auth_Level); GetDlgItem(IDC_BUTTON6)->SetWindowText(S1);
		m_Log_Path = Log_Path;
		m_Invite_Log_Check = Invite_Log_Check;
		m_Call_Log_Check = Call_Log_Check;
		m_Call_Flows_Check = Call_Flows_Check;
		m_Reg_Flows_Check = Reg_Flows_Check;
		m_Forwarding_GW_Check = Forwarding_GW_Check; GetDlgItem(IDC_CHECK9)->EnableWindow(Forwarding_GW_Check);
		m_Forwarding_Mode_Check = Forwarding_Mode_Check;
		m_Progressive_Expires_Check = Progressive_Expires_Check; GetDlgItem(IDC_EDIT5)->EnableWindow(!Progressive_Expires_Check);
		sprintf(S1,"%d",Locking_Expires); m_Locking_Expires = S1;
		m_SubServer_Mapping_Check = SubServer_Mapping_Check; GetDlgItem(IDC_BUTTON4)->EnableWindow(SubServer_Mapping_Check); GetDlgItem(IDC_BUTTON5)->EnableWindow(SubServer_Mapping_Check); 
		m_AutoCompose_PW_Check = AutoCompose_PW_Check;
		sprintf(S1,"%d",Expire_Limit_Weight); m_Expire_Limit_Weight = S1;
		m_ReplaceCode_Check = ReplaceCode_Check; GetDlgItem(IDC_BUTTON7)->EnableWindow(ReplaceCode_Check); GetDlgItem(IDC_BUTTON8)->EnableWindow(ReplaceCode_Check);
		UpdateData(FALSE);
		
		if (SubServer_Mapping_Check)	OnSubServer_Table_Reload(); else lpSIP_Server->Init_SubServer_Mapping();
		if (ReplaceCode_Check)	OnReplace_Phone_Reload(); else lpSIP_Server->Init_Replace_Code();
		//--- Debug ---
		TRACE("Update_SIP_Server_Param(GET), Log_Path=%s\r\n",Log_Path);	
		//-------------
	}
	else if (!strcmp(Method,"WRITE"))
	{	
		if(GetFileAttributes(SETUP_FOLDER)==0xFFFFFFFF)	Create_Directory(SETUP_FOLDER);
		UpdateData(TRUE);
		strcpy(Reg_Auth_Msg,m_Reg_Auth_Msg);
		strcpy(Proxy_Auth_Msg,m_Proxy_Auth_Msg);
		strcpy(Log_Path,m_Log_Path);
		Invite_Log_Check = m_Invite_Log_Check;
		Call_Log_Check = m_Call_Log_Check;
		Call_Flows_Check = m_Call_Flows_Check;
		Reg_Flows_Check = m_Reg_Flows_Check;
		Forwarding_GW_Check = m_Forwarding_GW_Check;
		Forwarding_Mode_Check = m_Forwarding_Mode_Check;
		Progressive_Expires_Check = m_Progressive_Expires_Check;
		Locking_Expires = atoi(m_Locking_Expires);
		SubServer_Mapping_Check = m_SubServer_Mapping_Check;
		AutoCompose_PW_Check = m_AutoCompose_PW_Check;
		Expire_Limit_Weight = atoi(m_Expire_Limit_Weight);
		ReplaceCode_Check = m_ReplaceCode_Check;
		UpdateData(FALSE);
		
		WritePrivateProfileString("Setup2 Info","Reg_Auth_Msg",m_Reg_Auth_Msg,SERVER_INFO_PATH);
		WritePrivateProfileString("Setup2 Info","Proxy_Auth_Msg",m_Proxy_Auth_Msg,SERVER_INFO_PATH);
		sprintf(S1,"%d",Reg_Auth_Level); WritePrivateProfileString("Setup2 Info","Reg_Auth_Level",S1,SERVER_INFO_PATH);
		sprintf(S1,"%d",Proxy_Auth_Level); WritePrivateProfileString("Setup2 Info","Proxy_Auth_Level",S1,SERVER_INFO_PATH);
		WritePrivateProfileString("Setup2 Info","Log_Path",m_Log_Path,SERVER_INFO_PATH);
		sprintf(S1,"%d",Invite_Log_Check); WritePrivateProfileString("Setup2 Info","Invite_Log_Check",S1,SERVER_INFO_PATH);
		sprintf(S1,"%d",Call_Log_Check); WritePrivateProfileString("Setup2 Info","Call_Log_Check",S1,SERVER_INFO_PATH);
		sprintf(S1,"%d",Call_Flows_Check); WritePrivateProfileString("Setup2 Info","Call_Flows_Check",S1,SERVER_INFO_PATH);
		sprintf(S1,"%d",Reg_Flows_Check); WritePrivateProfileString("Setup2 Info","Reg_Flows_Check",S1,SERVER_INFO_PATH);
		sprintf(S1,"%d",Forwarding_GW_Check); WritePrivateProfileString("Setup2 Info","Forwarding_GW_Check",S1,SERVER_INFO_PATH);
		sprintf(S1,"%d",Forwarding_Mode_Check); WritePrivateProfileString("Setup2 Info","Forwarding_Mode_Check",S1,SERVER_INFO_PATH);
		sprintf(S1,"%d",Progressive_Expires_Check); WritePrivateProfileString("Setup2 Info","Progressive_Expires_Check",S1,SERVER_INFO_PATH);
		WritePrivateProfileString("Setup2 Info","Locking_Expires",m_Locking_Expires,SERVER_INFO_PATH);
		sprintf(S1,"%d",SubServer_Mapping_Check); WritePrivateProfileString("Setup2 Info","SubServer_Mapping_Check",S1,SERVER_INFO_PATH);
		sprintf(S1,"%d",AutoCompose_PW_Check); WritePrivateProfileString("Setup2 Info","AutoCompose_PW_Check",S1,SERVER_INFO_PATH);
		WritePrivateProfileString("Setup2 Info","Expire_Limit_Weight",m_Expire_Limit_Weight,SERVER_INFO_PATH);
		sprintf(S1,"%d",ReplaceCode_Check); WritePrivateProfileString("Setup2 Info","ReplaceCode_Check",S1,SERVER_INFO_PATH);
		//--- Debug ---
		TRACE("Update_SIP_Server_Param(WRITE), Log_Path=%s\r\n",Log_Path);	
		//-------------
	}
	// Return Paramter to SIP_Server
	lpSIP_Server->Reg_Auth_Level = Reg_Auth_Level;
	lpSIP_Server->Proxy_Auth_Level = Proxy_Auth_Level;
	strcpy(lpSIP_Server->Log_Path,Log_Path);
	lpSIP_Server->Invite_Log_Check = (Invite_Log_Check)? TRUE:FALSE;
	lpSIP_Server->Call_Log_Check = (Call_Log_Check)? TRUE:FALSE;
	lpSIP_Server->Call_Flows_Check = (Call_Flows_Check)? TRUE:FALSE;
	lpSIP_Server->Reg_Flows_Check = (Reg_Flows_Check)? TRUE:FALSE;
	lpSIP_Server->Forwarding_GW_Check = (Forwarding_GW_Check)? TRUE:FALSE;
	lpSIP_Server->Forwarding_Mode_Check = (Forwarding_Mode_Check)? TRUE:FALSE;
	lpSIP_Server->Progressive_Expires_Check = (Progressive_Expires_Check)? TRUE:FALSE;
	lpSIP_Server->Locking_Expires = Locking_Expires;
	lpSIP_Server->SubServer_Mapping_Check = (SubServer_Mapping_Check)? TRUE:FALSE;
	lpSIP_Server->AutoCompose_PW_Check = (AutoCompose_PW_Check)? TRUE:FALSE;
	lpSIP_Server->Expire_Limit_Weight = Expire_Limit_Weight;
	lpSIP_Server->ReplaceCode_Check = (ReplaceCode_Check)? TRUE:FALSE;
	//--- Debug ---
	TRACE("Update_SIP_Server_Param(lpSOP_Server), Log_Path=%s\r\n",Log_Path);
	//-------------
	return 0;
}

int CSIP_Server_Setup::BrowseForFolder(HWND hWnd,char *szPath)
{
	char	szTitle[] = _T("Select a folder"); 
	char	szDisplayName[MAX_PATH] = _T("");
	BROWSEINFO bi;

	bi.hwndOwner = hWnd;
	bi.pidlRoot = NULL;
	bi.lpszTitle = szTitle;
	bi.pszDisplayName = szDisplayName;
	bi.ulFlags = BIF_RETURNONLYFSDIRS;
	bi.lpfn  = NULL;
	bi.lParam = 0;

	LPITEMIDLIST pItemIDList = SHBrowseForFolder(&bi);
	if(pItemIDList!=NULL)	{ SHGetPathFromIDList(pItemIDList,szPath); return 0;}
	else	{ *szPath=0; return -1;}
}

void CSIP_Server_Setup::OnBrowseForld() 
{
	char Log_Path[_MAX_PATH];

	// Browse Folder
	BrowseForFolder(m_hWnd,Log_Path);	
	if (strcmp(Log_Path,""))	{ if (strstr(Log_Path,SIP_LOG_PATH)==NULL)	strcat(Log_Path,SIP_LOG_PATH);}
	else	sprintf(Log_Path,"%s",SIP_LOG_PATH);
	// Create Directory
	Create_Directory(Log_Path);
	// Save and Show Log Path
	UpdateData(TRUE); m_Log_Path=Log_Path; UpdateData(FALSE);
	Update_SIP_Server_Param("WRITE");
	Advanced_Edit_Proc(TRUE);
	//--- Debug ---
	TRACE("OnBrowseForld, Log_Path=%s\r\n",Log_Path);
	//-------------
}

int CSIP_Server_Setup::Create_Directory(char *szPath)
{	
	int		i,j,Start_Cur;
	char	Buf[MAX_PATH];
	char	DirBuf[20][100];
	
	if ((int)strlen(szPath)>MAX_PATH || !strcmp(szPath,""))	return -1;
	else
	{	memset(DirBuf,0,sizeof(DirBuf));
		strcpy(Buf,szPath);
		for (i=j=Start_Cur=0;i<(int)strlen(Buf);i++)	
		{	if (Buf[i]=='\\'||Buf[i]=='/')	
			{	if (!(i==2 && Buf[i-1]==':'))	//若第一組有':'則跳過
				{	strncpy(DirBuf[j],&Buf[Start_Cur],i-Start_Cur); DirBuf[j++][i-Start_Cur]='\0'; 
					Start_Cur=i+1;
				}
			}
			else if (i==((int)strlen(Buf)-1))
			{	// 當最後面沒有'/' or '\'
				strncpy(DirBuf[j],&Buf[Start_Cur],i-Start_Cur+1); DirBuf[j++][i-Start_Cur+1]='\0'; 
				Start_Cur=i+1;
			}
		}	
		// Mkdir
		strcpy(Buf,"");
		for (i=0;(int)strlen(DirBuf[i])!=0;i++)	
		{ sprintf(Buf,"%s%s\\",Buf,DirBuf[i]); mkdir(Buf);}
	}
	return 0;
}

void CSIP_Server_Setup::OnClose() 
{
	CDialog::OnClose();
}

void CSIP_Server_Setup::OnForwarding_GW() 
{
	Update_SIP_Server_Param("WRITE");
	UpdateData(TRUE);  
	GetDlgItem(IDC_CHECK9)->EnableWindow(m_Forwarding_GW_Check);
	UpdateData(FALSE);
}

void CSIP_Server_Setup::OnForwarding_Mode() 
{
	Update_SIP_Server_Param("WRITE");
}

void CSIP_Server_Setup::OnProgressive_Expires() 
{
	Update_SIP_Server_Param("WRITE");
	UpdateData(TRUE);
	GetDlgItem(IDC_EDIT5)->EnableWindow(!m_Progressive_Expires_Check);
	UpdateData(FALSE);
}

void CSIP_Server_Setup::OnAuto_Compose_PW() 
{
	Update_SIP_Server_Param("WRITE");
}

void CSIP_Server_Setup::OnLocking_Expires() 
{
	Update_SIP_Server_Param("WRITE");
}

void CSIP_Server_Setup::OnExpire_Limit_Weight() 
{
	UpdateData(TRUE);
	if (atoi(m_Expire_Limit_Weight)<2)	m_Expire_Limit_Weight="2";
	else if (atoi(m_Expire_Limit_Weight)>10)	m_Expire_Limit_Weight="10";
	UpdateData(FALSE);
	Update_SIP_Server_Param("WRITE");
	Advanced_Edit_Proc(TRUE);
}

void CSIP_Server_Setup::OnSubServer_Mapping_Check() 
{
	Update_SIP_Server_Param("WRITE");
	UpdateData(TRUE);  
	if (m_SubServer_Mapping_Check)	OnSubServer_Table_Reload();
	else lpSIP_Server->Init_SubServer_Mapping();
	GetDlgItem(IDC_BUTTON4)->EnableWindow(m_SubServer_Mapping_Check);
	GetDlgItem(IDC_BUTTON5)->EnableWindow(m_SubServer_Mapping_Check);
	UpdateData(FALSE);
}

void CSIP_Server_Setup::OnReplaceCode_Check() 
{
	Update_SIP_Server_Param("WRITE");
	UpdateData(TRUE);
	if (m_ReplaceCode_Check)	OnReplace_Phone_Reload();
	else lpSIP_Server->Init_Replace_Code();
	GetDlgItem(IDC_BUTTON7)->EnableWindow(m_ReplaceCode_Check);
	GetDlgItem(IDC_BUTTON8)->EnableWindow(m_ReplaceCode_Check);
	UpdateData(FALSE);
}

void CSIP_Server_Setup::OnReplace_Phone_Table() 
{
	FILE	*Fio;
	char	Table_File_Name[_MAX_PATH];

	if(GetFileAttributes(SETUP_FOLDER)==0xFFFFFFFF)	Create_Directory(SETUP_FOLDER);
	if ((Fio=fopen(Replace_Code_TABLE,"a"))!=NULL)
	{
		fclose(Fio);
		sprintf(Table_File_Name,"%s\\%s",Current_Directory,Replace_Code_TABLE);
		if (strlen(Table_File_Name)>_MAX_PATH)	{ AfxMessageBox(_T("The file name length of Replace_Code_Table is too long.")); return;}
		ShellExecute(m_hWnd,"open",Table_File_Name,NULL,NULL,SW_SHOWNORMAL);
	}
}

void CSIP_Server_Setup::OnReplace_Phone_Reload() 
{
	FILE	*Fin;
	char	Buf[100],Msg[100],*Token;
	int		Read_In_Count,ReplaceCode_Count,Read_Empty_Count;
	BOOL	Get_ReplaceCode_State;

	Read_In_Count=ReplaceCode_Count=Read_Empty_Count=0;
	strcpy(Msg,"");

	if ((Fin=fopen(Replace_Code_TABLE,"r"))!=NULL)
	{
		lpSIP_Server->Init_Replace_Code();
		while ((Get_ReplaceCode_State=Get_ReplaceCode_Table(Fin,Buf))>=0)
		{
			Read_In_Count++;
			if (Get_ReplaceCode_State==1)
			{	// Get Replace Code Table
				if ((Token=strtok(Buf,","))!=NULL)	strcpy(lpSIP_Server->ReplaceCode_Table[ReplaceCode_Count][0],Token);
				if ((Token=strtok(NULL,":"))!=NULL)	strcpy(lpSIP_Server->ReplaceCode_Table[ReplaceCode_Count][1],Token);
				ReplaceCode_Count++;
			}
			else if (Get_ReplaceCode_State==0)	Read_Empty_Count++;
		}
		fclose(Fin);
	}else strcpy(Msg,"Replace Code Table file is not exist.");

	lpSIP_Server->ReplaceCode_Num=ReplaceCode_Count;
	if (Msg[0]==0)	sprintf(Msg,"Read in:%d-->ReplaceCode Table:%d, Read Empty:%d",Read_In_Count,ReplaceCode_Count,Read_Empty_Count);
	UpdateData(TRUE); m_ReplaceCode_Msg=Msg; UpdateData(FALSE);
}

void CSIP_Server_Setup::OnEdit_Mapping_List() 
{
	FILE	*Fio;
	char	Table_File_Name[_MAX_PATH];

	if(GetFileAttributes(SETUP_FOLDER)==0xFFFFFFFF)	Create_Directory(SETUP_FOLDER);
	if ((Fio=fopen(SubServer_Mapping_TABLE,"a"))!=NULL)
	{
		fclose(Fio);
		sprintf(Table_File_Name,"%s\\%s",Current_Directory,SubServer_Mapping_TABLE);
		if (strlen(Table_File_Name)>_MAX_PATH)	{ AfxMessageBox(_T("The file name length of Sub-Server_Mapping_Table is too long.")); return;}
		ShellExecute(m_hWnd,"open",Table_File_Name,NULL,NULL,SW_SHOWNORMAL);		
	}
}

void CSIP_Server_Setup::OnSubServer_Table_Reload() 
{
	FILE		*Fin;
	char		Buf[100],Msg[100],*Token;
	int			i,Read_In_Count,SubServer_Count,Mapping_Table_Count,Read_Fail_Count,Read_Empty_Count;
	int			Proxy_URL_Count,Proxy_URL_Num;	// 紀錄一行[]裡的數量
	bool		Url_OK;
	BOOL		Get_Mapping_State;
	hostent		*lpHost;
	char		SubServer_URL[SERVER_PORT_NUM][100],SubServer_IP[100],SubServer_Port[16];
	SUBSERVER_DATA	SubServer_Data[SERVER_PORT_NUM];
	MAPPING_DATA	Mapping_Data;

	// Initial Variable
	strcpy(Msg,"");
	UpdateData(TRUE); m_SubServer_Mapping_Msg=""; UpdateData(FALSE);
	memset(SubServer_URL,0,sizeof(SubServer_URL));
	SubServer_Count=Mapping_Table_Count=Read_Fail_Count=Read_In_Count=Read_Empty_Count=0;
	memset(SubServer_Data,0,sizeof(SubServer_Data));
	lpSIP_Server->Init_SubServer_Mapping();
		
	if ((Fin=fopen(SubServer_Mapping_TABLE,"r"))!=NULL)
	{
		while ((Get_Mapping_State=Get_Mapping_TABLE(Fin,Buf))>=0)
		{
			Read_In_Count++;
			if (Get_Mapping_State==0)	Read_Empty_Count++;
			else if (Get_Mapping_State==1)
			{	// Get Sub-Proxy_Server IP and Port
				Proxy_URL_Num=0;
				while ((Token=strtok((Proxy_URL_Num==0)? Buf:NULL,";"))!=NULL)	strcpy(SubServer_URL[Proxy_URL_Num++],Token);
				if (Proxy_URL_Num>SERVER_PORT_NUM)	Proxy_URL_Num=SERVER_PORT_NUM;	//Set limit of Proxy Mapping number 
				for (Proxy_URL_Count=0;Proxy_URL_Count<Proxy_URL_Num;Proxy_URL_Count++)
				{	
					if ((Token=strtok(SubServer_URL[Proxy_URL_Count],":"))!=NULL)	strcpy(SubServer_IP,Token);
					if ((Token=strtok(NULL,":"))!=NULL)	strcpy(SubServer_Port,Token); else strcpy(SubServer_Port,"5060");
					Url_OK=TRUE;
					for (i=0;i<(int)strlen(SubServer_IP);i++)	
					{	// Determine if the Proxy_IP is Domain Name	
						if ((SubServer_IP[i]<'0' || SubServer_IP[i]>'9') && SubServer_IP[i]!='.')	
						{	// Domain Name --> IP
							if ((lpHost=gethostbyname(SubServer_IP))!=NULL)	strcpy(SubServer_IP,inet_ntoa(*(LPIN_ADDR)(lpHost->h_addr)));
							else	Url_OK=FALSE;
							break;
						}
					}
					if (Url_OK)
					{
						// Determine if the Proxy_Port is correct
						if (!lpSIP_Server->Port_Filter(SubServer_Port))	Read_Fail_Count++;	
						else
						{	// Set SubServer_Addr
							SubServer_Data[Proxy_URL_Count].Flag=TRUE;
							lpSIP_Server->Set_SubServer_Data(&SubServer_Data[Proxy_URL_Count],SubServer_IP,SubServer_Port);
							SubServer_Count++;
							if (Proxy_URL_Count>=1)	Read_In_Count++; // 連續讀入兩筆以上Addr，讀入筆數隨之累加
						}
					}
				}
			}
			else if (Get_Mapping_State==2)
			{	// Reload Phone Number list
				memset(&Mapping_Data,0,sizeof(MAPPING_DATA));
				if (SubServer_Count>0 && strlen(Buf)>0 && strlen(Buf)<sizeof(Mapping_Data.Name))
				{
					// Set Mapping_Data
					Mapping_Data.Flag=TRUE;
					strcpy(Mapping_Data.Name,Buf);
					Mapping_Data.SubServer_Num=Proxy_URL_Num;
					memcpy(&Mapping_Data.SubServer_Data,&SubServer_Data,sizeof(SubServer_Data));
					// Save SubServer_Mapping
					if (lpSIP_Server->Save_SubServer_Mapping(&Mapping_Data)<0)	{ strcpy(Msg,"Save SubServer Mapping is fail."); break;}
					else	Mapping_Table_Count++;
				}else	Read_Fail_Count++;
			}
			else if (Get_Mapping_State==3)	Read_In_Count--;	// Doing no thing when "//..."
		}
		fclose(Fin);
	}else strcpy(Msg,"Sub-Server Mapping Table File is not exist.");

	if (Msg[0]==0)	sprintf(Msg,"Read in:%d-->Sub-Server Proxy:%d, Mapping Table:%d, Read Fail:%d, Read Empty:%d",Read_In_Count,SubServer_Count,Mapping_Table_Count,Read_Fail_Count,Read_Empty_Count);
	UpdateData(TRUE); m_SubServer_Mapping_Msg=Msg; UpdateData(FALSE);
}

int CSIP_Server_Setup::Get_Mapping_TABLE(FILE *Fin,char *Data)
{	
	char	Buf[100],*End_Cur;
	int		i,Get_Data_Flag;

	if (Fin!=NULL)
	{
		if (fgets(Buf,sizeof(Buf),Fin)!=NULL)
		{
			if (Buf[0]=='[' && (End_Cur=strchr(Buf,']'))!=NULL)
			{	// Get Proxy_Url
				Get_Data_Flag=1;
				*End_Cur=0;
				strcpy(Data,&Buf[1]);
			}
			else
			{	// Get Mapping Table List
				Get_Data_Flag=2;
				for (i=0;i<(int)strlen(Buf);i++)	
				{
					if (Buf[i]=='\r' || Buf[i]=='\n')	
					{	// Empty Data
						if (i==0)	{ Get_Data_Flag=0; Buf[i]=0; break;}
						else Buf[i]=0;
					}else if (Buf[i]=='\0')	{ Get_Data_Flag=0; break;}
				}
				strcpy(Data,Buf);
			}
		}else	Get_Data_Flag=-1;
	}else	Get_Data_Flag=-1;

	return Get_Data_Flag;
}

int CSIP_Server_Setup::Get_ReplaceCode_Table(FILE *Fin,char *Data)
{
	int		i,Get_Data_Flag;
	char	Buf[100];

	if (Fin!=NULL)
	{
		strcpy(Buf,"");
		if (fgets(Buf, sizeof(Buf), Fin)!=NULL)
		{
			for (i=0; i<(int)strlen(Buf); i++)
			{	// Get Replace Code Table
				Get_Data_Flag=1;
				if (Buf[i]=='\r' || Buf[i]=='\n')
				{	// Empty Data
					if (i==0)	{ Get_Data_Flag=0; Buf[i]=0; break;}
					else Buf[i]=0;
				}else if (Buf[i]=='\0')	{ Get_Data_Flag=0; break;}
			}
			strcpy(Data,Buf);
		}else	Get_Data_Flag=-1;
	}else	Get_Data_Flag=-1;

	return Get_Data_Flag;
}

