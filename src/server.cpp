
#include "server.h"

// #define BUFLEN 100
// static void set_sctp_event(struct sctp_event_subscribe* ses){
// 	CRASH_DEBUG_COMMAND;
// 	ses->sctp_data_io_event=1;
// 	ses->sctp_shutdown_event=1;
// /*	ses->sctp_association_event=1;
// 	ses->sctp_address_event=1;
// 	ses->sctp_send_failure_event=1;
// 	ses->sctp_peer_error_event=1;
// 	ses->sctp_partial_delivery_event=1;
// 	ses->sctp_adaptation_layer_event=1;
// 	ses->sctp_authentication_event=1;
// */	CRASH_DEBUG_COMMAND;
// }

// static void server_response(int &fd,int socketModeone_to_many){
// 	CRASH_DEBUG_COMMAND;
// 	mme* mme=mme::get_instance();
// 	// mme->init();
// 	mme->run(fd,socketModeone_to_many);
// 	CRASH_DEBUG_COMMAND;
// }
// void* ServerCB(void* arg){
// 	// CRASH_DEBUG_COMMAND;
// 	// spgw* spgw=spgw::get_instance();
// 	// spgw->init();
// 	// spgw->run();
// 	CNetIF	*lpNetIF = CNetIF::GetInstance();

// 	while (1)	sleep;

// 	RETURN NULL;
// }

// void* mme_start(void* arg){
// 	CRASH_DEBUG_COMMAND;
// 	while(1){//不停收下一則訊息
// 		if (*(int*)arg<0)	break;

// 		struct sctp_event_subscribe ses;
// 		set_sctp_event(&ses);
// 		if(setsockopt(*(int*)arg,IPPROTO_SCTP,
// 			SCTP_EVENTS,&ses,sizeof(ses))!=0) {
// 			DTRACE(DTL_Error,"set socket error\n");
// 			exit(1);
// 		}
// 		server_response(*(int*)arg,0);
// 	}
// 	CRASH_DEBUG_COMMAND;
// }

// void* mme_start2(void* arg){
// 	CRASH_DEBUG_COMMAND;
// 	while(1){//不停收下一則訊息
// 		if (*(int*)arg<0)	break;

// 		struct sctp_event_subscribe ses;
// 		set_sctp_event(&ses);
// 		if(setsockopt(*(int*)arg,IPPROTO_SCTP,
// 			SCTP_EVENTS,&ses,sizeof(ses))!=0) {
// 			DTRACE(DTL_Error,"set socket error\n");
// 			exit(1);
// 		}
// 		mme* mme=mme::get_instance();
// 		// mme->init();
// 		mme->run2(*(int*)arg);
// 	}
// 	CRASH_DEBUG_COMMAND;

// }

// int socket_start(char* IP,int PORT_NUM){
// 	CRASH_DEBUG_COMMAND;
// 	sockaddr_in sin;
// 	sin.sin_family=AF_INET;
// 	sin.sin_port=htons(PORT_NUM);
// 	// sin.sin_addr.s_addr=inet_addr(IP);
// 	sin.sin_addr.s_addr=htonl(INADDR_ANY);

// 	int soc;
// 	if((soc=socket(AF_INET,SOCK_STREAM,IPPROTO_SCTP))<0){
// 		DTRACE(DTL_Error,"socket error\n");
// 		RETURN -1;
// 	}
// 	if(bind(soc,(sockaddr*)&sin,sizeof(sin))<0){
// 		DTRACE(DTL_Error,"bind error\n");
// 		RETURN -1;
// 	}
// 	else	DTrace_CommonMsg("bind port successfully.\n");

// 	listen(soc,10);
// 	RETURN soc;
// }

int main()
{
	CRASH_DEBUG_COMMAND;

	printf("Hellow world!\n");
	pthread_t tid, tid1;
	std::string timestr;

	// CNetIF	*lpNetIF = CNetIF::GetInstance();
	// pthread_create(&tid,NULL,ServerCB,NULL);

	DTrace_CommonMsg("SIP Proxy Server\r\n"
					 "Searching SQL ....\r\n",
					 "Completely[%s]\r\n",
					 Jstringftime(timestr, "%Y/%m/%d %H:%M:%S", NULL).c_str());

	CSIP_Server *lpSIP_Server = new CSIP_Server();
	if (lpSIP_Server->Start() < 0)
	{
		DTrace_Error("proxy server error: %s\n", lpSIP_Server->ErrMsg);
	}
	else
	{
		DTrace_CommonMsg("SIP Server start OK\r\n");
	}

	//--- test
	// Jstring abc(64);
	// abc = "123";
	// printf("--------------------%s-----------------------\n", abc.c_str());


	// ==== Start CDMS Server ====
	// m_Edit1+="Searching SQL .... ";

	// Time1=CTime::GetCurrentTime();
	// sprintf(S1,"Completely[%s]\r\n", Jstringftime(timestr, "%Y/%m/%d %H:%M:%S", NULL)->c_str()));
	// m_Edit1+=S1;
	// // Start Server
	// lpSIP_Server=new CSIP_Server();
	// if (i=lpSIP_Server->Start())	{ m_Edit1+=lpSIP_Server->ErrMsg; m_Edit1+="\r\n"; }
	// m_Edit1+="SIP Start OK\r\n";
	// sprintf(S1,"Local IP = %s\r\n",lpSIP_Server->Local_IP); m_Edit1+=S1;
	// GetDlgItem(IDC_BUTTON1)->SetWindowText(m_Edit1);

	//--- Start MME
	// char LOCAL_IP_ADDRESS[15]="10.102.81.100";
	// // char LOCAL_IP_ADDRESS[15]="0.0.0.0";
	// // char LOCAL_IP_ADDRESS[15]="192.168.0.7";
	// int soc=socket_start(LOCAL_IP_ADDRESS,36412);
	// pthread_create(&tid1,NULL,mme_start2,&soc);

	// //--- Start SPgw
	// pthread_create(&tid,NULL,spgw_start,NULL);

	// //--- Start ServerDlg
	// CDlgQuery* lpDlgQuery=CDlgQuery::GetInstance();
	// if (!lpDlgQuery)	DTrace_FatalError("GetInstance of CNetIF fail\n");

	// // Timer
	// // create_timer();

	// //--- Test
	// // AnsiColorTable();
	// // DTraceColor();

	// // DTrace_CommonMsg("tick count in ms: %f (%d %s)\n", CalClockTime(false), __LINE__, __FILE__);
	// // for (int i,j=0; i<10; i++)
	// // {
	// // 	for (j=0; j<10000; j++);
	// // }
	// // DTrace_CommonMsg("tick count in ms: %f (%d %s)\n", CalClockTime(true), __LINE__, __FILE__);

	// // while(1){//避免socket建立失敗造成直接停止
	// // 	sockaddr_in accsin;
	// // 	unsigned int len=sizeof(sockaddr);
	// // 	int accsoc=accept(soc,(sockaddr*)&accsin,&len);
	// // 	DTrace_CommonMsg("=============================================================\n");
	// // 	DTrace_CommonMsg("accept = %d\n",accsoc);

	// // 	DTrace_CommonMsg("make pthread\n");
	// // 	pthread_create(&tid1,NULL,mme_start,&accsoc);
	// // 	sleep(1);
	// // }

	while (1)
		sleep(1);
	// pthread_join(tid, NULL);
	DTrace_CommonMsg("Terminate server\n");
	RETURN 0;
}