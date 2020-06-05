#ifndef __IPVINFO_H__
#define __IPVINFO_H__

#include "CSocket.h"

#define IP_STRING_MAX_LEN			64
#define ADAPTER_NAME_MAX_LEN		256

#define IPDNS_DOMAIN_NAME_BUF_SIZE	256
#define IPDNS_LOGIC_SIZE	8
#define IPDNS_IPV4_SIZE		4
#define IPDNS_IPV6_SIZE		16
#define IPDNS_WIDTH			(IPDNS_DOMAIN_NAME_BUF_SIZE + IPDNS_IPV6_SIZE + IPDNS_IPV4_SIZE + IPDNS_LOGIC_SIZE)

// old compatibility API:
#define GetLocalIPSize()	GetLIPNum(AF_INET)
#define GetNLocalIP(a, b)	GetNLIP(AF_INET, a, b)

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************************
 * function: GetLIPNum()
 * description:
 *      Get the number of local IP
 * parameters:
 *      af_inet	(IN) :   protocol families AF_INET or AF_INET6
 * return:
 *      The the number of local IP
 * note:
 *      It's recommended to use a variable to store the return value.
 *      This API will need to take a time to query IP when it's called.
 ***********************************************************************/
int GetLIPNum(const int af_inet);

/***********************************************************************
 * function: GetNLIP()
 * description:
 *      get the local IP
 * parameters:
 *      af_inet	(IN) :   protocol families AF_INET or AF_INET6
 *      index   (IN) :   an index to IP interface
 *      outIP   (OUT):   output IP string pointer
 *      ifname  (OUT):   output adapter name string pointer
 * return:
 *      If success, it will return a mask length,
 *      or return small than zero for error
 ***********************************************************************/
int GetNLIP(const int af_inet, const int index, char *outIP);
int GetNLIP2(const int af_inet, const int index, char *outIP, char* ifname);

/***********************************************************************
 * function: GetNBroadcastIP()
 * description:
 *      get the broadcast IP from local IP and maskmask length
 * parameters:
 *      LIP     (IN) :   local IP
 *      mskLen  (IN) :   mask length
 *      outIP   (OUT):   output IP string pointer
 * return:
 *      If success, it will return 0, or return -1 for error
 * note:
 *      IPv4 only, there is no broadcast IP for IPv6
 ***********************************************************************/
int GetNBroadcastIP(const char* LIP, const int mskLen, char *outIP);

/***********************************************************************
 * function: GetDefaultGateway() [beta]
 * description:
 *      get default gateway IP
 * parameters:
 *      af_inet	(IN) :   protocol families AF_INET or AF_INET6
 *      outIP   (OUT):   output IP string pointer
 * return:
 *      If success, it will return 0, or return -1 for error
 ***********************************************************************/
int GetDefaultGateway(const int af_inet, char *outIP);

/***********************************************************************
 * function: GetDefaultLIP()
 * description:
 *      get default local IP
 * parameters:
 *      af_inet	(OUT):   protocol families AF_INET or AF_INET6, can be NULL here
 *      outIP   (OUT):   output IP string pointer, can be NULL here
 * return:
 *      return a mask length
 * note:
 * 		output af_inet pointer must be int type here (4-bytes-length).
 ***********************************************************************/
int GetDefaultLIP(int *af_inet, char *outIP);

/***********************************************************************
 * function: GetDefaultIfName()
 * description:
 *      get default network interface name
 * parameters:
 *      ifName	(OUT):   network interface name
 * return:
 *      If success, it will return 0, or return -1 for error
 ***********************************************************************/
int GetDefaultIfName(char *ifName);

/***********************************************************************
 * function: GetDefaultIfName()
 * description:
 *      get default network interface MAC address
 * parameters:
 *      delimiter (IN):   Add delimiter to output MAC string, it can be 0
 *      capital   (IN):   Let output MAC string as capital letter then set it to 1
 *      outMAC	 (OUT):   MAC address (string style)
 * return:
 *      If success, it will return 0, or return -1 for error
 * note:
 *      This API is not allowed by iOS and Android,
 *      and it always return a constant value of 02:00:00:00:00:00.
 *      Because they already start to provide users with greater data protection,
 *      they removes programmatic access to the device's local hardware identifier.
 ***********************************************************************/
int GetDefaultMacAddr(char delimiter, char capital, char *outMAC);

/***********************************************************************
 * function: GetSpecificLIP()
 * description:
 *      get default local IP by specifying IPv4 or IPv6
 * parameters:
 *      af_inet	(IN) :   protocol families AF_INET or AF_INET6, can be NULL here
 *      outIP   (OUT):   output IP string pointer, can be NULL here
 * return:
 *      return a mask length
 ***********************************************************************/
int GetSpecificLIP(const int af_inet, char *outIP);

/***********************************************************************
 * function: GetDnsIPNum()
 * description:
 *      Get the number of DNS IP
 * parameters:
 *      af_inet	(IN) :   protocol families AF_INET or AF_INET6
 * return:
 *      The the number of DNS IP
 * note:
 *      It's recommended to use a variable to store the return value.
 *      This API will need to take a time to query IP when it's called.
 ***********************************************************************/
int GetDnsIPNum(const int af_inet);

/***********************************************************************
 * function: GetDnsIP()
 * description:
 *      get the DNS IP
 * parameters:
 *      af_inet	(IN) :   protocol families AF_INET or AF_INET6
 *      index   (IN) :   an index to DNS IP
 *      outIP   (OUT):   output IP string pointer
 * return:
 *      If success, it will return 0,
 *      or return small than zero for error
 ***********************************************************************/
int GetDnsIP(const int af_inet, const int index, char *outIP);

/***********************************************************************
 * function: SetDomainTable()
 * description:
 *      set own memory space for query DNS table
 * parameters:
 *      table	(IN) :   memory space pointer
 *      height  (IN) :   the number of domain name you want to save
 *      width   (IN) :   memory size for each domain information
 * return:
 *      If success, it will return 0 or return small than 0 for error.
 * note:
 *      The parameters "width" suggest value is IPDNS_WIDTH.
 *      It can be only called one time before you call GetAddrByDNS().
 *      It won't be effect if you already call GetAddrByDNS().
 ***********************************************************************/
int SetDomainTable(void *table, unsigned int height, unsigned int width);

/***********************************************************************
 * function: GetAddrByDNS() [NONE Blocking API]
 * description:
 *      get an IPv4/6 address string from DNS
 * parameters:
 *      domain  (IN) :   domain name
 *      outIPv4 (OUT):   output IPv4 string pointer, can be NULL here
 *      outIPv6 (OUT):   output IPv6 string pointer, can be NULL here
 * return:
 *      return 0 := no result
 *      return 1 := IPv4 only
 *      return 2 := IPv6 only
 *      return 3 := IPv4 & IPv6
 *      return example ::
 *      int ret = GetAddrByDNS(...);
 *      if (ret&1) { ... IPv4 result ...  }
 *      if (ret&2) { ... IPv6 result ...  }
 * note:
 *      It is a none blocking API, and a default recorded domain name table size is 32 or 16.
 *      If you want a bigger table size, see function SetDomainTable().
 *      When a new query is called, it gives "0.0.0.0" or "::" immediately.
 *      It gives a true IP address only when you try to query the same domain name once again...
 *      after DNS system get the result.
 ***********************************************************************/
int GetAddrByDNS(const char *domain, char *outIPv4, char *outIPv6);

/***********************************************************************
 * function: GetAddrInfox() [Blocking API]
 * description:
 *      get an IPv4/6 address string from DNS
 * parameters:
 *      domain  (IN) :   domain name
 *      outIPv4 (OUT):   output IPv4 string pointer, can be NULL here
 *      outIPv6 (OUT):   output IPv6 string pointer, can be NULL here
 * return:
 *      return 0 := no result
 *      return 1 := IPv4 only
 *      return 2 := IPv6 only
 *      return 3 := IPv4 & IPv6
 * note:
 *      This function will not record domain name information to table.
 ***********************************************************************/
int GetAddrInfox(const char *domain, char *outIPv4, char *outIPv6);

/***********************************************************************
 * function: GetIPv4PrefixLen()
 *           GetIPv6PrefixLen()
 * description:
 *      Get the prefix length of a IPv4/6 network mask
 * parameters:
 *      address	 (IN) :   IPv4 mask address, 4-bytes-length variable
 *      sin6addr (IN) :   a pointer to IPv6 mask address, 16-bytes-length variable
 * return:
 *      A mask length
 ***********************************************************************/
int GetIPv4PrefixLen(const unsigned int address);
int GetIPv6PrefixLen(const void *sin6addr);

/***********************************************************************
 * function: MakeIPv4MaskAddr()
 *           MakeIPv6MaskAddr()
 * description:
 *      Make an IPv4/6 network mask address & string
 * parameters:
 *      MaskLen	 (IN) :   mask length
 *      outIP    (OUT):   output IP string pointer, can be NULL here
 *      sinaddr  (OUT):   IPv4 mask address pointer, 4-bytes-length variable, can be NULL here
 *      sin6addr (OUT):   IPv6 mask address pointer, 16-bytes-length variable, can be NULL here
 ***********************************************************************/
void MakeIPv4MaskAddr(const int MaskLen, char *outIP, void *sinaddr);
void MakeIPv6MaskAddr(const int MaskLen, char *outIP, void *sin6addr);

/***********************************************************************
 * function: GetIPvStyle()
 * description:
 *      Get IP version from IP string
 * parameters:
 *      str	     (IN) :   IPv4/6 string
 * return:
 *      version 4, 6, or 0.
 ***********************************************************************/
int GetIPvStyle(const char* str);

/***********************************************************************
 * function: IPv4MappedIPv6()
 * description:
 *      Get IPv4 mapped IPv6 address string
 * parameters:
 *      outIP    (OUT):   output IP string pointer
 *      str	     (IN) :   IPv4/6 string
 *      af_inet  (IN) :   base on your environment
 * return:
 *      If success, it will return 0, or return -1 for error
 ***********************************************************************/
int IPv4MappedIPv6(char* outIP, const char* str, const int af_inet);

#ifdef __cplusplus
}
#endif

#endif
