#ifndef __NETERRORCODE_H__
#define __NETERRORCODE_H__

#define	NET_ERROR_WSA	-1024
#define	NET_ERROR_TOP	NET_ERROR_WSA*2

// Logical error
#define	MIXSOCKET_OCCUPY		-1
#define	INVALID_ADDR_FORM		-2
#define	INVALID_NET_TYPE		-3
#define	INVALID_OVERLAP			-4
#define	INVALID_EPOLLFD			-5
#define	INVALID_KQUEUEFD		-6
#define	INVALID_IOCP_HANDLE		-7
#define INVALID_MIXSOCKET		-8
#define	MISSING_WSABUF			-9
#define	ALREADY_RESIGN			-10
#define	WAIT_FD_WRITE			-11
#define	WAIT_FD_READ			-12
#define	POLLFD_PTR_OVERFLOW		-13
#define	POLLFD_NONMATCH			-14
#define	POLLFD_FULL				-15
#define	SELECT_PTR_OVERFLOW		-16
#define	SELECT_NONMATCH			-17
#define	SELECT_FULL				-18
#define	SELECT_RISSET			-19
#define	SELECT_WISSET			-20
#define	NET_SYS_OVERLOAD		-21
#define	NET_SYS_NOMEM			-22
#define	RECV_WSABUF_ZERO		-23

// Include system error
#define	WSA_SOCKET_ERR			NET_ERROR_WSA-1
#define	WSA_BIND_ERR			NET_ERROR_WSA-2
#define	WSA_LISTEN_ERR			NET_ERROR_WSA-3
#define	WSA_GETSOCKOPT_ERR		NET_ERROR_WSA-4
#define	WSA_SETSOCKOPT_ERR		NET_ERROR_WSA-5
#define	WSA_GETPEERNAME_ERR		NET_ERROR_WSA-6
#define	WSA_GETSOCKNAME_ERR		NET_ERROR_WSA-7
#define	WSA_NONBLOCK_ERR		NET_ERROR_WSA-8
#define	WSA_LOAD_EXFUNCTION_ERR	NET_ERROR_WSA-9
#define	WSA_RECV_FAIL			NET_ERROR_WSA-10
#define	WSA_RECVFROM_FAIL		NET_ERROR_WSA-11
#define	WSA_SEND_FAIL			NET_ERROR_WSA-12
#define	WSA_SENDTO_FAIL			NET_ERROR_WSA-13
#define	WSA_ACCEPT_FAIL			NET_ERROR_WSA-14
#define	WSA_CONNECT_FAIL		NET_ERROR_WSA-15
#define	WSA_IOC_ACCEPT_FAIL		NET_ERROR_WSA-16
#define	WSA_IOC_CONNECT_FAIL	NET_ERROR_WSA-17
#define	WSA_IOC_RECVFROM_FAIL	NET_ERROR_WSA-18
#define	WSA_IOC_RECV_FAIL		NET_ERROR_WSA-19
#define	WSA_IOC_SEND_FAIL		NET_ERROR_WSA-20
#define	WSA_IOC_SENDTO_FAIL		NET_ERROR_WSA-21
#define WSA_IOC_PENDING			NET_ERROR_WSA-22
#define	WSA_IOC_ASSOCIATED		NET_ERROR_WSA-23
#define WSA_EPOLL_WADD_FAIL		NET_ERROR_WSA-24
#define WSA_EPOLL_RADD_FAIL		NET_ERROR_WSA-25
#define WSA_EPOLL_WDEL_FAIL		NET_ERROR_WSA-26
#define WSA_EPOLL_RDEL_FAIL		NET_ERROR_WSA-27
#define WSA_EPOLL_WMOD_FAIL		NET_ERROR_WSA-28
#define WSA_EPOLL_RMOD_FAIL		NET_ERROR_WSA-29
#define WSA_KQUEUE_WADD_FAIL	NET_ERROR_WSA-30
#define WSA_KQUEUE_RADD_FAIL	NET_ERROR_WSA-31
#define WSA_KQUEUE_WDEL_FAIL	NET_ERROR_WSA-32
#define WSA_KQUEUE_RDEL_FAIL	NET_ERROR_WSA-33
#define WSA_KQUEUE_WMOD_FAIL	NET_ERROR_WSA-34
#define WSA_KQUEUE_RMOD_FAIL	NET_ERROR_WSA-35
#define WSA_IOCTL_FAIL			NET_ERROR_WSA-36
#define WSA_UDP_DROP			NET_ERROR_WSA-37
#define WSA_RTOS_ERR			NET_ERROR_WSA-38

// define for GetNetCPIErrorMsg()
#define GET_NET_ERROR_STR(code)	\
if		(code==MIXSOCKET_OCCUPY)		return "MixSocket Is Occupied";\
else if (code==INVALID_NET_TYPE)		return "Invalid Net Type";\
else if (code==INVALID_ADDR_FORM)		return "Invalid Address Format";\
else if (code==INVALID_OVERLAP)			return "Invalid Overlap";\
else if (code==INVALID_EPOLLFD)			return "Invalid EpollFd";\
else if (code==INVALID_IOCP_HANDLE)		return "Invalid Handle for IOCP";\
else if (code==INVALID_MIXSOCKET)		return "Invalid mixSocket";\
else if (code==MISSING_WSABUF)			return "Missing WsaBuf Parameter";\
else if (code==ALREADY_RESIGN)			return "Already Resign";\
else if (code==WAIT_FD_WRITE)			return "Please Wait FD_WRITE Event";\
else if (code==WAIT_FD_READ)			return "Please Wait FD_READ Event";\
else if (code==POLLFD_PTR_OVERFLOW)		return "Pointer Cur Is Overflow For Pollfd System";\
else if (code==POLLFD_NONMATCH)			return "Socket is not match For Pollfd System";\
else if (code==POLLFD_FULL)				return "Fully Loaded For Pollfd System";\
else if (code==SELECT_PTR_OVERFLOW)		return "Pointer Cur Is Overflow For Select System";\
else if (code==SELECT_NONMATCH)			return "Socket is not match For Select System";\
else if (code==SELECT_FULL)				return "Fully Loaded For Select System";\
else if (code==SELECT_RISSET)			return "Read FD Is Set For Select System";\
else if (code==SELECT_WISSET)			return "Write FD Is Set For Select System";\
else if (code==NET_SYS_OVERLOAD)		return "MixSocket System Is Full";\
else if (code==NET_SYS_NOMEM)			return "MixSocket System No Memory";\
else if (code==RECV_WSABUF_ZERO)		return "Recv WSABuf.len Is Zero";\
\
else if (code==WSA_SOCKET_ERR)			return "socket() Error";\
else if (code==WSA_BIND_ERR)			return "bind() Error";\
else if (code==WSA_LISTEN_ERR)			return "listen() Error";\
else if (code==WSA_GETSOCKOPT_ERR)		return "getsockopt() Error";\
else if (code==WSA_SETSOCKOPT_ERR)		return "setsockopt() Error";\
else if (code==WSA_GETPEERNAME_ERR)		return "getpeername() Error";\
else if (code==WSA_GETSOCKNAME_ERR)		return "getsockname() Error";\
else if (code==WSA_NONBLOCK_ERR)		return "Set Socket Nonblocking Fail";\
else if (code==WSA_LOAD_EXFUNCTION_ERR) return "Load Extension Function Fail";\
else if (code==WSA_RECV_FAIL)			return "recv() Error";\
else if (code==WSA_RECVFROM_FAIL)		return "recvfrom() Error";\
else if (code==WSA_SEND_FAIL)			return "send() Error";\
else if (code==WSA_SENDTO_FAIL)			return "sendto() Error";\
else if (code==WSA_ACCEPT_FAIL)			return "accept() Error";\
else if (code==WSA_CONNECT_FAIL)		return "connect() Error";\
else if (code==WSA_IOC_ACCEPT_FAIL)		return "AcceptEx() Error";\
else if (code==WSA_IOC_CONNECT_FAIL)	return "ConnectEx() Error";\
else if (code==WSA_IOC_RECVFROM_FAIL)	return "WSARecvFrom() Error";\
else if (code==WSA_IOC_RECV_FAIL)		return "WSARecv() Error";\
else if (code==WSA_IOC_SEND_FAIL)		return "WSASend() Error";\
else if (code==WSA_IOC_SENDTO_FAIL)		return "WSASendTo() Error";\
else if (code==WSA_IOC_PENDING)			return "IO Pending";\
else if (code==WSA_IOC_ASSOCIATED)		return "This socket is already associated by other IOCP";\
else if (code==WSA_EPOLL_RADD_FAIL)		return "Read epoll_ctl() ADD Fail";\
else if (code==WSA_EPOLL_WADD_FAIL)		return "Write epoll_ctl() ADD Fail";\
else if (code==WSA_EPOLL_RDEL_FAIL)		return "Read epoll_ctl() DEL Fail";\
else if (code==WSA_EPOLL_WDEL_FAIL)		return "Write epoll_ctl() DEL Fail";\
else if (code==WSA_EPOLL_RMOD_FAIL)		return "Read epoll_ctl() MOD Fail";\
else if (code==WSA_EPOLL_WMOD_FAIL)		return "Write epoll_ctl() MOD Fail";\
else if (code==WSA_KQUEUE_RADD_FAIL)	return "Read kevent() ADD Fail";\
else if (code==WSA_KQUEUE_WADD_FAIL)	return "Write kevent() ADD Fail";\
else if (code==WSA_KQUEUE_RDEL_FAIL)	return "Read kevent() DEL Fail";\
else if (code==WSA_KQUEUE_WDEL_FAIL)	return "Write kevent() DEL Fail";\
else if (code==WSA_KQUEUE_RMOD_FAIL)	return "Read kevent() MOD Fail";\
else if (code==WSA_KQUEUE_WMOD_FAIL)	return "Write kevent() MOD Fail";\
else if (code==WSA_IOCTL_FAIL)			return "Call ioctl() Fail";\
else if (code==WSA_UDP_DROP)			return "UDP packet is dropped";\
else if (code==WSA_RTOS_ERR)			return "RTOS network API Fail";\
\
return "Unused error code";

#ifdef __cplusplus
extern "C" {
#endif

const char*	GetNetCPIErrorMsg(int code);
int	GetWSAErrorCode(void);

#ifdef __cplusplus
}
#endif

///////////////////////////////////////////////////

#endif
