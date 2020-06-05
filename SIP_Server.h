#if !defined(AFX_SIP_SERVER_H__9870A5BE_30EC_48E2_93CF_EB83D413CEB6__INCLUDED_)
#define AFX_SIP_SERVER_H__9870A5BE_30EC_48E2_93CF_EB83D413CEB6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SIP_Server.h : header file
//

#include "StdHeader.h"
#include <direct.h>
#include <winsock2.h>
#include "Security.h"
#include "MD5/Authentication.h"
#include "SIP_Server_Setup.h"

/////////////////////////////////////////////////////////////////////////////
// CSIP_Server dialog
#define		SIP_SERVER_EVENT			WM_USER+0x1233
#define		INVITE_EVENT				WM_USER+0x1234
#define		RTP_RELAY_EVENT				WM_USER+1030
#define		MONITOR_EVENT				WM_USER+101

// Timer
#define		MONITOR_PORT				8809
#define		TIMERID_EXPIRES				66000
#define		TIMERID_RTP_RELAY			66001
#define		TIMERID_COMMAND				67000
#define		TIMERID_INVITE				(TIMERID_COMMAND+COMMAND_QUEUE_SIZE)
#define		TIMERID_SUBSERVER_EVENT		(TIMERID_INVITE+SIP_INVITE_SIZE)

// SIP Server Port
#define		SIP_COMMAND_NO				10
#define		SIP_SERVER_PORT				5060
#define		SIP_SERVER_PORT_HTTP		80
#define		SERVER_PORT_NUM				5
#define		SIP_SERVER_PORTS			{SIP_SERVER_PORT, SIP_SERVER_PORT+1, 80, 119, 110}

// Path
#define		SIP_LOG_PATH				"SIP_Server_Log\\INVITE_Log\\"
#define		LOG_FILE_NAME				"c:/SIP_Log.txt"
#define		SETUP_FOLDER				"Setup"
#define		SERVER_INFO_PATH			"Setup/setup.ini"
#define		SIP_ONLINE_FILE				"Setup/SIP_OnLine.dat"
#define		SIP_MONITOR_FILE			"Setup/SIP_Monitor.dat"
#define		SubServer_Mapping_TABLE		"Setup/Sub-Server Mapping Table.ini"
#define		Replace_Code_TABLE			"Setup/Phone_Table.ini"

#define		ALREADY_RELAY				"Already_RL"
#define		HEAD_SEND					"send"
#define		HEAD_SEND_YET				"send_yet"
#define		HEAD_RECV					"recv"
#define		HEAD_RECVFROM				"recvfrom"
#define		HEAD_SENDTO					"sendto"

// Crypto Type
#define		NO_Setting_TAG				"NoSetting"
#define		NO_CRYPTO_TAG				"NoCrypto"
#define		SIP_CRYPTO_TAG				"SIP_Crypto"
#define		WALKERSUN_CRYPTO_TAG		"Walkersun_Crypto"
#define		Ctypto_Error_TAG			"Crypto_Error"

// Time
#define		ONE_SECOND					1000
#define		TWO_SECOND					2000
#define		UDP_ALIVE_TIME				120
#define		TCP_TIMEOUT_DUR				10000
#define		COMMAND_NUM					20
#define		COMMAND_LEN					300
#define		ABANDON_TIME				30*ONE_SECOND	//30秒
#define		Forwarding_GW_TIME			15*ONE_SECOND	//15秒
#define		EXPIRES_LIMIT				60				//60秒	
#define		MIN_EXPIRES					10				//10秒	
#define		MAX_EXPIRES					1200			//1200秒
#define		CONTACT_TIME				20*ONE_SECOND	//20秒
#define		SUB_SERVER_TIME				2*CONTACT_TIME	//40秒

// Length
#define		EXTENSION_NO_LENGTH			6
#define		MAX_LENGTH					255
#define		PHONENO_LEGNTH				30
// SIP_Msg Length
#define		SIP_COMMAND_LENGTH			200
#define		SIP_NAME_LENGTH				200
#define		SIP_URL_LENGTH				200
#define		SIP_PORT_LENGTH				200
#define		SIP_TAG_LENGTH				200
#define		SIP_BRANCH_LENGTH			200
#define		SIP_EXPIRES_LENGTH			200
#define		SIP_SDP_LENGTH				712
#define		SIP_MSG_LENGTH				200
#define		SIP_RECEIVED_LENGTH			200
#define		SIP_AUTH_LENGTH				300
#define		SIP_MSG_LEGNTH				2000

// Via
#define		NOCHANGE_VIA				0
#define		ADD_VIA						1
#define		DELETE_VIA					2

// Send Flow
#define		FROM						0
#define		TO							1
#define		EXTRA						2

// Else
#define		EXPIRE_LIMIT_WEIGHT			10

// Crypto Type
enum	CRYPTO_TYPE			{ CT_NoSetting=-1, CT_NoCrypto=0, CT_SIPCrypto=1, CT_Walkersun1=2};

#define	CDMS_DATA_SIZE		1000
typedef struct _CDMS_DATA
{
	char	PhoneNo[50];
	char	ID[50];
	char	IP[64];
	char	IP1[64];
	char	Port[16];
	char	LeftTime[50];
	char	Remarks[20];
	char	Mobile[30];
	char	Email[50];
	char	MAC[64];
	char	Name[50];
	char	Password[50];
	char	Addr[256];
	char	TrunkGW[50];
	bool	Active;
} CDMS_DATA, *pCDMS_DATA;

struct SIP_MSG
{	
	char		Cmd_Str[SIP_MSG_LEGNTH];
	char		Command		   [SIP_COMMAND_LENGTH];
 	char		Command_Name   [SIP_NAME_LENGTH]   ,Command_URL[SIP_URL_LENGTH]   ,Command_Port[SIP_PORT_LENGTH],   Command_Tag[SIP_TAG_LENGTH],   Command_Branch[SIP_BRANCH_LENGTH],   Command_expires[SIP_EXPIRES_LENGTH],Command_Msg[SIP_MSG_LENGTH];
    char		Via_Name       [SIP_NAME_LENGTH]       ,Via_URL[SIP_URL_LENGTH]       ,Via_Port[SIP_PORT_LENGTH],       Via_Tag[SIP_TAG_LENGTH],       Via_Branch[SIP_BRANCH_LENGTH],       Via_expires[SIP_EXPIRES_LENGTH],Via_Received[SIP_RECEIVED_LENGTH];
	char		Contact_Name   [SIP_NAME_LENGTH]   ,Contact_URL[SIP_URL_LENGTH]   ,Contact_Port[SIP_PORT_LENGTH],   Contact_Tag[SIP_TAG_LENGTH],   Contact_Branch[SIP_BRANCH_LENGTH],   Contact_expires[SIP_EXPIRES_LENGTH];
	char		From_Name      [SIP_NAME_LENGTH]      ,From_URL[SIP_URL_LENGTH]      ,From_Port[SIP_PORT_LENGTH],      From_Tag[SIP_TAG_LENGTH],      From_Branch[SIP_BRANCH_LENGTH],      From_expires[SIP_EXPIRES_LENGTH];
	char		To_Name        [SIP_NAME_LENGTH]        ,To_URL[SIP_URL_LENGTH]        ,To_Port[SIP_PORT_LENGTH],        To_Tag[SIP_TAG_LENGTH],        To_Branch[SIP_BRANCH_LENGTH],        To_expires[SIP_EXPIRES_LENGTH];
	char		CallerID_Name  [SIP_NAME_LENGTH]  ,CallerID_URL[SIP_URL_LENGTH]  ,CallerID_Port[SIP_PORT_LENGTH],  CallerID_Tag[SIP_TAG_LENGTH],  CallerID_Branch[SIP_BRANCH_LENGTH],  CallerID_expires[SIP_EXPIRES_LENGTH];
	char		ReferTo_Name   [SIP_NAME_LENGTH]	 ,ReferTo_URL[SIP_URL_LENGTH] ,ReferTo_Port[SIP_PORT_LENGTH],   ReferTo_Tag[SIP_TAG_LENGTH],   ReferTo_Branch[SIP_BRANCH_LENGTH],	ReferTo_expires[SIP_EXPIRES_LENGTH];
	
	BOOL		Via_Flag,Contact_Flag,From_Flag,To_Flag,CallerID_Flag,Command_Flag,ReferTo_Flag;	// int型態
    SOCKET		Socket,TSocket,FSocket;
	sockaddr_in	Addr,TAddr,FAddr;
	CRYPTO_TYPE	From_Crypto_Type,To_Crypto_Type;
	char		CallerID[SIP_MSG_LENGTH],CSeq[SIP_MSG_LENGTH],Expires[SIP_MSG_LENGTH],Content_Length[SIP_MSG_LENGTH],Content_Type[SIP_MSG_LENGTH],Max_Forwards[SIP_MSG_LENGTH],Subject[SIP_MSG_LENGTH],User_Agent[SIP_MSG_LENGTH];
	char		WWW_Authenticate_Realm[SIP_AUTH_LENGTH]  ,WWW_Authenticate_Nonce[SIP_AUTH_LENGTH]  ,WWW_Authenticate_Algorithm[SIP_AUTH_LENGTH];
	char		Authorization_Algorithm[SIP_AUTH_LENGTH],Authorization_Nonce[SIP_AUTH_LENGTH],Authorization_Realm[SIP_AUTH_LENGTH],Authorization_Response[SIP_AUTH_LENGTH],Authorization_Uri[SIP_AUTH_LENGTH],Authorization_Username[SIP_AUTH_LENGTH];
	bool		Authorization_Flag;
	char		Proxy_Authenticate_Realm[SIP_AUTH_LENGTH],Proxy_Authenticate_Nonce[SIP_AUTH_LENGTH],Proxy_Authenticate_Algorithm[SIP_AUTH_LENGTH];
	char		SDP[SIP_SDP_LENGTH],SDP_IP[SIP_URL_LENGTH],SDP_Port[SIP_PORT_LENGTH];
	char		TV_IP[SIP_URL_LENGTH],FV_IP[SIP_URL_LENGTH];
	char		TR_IP[SIP_URL_LENGTH],FR_IP[SIP_URL_LENGTH];
	char		ViaBuf[1000],RecordBuf[1000];
	int			Via_No;
	bool		UDP_C_Flag;
	bool		RTP_Relay_Flag,RL1_Flag;
    char		Relay_TCP_IP[200],Relay_TCP_Port[200];
	bool		Forwarding_GW_Flag;
	char		Cancel_Branch[SIP_BRANCH_LENGTH];	// CANCEL's branch must be the same INVITE's branch

};

//===========================================
#define SUBSERVER_DATA_SIZE	(10*SERVER_PORT_NUM)
typedef struct _SUBSERVER_DATA
{
	bool		Flag;
	SOCKET		Socket;
	sockaddr_in	Addr;
	char		IP[100],Port[16];
}SUBSERVER_DATA, DEST_DATA, *pSUBSERVER_DATA;

#define	MONITOR_DATA_SIZE   500
typedef struct _MONITOR_DATA
{   bool Flag;
	char PhoneNo[20];
    char Monitor_TCP_IP[20];
	char Monitor_TCP_Port[20];
    //char Monitor_UDP_IP[20];
	//char Monitor_UDP_Port[20];
}MONITOR_DATA, *pMONITOR_DATA;

// 紀錄 NAT 在線上之使用者
#define	SIP_ONLINE_SIZE   5000
typedef struct _SIP_ONLINE_DATA
{   
	bool		Flag;
    SOCKET		Socket,Socket1;
	sockaddr_in	Addr;
	char		R_IP[50],V_IP[50],PhoneNo[PHONENO_LEGNTH],Mobile[50],Password[50],Name[100];
	//int		Expires,Expires1,Expires2;
	char		Nonce[100],Proxy_Nonce[100];
	int			Expires_Client,Expires_Counter,Expires_Real,Expires_Limit;
	WORD		S_Port,Server_Port;
	bool		MD5_OK,Expires_OK,Auth_OK,First_Flag,Proxy_MD5_OK,Proxy_Auth_OK;
	int			CDMS_OK;
	DEST_DATA	Trunk_GW;
	CRYPTO_TYPE	Crypto_Type;
}SIP_ONLINE_DATA, *pSIP_ONLINE_DATA;

// 紀錄 SIP Command Queue Buffer
#define	COMMAND_QUEUE_SIZE   2000
enum SIP_STATE { SIP_READY, SIP_INVITE, SIP_CANCEL, SIP_BYE, SIP_100, SIP_REG, SIP_REFER, SIP_NOTIFY, SIP_ERROR,
                 SIP_TALK,  SIP_FAIL, SIP_RELAY_CONNECT, SIP_RELAY_READ, SIP_200, SIP_ACK, SIP_PROXY_AUTH, SIP_ABANDON };
typedef struct  _COMMAND_QUEUE
{   
	SIP_STATE	State;
    SIP_MSG		SIP_Msg,Response_SIP_Msg;
	int			Timer_Counter;
	int			Relay_Cur;
	SOCKET		Relay_TCP_Socket;
	char		Relay_UDP_IP[32],Relay_UDP_FPort[16],Relay_UDP_TPort[16];
//	bool		Forwarding_GW_Flag;
	char		Modify_URL[100];	
	bool		PSTN_Flag,Disconnect_Flag;
	int			Abandon_Time;
	BOOL		Abandon_Flag;	// int型態
}COMMAND_QUEUE, *pCOMMAND_QUEUE;

// Sub-Server Mapping Data
#define MAPPING_DATA_SIZE   1000
typedef struct  _SUBSERVER_MAPPING_DATA
{   
	bool		Flag;
	char		Name[30];
	SUBSERVER_DATA	SubServer_Data[SERVER_PORT_NUM];
	int			SubServer_Num;
}MAPPING_DATA, *pMAPPING_DATA;

// 紀錄在線上之通話
#define SIP_INVITE_SIZE   1000
enum ANSWER_STATE { NoCall, Inviting, Abandon, Answer, Busy, Forwarding, Terminating, Issue, Temporary};
typedef struct  _SIP_INVITE_DATA
{   
	bool			Flag;
    char			FR_IP[20],FV_IP[20],TR_IP[20],TV_IP[20];
	sockaddr_in		FAddr,TAddr;
	SOCKET			FSocket,TSocket;
	char			From_PhoneNo[PHONENO_LEGNTH],To_PhoneNo[PHONENO_LEGNTH];
	char			From_User_Name[64],To_User_Name[64];
	char			Forwarded_PhoneNo[PHONENO_LEGNTH];
	char			CallerID_Name[64];
	char			CallerID[256];
	char			Log_File_Name[64];
	char			Partial_PhoneNo[PHONENO_LEGNTH],Complete_PhoneNo[PHONENO_LEGNTH];
	char			Replace_Code[32],Replaced_PhoneNo[32];
	CTime			Start_Time,End_Time,Invite_Time;
	SYSTEMTIME		Start_TimeDest;
	bool			Forwarding_GW_Flag,OutBound_Flag,InBound_Flag,OutBound_SubServer_Flag;
	bool			ReplaceCode_Flag;
	ANSWER_STATE	Answer_State;
	DEST_DATA		Dest_Data;
	int				SubServer_Count;
	char			Cancel_Branch[SIP_BRANCH_LENGTH];	// CANCEL's branch must be the same INVITE's branch
}SIP_INVITE_DATA, *pSIP_INVITE_DATA;

class CSIP_Server : public CDialog
{
// Construction
public:
	CSIP_Server(CWnd* pParent = NULL);   // standard constructor

	//====================================== Variable ======================================
	// Class
	FILE				*HsfOut1;
	CSIP_Server_Setup	*lpSIP_Server_Setup;
		
	// Winsock Variable
	WORD			SIP_Server_Port[SERVER_PORT_NUM+1];
    SOCKET          SIP_Server_Socket[SERVER_PORT_NUM],Monitor_Socket;
	int				Server_Port_Num;
	char	        ErrMsg[200];
	
	// System Variable
	char			SIP_COMMAND[SIP_COMMAND_NO][20];
    char			Command[COMMAND_NUM][COMMAND_LEN];
	SIP_MSG         SIP_Msg,SIP_Msg2;
	COMMAND_QUEUE   Command_Data[COMMAND_QUEUE_SIZE];
	SIP_ONLINE_DATA SIP_OnLine_Data[SIP_ONLINE_SIZE];
	SIP_INVITE_DATA	SIP_Invite_Data[SIP_INVITE_SIZE];
	int				SIP_OnLine_Num,Command_Queue_Num,SIP_Invite_Num;
	int				Tab1_Item;
	char            NTUT_GW_IP[200],NTUT_GW_Port[200];
	sockaddr_in		NTUT_GW_Addr;
	SOCKET			NTUT_GW_Socket;
	char            Local_IP[100],Outbound_PhoneNo[100],Log_File[100],Search_PhoneNo[100];
	char			*lpSIP_Event_Table[600];
	char			ReplaceCode_Table[100][2][PHONENO_LEGNTH];
	int				ReplaceCode_Num;
	MAPPING_DATA	Mapping_Data[MAPPING_DATA_SIZE];
	int				Mapping_Num;
	CDMS_DATA		CDMS_Data[CDMS_DATA_SIZE];
	int				CDMS_Data_Num;
	
	
	// Moniting & RTP_Relay Variable
	int				Relay_100_Flag;
	MONITOR_DATA    Monitor_Data[MONITOR_DATA_SIZE];
	//RTP_RELAY		RTP_Data[RTP_RELAY_SIZE];
	int             RTP_Relay_Num,Monitor_Data_Num;
    char            RTP_Relay_IP[200],RTP_Relay_Port[200];
	
	// Write Log Variable
	bool			Write_Flag;
	bool			Invite_Log_Check,Call_Log_Check,Call_Flows_Check,Reg_Flows_Check;
	char			Log_Path[_MAX_PATH];
	char			Current_Directory[_MAX_PATH];
	
	// Register & Proxy_Auth Variable
	int				Reg_Auth_Level,Proxy_Auth_Level;
	bool			Reg_Auth,Reg_VAddr_Check,Reg_CNonce_Check,Modify_Expire_Check;
	bool			Proxy_Auth,Proxy_RMD5_Check,Proxy_CNonce_Check;

	// Else
	bool			Forwarding_GW_Check,Forwarding_Mode_Check;
	bool			Progressive_Expires_Check;
	int				Locking_Expires;
	bool			SubServer_Mapping_Check;
	bool			AutoCompose_PW_Check;
	int				Expire_Limit_Weight;
	bool			ReplaceCode_Check;
	int				SubServer_Num;
	SUBSERVER_DATA	SubServer_Data[SUBSERVER_DATA_SIZE];
	char			Main_Server_IP[100],Sub_Server_IP[2][100];
	bool			Access_CDMS_Flag;
	
	
	//====================================== Function ======================================
    //=====----- SIP_Server Function -----=====
	int		Start();
	int		Set_User_Accont();
	void	OnSIP_Server(WPARAM wParam, LPARAM lParam);
	void	OnRTP_Relay(WPARAM wParam,LPARAM lParam);
	void	OnMonitor(WPARAM wParam,LPARAM lParam);
	//----- Process major Function -----
	void	Process_Proxy_Auth(SIP_MSG *lpSIP_Msg);
    void	Process_REGISTER(SIP_MSG *lpSIP_Msg);
    void	Process_ACK(SIP_MSG *lpSIP_Msg);
    void	Process_INVITE(SIP_MSG *lpSIP_Msg);
    void	Process_STATUS(SIP_MSG *lpSIP_Msg);
    void	Process_CANCEL(SIP_MSG *lpSIP_Msg);
	void	Process_PRACK(SIP_MSG *lpSIP_Msg);
    void	Process_BYE(SIP_MSG *lpSIP_Msg);
    void	Process_REFER(SIP_MSG *lpSIP_Msg);
	void	Process_NOTIFY(SIP_MSG *lpSIP_Msg);
	void	Process_SIP_Error(SIP_MSG *lpSIP_Msg,int Error_Code);
	int		Invite_Proc(int Send_Flag,SIP_MSG *lpSIP_Msg,SIP_STATE State,char *Command=NULL);
	//----- UI -----
	void	Show_OnLine();
	void	Show_Invite();
	void	Show_Monitoring();
	//----- System Param -----
	int		Update_SIP_Server_Param(char *Method);
	
	//=====----- SIP_file Function -----=====
	//----- Message Process -----
	int		SIP_Message_Analyzer(SOCKET Socket,sockaddr_in *Addr,char *S1,SIP_MSG *lpData,CRYPTO_TYPE Crypto_Type);
	int		Check_Calling_Party(SIP_MSG *Data);
    int		Check_Relay_Monitor(SIP_MSG *Data); 
    int		Check_Called_Party(SIP_MSG *Data);
	int		Transform_PhoneNo(SIP_MSG *lpSIP_Msg, bool Before_Flag);
	int		Partial_Transform_PhoneNo(SIP_MSG *lpSIP_Msg, bool Before_Flag);
	bool	Partial_Dial_Filter(SIP_MSG *lpSIP_Msg);
	int		OutBound_Transform_PhoneNo(SIP_MSG *lpSIP_Msg, bool Before_Flag);
	bool	OutBound_Filter(SIP_MSG *lpSIP_Msg);
	int		InBound_Transform_PhoneNo(SIP_MSG *lpSIP_Msg, bool Before_Flag);
	bool	InBound_Filter(SIP_MSG *lpSIP_Msg);
	int		ReplaceCode_Transform_PhoneNo(SIP_MSG *lpSIP_Msg, bool Before_Flag);
	bool	ReplaceCode_Filter(SIP_MSG *lpSIP_Msg);
	//----- Modify Message -----
	int		Modify_SDP(SIP_MSG *lpData,char *IP,char *FPort,char *TPort);
    int		Modify_Param(char *S1,char *Head,char *Param);
	int		Modify_Expires(SIP_MSG *lpData,int Step);
	int		Modify_NOTIFY(SIP_MSG *lpSIP_Msg);
	int		Update_Expire(SIP_MSG *lpData, int Online_Cur);
	int		Add_Name(char *Cmd_Str,char *Head,char *Outbound_PhoneNo);
    int		Delete_Name(char *Cmd_Str,char *Head,char *Outbound_PhoneNo);
	int		Edit_Name(char *Cmd_Str,char *Head,char *Forwarding_No);
    int		Add_Record_Route(char *S1,char *Port);
	int		Delete_Record_Route(char *S1);
    int		Delete_Via(char *S1);
	int		Add_Via(char *S1);
    int		Delete_Via111(char *S1,char *ViaBuf);
	int		Add_Via111(char *S1,char *Port,const char *Branch);
	int		Deletion(char *S1,int Beg,int End);
    int		Insertion(char *S1,int Beg,char *Buf);
    int		Remove_Via(char *S1,char *ViaBuf);
    int		Recover_Via(char *S1,char *ViaBuf);
	int		Modify_CSeq(char *S1,char *Head);
	int		Delete_OneLine(char *S1,char *Head);
    //----- String Analysis -----
	int		Filter_String(char *S1,char *S2,char *Name,char *URL,char *Port,char *Tag,char *Branch,char *expires,BOOL *Flag);//,int *Name_Locate);
	int		Get_Field(char *S1,char *Head,char *Field);//int *No);
    int		Get_CSeq_Num(char *S1,char *Head,char *Buf);
    int		Get_SDP(char *S1,char *SDP);
    int		Get_Auth(char *S1,char *Head,char *Buf);
	int		Get_Position(char *S1,char *Head);
	int		Get_NextLine(char *S1);
	int		Extract_Param(char *S1,char *Head,char *Param);
	int		Get_Via(char *S1,char *ViaBuf);
	//----- Monitor -----
	int		Init_Monitor();
    int		Check_Monitor(SIP_MSG *lpSIP_Data);
    int		Check_Monitor_By_PhoneNo(char *PhoneNo);
    int		Insert_Monitor(char *PhoneNo,char *Monitor_TCP_IP,char *Monitor_TCP_Port);
	int		Delete_Monitor(char *PhoneNo);
	//----- Write Log -----
	int		Write_Log(char *Head,char *S1,sockaddr_in *lpAddr,SIP_MSG *lpSIP_Msg);
	int		Write_Call_Log(char *Head, char *S1, SIP_MSG *lpSIP_Msg, sockaddr_in *lpAddr);
	int		Get_Network_Mothod(const char *Head,const SIP_MSG *lpSIP_Msg,CRYPTO_TYPE &Crypto_Type);
    //----- Online Data Process -----
	int		Init_OnLine();
    int		Check_OnLine(char *PhoneNo,sockaddr_in *Addr=NULL);
    int		Save_OnLine  (SIP_MSG *lpData,int Flag);
    int		Clear_OnLine (SIP_MSG *lpData);
	int		Clear_OnLine_By_Cur(int Cur);
	void	Set_OnLine(SIP_MSG *lpData,int Cur,int Flag);
	//----- Command Data Process -----
	int		Init_Command();
	int		Check_Command(SIP_MSG *lpSIP_Data); 
	int		Check_Command_By_Relay_Socket(SOCKET Socket);
	int		Save_Command(SIP_MSG *lpSIP_Data); 
    int		Clear_Command(int Cur);
    int		Update_Command(int Cur,char *Command);
    int		Update_Command_By_SIP_Msg(int Cur,SIP_MSG *lpSIP_Msg);
    int		Update_Command_By_Response(int Cur,SIP_MSG *lpSIP_Msg);
	//----- Invite Data Process -----
	int		Init_Invite();
	int		Clear_Invite(SIP_MSG *lpSIP_Msg);
	int		Clear_Invite_By_Cur(int Invite_Cur);
	int		Save_Invite(SIP_MSG *lpSIP_Msg);
	int		Set_Invite(SIP_MSG *lpSIP_Msg, int Invite_Cur,bool First_Flag);
	int		Write_Invite_Log(SIP_INVITE_DATA *lpInvite_Data,ANSWER_STATE State);
	int		Check_Invite(SIP_MSG *lpSIP_Msg);
	//----- RTP Relay -----
	int		Init_RTP_Relay();
	int		Get_RTP_Relay(SIP_MSG *lpSIP_Msg,BOOL Flag,char *Relay_IP,char *Relay_Port);
    int		Delete_RTP_Relay_By_Socket(SOCKET Socket);
    int		Check_RTP_Relay_By_Socket(SOCKET Socket);
    int		Delete_RTP_Relay_By_SIP_MSG(SIP_MSG *lpSIP_Msg);
    int		Check_RTP_Relay_By_SIP_MSG(SIP_MSG *lpSIP_Msg);
    int		Delete_RTP_Relay_By_Cur(int Cur);
	//----- SubServer Mapping Process -----
	int		Init_SubServer_Mapping();
	int		Check_SubServer_Mapping(char *Name/*,WORD Port*/);
	int		Save_SubServer_Mapping(MAPPING_DATA *lpMapping_Data);
	//----- CDMS Data -----
	int		Init_CDMS_Data();
	int		Check_CDMS_Data(const char *Account);
	int		Save_CDMS_Data(const CDMS_DATA *lpCdms);
	int		Clear_CDMS_Data(int Cur);
	//----- Authenrozation -----
	int		Check_MD5(SIP_MSG *lpData,int Online_Cur,bool Access_Flag);
	int		Proc_Auth(SIP_MSG *lpSIP_Msg, int Command_Cur, int Online_Cur, char *Command);	
	//----- Socket Funciton -----
#if defined(OS_WINDOWS)
	int		Start_TCP_Server (SOCKET *psockfd,WORD Port,DWORD EVENT,HWND Hwnd);
	int		Start_TCP_Client (SOCKET *psockfd,WORD L_Port,WORD R_Port,char *IP,DWORD EVENT,HWND Hwnd);
	int		Start_UDP_Server (SOCKET *psockfd,WORD Port,DWORD EVENT,HWND Hwnd);
	int		Start_UDP_Client (SOCKET *psockfd,sockaddr_in *udpclient,WORD L_Port,WORD R_Port,char *IP,DWORD EVENT,HWND Hwnd);
	int		Close_Socket(WPARAM wParam,HWND Hwnd);
#endif
	int		WSA_Get_ErrTxt(char *S1);
	//----- Otherwise -----
	void	Send_Relay_Command(char *Command,SIP_MSG *lpSIP_Msg,COMMAND_QUEUE *lpCommand);
    void	Send_SIP_Command(char *Command,SIP_MSG *lpData,int Via_Flag,int Send_Flag);
	int		Get_Response_Code(char *Response_Code, int Code);
	bool	Addrcmp(sockaddr_in *lpAddr1,sockaddr_in *lpAddr2);
	int		Create_Directory(char *szPath);
	int		Send_Auth_Msg(char *Command,SIP_MSG *lpSIP_Data,int Via_Flag,int Send_Flag);
	
	//----------------------------------------------------------------------
	int		Check_Flow_State(SIP_MSG *lpSIP_Msg,bool Transmission_Flag);
	bool	OutBound_SubServer_Filter(SIP_MSG *lpSIP_Msg,SUBSERVER_DATA *lpSubServer_Data);
	int		Init_Replace_Code();
	int		Init_SubServer_Data();
	int		Set_SubServer_Data(SUBSERVER_DATA *lpSubServer_Data,char *SubServer_IP,char *SubServer_Port);
	int		Save_SubServer_Data(SUBSERVER_DATA *lpSubServer_Data);
	int		Check_SubServer_Data(SUBSERVER_DATA *lpSubServer_Data);
	BOOL	URL_Filter(char *Url);
	int		Update_SubServer_Data(SUBSERVER_DATA *lpSubServer_Data);
	int		Clear_SubServer_Data(int SubServer_Cur);
	int		Access_OnLine_State(SIP_MSG *lpSIP_Msg);
	int		SIP_Syntax_Filter(SIP_MSG *lpSIP_Msg);
	int		Domain_Name_Analysis(char *Url,char *IP);
	bool	Port_Filter(char *Port);
	int		Trunk_GW_Set(DEST_DATA	*lpTrunk_GW,char *Url);
	int		Update_Trunk_GW(char *Old_GW_IP,char *Old_GW_Port);
	int		Edit_URL(char *S1,char *Head,char *URL);
	int		Edition(char *S1,int Beg,int End,char *Buf);
	int		Edit_Port(char *S1,char *Head,char *Port);
	int		Command_Process(char *S1,char *Command,char Flag,int Command_Num,int Command_Len);

// Dialog Data
	//{{AFX_DATA(CSIP_Server)
	enum { IDD = IDD_SIP_SERVER };
	CTabCtrl	m_Tab1;
	CListCtrl	m_List1;
	CString	m_IP;
	CString	m_Port;
	CString	m_NTUT_IP;
	CString	m_NTUT_Port;
	CString	m_Outbound_PhoneNo;
	CString	m_Log_File;
	CString	m_Number_Search;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSIP_Server)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSIP_Server)
	afx_msg void OnTimer(UINT nIDEvent);
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeEdit1();
	afx_msg void OnChangeEdit2();
	afx_msg void OnChangeEdit3();
	afx_msg void OnChangeEdit4();
	afx_msg void OnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButton1();
	afx_msg void OnChangeEdit5();
	afx_msg void OnChangeEdit6();
	afx_msg void OnButton2();
	afx_msg void OnButton3();
	afx_msg void OnChangeEdit7();
	afx_msg void OnBrowse();
	afx_msg void OnAccess_DB();
	afx_msg void OnServer_Setup();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SIP_SERVER_H__9870A5BE_30EC_48E2_93CF_EB83D413CEB6__INCLUDED_)
