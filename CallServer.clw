; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CSIP_Server_Setup
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "CallServer.h"

ClassCount=17
Class1=CCallServerApp
Class2=CCallServerDlg
Class3=CAboutDlg

ResourceCount=13
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Class4=CCDMS
Resource3=IDD_CALLSERVER_DIALOG (Chinese (Taiwan))
Resource4=IDD_GATEKEEPER (Chinese (Taiwan))
Resource5=IDD_BILLING (Chinese (Taiwan))
Class5=CLocation
Class6=CGatekeeper
Class7=CBilling
Class8=CSearch_DNS
Resource6=IDD_SIP_SERVER
Resource7=IDD_SERVER_SETUP
Resource8=IDD_LOCATION (Chinese (Taiwan))
Resource9=IDD_ABOUTBOX (Chinese (Taiwan))
Resource10=IDD_CALLSERVER_DIALOG
Resource11=IDD_SIP_PROC (English (U.S.))
Resource12=IDD_SEARCH_DNS (Chinese (Taiwan))
Class9=CCDD
Class10=CCDMS1
Class11=CSIP_Processor
Class12=CSIP_Server
Class13=CSIP_Server_Setup
Class14=CCDMS_DR
Class15=CCDMS_DDD
Class16=CCDMS_LUS
Class17=CHash
Resource13=IDD_SIP_SERVER (English (U.S.))

[CLS:CCallServerApp]
Type=0
HeaderFile=CallServer.h
ImplementationFile=CallServer.cpp
Filter=N

[CLS:CCallServerDlg]
Type=0
HeaderFile=CallServerDlg.h
ImplementationFile=CallServerDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=CCallServerDlg

[CLS:CAboutDlg]
Type=0
HeaderFile=CallServerDlg.h
ImplementationFile=CallServerDlg.cpp
Filter=D
LastObject=CAboutDlg
BaseClass=CDialog
VirtualFilter=dWC

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=5
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889
Control5=IDC_EDIT1,edit,1350633600

[DLG:IDD_CALLSERVER_DIALOG]
Type=1
Class=CCallServerDlg
ControlCount=1
Control1=IDC_BUTTON1,button,1342285056

[CLS:CCDMS]
Type=0
HeaderFile=CDMS.h
ImplementationFile=CDMS.cpp
BaseClass=CRecordset
Filter=N
VirtualFilter=r
LastObject=CCDMS

[DB:CCDMS]
DB=1
DBType=ODBC
ColumnCount=16
Column1=[m_PhoneNo], 12, 100
Column2=[m_ID], 12, 100
Column3=[m_IP], 12, 100
Column4=[m_IP1], 12, 100
Column5=[m_Port], 12, 100
Column6=[m_LeftTime], 12, 100
Column7=[Remarks], 12, 100
Column8=[m_Mobile], 12, 40
Column9=[m_Email], 12, 100
Column10=[m_MAC], 12, 100
Column11=[active], 12, 2
Column12=[m_Name], 12, 100
Column13=[m_Password], 12, 100
Column14=[LockOutside], 12, 100
Column15=[m_Addr], 12, 500
Column16=[m_TrunkGW], 12, 100

[CLS:CLocation]
Type=0
HeaderFile=Location.h
ImplementationFile=Location.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_EDIT11

[CLS:CGatekeeper]
Type=0
HeaderFile=Gatekeeper.h
ImplementationFile=Gatekeeper.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CGatekeeper

[CLS:CBilling]
Type=0
HeaderFile=Billing.h
ImplementationFile=Billing.cpp
BaseClass=CDialog
Filter=D
LastObject=CBilling

[CLS:CSearch_DNS]
Type=0
HeaderFile=Search_DNS.h
ImplementationFile=Search_DNS.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CSearch_DNS

[DLG:IDD_LOCATION (Chinese (Taiwan))]
Type=1
Class=CLocation
ControlCount=85
Control1=IDC_EDIT1,edit,1350631552
Control2=IDC_EDIT2,edit,1350631552
Control3=IDC_STATIC,static,1342308352
Control4=IDC_STATIC,static,1342308352
Control5=IDC_EDIT3,edit,1353783492
Control6=IDC_STATIC,static,1342308352
Control7=IDC_STATIC,static,1342308352
Control8=IDC_EDIT4,edit,1350631552
Control9=IDC_TAB1,SysTabControl32,1342177280
Control10=IDC_STATIC,static,1476526081
Control11=IDC_STATIC,static,1476526081
Control12=IDC_STATIC,static,1476526081
Control13=IDC_STATIC,static,1476526081
Control14=IDC_STATIC,static,1476526081
Control15=IDC_STATIC,static,1476526081
Control16=IDC_STATIC,static,1476526081
Control17=IDC_STATIC,static,1476526081
Control18=IDC_STATIC,static,1476526081
Control19=IDC_STATIC,static,1476526081
Control20=IDC_STATIC,static,1476526081
Control21=IDC_STATIC,static,1476526081
Control22=IDC_STATIC,static,1476526081
Control23=IDC_STATIC,static,1476526081
Control24=IDC_STATIC,static,1476526081
Control25=IDC_STATIC,static,1476526081
Control26=IDC_STATIC,static,1476526081
Control27=IDC_STATIC,static,1476526081
Control28=IDC_STATIC,static,1476526081
Control29=IDC_STATIC,static,1476526081
Control30=IDC_STATIC,static,1476526081
Control31=IDC_STATIC,static,1476526081
Control32=IDC_STATIC,static,1476526081
Control33=IDC_STATIC,static,1476526081
Control34=IDC_STATIC12,static,1350696961
Control35=IDC_STATIC13,static,1350696961
Control36=IDC_STATIC14,static,1350696961
Control37=IDC_STATIC15,static,1350696961
Control38=IDC_STATIC16,static,1350696961
Control39=IDC_STATIC17,static,1350696961
Control40=IDC_STATIC18,static,1350696961
Control41=IDC_STATIC19,static,1350696961
Control42=IDC_STATIC20,static,1350696961
Control43=IDC_STATIC21,static,1350696961
Control44=IDC_STATIC22,static,1350696961
Control45=IDC_STATIC23,static,1350696961
Control46=IDC_STATIC0,static,1350696961
Control47=IDC_STATIC1,static,1350696961
Control48=IDC_STATIC2,static,1350696961
Control49=IDC_STATIC3,static,1350696961
Control50=IDC_STATIC4,static,1350696961
Control51=IDC_STATIC5,static,1350696961
Control52=IDC_STATIC6,static,1350696961
Control53=IDC_STATIC7,static,1350696961
Control54=IDC_STATIC8,static,1350696961
Control55=IDC_STATIC9,static,1350696961
Control56=IDC_STATIC10,static,1350696961
Control57=IDC_STATIC11,static,1350696961
Control58=IDC_STATIC,static,1476526081
Control59=IDC_STATIC24,static,1350696961
Control60=IDC_EDIT5,edit,1353781444
Control61=IDC_EDIT6,edit,1353781444
Control62=IDC_STATIC,static,1342308352
Control63=IDC_STATIC,static,1342308352
Control64=IDC_STATIC,static,1342308352
Control65=IDC_EDIT7,edit,1350631552
Control66=IDC_BUTTON1,button,1342242816
Control67=IDC_STATIC,static,1342308352
Control68=IDC_TAB2,SysTabControl32,1342177280
Control69=IDC_STATIC,static,1342308352
Control70=IDC_STATIC,static,1342308352
Control71=IDC_BUTTON2,button,1342242816
Control72=IDC_TAB3,SysTabControl32,1342177280
Control73=IDC_STATIC,static,1342308352
Control74=IDC_EDIT8,edit,1350631552
Control75=IDC_STATIC,static,1342308352
Control76=IDC_BUTTON3,button,1342242816
Control77=IDC_EDIT9,edit,1350631552
Control78=IDC_EDIT10,edit,1350631552
Control79=IDC_BUTTON4,button,1342242816
Control80=IDC_STATIC,static,1342308352
Control81=IDC_EDIT12,edit,1350631552
Control82=IDC_COMBO1,combobox,1344340226
Control83=IDC_LIST2,listbox,1352728835
Control84=IDC_STATIC,static,1342308352
Control85=IDC_EDIT11,edit,1350631552

[DLG:IDD_ABOUTBOX (Chinese (Taiwan))]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_CALLSERVER_DIALOG (Chinese (Taiwan))]
Type=1
Class=CCallServerDlg
ControlCount=5
Control1=IDC_BUTTON1,button,1342242816
Control2=IDC_EDIT1,edit,1352732868
Control3=IDC_BUTTON2,button,1342242816
Control4=IDC_BUTTON3,button,1342242816
Control5=IDC_BUTTON4,button,1342242816

[DLG:IDD_BILLING (Chinese (Taiwan))]
Type=1
Class=CBilling
ControlCount=0

[DLG:IDD_SEARCH_DNS (Chinese (Taiwan))]
Type=1
Class=CSearch_DNS
ControlCount=23
Control1=IDC_EDIT1,edit,1350631552
Control2=IDC_EDIT2,edit,1353789636
Control3=IDC_BUTTON1,button,1342242816
Control4=IDC_COMBO1,combobox,1344340226
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342308352
Control7=IDC_COMBO2,combobox,1344340226
Control8=IDC_STATIC,static,1342308352
Control9=IDC_STATIC,button,1476395015
Control10=IDC_STATIC,static,1342308352
Control11=IDC_STATIC,static,1342308352
Control12=IDC_EDIT3,edit,1350631552
Control13=IDC_EDIT4,edit,1350631552
Control14=IDC_STATIC_1987,static,1342308352
Control15=IDC_STATIC_1988,static,1342308352
Control16=IDC_STATIC,button,1476395015
Control17=IDC_BUTTON2,button,1342242816
Control18=IDC_BUTTON3,button,1342242816
Control19=IDC_EDIT5,edit,1350631552
Control20=IDC_EDIT6,edit,1350631552
Control21=IDC_STATIC_1989,static,1342308352
Control22=IDC_EDIT7,edit,1350631552
Control23=IDC_EDIT8,edit,1350631552

[DLG:IDD_GATEKEEPER (Chinese (Taiwan))]
Type=1
Class=?
ControlCount=2
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816

[CLS:CCDD]
Type=0
HeaderFile=CDD.h
ImplementationFile=CDD.cpp
BaseClass=CRecordset
Filter=N
VirtualFilter=r

[DB:CCDD]
DB=1
DBType=ODBC
ColumnCount=11
Column1=[m_PhoneNo], 12, 100
Column2=[m_ID], 12, 100
Column3=[m_IP], 12, 100
Column4=[m_IP1], 12, 100
Column5=[m_Port], 12, 100
Column6=[m_LeftTime], 12, 100
Column7=[Remarks], 12, 100
Column8=[m_Mobile], 12, 40
Column9=[m_Email], 12, 100
Column10=[m_MAC], 12, 100
Column11=[active], 12, 100

[CLS:CCDMS1]
Type=0
HeaderFile=CDMS1.h
ImplementationFile=CDMS1.cpp
BaseClass=CRecordset
Filter=N
VirtualFilter=r

[DB:CCDMS1]
DB=1
DBType=ODBC
ColumnCount=14
Column1=[m_PhoneNo], 12, 100
Column2=[m_ID], 12, 100
Column3=[m_IP], 12, 100
Column4=[m_IP1], 12, 100
Column5=[m_Port], 12, 100
Column6=[m_LeftTime], 12, 100
Column7=[Remarks], 12, 100
Column8=[m_Mobile], 12, 40
Column9=[m_Email], 12, 100
Column10=[m_MAC], 12, 100
Column11=[active], 12, 100
Column12=[m_Name], 12, 100
Column13=[m_Password], 12, 100
Column14=[m_Addr], 12, 500

[CLS:CSIP_Processor]
Type=0
HeaderFile=SIP_Processor.h
ImplementationFile=SIP_Processor.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CSIP_Processor

[DLG:IDD_SIP_SERVER]
Type=1
Class=CSIP_Server
ControlCount=21
Control1=IDC_STATIC,static,1342308352
Control2=IDC_STATIC,static,1342308352
Control3=IDC_EDIT1,edit,1350631552
Control4=IDC_EDIT2,edit,1350631552
Control5=IDC_STATIC,button,1342177287
Control6=IDC_STATIC,button,1342177287
Control7=IDC_STATIC,static,1342308352
Control8=IDC_STATIC,static,1342308352
Control9=IDC_EDIT3,edit,1350631552
Control10=IDC_EDIT4,edit,1350631552
Control11=IDC_TAB1,SysTabControl32,1342177280
Control12=IDC_LIST1,SysListView32,1350631685
Control13=IDC_BUTTON1,button,1342242816
Control14=IDC_STATIC,static,1342308352
Control15=IDC_EDIT5,edit,1350631552
Control16=IDC_EDIT6,edit,1350631552
Control17=IDC_STATIC,button,1342177287
Control18=IDC_STATIC,static,1342308352
Control19=IDC_BUTTON2,button,1342242816
Control20=IDC_BUTTON3,button,1342242816
Control21=IDC_EDIT7,edit,1353781444

[CLS:CSIP_Server]
Type=0
HeaderFile=sip\sip_server.h
ImplementationFile=sip\sip_server.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CSIP_Server

[DLG:IDD_SIP_PROC (English (U.S.))]
Type=1
Class=?
ControlCount=0

[DLG:IDD_SIP_SERVER (English (U.S.))]
Type=1
Class=CSIP_Server
ControlCount=24
Control1=IDC_STATIC,static,1342308352
Control2=IDC_STATIC,static,1342308352
Control3=IDC_EDIT1,edit,1350631552
Control4=IDC_EDIT2,edit,1350631552
Control5=IDC_STATIC,button,1342177287
Control6=IDC_STATIC,button,1342177287
Control7=IDC_STATIC,static,1342308352
Control8=IDC_STATIC,static,1342308352
Control9=IDC_EDIT3,edit,1350631552
Control10=IDC_EDIT4,edit,1350631552
Control11=IDC_TAB1,SysTabControl32,1342177280
Control12=IDC_LIST1,SysListView32,1350631685
Control13=IDC_BUTTON1,button,1342242816
Control14=IDC_STATIC,static,1342308352
Control15=IDC_EDIT5,edit,1350631552
Control16=IDC_EDIT6,edit,1350631552
Control17=IDC_STATIC,button,1342177287
Control18=IDC_STATIC,static,1342308352
Control19=IDC_BUTTON2,button,1342242816
Control20=IDC_BUTTON3,button,1342242816
Control21=IDC_EDIT7,edit,1350631552
Control22=IDC_BUTTON5,button,1342242816
Control23=IDC_BUTTON4,button,1342275584
Control24=IDC_BUTTON7,button,1073807360

[DLG:IDD_SERVER_SETUP]
Type=1
Class=CSIP_Server_Setup
ControlCount=35
Control1=IDC_EDIT1,edit,1350633600
Control2=IDC_STATIC,button,1342177287
Control3=IDC_BUTTON1,button,1342242816
Control4=IDC_EDIT2,edit,1350633600
Control5=IDC_BUTTON6,button,1342242816
Control6=IDC_STATIC,button,1342177287
Control7=IDC_EDIT4,edit,1484851328
Control8=IDC_STATIC,static,1342308352
Control9=IDC_CHECK4,button,1476460547
Control10=IDC_CHECK5,button,1476460547
Control11=IDC_CHECK6,button,1476460547
Control12=IDC_CHECK7,button,1476460547
Control13=IDC_BUTTON2,button,1342275584
Control14=IDC_BUTTON3,button,1476460544
Control15=IDC_CHECK8,button,1342242819
Control16=IDC_BUTTON4,button,1476460544
Control17=IDC_CHECK9,button,1476460547
Control18=IDC_STATIC,button,1342177287
Control19=IDC_STATIC,button,1342177287
Control20=IDC_EDIT5,edit,1350639616
Control21=IDC_CHECK10,button,1342242819
Control22=IDC_STATIC,static,1342308352
Control23=IDC_STATIC,button,1342177287
Control24=IDC_BUTTON5,button,1476460544
Control25=IDC_CHECK11,button,1342242819
Control26=IDC_EDIT6,edit,1350633600
Control27=IDC_CHECK12,button,1342242819
Control28=IDC_CHECK13,button,1342242819
Control29=IDC_EDIT7,edit,1484857344
Control30=IDC_STATIC,static,1342308352
Control31=IDC_STATIC,button,1342177287
Control32=IDC_BUTTON7,button,1476460544
Control33=IDC_BUTTON8,button,1476460544
Control34=IDC_EDIT13,edit,1350633600
Control35=IDC_STATIC,button,1342177287

[CLS:CSIP_Server_Setup]
Type=0
HeaderFile=SIP_Server_Setup.h
ImplementationFile=SIP_Server_Setup.cpp
BaseClass=CDialog
Filter=D
LastObject=CSIP_Server_Setup
VirtualFilter=dWC

[CLS:CCDMS_DR]
Type=0
HeaderFile=CDMS_DR.h
ImplementationFile=CDMS_DR.cpp
BaseClass=CRecordset
Filter=N
VirtualFilter=r
LastObject=CCDMS_DR

[DB:CCDMS_DR]
DB=1
DBType=ODBC
ColumnCount=12
Column1=[AnswerState], 12, 60
Column2=[FilePath], 12, 510
Column3=[CallerName], 12, 60
Column4=[Date], 12, 60
Column5=[CalleeName], 12, 60
Column6=[CallerTel], 12, 60
Column7=[CalleeTel], 12, 60
Column8=[ElapsedTime], 12, 60
Column9=[StartTime], 12, 60
Column10=[EndTime], 12, 60
Column11=[CallerIP], 12, 60
Column12=[CalleeIP], 12, 60

[CLS:CCDMS_DDD]
Type=0
HeaderFile=CDMS_DDD.h
ImplementationFile=CDMS_DDD.cpp
BaseClass=CRecordset
Filter=N
VirtualFilter=r
LastObject=CCDMS_DDD

[DB:CCDMS_DDD]
DB=1
DBType=ODBC
ColumnCount=7
Column1=[DDD_Time], 12, 60
Column2=[DDD_Call_ID], 12, 256
Column3=[DDD_Date], 12, 60
Column4=[DDD_Caller], 12, 60
Column5=[DDD_Callee], 12, 60
Column6=[DDD_MsgType], 12, 60
Column7=[DDD_MsgData], -1, 2147483646

[CLS:CCDMS_LUS]
Type=0
HeaderFile=CDMS_LUS.h
ImplementationFile=CDMS_LUS.cpp
BaseClass=CRecordset
Filter=N
VirtualFilter=r

[DB:CCDMS_LUS]
DB=1
DBType=ODBC
ColumnCount=10
Column1=[PhoneNo], 12, 60
Column2=[IP], 12, 60
Column3=[Port], 12, 60
Column4=[Expires], 12, 60
Column5=[Account], 12, 60
Column6=[Password], 12, 60
Column7=[Proxy], 12, 60
Column8=[State], 12, 60
Column9=[CalleeId], 12, 60
Column10=[Name], 12, 200

[CLS:CHash]
Type=0
HeaderFile=Hash.h
ImplementationFile=Hash.cpp
BaseClass=CDialog
Filter=D

