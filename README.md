# P_CSCF

The server supports basic initial attach and volte functions.
The version is developed based on the purpose of feasibility verification of internal calls of cellphones, so the server is somewhat unstable, incomplete.
The server has two roles, Caller and Callee, and it can play one of the caller or callee roles at the same time.

#### Dedicated bearer creation session
For the feasibility verification purpose, the server sends 0sSIP_CQI1 command as "create bearer request" which use s1ap protocol channel to deliver this command instead of sgi-s5-s11 protocols.


```
    ┌───────────────┐               ┌───────────────┐               ┌───────────────┐               ┌───────────────┐
    │       UE1     │               │      eN       │               │      MME      │               │     P_CSCF    │
    │  (CellPhone)  │               │               │               │               │               │               │
    └───────┬───────┘               └───────┬───────┘               └───────┬───────┘               └───────┬───────┘ 
            │                               │                               │         0SIP_CQI1|...         │
            │                               │     E-RAB Setup Request       │◀──────────────────────────────┤
            │     RRC Conn Reconfiguration  │◀──────────────────────────────┤                               │
            │◀──────────────────────────────┤                               │                               │
            │ RRC Conn Reconfiguraton Complete                              │                               │
            ├──────────────────────────────▶│     E-RAB Setup Response      │                               │
            │                               ├──────────────────────────────▶│                               │
            │                               │                               │                               │
    ┌───────────────┐                       │                               │                               │
    │       UE2     │                       │                               │                               │
    │  (CellPhone)  │                       │                               │                               │
    │               │                       │                               │                               │
    └───────┬───────┘                       │                               │                               │
            │                               │     E-RAB Setup Request       │                               │
            │     RRC Conn Reconfiguration  │◀──────────────────────────────┤                               │
            │◀──────────────────────────────┤                               │                               │
            │ RRC Conn Reconfiguraton Complete                              │                               │
            ├──────────────────────────────▶│     E-RAB Setup Response      │                               │
            │                               ├──────────────────────────────▶│           SIP_CQI10           │
            │                               │                               ├──────────────────────────────▶│
            │                               │                               │                               │
```

#### SIP application layer gateway (ALG) mechanism
The concept of SIP ALG is to modify SDP IP and port to external address information when the SIP UE under a router supporting SIP ALG.
The function is usually designed on a router, but here is designed on this server. 
Based on the feasibility verification purpose and fragmented packet consideration of spgw, the function is designed in the server to reduce developing time.

```
    SIP ALG:
    ┌───────────────┐       ┌───────────────┐               ┌───────────────┐
    │       UE1     │       │    SGW/PGW    │               │    P_CSCF     │
    │  (CellPhone)  │       │               │               │               │
    │               │       │               │  INVITE/ACK   │               │ INVITE/ACK
    │               ├──────▶│               ├──────────────▶│               ├───────┐
    │               │       │               │   REGISTER    │               │       │
    │               │       │               │               │               │       │
    │               │       │ Alg_External_Media_Port1      │               │       │
    │               │◀═════▶│               │◀═══════╗      │               │       │
    │               │       │               │        ║      │               │       │
    └───────┬───────┘       │               │        ║      │               │       │
            └───────────┐   │               │        ║      │               │       │
    ┌───────────────┐   │   │               │        ║      │               │       │
    │       UE2     │   │   │               │        ║      │               │       │
    │  (CellPhone)  │   │   │               │        ║      │               │       │
    │               │   │   │               │        ║      │               │       │
    │               │◀═════▶│               │◀═══════╝      │               │       │
    │               │   │   │ Alg_External_Media_Port2      │               │       │
    │               │   │   │               │               │               │       │
    │               │   │   │               │   REGISTER    │               │       │
    │               ├──────▶│               ├──────────────▶│               │◀──────┘
    │               │   │   │               │               │               │
    └───────┬───────┘   │   └───────┬───────┘               └───────┬───────┘
            │           │           │                               │
            │           │           │                               │
            │           │           │                               │
    ────────▼───────────▼───────────▼───────────────────────────────▼─────────
         (PDN_IP2)  (PDN_IP1)    (PGW_IP)                      (P_CSCF_IP)  
```

## Architecture
```
    ┌───────────────┐       ┌───────────────┐       ┌───────────────┐       ┌───────────────────┐       ┌───────────────┐       ┌───────────────┐
    │               │       │               │       │               │       │                   │       │               │       │               │
    │               │       │               │       │               │       │                   │       │               │       │   CellPhone / │
    │       UE      ├──────▶│       eNB     ├──────▶│       MME     ├──────▶│       SGW/PGW     ├──────▶│     P_CSCF    │◀──────┤   IMS_UDP_S   │
    │               │       │               │       │               │       │                   │       │               │       │               │
    │               │       │               │       │               │       │                   │       │               │       │   (SIP UA)    │
    │               │       │               │       │               │       │                   │       │               │       │               │
    └───┬───────────┘       └───┬───────────┘       └───┬───────────┘       └───┬───────────┬───┘       └───┬───────────┘       └───┬───────────┘
        │                       │                       │                       │           │               │                       │
        │                       │                       │                       │           │               │                       │
        │                   ────▼───────────────────────▼───────────────────────▼──         │               │                       │
        │                 10.102.81.59            10.102.81.100           10.102.81.102     │               │                       │
        │                                            (MME_IP)                 (SGW_IP)      │               │                       │
        │                                                                                   │               │                       │
    ────▼───────────────────────────────────────────────────────────────────────────────────▼───────────────▼───────────────────────▼───
     (PDN_IP)                                                                            (PGW_IP)       (P_CSCF_IP)            (SIP UA IP)

```

## OS
ubuntu 18.04

## Installation and Configuration
* set network environment
	```shell
 	 $ sudo sh setip.sh
	```
* Adjust configuration
	```shell
	# edit parameters and save file function
	$ vim StdAfx.h
	```

* Build code and run
	```shell
	# build code
	$ make

	# run server after the EPC_Server starts up
	$ ./p_cscf.out
	```

* unset network environment
	```shell
  	$ sudo sh unset_ip.sh
	```

## Modify configuration
	```shell
	# edit parameters of setup.ini
	$ vim Setup/setup.ini

	# restart server
	$ ⌃C and ./p_cscf.out
	```

## Troubleshooting
#### Registering is no reaction
So far, the server hasn't supported aka-md5 authentication, so the current version is set as every registration must be successful.
However, if the registration failed, please check follows:
* **Check if the account is existed in SIP_Server/User.cpp?**
	Add the account on User.cpp.

#### Register failed
If wireshark shows the server always responds 401 responses for register packets, it has follows causes:
* **Check if the parameters of SIP content are filled out wrong values, such as Call-ID, Branch?**
	It probably has bugs. Debug or re-execute the server.
* **Check if 401 responses with wrong gtp teid?**
	It probably has bugs. Debug or re-execute the server.
* **Check if SIP TCP connection established or rejected successfully?**
	The cause of the problem is that TCP forwarding failed in EPC_Server, which has bugs. Debug or re-execute the server.

#### 404 Not Found
If wireshark shows 404 Not Found when inviting, please check follows:
* **Check if the callee has registered successfully?**
  	Make the callee register to the server.
* **Check if the dialing phone number is correct?**
	Correct the dialing phone number.

#### No RTP packet
If wireshark don't show any rtp packets after sip inviting procedure acknowledgement, please check follows:
* **Check if ID (Phone ID) in User.cpp are the same as cellphones' IMSIs?**
	Give it correct values.
* **Check if the SIP_Alg_Flag parameter in Setup/setup.ini is disabled?**
	Give it a false value, and restart the server to reload the configuration.
* **Check if EPC_Server sends out E-RAB Setup Requests?**
	The cause is that EPC_Server will assign different MME_ID for each attachment process. 
	If the attachment process doesn't be released normally, the UE data will be retained in the ue_list queue and be redundant.
	Thus, please restart EPC_Server to reset the ue_list queue.
* **Check if wireshark shows E-RABSetupResponse [RadioNetwork-cause=unknown-enb-ue-s1ap-id]**
	Enable the cellphone's airplane mode and disable it, and make a call again.
* **Check if wireshark shows E-RABSetupResponse [RadioNetwork-cause=multiple-E-RAB-ID-instances]**
	Enable the cellphone's airplane mode and disable it, and make a call again.

#### No sound but Wireshark shows rpt packets
If there is no sound but Wireshark shows rpt packets, please check follows:
* **Check if SDP IP and port are the same as expected?**
	Debug it or re-execute the server.

#### Program crash
Debug it or re-execute the server.