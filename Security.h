#include	"StdHeader.h"
#ifdef	OS_LINUX
#include	"../definition.h"
#include	"../WinSock_API.h"
#include	"Def_Header/Functions.h"
#endif


//extern char *API_ErrMsg;
int Command_Process(char *S1,char *Command,char Flag,int Command_Num,int Command_Len);
int Check_File_Size(char *Name);
int Str2Bin(char *C_keypart_B,unsigned char *keypart_B);
int Bin2Str(unsigned char *keypart_B,char *C_keypart_B,int Len);

// === Security Function ===
UINT Get_Total(WPARAM wParam,char *Head,char *IDD,char *VIP,UINT Year,UINT Month,UINT Date);
BOOL Check_Total(WPARAM wParam,char *Command22,char *Security22,char *ID22,char *V_IP,char *Addrr,int *Ret,UINT Year,UINT Month,UINT Date);
UINT HSF_Security_Set(char *Command,UINT Year,UINT Month,UINT Date,UINT R_IP,UINT V_IP,UINT Port,UINT PNo,UINT Password);
BOOL HSF_Security_Check(char *Command,UINT Year,UINT Month,UINT Date,UINT R_IP,UINT V_IP,UINT Port,UINT PNo,UINT Password,UINT Total);


void Taichi_ID_Encoder(char *S1);
void Taichi_ID_Decoder(char *S1);
int Txt_HSF(char *Buf,int Len);
int Txt_HSF1(char *Buf,int Len);
int HSF_Txt(char *Buf,int Len1);

// 具有編解碼 + http format 功能
int Html_Txt(char *S1);
int Txt_Html(char *S1);
int Html_recv(SOCKET Socket,char *Buf,int Len,int Flag);
int Html_send(SOCKET Socket,char *Buf,int Len,int Flag,unsigned short g_Proxy_Port);
int Hsf_recv(SOCKET Socket,char *Buf,int Len,int Flag);
int Hsf_send(SOCKET Socket,char *Buf,int Len,int Flag);
int Hsf_recvfrom(SOCKET Socket,char *Buf,int Len,int Flag,sockaddr *lpAddr,int *lpLen);
int Hsf_sendto(SOCKET Socket,char *Buf,int Len,int Flag,sockaddr *lpAddr,int SLen);
int Hsf_recvfrom1(SOCKET Socket,char *Buf,int Len,int Flag,sockaddr *lpAddr,int *lpLen,BOOL *lpFlag);
int Hsf_sendto1(SOCKET Socket,char *Buf,int Len,int Flag,sockaddr *lpAddr,int SLen,BOOL SFlag);

int Write_Log(const char *Head,char *S1,sockaddr_in *lpAddr);//,SIP_MSG *lpSIP_Msg)
int Enable_Write_Log(BOOL Flag);
