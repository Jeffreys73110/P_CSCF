#ifndef NETCPI_SRC_INCLUDE_NETCPI_VER_H_
#define NETCPI_SRC_INCLUDE_NETCPI_VER_H_

// version
// 4.7.75.20160809	:	start version loging
// 4.7.76.20160810	:	fix NetSend() return data length issue
// 4.7.77.20160810	:	add a special process for false UFD_WRITE event (free-rtos-ameba issue)
// 4.7.78.20160811	:	fix windows IOCP missing accept event when ERROR_NETNAME_DELETED happend
// 4.7.8 .20160811	:	add a new event TFD_EACCEPT for IOCP
// 4.7.9 .20160905	:	fix UDP IO recv 0 length issue
// 4.7.10.20161110	:	change API inet_ntopx()
// 4.8.0 .20161124	:	add a return error code WSA_UDP_DROP
// 4.8.1 .20170508	:	support multi library users for RTOS if nsocket layer support
// 4.8.2 .20170601	:	NetMakeAddrStr() ipv6 scope id is removed now
// 4.8.5 .20170717	:	fix CPOLL and CSELECT wake up broken pipe issue, and support auto IPv4 mapped IPv6 string
// 4.8.6 .20180403	:	call shutdown() before closesocket() for TCP connection
// 4.8.7 .20180503	:	try to re-send TCP data when FD_WRITE timeout for CPOLL
// 4.9.0 .20180517	:	Windows IOCP can (fake-)disassociate a socket now, and (Re-)JoinMixSocket to own self now
// 4.9.1 .20180705	:	support reviver WSABUF NULL setting for RTOS, and fix Windows IOCP disassociate and (Re-)JoinMixSocket bug
// 4.9.2 .20190508	:	add new API NetSend2() about without IO Pending
// 4.9.3 .20190509	:	fix start UDP server without WSABUF issue
// 4.9.4 .20190510	:	add get_libnetcpi_ver()

#define NETCPI_VER_MAJOR	"4"
#define NETCPI_VER_MINOR	"9"
#define NETCPI_VER_BUILD	"4"
#define NETCPI_VER_DATE		"20190510"

#ifdef __cplusplus
extern "C" {
#endif
int get_libnetcpi_ver(void);
#ifdef __cplusplus
}
#endif

#endif
