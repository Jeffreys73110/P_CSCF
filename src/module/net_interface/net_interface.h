#ifndef __NET_INTERFACE_H__
#define __NET_INTERFACE_H__

#include "../DTrace/DTrace.h"

#define SERVERDLG_GET_CALL_LIST_EVENT 0x01

enum HTTP_GET_TYPE
{
	HGT_XML,
	HGT_Server_INFO,
	HGT_CALL_LIST,
	HGT_CONFIG_SETTING,
	HGT_UNKNOW
};
enum HTTP_CONTENT_TYPE
{
	HCT_Html,
	HCT_Css,
	HCT_Js,
	HCT_Other
};

typedef void (*SERVERDLG_CALLBACK)(void *lpCallbackObject, std::string message, std::string &data_str);

typedef struct _WSABUF_EX
{
	WSABUF wsabuf;
	size_t size;

	_WSABUF_EX()
	{
		memset(this, 0, sizeof(_WSABUF_EX));
	}
	~_WSABUF_EX()
	{
		// dealloc();	// consider buffer might be deallocated when nc copy_from
	}
	int alloc(size_t s)
	{
		if (s <= 0 || s > 65536)
			return -1;
		if (wsabuf.buf)
			return -2;
		wsabuf.buf = new char[s];
		wsabuf.len = s;
		size = s;
		empty_buf();
		return 0;
	}
	void dealloc()
	{
		if (wsabuf.buf)
		{
			delete[] wsabuf.buf;
			wsabuf.buf = NULL;
			wsabuf.len = 0;
		}
	}
	int copy_buf(const char *buf, unsigned long len)
	{
		if (!wsabuf.buf)
			return -2;
		memcpy(wsabuf.buf, buf, len);
		wsabuf.len = len;
		return 0;
	}
	int empty_buf()
	{
		if (!wsabuf.buf)
			return -1;
		memset(wsabuf.buf, 0, size);
		wsabuf.len = size;
		return 0;
	}
} WSABUF_EX, *LPWSABUF_EX;

#define WsaSBuf_SIZE 4096
#define WsaRBuf_SIZE 4096
struct NetConnect_t
{
	// int 	index;
	//--- setting param
	int af_inet;	   // AF_INET, AF_INET6
	int socket_type;   // SOCK_DGRAM, SOCK_STREAM
	int role;		   // 0: server, 1: client
	bool tcpkeepalive; // a flag control if enable tcp keep alive
	//--- connection param
	MIXSOCKET mixsocket;
	sockaddr_mix Laddr; // local addr
	WSABUF_EX SendBuf;
	WSABUF_EX RecvBuf;
	// char			RealIP[64];
	// char			VirtualIP[64];
	void *lpObject;
	MIXS_CALLBACK lpFunction;
	uint32_t Event;
	//--- auxiliary param
	// MIXTIMER		MixTimer;
	// ymutex			Mutex;

	NetConnect_t()
	{
		memset(this, 0, sizeof(NetConnect_t));
		mixsocket = NULL_MIXSOCKET;
	}
	~NetConnect_t()
	{
		// dealloc();	// consider buffer might be deallocated when nc copy_from
	}
	bool is_used() const
	{
		if (this->mixsocket == NULL_MIXSOCKET)
			return false;
		return true;
	}
	void copy_from(NetConnect_t *nc)
	{
		memcpy(this, nc, sizeof(NetConnect_t));
	}
	int alloc()
	{
		int ret;
		if ((ret = SendBuf.alloc(WsaSBuf_SIZE)) < 0)
			return ret;
		if ((ret = RecvBuf.alloc(WsaSBuf_SIZE)) < 0)
			return ret;
		return 0;
	}
	void dealloc()
	{
		SendBuf.dealloc();
		RecvBuf.dealloc();
	}
	void reset()
	{
		// dealloc();
		memset(this, 0, sizeof(NetConnect_t));
		mixsocket = NULL_MIXSOCKET;
	}
};

// Crypto Type
enum CRYPTO_TYPE
{
	CT_NoCrypto = 0,
	CT_SIPCrypto = 1,
	CT_Walkersun1 = 2
};

typedef struct _DEST_DATA
{
private:
	NetConnect_t Nc;

public:
	// similar socket
	NetConnect_t *pNc;
	// remote address
	sockaddr_mix Raddr;
	unsigned short Port;
	char Real_IP[64];
	char Virtual_IP[64];
	// crypt
	CRYPTO_TYPE Crypto_Type;

	_DEST_DATA()
	{
		reset();
	}
	void copy_from(_DEST_DATA *lpDest)
	{
		memcpy(this, lpDest, sizeof(_DEST_DATA));
	}
	int pack(NetConnect_t *nc, sockaddr_mix *addr, const char *real_ip, const char *virtual_ip, CRYPTO_TYPE type)
	{
		if (!nc || !addr || !virtual_ip)
			return -1;

		if (pNc != nc)
			pNc = nc;
		if (&Raddr != addr)
			memcpy(&Raddr, addr, sizeof(Raddr));
		Port = ntohs(Raddr.sin_port);
		if (!real_ip)
			NetMakeAddrStr(Raddr, Real_IP);
		else
			Jstrncpy(Real_IP, real_ip, sizeof(Real_IP));
		Jstrncpy(Virtual_IP, virtual_ip, sizeof(Virtual_IP));
		Crypto_Type = type;
		return 0;
	}
	bool is_used() const
	{
		return pNc->is_used();
	}
	void reset()
	{
		Nc.reset();
		memset(&Raddr, 0, sizeof(Raddr));
		Port = 0;
		Real_IP[0] = 0;
		Virtual_IP[0] = 0;
		Crypto_Type = CT_NoCrypto;
		pNc = &Nc;
	}
	int clean_nc()
	{
		if (Nc.is_used())
		{
			int ret = NetCleanMixSocket(Nc.mixsocket);
			Nc.dealloc();
			Nc.reset();
			return ret;
		}
	}

} DEST_DATA;

class CNetIF
{
public:
	static CNetIF *GetInstance();
	static int ReleaseInstance(CNetIF **ins);

private:
	static CNetIF *m_instance;
	// Construction
public:
	CNetIF();
	virtual ~CNetIF();

private:
	// void 	UnitTest();
	bool m_UnitTestFlag;

public:
	int StartNcServer(NetConnect_t *nc, int af_inet, int socket_type, const char *Server_IP, WORD ServerPort, MIXS_CALLBACK lpFunction, void *lpObject, uint32_t Event);
	// int StartTcpServer(NetConnect_t *nc, int af_inet, WORD ServerPort, MIXS_CALLBACK lpFunction, void *lpObject, uint32_t Event);
	// int StartTcpClient(NetConnect_t *nc, int af_inet, WORD ServerPort, char *RIP, unsigned short RPort, bool keepalive, MIXS_CALLBACK lpFunction, void *lpObject, uint32_t Event);
	// int StartUdpServer(NetConnect_t *nc, int af_inet, WORD ServerPort, MIXS_CALLBACK lpFunction, void *lpObject, uint32_t Event);
	// int StartUdpClient(NetConnect_t *nc, int af_inet, WORD ServerPort, char *RIP, unsigned short RPort, sockaddr_mix *lpRaddr, MIXS_CALLBACK lpFunction, void *lpObject, uint32_t Event);
	int StartNcClient(NetConnect_t *nc, int af_inet, int socket_type, WORD ServerPort, const char *RIP, unsigned short RPort, bool tcpkeepalive, sockaddr_mix *lpRaddr, MIXS_CALLBACK lpFunction, void *lpObject, uint32_t Event);
	int StartDestClient(DEST_DATA *lpDest, int af_inet, int socket_type, CRYPTO_TYPE Crypto_Type, WORD ServerPort, const char *RIP, unsigned short RPort, bool tcpkeepalive, MIXS_CALLBACK lpFunction, void *lpObject, uint32_t Event);
	int NetNcAccept(const NetConnect_t *nc, NetConnect_t *subnc, struct sockaddr_mix *addr);
	int NetNcRecv(NetConnect_t *nc, char **lpbuf, int *len, struct sockaddr_mix *addr /*=NULL*/);
	int NetNcSend(NetConnect_t *nc, const char *lpbuf, size_t len, const struct sockaddr_mix *to /*=NULL*/);
	int NetNcResignInEvent(NetConnect_t *nc);
	int NetNcClean(NetConnect_t *nc);
	int NetNcPostSockEvent(NetConnect_t *nc);

	int InitTimer(LPMIXTIMER mixTimer, void *lpObj, MIXT_CALLBACK callback);
	int SetTimer(LPMIXTIMER mixTimer, int timeout, unsigned int nID);
	int KillTimer(LPMIXTIMER mixTimer);
	unsigned int GetTimerID(LPMIXTIMER mixTimer);
	// int StartDlgServer(unsigned short Port);
	// void SetDlgCallback(SERVERDLG_CALLBACK lpfunction, void *callbackObject);
	// void GetDataFromCallback(std::string message, std::string& data_str);
	// void OnTimer(MIXTIMER *wParam);
	// int OnWindowProc(unsigned int message, MIXSOCKET Socket, unsigned int lParam, void *lpobj);

private:
	int m_NetcpiId;
	int m_MixTimerCoreObjectId;
	// MIXSOCKET		m_WebMixSocket;
	// int				m_WebConnectCount;
	// WEB_CONNECT		m_WebConnectInfo[WEB_CONNECT_SIZE];
	// SERVERDLG_CALLBACK callbackFun;
	// void *callbackObject;
private:
	// // Web info
	// int 	AssignFreeWebInfo();
	// int 	ClearWebinfo(WEB_CONNECT* pWeb_conn);
	// // html
	// void 	Send_200OK_Xml(WEB_CONNECT* web_conn, std::string path);
	// void 	Send_200OK_Query(WEB_CONNECT* web_conn, std::string jasonCallback, std::string data_str);
	// int 	SendErrorCode(WEB_CONNECT* web_conn, HttpStatus::Code code);
	// // Data process
	// HTTP_GET_TYPE		Get_Http_Type(std::string path);
	// int 	Get_JasonCallback(const char *packet, std::string *jasonCallback);
	// HTTP_CONTENT_TYPE	GetContentType(std::string path);
	// int 	GetHttpPath(const char *packet, std::string& path);
	// int		GetEventTypeByPath(std::string path, std::string& event_type);
	// int 	GetEventTypeByPacket(std::string packet, std::string& event_type);
	// Netcpi
	int NetObjectInit();

	// Timer
	int TimerObjectInit();

protected:
	// static void	SocketModule_Callbcak_Function(MIXSOCKET wParam, long lParam, void* lpObj);
	// static void	MixTimer_Callbcak_Function(MIXTIMER *wParam, void* lpObj);
};

#endif // __NET_INTERFACE_H__