/*****************************************************************************
 rtos-common/nsocket.c Configuration (set of defines)
 ====================================================
 To setting network buffer size
*****************************************************************************/

#ifndef __CONFIG_UIPX_H__
#define __CONFIG_UIPX_H__

/* UIP_TCP_SOCK_NUM := the number of TCP sockets
 * UIP_UDP_SOCK_NUM := the number of UDP sockets
 * UIP_MSS_SIZE     := maximum segment size
 * UIP_CON_SEND     := the number of send buffer per TCP & UDP socket
 * UIP_TCP_RECV     := the number of receive buffer per TCP socket
 * UIP_TCP_WRITABLE := check TCP writable (less than UIP_CON_SEND) */

// the number of TCP sockets
#ifndef UIP_TCP_SOCK_NUM
#define UIP_TCP_SOCK_NUM	9
#endif

// the number of UDP sockets
#ifndef UIP_UDP_SOCK_NUM
#define UIP_UDP_SOCK_NUM	2
#endif

// maximum segment size
#ifndef UIP_MSS_SIZE
#define UIP_MSS_SIZE		1460
#endif

// the number of send buffer per TCP & UDP socket
#ifndef UIP_CON_SEND
#define UIP_CON_SEND		16
#endif

// the number of receive buffer per TCP socket
#ifndef UIP_TCP_RECV
#define UIP_TCP_RECV		5
#endif

// -----------------------------------------------------------------------------------------------------------------------------------------
// Don't touch these defines. Unless you know what you're doing.
#define UIP_TCP_WRITABLE	(UIP_CON_SEND/1)								 // check TCP writable (less than UIP_CON_SEND)
#define	UIP_SOCK_NUM        (UIP_TCP_SOCK_NUM + UIP_UDP_SOCK_NUM)			 // the total number of sockets
//
#define	UIP_NET_BUF_TSIZE   (UIP_SOCK_NUM * UIP_MSS_SIZE * UIP_CON_SEND)     // total network send buffer size (can modify it here)
#define	UIP_NET_BUF_SSIZE   (UIP_NET_BUF_TSIZE / UIP_SOCK_NUM)               // send buffer size per socket (UIP_MSS_SIZE*UIP_CON_SEND)
#define	UIP_NET_BUF_NSIZE   (UIP_NET_BUF_SSIZE / UIP_MSS_SIZE)               // the number of send buffer (UIP_CON_SEND)
//
#define	UIP_TCP_BUF_TSIZE   (UIP_TCP_SOCK_NUM * UIP_MSS_SIZE * UIP_TCP_RECV) // total TCP receive buffer size (can modify it here)
#define	UIP_TCP_BUF_SSIZE   (UIP_TCP_BUF_TSIZE / UIP_TCP_SOCK_NUM)           // receive buffer size per socket (UIP_MSS_SIZE*UIP_TCP_RECV)
#define	UIP_TCP_BUF_NSIZE   (UIP_TCP_BUF_SSIZE / UIP_MSS_SIZE)               // the number of receive buffer (UIP_TCP_RECV)


#endif
