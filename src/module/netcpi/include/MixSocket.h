#ifndef __MIXSOCKET_H__
#define __MIXSOCKET_H__

#include "../../wincmpt/include/CSocket.h"

// mixSocket
typedef int	MIXSOCKET, *LPMIXSOCKET;

// Callback
typedef void (*MIXS_CALLBACK)(MIXSOCKET wParam, long lParam, void* lpObj);

// lParam event
#define TFD_CONNECTED		0x10
#define TFD_CONNECT_FAIL	0x11
#define TFD_ACCEPT			0x12
#define TFD_READ			0x13
#define TFD_POST			0x14
#define TFD_WRITE			0x15
#define TFD_CLOSE			0x16
#define TFD_ICMP			0x17
#define TFD_ERROR			0x18
#define TFD_EACCEPT			0x19 // an error happened for win IOCP

#endif

