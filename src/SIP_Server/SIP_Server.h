#if !defined(AFX_SIP_SERVER_H__9870A5BE_30EC_48E2_93CF_EB83D413CEB6__INCLUDED_)
#define AFX_SIP_SERVER_H__9870A5BE_30EC_48E2_93CF_EB83D413CEB6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SIP_Server.h : header file
//

#include "../StdHeader.h"
// #include <direct.h>
// #include <winsock2.h>
// #include "Security.h"
#include "../module/MD5/Authentication.h"
#include "../module/Crypt/crypt.h"
// #include "SIP_Server_Setup.h"
// #include "CallServerDlg.h"
// #include "MD5/Authentication.cpp"
// #include "type_def.h"
#include "../module/net_interface/net_interface.h"
#include "../module/sctp_client/sctp_client.h"

/////////////////////////////////////////////////////////////////////////////
// CSIP_Server dialog
#define SIP_SERVER_EVENT WM_USER + 0x1233
#define INVITE_EVENT WM_USER + 0x1234
#define RTP_RELAY_EVENT WM_USER + 1030
// #define		MONITOR_EVENT				WM_USER+101

// Timer
#define MONITOR_PORT 8809
#define TIMERID_EXPIRES 66000
#define TIMERID_RTP_RELAY 66001
#define TIMERID_COMMAND 67000
#define TIMERID_INVITE (TIMERID_COMMAND + COMMAND_QUEUE_SIZE)
#define TIMERID_SUBSERVER_EVENT (TIMERID_INVITE + SIP_INVITE_SIZE)

// SIP Server Port
#define SIP_SERVER_PORT 5060
#define SIP_SERVER_PORT_HTTP 80
#define SERVER_PORT_NUM 1
#define SIP_SERVER_PORTS \
	{                    \
		SIP_SERVER_PORT  \
	}

// Path
#define SIP_LOG_PATH "SIP_Server_Log\\INVITE_Log\\"
#define LOG_FILE_NAME "c:/SIP_Log.txt"
#define SETUP_FOLDER "Setup"
#define SERVER_INFO_PATH "Setup/setup.ini"
#define SIP_ONLINE_FILE "Setup/SIP_OnLine.dat"
// #define		SIP_MONITOR_FILE			"Setup/SIP_Monitor.dat"
#define SubServer_Mapping_TABLE "Setup/Sub-Server Mapping Table.ini"
#define Replace_Code_TABLE "Setup/Phone_Table.ini"

#define ALREADY_RELAY "Already_RL"
#define HEAD_SEND "send"
#define HEAD_SEND_YET "send_yet"
#define HEAD_RECV "recv"
#define HEAD_RECVFROM "recvfrom"
#define HEAD_SENDTO "sendto"

// Crypto Type
#define NO_Setting_TAG "NoSetting"
#define NO_CRYPTO_TAG "NoCrypto"
#define SIP_CRYPTO_TAG "SIP_Crypto"
#define WALKERSUN_CRYPTO_TAG "Walkersun_Crypto"
#define Ctypto_Error_TAG "Crypto_Error"

// Time
#define ONE_SECOND 1000
#define TWO_SECOND 2000
#define UDP_ALIVE_TIME 120
#define TCP_TIMEOUT_DUR 10000
#define COMMAND_NUM 20
#define COMMAND_LEN 300
#define ABANDON_TIME 30 * ONE_SECOND	   //30秒
#define Forwarding_GW_TIME 15 * ONE_SECOND //15秒
#define EXPIRES_LIMIT 60				   //60秒
#define MIN_EXPIRES 10					   //10秒
#define MAX_EXPIRES 1200				   //1200秒
#define CONTACT_TIME 20 * ONE_SECOND	   //20秒
#define SUB_SERVER_TIME 2 * CONTACT_TIME   //40秒

// Length
#define EXTENSION_NO_LENGTH 6
#define MAX_LENGTH 255
#define PHONENO_LEGNTH 30
// SIP_Msg Length
#define SIP_COMMAND_LENGTH 200
#define SIP_NAME_LENGTH 200
#define SIP_URL_LENGTH 200
#define SIP_PORT_LENGTH 200
#define SIP_TAG_LENGTH 200
#define SIP_BRANCH_LENGTH 200
#define SIP_EXPIRES_LENGTH 200
#define SIP_SDP_LENGTH 712
#define SIP_MSG_LENGTH 200
#define SIP_RECEIVED_LENGTH 200
#define SIP_AUTH_LENGTH 300
#define SIP_MSG_LEGNTH 4096
#define SIP_URI_LENGTH 512

// Via
#define NOCHANGE_VIA 0
#define ADD_VIA 1
#define DELETE_VIA 2

// Send Flow
#define FROM 0
#define TO 1
#define EXTRA 2

// Else
#define MAX_REG_LEVEL 4
#define MAX_PROXY_LEVEL 3
#define EXPIRE_LIMIT_WEIGHT 2

#define SUBSERVER_DATA_SIZE (10 * SERVER_PORT_NUM)


#define SIP_COMMAND_NO 13
const char SIP_COMMAND[SIP_COMMAND_NO][20] =
{
	"REGISTER",		// SIP_COMMAND[0]
	"INVITE",		// SIP_COMMAND[1]
	"CANCEL",		// SIP_COMMAND[2]
	"ACK",			// SIP_COMMAND[3]
	"BYE",			// SIP_COMMAND[4]
	"OPTIONS",		// SIP_COMMAND[5]
	"REFER",		// SIP_COMMAND[6]
	"NOTIFY",		// SIP_COMMAND[7]
	"SUBSCRIBE",	// SIP_COMMAND[8]
	"PUBLISH",		// SIP_COMMAND[9]
	"PRACK",		// SIP_COMMAND[10]
	"UPDATE",		// SIP_COMMAND[11]
	"SIP/2.0"		// SIP_COMMAND[12]
};
	
#define CDMS_DATA_SIZE 50
typedef struct _CDMS_DATA
{
	char PhoneNo[50];
	char ID[50];
	char IP[64];
	char IP1[64];
	char Port[16];
	char LeftTime[50];
	char Remarks[20];
	char Mobile[30];
	char Email[50];
	char MAC[64];
	char Name[50];
	char Password[50];
	char Addr[256];
	char TrunkGW[50];
	bool Active;

	void reset()
	{
		memset(this, 0, sizeof(_CDMS_DATA));
	}
} CDMS_DATA, *pCDMS_DATA;

struct SIP_MSG
{
	char Cmd_Str[SIP_MSG_LEGNTH];
	char Command[SIP_COMMAND_LENGTH];
	char Command_Name[SIP_NAME_LENGTH], Command_URL[SIP_URL_LENGTH], Command_Port[SIP_PORT_LENGTH], Command_Tag[SIP_TAG_LENGTH], Command_Branch[SIP_BRANCH_LENGTH], Command_expires[SIP_EXPIRES_LENGTH], Command_Msg[SIP_MSG_LENGTH];
	char Via_Name[SIP_NAME_LENGTH], Via_URL[SIP_URL_LENGTH], Via_Port[SIP_PORT_LENGTH], Via_Tag[SIP_TAG_LENGTH], Via_Branch[SIP_BRANCH_LENGTH], Via_expires[SIP_EXPIRES_LENGTH], Via_Received[SIP_RECEIVED_LENGTH];
	char Contact_Name[SIP_NAME_LENGTH], Contact_URL[SIP_URL_LENGTH], Contact_Port[SIP_PORT_LENGTH], Contact_Tag[SIP_TAG_LENGTH], Contact_Branch[SIP_BRANCH_LENGTH], Contact_expires[SIP_EXPIRES_LENGTH];
	char From_Name[SIP_NAME_LENGTH], From_URL[SIP_URL_LENGTH], From_Port[SIP_PORT_LENGTH], From_Tag[SIP_TAG_LENGTH], From_Branch[SIP_BRANCH_LENGTH], From_expires[SIP_EXPIRES_LENGTH];
	char To_Name[SIP_NAME_LENGTH], To_URL[SIP_URL_LENGTH], To_Port[SIP_PORT_LENGTH], To_Tag[SIP_TAG_LENGTH], To_Branch[SIP_BRANCH_LENGTH], To_expires[SIP_EXPIRES_LENGTH];
	char CallerID_Name[SIP_NAME_LENGTH], CallerID_URL[SIP_URL_LENGTH], CallerID_Port[SIP_PORT_LENGTH], CallerID_Tag[SIP_TAG_LENGTH], CallerID_Branch[SIP_BRANCH_LENGTH], CallerID_expires[SIP_EXPIRES_LENGTH];
	char ReferTo_Name[SIP_NAME_LENGTH], ReferTo_URL[SIP_URL_LENGTH], ReferTo_Port[SIP_PORT_LENGTH], ReferTo_Tag[SIP_TAG_LENGTH], ReferTo_Branch[SIP_BRANCH_LENGTH], ReferTo_expires[SIP_EXPIRES_LENGTH];

	BOOL Via_Flag, Contact_Flag, From_Flag, To_Flag, CallerID_Flag, Command_Flag, ReferTo_Flag; // int型態

	DEST_DATA *pDest, FromDest, ToDest;
	// NetConnect_t *pNc, ToNc, FromNc;
	// sockaddr_mix *pAddr, TAddr, FAddr;
	// CRYPTO_TYPE From_Crypto_Type, To_Crypto_Type;
	// char TV_IP[SIP_URL_LENGTH], FV_IP[SIP_URL_LENGTH];
	// char TR_IP[SIP_URL_LENGTH], FR_IP[SIP_URL_LENGTH];

	char CallerID[SIP_MSG_LENGTH], CSeq[SIP_MSG_LENGTH], Expires[SIP_MSG_LENGTH], Content_Length[SIP_MSG_LENGTH], Content_Type[SIP_MSG_LENGTH], Max_Forwards[SIP_MSG_LENGTH], Subject[SIP_MSG_LENGTH], User_Agent[SIP_MSG_LENGTH];
	char WWW_Authenticate_Realm[SIP_AUTH_LENGTH], WWW_Authenticate_Nonce[SIP_AUTH_LENGTH], WWW_Authenticate_Algorithm[SIP_AUTH_LENGTH], WWW_Authenticate_Opaque[SIP_AUTH_LENGTH], WWW_Authenticate_Qop[SIP_AUTH_LENGTH];
	char Authorization_Algorithm[SIP_AUTH_LENGTH], Authorization_Nonce[SIP_AUTH_LENGTH], Authorization_Opaque[SIP_AUTH_LENGTH], Authorization_Realm[SIP_AUTH_LENGTH], Authorization_NC[SIP_AUTH_LENGTH], Authorization_CNonce[SIP_AUTH_LENGTH], Authorization_Qop[SIP_AUTH_LENGTH], Authorization_Response[SIP_AUTH_LENGTH], Authorization_Uri[SIP_AUTH_LENGTH], Authorization_Username[SIP_AUTH_LENGTH];
	bool Authorization_Flag;
	char Proxy_Authenticate_Realm[SIP_AUTH_LENGTH], Proxy_Authenticate_Nonce[SIP_AUTH_LENGTH], Proxy_Authenticate_Algorithm[SIP_AUTH_LENGTH];
	bool Supported_100rel, Supported_Replaces;
	bool Require_100rel, Require_Replaces;
	char RSeq[20];
	char SDP[SIP_SDP_LENGTH], SDP_IP[SIP_URL_LENGTH], SDP_Audio_Port[SIP_PORT_LENGTH], SDP_Image_Port[SIP_PORT_LENGTH];
	char ViaBuf[1000], RecordBuf[1000];
	int Via_No;
	bool UDP_C_Flag;
	bool RTP_Relay_Flag, RL1_Flag;
	char Relay_TCP_IP[200], Relay_TCP_Port[200];
	bool Forwarding_GW_Flag;
	char Cancel_Branch[SIP_BRANCH_LENGTH]; // CANCEL's branch must be the same INVITE's branch
	bool IMS_3GPP_Flag;					   // 3G, 4G, 5G
	char P_Associated_URI[SIP_URI_LENGTH];
	char FromPhoneId[50], ToPhoneId[50];


	SIP_MSG()
	{
		reset();
	}
	void reset()
	{

		Cmd_Str[0] = 0;
		Command[0] = 0;
		Command_Name[0] = Command_URL[0] = Command_Port[0] = Command_Tag[0] = Command_Branch[0] = Command_expires[0] = Command_Msg[0] = 0;
		Via_Name[0] = Via_URL[0] = Via_Port[0] = Via_Tag[0] = Via_Branch[0] = Via_expires[0] = Via_Received[0] = 0;
		Contact_Name[0] = Contact_URL[0] = Contact_Port[0] = Contact_Tag[0] = Contact_Branch[0] = Contact_expires[0] = 0;
		From_Name[0] = From_URL[0] = From_Port[0] = From_Tag[0] = From_Branch[0] = From_expires[0] = 0;
		To_Name[0] = To_URL[0] = To_Port[0] = To_Tag[0] = To_Branch[0] = To_expires[0] = 0;
		CallerID_Name[0] = CallerID_URL[0] = CallerID_Port[0] = CallerID_Tag[0] = CallerID_Branch[0] = CallerID_expires[0] = 0;
		ReferTo_Name[0] = ReferTo_URL[0] = ReferTo_Port[0] = ReferTo_Tag[0] = ReferTo_Branch[0] = ReferTo_expires[0] = 0;
		Via_Flag = Contact_Flag = From_Flag = To_Flag = CallerID_Flag = Command_Flag = ReferTo_Flag; // int型態
		pDest = NULL;
		FromDest.reset();
		ToDest.reset();
		CallerID[0] = CSeq[0] = Expires[0] = Content_Length[0] = Content_Type[0] = Max_Forwards[0] = Subject[0] = User_Agent[0] = 0;
		WWW_Authenticate_Realm[0] = WWW_Authenticate_Nonce[0] = WWW_Authenticate_Algorithm[0] = WWW_Authenticate_Qop[0] = WWW_Authenticate_Opaque[0] = 0;
		Authorization_Algorithm[0] = Authorization_Nonce[0] = Authorization_Realm[0] = Authorization_NC[0] = Authorization_CNonce[0] = Authorization_Qop[0] = Authorization_Response[0] = Authorization_Uri[0] = Authorization_Username[0] = 0;
		Authorization_Flag = false;
		Proxy_Authenticate_Realm[0] = Proxy_Authenticate_Nonce[0] = Proxy_Authenticate_Algorithm[0] = 0;
		Supported_100rel = Supported_Replaces = false;
		Require_100rel = Require_Replaces = false;
		SDP[SIP_SDP_LENGTH] = SDP_IP[0] = SDP_Audio_Port[0] = SDP_Image_Port[0] = 0;
		ViaBuf[0] = RecordBuf[0];
		Via_No = 0;
		UDP_C_Flag = false;
		RTP_Relay_Flag = RL1_Flag = false;
		Relay_TCP_IP[0] = Relay_TCP_Port[0] = 0;
		Forwarding_GW_Flag = false;
		Cancel_Branch[0] = 0;
		IMS_3GPP_Flag = false;
		P_Associated_URI[0] = 0;
		FromPhoneId[0] = ToPhoneId[0] = 0;
	}
};

//===========================================

// #define	MONITOR_DATA_SIZE   500
// typedef struct _MONITOR_DATA
// {   bool Flag;
// 	char PhoneNo[20];
//     char Monitor_TCP_IP[20];
// 	char Monitor_TCP_Port[20];
//     //char Monitor_UDP_IP[20];
// 	//char Monitor_UDP_Port[20];
// }MONITOR_DATA, *pMONITOR_DATA;

// 紀錄 NAT 在線上之使用者
#define SIP_ONLINE_SIZE 50
typedef struct _SIP_ONLINE_DATA
{
	bool Flag;
	DEST_DATA Dest;
	// NetConnect_t Nc;
	// sockaddr_mix Addr;
	// char R_IP[64], V_IP[64];
	char PhoneId[PHONENO_LEGNTH];	// for Auth
	char PhoneNo[PHONENO_LEGNTH];
	char Mobile[50];
	char Password[50];
	char Name[100];
	//int			Expires,Expires1,Expires2;
	char Nonce[100], Proxy_Nonce[100];
	int Expires_Client, Expires_Counter, Expires_Real, Expires_Limit;
	WORD S_Port, Server_Port;
	bool MD5_OK, Expires_OK, Auth_OK, First_Flag, Proxy_MD5_OK, Proxy_Auth_OK;
	int CDMS_OK;
	DEST_DATA Trunk_GW;
	CRYPTO_TYPE Crypto_Type;
	MIXTIMER MixTimer;

	void reset()
	{
		Flag = false;
		Dest.reset();
		PhoneId[0] = 0;
		PhoneNo[0] = 0;
		Mobile[0] = 0;
		Password[0] = 0;
		Name[0] = 0;
		//int			Expires,Expires1,Expires2;
		Nonce[0] = Proxy_Nonce[0] = 0;
		Expires_Client = Expires_Counter = Expires_Real = Expires_Limit = 0;
		S_Port = Server_Port = 0;
		MD5_OK = Expires_OK = Auth_OK = First_Flag = Proxy_MD5_OK = Proxy_Auth_OK = false;
		CDMS_OK = 0;
		Trunk_GW.reset();
		Crypto_Type = CT_NoCrypto;
		memset(&MixTimer, 0, sizeof(MixTimer));
	}
} SIP_ONLINE_DATA, *pSIP_ONLINE_DATA;

// 紀錄 SIP Command Queue Buffer
#define COMMAND_QUEUE_SIZE 20
enum SIP_STATE
{
	SIP_READY,
	SIP_INVITE,
	SIP_CANCEL,
	SIP_BYE,
	SIP_100,
	SIP_REG,
	SIP_REFER,
	SIP_NOTIFY,
	SIP_PRACK,
	SIP_UPDATE,
	SIP_SUBSCRIBE,
	SIP_PUBLISH,
	SIP_ERROR,
	SIP_TALK,
	SIP_FAIL,
	SIP_RELAY_CONNECT,
	SIP_RELAY_READ,
	SIP_200,
	SIP_ACK,
	SIP_PROXY_AUTH,
	SIP_ABANDON
};

const char Command_State_Table[21][20] = 
{
	"SIP_READY", 
	"SIP_INVITE", 
	"SIP_CANCEL", 
	"SIP_BYE", 
	"SIP_100", 
	"SIP_REG", 
	"SIP_REFER", 
	"SIP_NOTIFY", 
	"SIP_PRACK", 
	"SIP_UPDATE",
	"SIP_SUBSCRIBE", 
	"SIP_PUBLISH", 
	"SIP_ERROR", 
	"SIP_TALK", 
	"SIP_FAIL", 
	"SIP_RELAY_CONNECT", 
	"SIP_RELAY_READ", 
	"SIP_200", 
	"SIP_ACK", 
	"SIP_PROXY_AUTH",
	"SIP_ABANDON"
};

typedef struct _COMMAND_QUEUE
{
	SIP_STATE State;
	SIP_MSG SIP_Msg, Response_SIP_Msg;
	int Timer_Counter;
	char FromPhoneId[50], ToPhoneId[50];
	char From_SDP_IP[SIP_URL_LENGTH], From_External_SDP_IP[SIP_URL_LENGTH];
	char To_SDP_IP[SIP_URL_LENGTH], To_External_SDP_IP[SIP_URL_LENGTH];
	uint16_t	From_SDP_Audio_Port, From_SDP_Image_Port, From_External_SDP_Audio_Port, From_External_SDP_Image_Port;
	uint16_t	To_SDP_Audio_Port, To_SDP_Image_Port, To_External_SDP_Audio_Port, To_External_SDP_Image_Port;
	int Relay_Cur;
	DEST_DATA Relay_TCP_Dest;
	char Relay_UDP_IP[32], Relay_UDP_FPort[16], Relay_UDP_TPort[16];
	//	bool		Forwarding_GW_Flag;
	char Modify_URL[100];
	bool PSTN_Flag, Disconnect_Flag;
	int Abandon_Time;
	BOOL Abandon_Flag; // int型態
	MIXTIMER MixTimer;
	// For 3GPP
	bool	SipCqi1_Flag;

	void reset()
	{
		State = SIP_READY;
		SIP_Msg.reset();
		Response_SIP_Msg.reset();
		Timer_Counter = 0;
		FromPhoneId[0] = ToPhoneId[0] = 0;
		From_SDP_IP[0] = From_External_SDP_IP[0] = 0;
		To_SDP_IP[0] = To_External_SDP_IP[0] = 0;
		From_SDP_Audio_Port = From_SDP_Image_Port = From_External_SDP_Audio_Port = From_External_SDP_Image_Port = 0;
		To_SDP_Audio_Port = To_SDP_Image_Port = To_External_SDP_Audio_Port = To_External_SDP_Image_Port = 0;
		Relay_Cur = 0;
		Relay_TCP_Dest.reset();
		Relay_UDP_IP[0] = Relay_UDP_FPort[0] = Relay_UDP_TPort[0] = 0;
		// Forwarding_GW_Flag = false;
		Modify_URL[0] = 0;
		PSTN_Flag = false, Disconnect_Flag = false;
		Abandon_Time = ABANDON_TIME;
		Abandon_Flag = false;
		memset(&MixTimer, 0, sizeof(MixTimer));
		SipCqi1_Flag = false;
	}
} COMMAND_QUEUE, *pCOMMAND_QUEUE;

// Sub-Server Mapping Data
#define MAPPING_DATA_SIZE 10
typedef struct _SUBSERVER_MAPPING_DATA
{
	bool Flag;
	char Name[30];
	DEST_DATA SubServer_Data[SERVER_PORT_NUM];
	int SubServer_Num;

	void reset()
	{
		Flag = false;
		Name[0] = 0;
		for (int i = 0; i < SERVER_PORT_NUM; i++)
			SubServer_Data[i].reset();
		SubServer_Num = 0;
	}
} MAPPING_DATA, *pMAPPING_DATA;

// 紀錄在線上之通話
#define SIP_INVITE_SIZE 10
enum ANSWER_STATE
{
	NoCall,
	Inviting,
	Abandon,
	Answer,
	Busy,
	Forwarding,
	Terminating,
	Issue,
	Temporary
};
typedef struct _SIP_INVITE_DATA
{
	bool Flag;
	DEST_DATA *pFromDest, *pToDest;
	// char FR_IP[20], FV_IP[20], TR_IP[20], TV_IP[20];
	// sockaddr_mix FAddr, TAddr;
	// NetConnect_t *pToNc, *pFromNc;
	// CRYPTO_TYPE From_Crypto_Type, To_Crypto_Type;
	char From_PhoneNo[PHONENO_LEGNTH], To_PhoneNo[PHONENO_LEGNTH];
	char From_User_Name[64], To_User_Name[64];
	char Forwarded_PhoneNo[PHONENO_LEGNTH];
	char CallerID_Name[64];
	char CallerID[256];
	char Log_File_Name[64];
	char Partial_PhoneNo[PHONENO_LEGNTH], Complete_PhoneNo[PHONENO_LEGNTH];
	char Replace_Code[32], Replaced_PhoneNo[32];
	timeb Start_Time, End_Time, Invite_Time;
	timeval Start_TimeDest;
	bool Forwarding_GW_Flag, OutBound_Flag, InBound_Flag, OutBound_SubServer_Flag;
	bool ReplaceCode_Flag;
	ANSWER_STATE Answer_State;
	DEST_DATA Dest_Data;
	int SubServer_Count;
	char Cancel_Branch[SIP_BRANCH_LENGTH]; // CANCEL's branch must be the same INVITE's branch
	MIXTIMER MixTimer;

	void reset()
	{
		Flag = false;
		pFromDest = pToDest = NULL;
		From_PhoneNo[0] = To_PhoneNo[0] = 0;
		From_User_Name[0] = To_User_Name[0] = 0;
		Forwarded_PhoneNo[0] = 0;
		CallerID_Name[0] = 0;
		CallerID[0] = 0;
		Log_File_Name[0] = 0;
		Partial_PhoneNo[0] = Complete_PhoneNo[0] = 0;
		Replace_Code[0] = Replaced_PhoneNo[0] = 0;
		memset(&Start_Time, 0, sizeof(Start_Time));
		memset(&End_Time, 0, sizeof(End_Time));
		memset(&Invite_Time, 0, sizeof(Invite_Time));
		memset(&Start_TimeDest, 0, sizeof(Start_TimeDest));
		Forwarding_GW_Flag = OutBound_Flag = InBound_Flag = OutBound_SubServer_Flag = false;
		ReplaceCode_Flag = false;
		Answer_State = NoCall;
		Dest_Data.reset();
		SubServer_Count = 0;
		Cancel_Branch[0] = 0;
		memset(&MixTimer, 0, sizeof(MixTimer));
	}
} SIP_INVITE_DATA, *pSIP_INVITE_DATA;

class CSIP_Server
{
	// Construction
public:
	CSIP_Server(); // standard constructor
	~CSIP_Server();

	//====================================== Variable ======================================
	// Class
	FILE *HsfOut1;
	// CSIP_Server_Setup	*lpSIP_Server_Setup;
	CNetIF *m_lpNetIF;

	// Winsock Variable
	WORD SIP_Server_Port[SERVER_PORT_NUM + 1];
	// SOCKET 			SIP_Server_Socket[SERVER_PORT_NUM];
	// SOCKET			Monitor_Socket;
	NetConnect_t SIP_Server_Nc[SERVER_PORT_NUM];
	int Server_Port_Num;
	char ErrMsg[200];

	// System Variable
	char Command[COMMAND_NUM][COMMAND_LEN];
	SIP_MSG SIP_Msg, SIP_Msg2;
	COMMAND_QUEUE Command_Data[COMMAND_QUEUE_SIZE];
	SIP_ONLINE_DATA SIP_OnLine_Data[SIP_ONLINE_SIZE];
	SIP_INVITE_DATA SIP_Invite_Data[SIP_INVITE_SIZE];
	MIXTIMER ExpiresMixTimer;
	int SIP_OnLine_Num, Command_Queue_Num, SIP_Invite_Num;
	int Tab1_Item;
	DEST_DATA NTUT_GW_Dest;
	char NTUT_GW_IP[200], NTUT_GW_Port[200];
	// sockaddr_mix NTUT_GW_Addr;
	// NetConnect_t NTUT_GW_Nc;
	// CRYPTO_TYPE NTUT_GW_Crypto_Type;
	char Local_IP[64], Outbound_PhoneNo[100], Log_File[100], Search_PhoneNo[100];
	char *lpSIP_Event_Table[600];
	char ReplaceCode_Table[100][2][PHONENO_LEGNTH];
	int ReplaceCode_Num;
	MAPPING_DATA Mapping_Data[MAPPING_DATA_SIZE];
	int Mapping_Num;
	CDMS_DATA CDMS_Data[CDMS_DATA_SIZE];
	int CDMS_Data_Num;

	// Moniting & RTP_Relay Variable
	int Relay_100_Flag;
	// MONITOR_DATA    Monitor_Data[MONITOR_DATA_SIZE];
	//RTP_RELAY		RTP_Data[RTP_RELAY_SIZE];
	int RTP_Relay_Num;
	// int				Monitor_Data_Num;
	char RTP_Relay_IP[200], RTP_Relay_Port[200];

	// Write Log Variable
	bool Write_Flag;
	bool Invite_Log_Check, Call_Log_Check, Call_Flows_Check, Reg_Flows_Check;
	char Log_Path[_MAX_PATH];
	char Current_Directory[PATH_MAX];

	// Register & Proxy_Auth Variable
	int Reg_Auth_Level, Proxy_Auth_Level;
	bool Reg_Auth, Reg_VAddr_Check, Reg_CNonce_Check, Modify_Expire_Check;
	bool Proxy_Auth, Proxy_RMD5_Check, Proxy_CNonce_Check;

	// Else
	bool Forwarding_GW_Check, Forwarding_Mode_Check;
	bool Progressive_Expires_Check;
	int Locking_Expires;
	bool SubServer_Mapping_Check;
	bool AutoCompose_PW_Check;
	int Expire_Limit_Weight;
	bool ReplaceCode_Check;
	int SubServer_Num;
	DEST_DATA SubServer_Data[SUBSERVER_DATA_SIZE];
	// char Main_Server_IP[100], Sub_Server_IP[2][100];
	bool Access_CDMS_Flag;
	char S1AP_Server_IP[64];
	uint16_t S1AP_Server_Port;
	int	S1AP_Socket;
	struct sockaddr_in S1AP_Addr;
	bool SIP_Alg_Flag;


	//====================================== Function ======================================
	//=====----- SIP_Server Function -----=====
	int Start();
	int Set_User_Accont();
	void OnSIP_Server(NetConnect_t *wParam, LPARAM lParam);
	void OnRTP_Relay(NetConnect_t *wParam, LPARAM lParam);
	// void	OnMonitor(WPARAM wParam, LPARAM lParam);
	//----- Process major Function -----
	void Process_Proxy_Auth(SIP_MSG *lpSIP_Msg);
	void Process_REGISTER(SIP_MSG *lpSIP_Msg);
	void Process_ACK(SIP_MSG *lpSIP_Msg);
	void Process_INVITE(SIP_MSG *lpSIP_Msg);
	void Process_STATUS(SIP_MSG *lpSIP_Msg);
	void Process_CANCEL(SIP_MSG *lpSIP_Msg);
	void Process_PRACK(SIP_MSG *lpSIP_Msg);
	void Process_UPDATE(SIP_MSG *lpSIP_Msg);
	void Process_BYE(SIP_MSG *lpSIP_Msg);
	void Process_REFER(SIP_MSG *lpSIP_Msg);
	void Process_NOTIFY(SIP_MSG *lpSIP_Msg);
	void Process_SUBSCRIBE(SIP_MSG *lpSIP_Msg);
	void Process_PUBLISH(SIP_MSG *lpSIP_Msg);
	void Process_SIP_Error(SIP_MSG *lpSIP_Msg, int Error_Code);
	int Invite_Proc(int Send_Flag, SIP_MSG *lpSIP_Msg, SIP_STATE State, const char *Command = NULL);
	// //----- UI -----
	// void	Show_OnLine();
	// void	Show_Invite();
	// void	Show_Monitoring();
	//----- System Param -----
	int Update_SIP_Server_Param(char *Method);

	//=====----- SIP_file Function -----=====
	//----- Message Process -----
	int SIP_Message_Analyzer(NetConnect_t *nc, sockaddr_mix *lpAddr, char *S1, SIP_MSG *Data, CRYPTO_TYPE Crypto_Type);
	int Check_Calling_Party(SIP_MSG *Data);
	// int		Check_Relay_Monitor(SIP_MSG *Data);
	int Check_Called_Party(SIP_MSG *Data);
	int Transform_PhoneNo(SIP_MSG *lpSIP_Msg, bool Before_Flag);
	int Partial_Transform_PhoneNo(SIP_MSG *lpSIP_Msg, bool Before_Flag);
	bool Partial_Dial_Filter(SIP_MSG *lpSIP_Msg);
	int OutBound_Transform_PhoneNo(SIP_MSG *lpSIP_Msg, bool Before_Flag);
	bool OutBound_Filter(SIP_MSG *lpSIP_Msg);
	int InBound_Transform_PhoneNo(SIP_MSG *lpSIP_Msg, bool Before_Flag);
	bool InBound_Filter(SIP_MSG *lpSIP_Msg);
	int ReplaceCode_Transform_PhoneNo(SIP_MSG *lpSIP_Msg, bool Before_Flag);
	bool ReplaceCode_Filter(SIP_MSG *lpSIP_Msg);
	//----- Modify Message -----
	int Modify_SDP(SIP_MSG *lpData, char *IP, char *FPort, char *TPort);
	int Modify_SDP(SIP_MSG *lpData, char *IP, uint16_t FPort, uint16_t TPort);
	int Modify_Param(char *S1, const char *Head, char *Param);
	int Modify_Expires(SIP_MSG *lpData, int Step);
	int Modify_NOTIFY(SIP_MSG *lpSIP_Msg);
	int Update_Expire(SIP_MSG *lpData, int Online_Cur);
	int Add_Name(char *Cmd_Str, const char *Head, char *Outbound_PhoneNo);
	int Delete_Name(char *Cmd_Str, const char *Head, char *Outbound_PhoneNo);
	int Edit_Name(char *Cmd_Str, const char *Head, char *Forwarding_No);
	int Add_Record_Route(char *S1, char *Port);
	int Delete_Record_Route(char *S1);
	int Delete_Via(char *S1);
	int Add_Via(char *S1);
	int Delete_Via111(char *S1, char *ViaBuf);
	int Add_Via111(char *S1, char *Port, const char *Branch);
	int Deletion(char *S1, int Beg, int End);
	int Deletion(char *S1, size_t S1_size, int Beg, int End);
	int Insertion(char *S1, int Beg, const char *Buf);
	int Insertion(char *S1, size_t S1_size, int Beg, const char *Buf);
	int Edition(char *S1, size_t S1_size, int Beg, int End, const char *Buf);
	int Remove_Via(char *S1, char *ViaBuf);
	int Recover_Via(char *S1, char *ViaBuf);
	int Modify_CSeq(char *S1, const char *Head);
	int Delete_OneLine(char *S1, const char *Head);
	int Add_Header(char *Cmd_Msg, size_t CM_size, const char *Header, const char *Field, const char *Upper_Head1, const char *Upper_Head2);
	int Delete_Param(char *S1, size_t S1_size, const char *Head);
	int Edit_URL(char *S1, size_t S1_size, const char *Head, const char *URL);
	int Edit_Port(char *S1, size_t S1_size, const char *Head, const char *Port);
	//----- String Analysis -----
	int Filter_String(char *S1, const char *S2, char *Name, char *URL, char *Port, char *Tag, char *Branch, char *expires, BOOL *Flag); //, int *Name_Locate);
	int Get_Field(char *S1, const char *Head, char *Field);																				//int *No);
	int Get_CSeq_Num(char *S1, const char *Head, char *Buf);
	int Get_SDP(char *S1, char *SDP);
	int Get_Auth(char *S1, const char *Head, char *Buf);
	int Get_Position(char *S1, const char *Head);
	int Get_NextLine(char *S1);
	int Extract_Param(char *S1, const char *Head, char *Param);
	int Get_Via(char *S1, char *ViaBuf);
	int Get_Line_Field(char *S1, const char *Head, char *Field, size_t Field_size);
	int Get_Supported(bool Support_Require, char *S1, SIP_MSG *lpSIP_Msg);

	// //----- Monitor -----
	// int		Init_Monitor();
	// int		Check_Monitor(SIP_MSG *lpSIP_Data);
	// int		Check_Monitor_By_PhoneNo(char *PhoneNo);
	// int		Insert_Monitor(char *PhoneNo, char *Monitor_TCP_IP, char *Monitor_TCP_Port);
	// int		Delete_Monitor(char *PhoneNo);
	//----- Write Log -----
	int Write_Log(const char *Head, char *S1, sockaddr_mix *lpAddr, SIP_MSG *lpSIP_Msg);
	int Write_Call_Log(const char *Head, char *S1, SIP_MSG *lpSIP_Msg, sockaddr_mix *lpAddr);
	int Get_Network_Mothod(const char *Head, const SIP_MSG *lpSIP_Msg, CRYPTO_TYPE &Crypto_Type);
	//----- Online Data Process -----
	int Init_OnLine();
	int Show_OnLine();
	int Check_OnLine(char *PhoneNo, sockaddr_mix *Addr = NULL);
	int Save_OnLine(SIP_MSG *lpData, int Flag);
	int Clear_OnLine(SIP_MSG *lpData);
	int Clear_OnLine_By_Cur(int Cur);
	void Set_OnLine(SIP_MSG *lpData, int Cur, int Flag);
	int Clean_All_OnLine_Data();
	//----- Command Data Process -----
	int Init_Command();
	int Check_Command(SIP_MSG *lpSIP_Data);
	int Check_Command_By_Relay_Socket(MIXSOCKET Socket);
	int Save_Command(SIP_MSG *lpSIP_Data);
	int Clear_Command(int Cur);
	int Clean_All_Command_Data();
	int Update_Command(int Cur, const char *Command);
	int Update_Command_By_SIP_Msg(int Cur, SIP_MSG *lpSIP_Msg);
	int Update_Command_By_Response(int Cur, SIP_MSG *lpSIP_Msg);
	//----- Invite Data Process -----
	int Init_Invite();
	int Clear_Invite(SIP_MSG *lpSIP_Msg);
	int Clear_Invite_By_Cur(int Invite_Cur);
	int Save_Invite(SIP_MSG *lpSIP_Msg);
	int Set_Invite(SIP_MSG *lpSIP_Msg, int Invite_Cur, bool First_Flag);
	int Write_Invite_Log(SIP_INVITE_DATA *lpInvite_Data, ANSWER_STATE State);
	int Check_Invite(SIP_MSG *lpSIP_Msg);
	int Clean_All_Invite_Data();
	//----- RTP Relay -----
	int Init_RTP_Relay();
	int Get_RTP_Relay(SIP_MSG *lpSIP_Msg, BOOL Flag, char *Relay_IP, char *Relay_Port);
	// int		Delete_RTP_Relay_By_Socket(SOCKET Socket);
	// int		Check_RTP_Relay_By_Socket(SOCKET Socket);
	int Delete_RTP_Relay_By_SIP_MSG(SIP_MSG *lpSIP_Msg);
	int Check_RTP_Relay_By_SIP_MSG(SIP_MSG *lpSIP_Msg);
	int Delete_RTP_Relay_By_Cur(int Cur);
	//----- SubServer Mapping Process -----
	int Init_SubServer_Mapping();
	int Check_SubServer_Mapping(char *Name /*,WORD Port*/);
	int Save_SubServer_Mapping(MAPPING_DATA *lpMapping_Data);
	int Clear_SubServer_Mapping(int Mapping_Cur);
	int Clean_All_SubServer_Mapping();
	//----- CDMS Data -----
	int Init_CDMS_Data();
	int Check_CDMS_Data(const char *Account);
	int Check_CDMS_Data_By_Id(const char *Id);
	int Save_CDMS_Data(const CDMS_DATA *lpCdms);
	int Clear_CDMS_Data(int Cur);
	//----- Authenrozation -----
	int Check_MD5(SIP_MSG *lpData, int Online_Cur, bool Access_Flag);
	int Proc_Auth(SIP_MSG *lpSIP_Msg, int Command_Cur, int Online_Cur, const char *Command);
	//----- Socket Funciton -----
#if defined(OS_WINDOWS)
	int Start_TCP_Server(SOCKET *psockfd, WORD Port, DWORD EVENT, HWND Hwnd);
	int Start_TCP_Client(SOCKET *psockfd, WORD L_Port, WORD R_Port, char *IP, DWORD EVENT, HWND Hwnd);
	int Start_UDP_Server(SOCKET *psockfd, WORD Port, DWORD EVENT, HWND Hwnd);
	int Start_UDP_Client(SOCKET *psockfd, sockaddr_in *udpclient, WORD L_Port, WORD R_Port, char *IP, DWORD EVENT, HWND Hwnd);
	int Close_Socket(WPARAM wParam, HWND Hwnd);
#endif
	// int		WSA_Get_ErrTxt(char *S1);
	//----- Otherwise -----
	int Send_Relay_Command(const char *Command, SIP_MSG *lpSIP_Msg, COMMAND_QUEUE *lpCommand);
	int Send_SIP_Command(const char *Command, SIP_MSG *lpData, int Via_Flag, int Send_Flag);
	int Send_SIPCQI1_Command(COMMAND_QUEUE *lpCommand, char* s1ap_server_ip, uint16_t s1ap_server_port);
	int Get_Response_Code(char *Response_Code, int Code);
	bool Addrcmp(sockaddr_mix *lpAddr1, sockaddr_mix *lpAddr2);
	// int		Create_Directory(char *szPath);
	int Send_Auth_Msg(const char *Command, SIP_MSG *lpSIP_Data, int Via_Flag, int Send_Flag);
	char *transfer_imsi_data_format(const char *str_imsi, char *hexstr_imsi);

	//----------------------------------------------------------------------
	int Check_Flow_State(SIP_MSG *lpSIP_Msg, bool Transmission_Flag);
	bool OutBound_SubServer_Filter(SIP_MSG *lpSIP_Msg, DEST_DATA *lpSubServer_Data);
	int Init_Replace_Code();
	int Init_SubServer_Data();
	int Set_SubServer_Data(DEST_DATA *lpSubServer_Data, const char *SubServer_IP, unsigned short SubServer_Port, CRYPTO_TYPE Crypto_Type);
	int Save_SubServer_Data(DEST_DATA *lpSubServer_Data);
	int Check_SubServer_Data(DEST_DATA *lpSubServer_Data);
	BOOL URL_Filter(char *Url);
	int Update_SubServer_Data(const char *SubServer_IP, unsigned short SubServer_Port, CRYPTO_TYPE Crypto_Type);
	int Clear_SubServer_Data(int SubServer_Cur);
	int Clean_All_SubServer_Data();
	int Access_OnLine_State(SIP_MSG *lpSIP_Msg);
	int SIP_Syntax_Filter(SIP_MSG *lpSIP_Msg);
	int Domain_Name_Analysis(char *Url, char *IP);
	bool Port_Filter(char *Port);
	int Trunk_GW_Set(DEST_DATA *lpTrunk_GW, char *Url);
	int Update_Trunk_GW(char *Old_GW_IP, unsigned short Old_GW_Port);
	int Command_Process(char *S1, char *Command, char Flag, int Command_Num, int Command_Len);
	int Txt_HSF(char *Buf, int Len);
	int HSF_Txt(char *Buf, int Len1);
	int Hsf_recv(SOCKET Socket, char *Buf, int Len, int Flag);
	int Hsf_send(SOCKET Socket, char *Buf1, int Len, int Flag);

	// Test
	void res_auth_401(char *S44, char *S11);

	// // Dialog Data
	// 	//{{AFX_DATA(CSIP_Server)
	// 	enum { IDD = IDD_SIP_SERVER };
	// CTabCtrl	m_Tab1;
	// CListCtrl	m_List1;
	std::string m_IP;
	std::string m_Port;
	std::string m_NTUT_IP;
	std::string m_NTUT_Port;
	std::string m_Outbound_PhoneNo;
	std::string m_Log_File;
	std::string m_Number_Search;
	// 	//}}AFX_DATA

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSIP_Server)
public:
	// virtual BOOL DestroyWindow();

protected:
	// virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

private:
	static void SocketModule_Callbcak_Function(MIXSOCKET wParam, long lParam, void *lpObj);
	static void MixTimer_Callbcak_Function(MIXTIMER *wParam, void *lpObj);

	// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CSIP_Server)
	void OnTimer(MIXTIMER *wParam);
	// virtual BOOL OnInitDialog();
	// void OnChangeEdit1();
	// void OnChangeEdit2();
	// void OnChangeEdit3();
	// void OnChangeEdit4();
	// void OnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult);
	// void OnButton1();
	// void OnChangeEdit5();
	// void OnChangeEdit6();
	// void OnButton2();
	// void OnButton3();
	// void OnChangeEdit7();
	// void OnBrowse();
	// void OnAccess_DB();
	// void OnServer_Setup();
	//}}AFX_MSG
	// DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SIP_SERVER_H__9870A5BE_30EC_48E2_93CF_EB83D413CEB6__INCLUDED_)
