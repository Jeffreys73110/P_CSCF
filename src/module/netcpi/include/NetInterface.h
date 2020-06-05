/////////////////////////////////////////////////////////////////////////////
// Module Library implement files
// =======================================================
// 
// This is a implement file for building library.
/////////////////////////////////////////////////////////////////////////////

#ifndef __NETINTERFACEX_H__
#define __NETINTERFACEX_H__

#include "NetCMod.h"
#include "MixTimer.h"
#include "MixSocket.h"
#include "NetErrorCode.h"

#ifdef __cplusplus
extern "C" {
#endif

//===========================================================================
// NetCPI Function
//===========================================================================

// NetCPI initialization
int NetStartup(void);

// Create a network platform object for NetCPI (it will return an Object-ID)
int NetCreate(int Itsize, int Otsize, int Rssize, int Wssize, int Resize, int Wesize, int PostTsize/*=0*/);

// Create a network platform object for NetCPI by using class code (it will return an Object-ID)
int NetCreateByCode(int ClassCode, int Itsize, int Otsize, int Rssize, int Wssize, int Resize, int Wesize, int PostTsize/*=0*/);

// Get the network platform object name
int NetModuleName(unsigned int objID);

// Create a socket and associate with NetCPI
int NetCreateSocket(unsigned int objID, LPMIXSOCKET lpmixSock, int type, int af_inet, void *lpObj, MIXS_CALLBACK callback);

// Create a SCTP socket (beta)
int NetCreateSocketWithSCTP(unsigned int objID, LPMIXSOCKET lpmixSock, int af_inet, void *lpObj, MIXS_CALLBACK callback);

// Let connected TCP or UDP socket associate with NetCPI and delivery a readable mission
int NetJoinMixSocket(unsigned int objID, LPMIXSOCKET lpmixSock, int psockfd, WSABUF WsaRBuf, void *lpObj, MIXS_CALLBACK callback);

// Set socket reusable
int NetSetSocketReusable(MIXSOCKET mixSock);

// Re-delivery a readable mission
int NetReSignInEvent(MIXSOCKET mixSock, WSABUF WsaRBuf);

// Re-delivery a readable mission
int NetResignInEvent(MIXSOCKET mixSock);

// Start a TCP services and delivery a readable mission
int NetStartTServer(MIXSOCKET mixSock, const char *LIP, unsigned short nPort);

// Start a UDP services and delivery a readable mission
int NetStartUServer(MIXSOCKET mixSock, WSABUF WsaRBuf, const char *LIP, unsigned short nPort);

// Start a connection and delivery a readable mission
int NetConnect(MIXSOCKET mixSock, WSABUF WsaRBuf, const char *LIP, unsigned short LPort, const char *RIP, unsigned short RPort, struct sockaddr_mix *addr/*=NULL*/);
	
// Accept TCP client socket
int NetAccept(MIXSOCKET mixSock, int *psockfd, struct sockaddr_mix *addr);

// Post an event
int NetPostMixSockEvent(MIXSOCKET mixSock);

// Send data
int NetSend(MIXSOCKET mixSock, WSABUF WsaSBuf, const struct sockaddr_mix *to/*=NULL*/);

// Send data without IO Pending
int NetSend2(MIXSOCKET mixSock, WSABUF WsaSBuf, const struct sockaddr_mix *to/*=NULL*/);

// Get recv data information
int NetRecv(MIXSOCKET mixSock, char **lpbuf, int *len, struct sockaddr_mix *addr/*=NULL*/);

// Close socket & disassociate
int NetCleanMixSocket(MIXSOCKET mixSock);

// Let the socket disassociate
int NetDisAssociate(MIXSOCKET mixSock);

// Only close socket
int NetCloseSocket(MIXSOCKET mixSock);

// Get socket value
int NetGetSocketValue(MIXSOCKET mixSock);

// Get peer name
int NetGetPeerName(MIXSOCKET mixSock, struct sockaddr_mix *addr);

// Get socket name
int NetGetSockName(MIXSOCKET mixSock, struct sockaddr_mix *addr);

// Change callback
int NetChangeCallback(MIXSOCKET mixSock, void *lpObj, MIXS_CALLBACK callback);

// Get errno in callback
int NetGetCallbackErrno(MIXSOCKET mixSock);

// Make address string 
const char* NetMakeAddrStr(struct sockaddr_mix addr, char *out);

// Make address 
int NetMakeAddrin(struct sockaddr_mix *addr, const char *RIP, unsigned short RPort);		
	
// Free network platform object
void NetObjectFree(unsigned int objID);

// Release
void NetRelease(void);

// Traditional IO
int netcpi_recv(int sockfd, void *buf, int len, int flags);
int netcpi_recvfrom(int sockfd, void *buf, int len, int flags, void *addr, void *addrlen);

//===========================================================================
// NetCPI Function without WsaRBuf
//===========================================================================

#define OWSARBUF_DECLARE		static WSABUF __OWSARBuf__={0,0}
#define ONetStartup				NetStartup
#define ONetCreate				NetCreate
#define ONetCreateByCode		NetCreateByCode
#define ONetModuleName			NetModuleName
#define ONetCreateSocket		NetCreateSocket
#define ONetSetSocketReusable	NetSetSocketReusable
#define ONetResignInEvent		NetResignInEvent
#define ONetStartTServer		NetStartTServer
#define ONetAccept				NetAccept
#define ONetPostMixSockEvent	NetPostMixSockEvent
#define ONetSend				NetSend
#define ONetCleanMixSocket		NetCleanMixSocket
#define ONetDisAssociate		NetDisAssociate
#define ONetCloseSocket			NetCloseSocket
#define ONetGetSocketValue		NetGetSocketValue
#define ONetGetPeerName			NetGetPeerName
#define ONetGetSockName			NetGetSockName
#define ONetGetCallbackErrno	NetGetCallbackErrno
#define ONetMakeAddrStr			NetMakeAddrStr
#define ONetMakeAddrin			NetMakeAddrin
#define ONetObjectFree			NetObjectFree
#define ONetRelease				NetRelease

#define ONetJoinMixSocket(objID, lpmixSock, psockfd, lpObj, callback) \
		NetJoinMixSocket(objID, lpmixSock, psockfd, __OWSARBuf__, lpObj, callback)

#define ONetStartUServer(mixSock, LIP, nPort) \
		NetStartUServer(mixSock, __OWSARBuf__, LIP, nPort)

#define ONetConnect(mixSock, LIP, LPort, RIP, RPort, addr) \
		NetConnect(mixSock, __OWSARBuf__, LIP, LPort, RIP, RPort, addr)

//===========================================================================
// MixTimer Function
//===========================================================================

// mixTimer Lib startup
void MixTimerLibInit(void);

// Create a core object for mixTimer (it will return an Object-ID)
int CreateMixTimerCore(int PerMsec/*=10*/, int ThreadingNum/*=1*/);

// MixTimer structure initialization
void InitMixTimer(unsigned int objID, LPMIXTIMER mixTimer, void *lpObj, MIXT_CALLBACK callback);

// Set timer (in m-sec)
void SetMixTimer(unsigned int objID, LPMIXTIMER mixTimer, int timeout, unsigned int nID/*=0*/);

// Set timer ID
void SetMixTimerID(unsigned int objID, LPMIXTIMER mixTimer, unsigned int nID);

// Set timer logic counter
void SetMixTimerCnt(unsigned int objID, LPMIXTIMER mixTimer, int cnt);

// Let timer timeout directly
void DirectMixTimeout(unsigned int objID, LPMIXTIMER mixTimer);

// Reset timer
void ResetMixTimer(unsigned int objID, LPMIXTIMER mixTimer);

// Kill the timer
void KillMixTimer(unsigned int objID, LPMIXTIMER mixTimer);

// Get timer ID
unsigned int GetMixTimerID(unsigned int objID, LPMIXTIMER mixTimer);

// Get timer logic counter
int GetMixTimerCnt(unsigned int objID, LPMIXTIMER mixTimer);

// Get timeout
int GetMixTimeout(unsigned int objID, LPMIXTIMER mixTimer);

// Is timer killed ? (0/1)
unsigned char IsMixTimerKilled(unsigned int objID, LPMIXTIMER mixTimer);

// Clean timer (use it when you free your object)
int CleanMixTimer(unsigned int objID, LPMIXTIMER mixTimer);

// Main loop for RTOS
void MixTimerLoop(unsigned int objID);

// Free MixTimer object core
void MixTimerObjectFree(unsigned int objID);

// Release MixTimer Lib
void MixTimerCoreRelease(void);

//===========================================================================
// Combine object ID
//===========================================================================

void SetNetCPITaskSize(unsigned int net_stack_ksize, unsigned int timer_stack_ksize);
int MixNetObjectID(unsigned int H_ObjID, unsigned int L_ObjID);
int DeMixNetObjectID_H(int mixID);
int DeMixNetObjectID_L(int mixID);

#ifdef __cplusplus
}
#endif

#endif
