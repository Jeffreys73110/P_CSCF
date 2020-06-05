#ifndef WINCMPT_SRC_INCLUDE_WINCMPT_VER_H_
#define WINCMPT_SRC_INCLUDE_WINCMPT_VER_H_

// General OS compatibility library version
// 2.0.0.20160809	:	start version loging
// 2.0.1.20160824	:	correct mix-sockaddr_in sim_family position define
// 2.1.0.20160830	:	addrinfo can be used in VC6.0 & support to get local IPv4/6 include IP mask
// 2.1.1.20160905	:	add query DNS API (IPv4/6)
// 2.1.3.20160907	:	add an API for getting default gateway IPv4/6 & auto get a default local IPv4/6
// 2.1.4.20161017	:	add API NCommandCutter() & NCommandRecover() for BaseInclude.h
// 2.1.5.20161020	:	skip IPv4 address 0.0.0.0 when getting default gateway & local IP for Windows
// 2.1.6.20161110	:	add MutexRecursiveInit() [Recursive mutex]
// 2.1.7.20161110	:	change API inet_ntopx(), and fix iOS device default local IP issue
// 2.1.8.20161115	:	iOS devices can get default gateway IP now
// 2.1.9.20161128	:	skip IPv4 address 169.254.xx.xx (IANA) when getting default gateway & local IP
// 2.2.0.20161129	:	add new API GetSpecificLIP()
// 2.2.1.20161206	:	add new API GetDnsIPNum() & GetDnsIP()
// 2.3.1.20170322	:	improve sram_*alloc() performance
// 2.3.2.20170508	:	cancel #define nsocket_init()
// 2.3.5.20170517	:	add a new memory DB API
// 2.3.6.20170608	:	fix some time_t warning issue
// 2.3.7.20170621	:	add sram_is_init()
// 2.3.9.20170717	:	add IPv4MappedIPv6() and Red-Black Tree module (alpha version for now)
// 2.4.0.20170718	:	add GetIPv6StackStyle(), it can get local IPv6 stack style
// 2.4.1.20170728	:	auto detect IPv4 mapped IPv6 after DNS query
// 2.4.2.20170815	:	add a new method to get local IP for Android
// 2.4.3.20170828	:	add a new API that can get adapter name & broadcast IP
// 2.4.4.20170906	:	add INI file reader
// 2.4.5.20171017	:	change the implementing Recursive Mutex for UNIX
// 2.5.0.20171110	:	change debug log display style, and fix some issue
// 2.5.1.20171121	:	add Base64, HexString and AES module
// 2.5.2.20171129	:	change gettid() method at APPLE
// 2.5.5.20180301	:	add new feature for debug module
// 2.5.6.20180517	:	change Windows threading implement method
// 2.6.0.20180913	:	add JSON model, adjust none blocking DNS query timing
// 2.6.1.20190319	:	reduce debug API stack buffer usage
// 2.6.2.20190508	:	add GetDefaultIfName(), GetDefaultMacAddr(), and change method about getting local IP
// 2.6.3.20190510	:	add get_libwincmpt_ver(), get_libwincmpt_uos_ver()
// 2.6.4.20190527	:	fix JSON parser checking issue about a string with comma
// 2.6.5.20191023	:	add new system event CQE_DROPPING_EVENT for CQueueEvent

#define WINCMPT_VER_MAJOR	"2"
#define WINCMPT_VER_MINOR	"6"
#define WINCMPT_VER_BUILD	"5"
#define WINCMPT_VER_DATE	"20191023"

// Unusual OS uniform interface version
// 1.0.0.20160809	:	start version loging
// 1.0.1.20160810	:	fix free-rtos-ameba TCP send() no FD_WRITE event & patch the errno
// 1.0.2.20160818	:	fix ov788 IO FILE searh function
// 1.0.3.20160830	:	add local IP information for ov788
// 1.0.4.20160905	:	add query DNS API
// 1.0.5.20160905	:	add gettimeofday()
// 1.0.6.20160907	:	add uniform API for getting default gateway IP
// 1.0.7.20161021	:	fix uos_feof() bug for OV788
// 1.0.8.20161110	:	add uos_mutex_rinit() [Recursive mutex]
// 1.0.9.20161118	:	add uos_rand(), uos_srand()
// 1.1.0.20170110	:	fix ov788 socket memory free issuse
// 1.1.1.20170323	:	add uos_time()
// 1.1.2.20170410	:	rename strtoul() and strtol()
// 1.1.3.20170420	:	fix ov788 uos_send() and uos_sendto() about new performance issue
// 1.1.4.20170508	:	nsocket layer can support multi library design now
// 1.1.5.20171129	:	OV788 nsocket will not call resolv_init() anymore
// 1.2.0.20180705	:	add free-rtos-ameba
// 1.2.1.20180913	:	support print the string about "%.*s" case
// 1.3.0.20190319	:	add new RTOS platform for ThreadingX
// 1.4.0.20190508	:	add new RTOS platform for LiteOS
// 1.5.0.20190508	:	add new MTK platform for FreeRTOS
// 1.5.1.20191025	:	change the number of ticks per second to 100 for ThreadingX, add dss_set_notification() for DSS module
// 1.5.2.20191223	:	add remove() & update cURL HTTPC handler can do the file writing (-o)

#define UOSCMPT_VER_MAJOR	"1"
#define UOSCMPT_VER_MINOR	"5"
#define UOSCMPT_VER_BUILD	"2"
#define UOSCMPT_VER_DATE	"20191223"

#ifdef __cplusplus
extern "C" {
#endif
int get_libwincmpt_ver(void);
int get_libwincmpt_uos_ver(void);
#ifdef __cplusplus
}
#endif

#endif
