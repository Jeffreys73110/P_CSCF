#ifndef __CSOCKET_H__
#define __CSOCKET_H__

// avoid LiteOS define __linux__
#ifdef __LITEOS__
#undef __linux__
#endif

// Windows network include
#if	defined(_MSC_VER)
#include <winsock2.h>
#include <ws2tcpip.h>

// Linux network include
#elif defined(__linux__)
#define STANDARD_UNIX_NET_INCLUDE

// Free BSD network include
#elif defined(__FreeBSD__)
#define STANDARD_UNIX_NET_INCLUDE
#include <sys/types.h>
#include <net/if_dl.h>

// OSX network include
#elif defined(__MACH__)
#define STANDARD_UNIX_NET_INCLUDE
#include <sys/types.h>
#include <net/if_dl.h>

// Android network include
#elif defined(ANDROID)
#define STANDARD_UNIX_NET_INCLUDE

// Other OS network include
#else
#ifndef UNUSUAL_OS
#define UNUSUAL_OS
#endif
#include "../unusual-os/usocket.h"
#include "../unusual-os/standardlib.h"

#endif

// Standard Unix network
#ifdef STANDARD_UNIX_NET_INCLUDE
#include <netdb.h>
#include <resolv.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#endif

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Compatibility define
#ifdef _MSC_VER
typedef	int		socklen_t;
#endif
#ifndef _MSC_VER
#ifndef __WSABUF__
#define __WSABUF__
typedef struct _WSABUF
{
    unsigned long	len;		/* the length of the buffer */
    char*			buf;		/* the pointer to the buffer */
} 
WSABUF, *LPWSABUF;
#endif
#ifndef SOCKET
#define	SOCKET	int
#endif
#ifndef INVALID_SOCKET
#define INVALID_SOCKET  -1
#endif
#endif

// sockaddr_in struct size for IPv6 & IPv4
#define	SOCKADDR_IN4_SIZE	sizeof(struct sockaddr_in)
#define	SOCKADDR_IN6_SIZE	28

// IPv6 compatibility define
#ifndef AF_INET6
#define AF_INET6	0xFF		// unknown code
#endif
#ifndef INET6_ADDRSTRLEN
#define INET6_ADDRSTRLEN	60	// 46
#endif // INET6_ADDRSTRLEN

// IPv6 Scope ID char
#define IPV6_ID_CHAR	'%'

// Unified interface by definition: ygetsockopt, ysetsockopt, ioctlsocket, closesocket
#if	defined(_MSC_VER)
#define ygetsockopt(x, y, z, ov, ol)	getsockopt(x, y, z, (char*)ov, ol)
#define ysetsockopt(x, y, z, ov, ol)	setsockopt(x, y, z, (char*)ov, ol)	
#elif defined(STANDARD_UNIX_NET_INCLUDE)
#define ygetsockopt(x, y, z, ov, ol)	getsockopt(x, y, z, ov, ol)
#define ysetsockopt(x, y, z, ov, ol)	setsockopt(x, y, z, ov, ol)
#define ioctlsocket(x, y, z)			ioctl(x, y, z)
#define closesocket(x)					close(x)
#else
#define ygetsockopt(x, y, z, ov, ol)	uos_getsockopt(x, y, z, ov, ol)
#define ysetsockopt(x, y, z, ov, ol)	uos_setsockopt(x, y, z, ov, ol)
#define ioctlsocket(x, y, z)			uos_ioctl(x, y, z)
#endif

// MSG_NOSIGPIPE define (MSG_NOSIGNAL is not defined on Mac OSX)
#ifndef MSG_NOSIGNAL
#define MSG_NOSIGNAL 0
#endif

// Get a member size from a struct in order to check sin_family problem for FreeBSD
#define	MEMBER_SIZE(type, member)	sizeof(((type *)0)->member)
#define GET_FAMILY_LEN				MEMBER_SIZE(struct sockaddr, sa_family)
#define FL2_ENDIAN_POS				((cstest_big_endian())? 1 : 0)
#define SIN_FAMILY_POS				((GET_FAMILY_LEN==1)? 1 : FL2_ENDIAN_POS)

// special in_addr define for unusual-os
#ifdef UNUSUAL_OS
#ifndef in_addr
#define in_addr uin_addr
#endif
#ifndef sockaddr_in
#define sockaddr_in usockaddr_in
#endif
#endif

#ifndef __SOCKADDR_MIX__
#define __SOCKADDR_MIX__
struct mix_addr 
{
	union // IPv6 address
	{
		unsigned char	u6_addr8[16];
		unsigned short	u6_addr16[8];
		unsigned int	u6_addr32[4];
	};
	#define s6_addrx	u6_addr8
};
struct sockaddr_mix
{
	unsigned char		len_family[2];	// sin_family problem for FreeBSD; [0] is sin_len, [1] is sin_family
	#define bsd_len		len_family[0]
	#define bsd_family	len_family[1]
	#define sim_family	len_family[SIN_FAMILY_POS]
	unsigned short		sin_port;
	union 
	{
		struct in_addr	sin_addr;
		unsigned int	sin6_flowinfo;
	};
	struct mix_addr		sin6_addr;
	unsigned int		sin6_scope_id;
};
#endif

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
extern "C" {
#endif

// test big endian problem
unsigned char cstest_big_endian(void);

// IPv6 compatibility inet function
int	mixaddr_len(struct sockaddr_mix addr); // detect the mix-sockaddr_in length
int inet_ptonx(int af, const char *src, void *dst);
const char *inet_ntopx(int af, const void *src, char *dst, int size);

// unusual OS network control
#ifndef UNUSUAL_OS
#define nsocket_reflash()
#define nsocket_transform(oid, a, b, c, d)  (*(a))=(d)
#define nsocket_showlog(a)
#endif

// unusual OS parameter definition (Common definitions for NetCPI API)
#ifndef UNUSUAL_OS
#define uAF_INET		AF_INET
#define uAF_INET6		AF_INET6
#define uSOCK_STREAM	SOCK_STREAM
#define uSOCK_DGRAM		SOCK_DGRAM
#endif

// Socket function
int SetSocketNonblock(SOCKET sock);
int SetSocketNoSIGPIPE(SOCKET sock);
int SetTcpSocketKeepAlive(SOCKET sock, int keep_idle, int keep_intvl);
SOCKET CreateBroadcastSocket(int af_inet, unsigned short Port);

#ifdef __cplusplus
}
#endif

#endif // #ifndef __CSOCKET_H__
