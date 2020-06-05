
#include "Security.h"

// ===========================================================================================
BOOL g_Write_Log_Flag=FALSE;
int Enable_Write_Log(BOOL Flag)
{   g_Write_Log_Flag=Flag;
    return 0;
}
int Write_Log(const char *Head,char *S1,sockaddr_in *lpAddr)//,SIP_MSG *lpSIP_Msg)
{
//CTime Time1;
FILE  *HsfOut1;
int   Port;
char  IP[20];//,Name[100];

      if (!g_Write_Log_Flag) return -1;
      // 顯示部分訊息
      if ( (HsfOut1=fopen("T2Free_Log.txt","a+"))==NULL ) return -1;
	  else
	  {  if (lpAddr!=NULL) { Port=lpAddr->sin_port; Port=((Port&0xFF)<<8)+((Port>>8)&0xFF); strcpy(IP,inet_ntoa(lpAddr->sin_addr)); } 
	     else              { Port=0; IP[0]=0; }
	     fprintf(HsfOut1,"         << %s %s:%d   >>\r\n",Head,IP,Port );
			//             Time1.GetYear(),Time1.GetMonth(),Time1.GetDay(),Time1.GetHour(),Time1.GetMinute(),Time1.GetSecond() );
		 fprintf(HsfOut1,"%s\r\n\r\n",S1);
		 fclose(HsfOut1);       
	  }
	  return 0;
}

int Check_File_Size(char *Name)
{
int Len;
FILE *In1;

     if ( (Name!=NULL) && (strlen(Name)>0) && ((In1=fopen(Name,"rb"))!=NULL) )
	 {  fseek(In1,0,SEEK_END);
	    Len=ftell(In1);
		fclose(In1);    }
	 else Len=-1;

	 return Len;
}
int Bin2Str(unsigned char *keypart_B,char *C_keypart_B,int Len)
{
int  i;
char S1[100];

    C_keypart_B[0]=0; for (i=0;i<Len;i++) { sprintf(S1,"%02x",keypart_B[i]); strcat(C_keypart_B,S1); }
	return 0;
}
int Str2Bin(char *C_keypart_B,unsigned char *keypart_B)
{
int i,j,k;

	if (strlen(C_keypart_B)<=0) return -1;
    else
	{  for (i=0;i<(int)strlen(C_keypart_B);i+=2) 
	   {   if      ( (C_keypart_B[i]>='0')&&(C_keypart_B[i]<='9') ) j=C_keypart_B[i]-'0';
	       else if ( (C_keypart_B[i]>='A')&&(C_keypart_B[i]<='F') ) j=C_keypart_B[i]-'A'+10;
		   else if ( (C_keypart_B[i]>='a')&&(C_keypart_B[i]<='f') ) j=C_keypart_B[i]-'a'+10;
		   else j=0;
		   if      ( (C_keypart_B[i+1]>='0')&&(C_keypart_B[i+1]<='9') ) k=C_keypart_B[i+1]-'0';
		   else if ( (C_keypart_B[i+1]>='A')&&(C_keypart_B[i+1]<='F') ) k=C_keypart_B[i+1]-'A'+10;
		   else if ( (C_keypart_B[i+1]>='a')&&(C_keypart_B[i+1]<='f') ) k=C_keypart_B[i+1]-'a'+10;
		   else k=0;
		   keypart_B[i/2]=(unsigned char)(j*16+k);  }
	   return (i/2);
	}
}

void Taichi_ID_Encoder(char *S1)
{
int  i,k;
char j;
	
    // == Encoding Process ==
    k=strlen(S1);
    for (i=0;i<k;i++)   S1[i]-=0x30; 
	for (i=0;i<k;i++)   S1[i]^=0x05;
	for (i=0;i<k/2;i++) { j=S1[i]; S1[i]=S1[k-1-i]; S1[k-1-i]=j; }
	for (i=0;i<k/2;i++) { j=S1[2*i]; S1[2*i]=S1[2*i+1]; S1[2*i+1]=j; }
	for (i=0;i<k;i++)   S1[i]+='G';
}
void Taichi_ID_Decoder(char *S1)
{
int  i,k;
char j;
	
    // == Decoding Process ==
    k=strlen(S1);
	for (i=0;i<k;i++)   S1[i]-='G';
	for (i=0;i<k/2;i++) { j=S1[2*i]; S1[2*i]=S1[2*i+1]; S1[2*i+1]=j; }
	for (i=0;i<k/2;i++) { j=S1[i]; S1[i]=S1[k-1-i]; S1[k-1-i]=j; }
	for (i=0;i<k;i++)   S1[i]^=0x05;
    for (i=0;i<k;i++)   S1[i]+=0x30;
}
// ========================================================
// Text Encryption Process
// ========================================================
int Txt_HSF(char *Buf,int Len)
{
int i;//,j;
char Buf1[2000];
char A;
 
    //return Len;

    if ((Len<=0)||(Len>=1000)) return Len;

    // 為了與舊版 Compatible 而保留 for Server
	//for (j=0;j<Len;j++) if (Buf[j]=='|') break;
	//if ( ((Buf[0]!=0x30)&&(j<=3)) || (j>=Len) || ((j<Len)&&(j>=9)) ) return Len;
	
    for (i=0;i<Len;i++) Buf1[i]=Buf[i]; Buf[0]='?';
	for (i=0;i<Len;i++) Buf[i+1]=char(Buf1[i]-Buf[i]);
	for (i=0;i<Len/2;i++) { A=Buf[2*i+1]; Buf[2*i+1]=Buf[2*i+2]; Buf[2*i+2]=A; }
	for (i=0;i<Len/2;i++) { A=Buf[i+1]; Buf[i+1]=Buf[Len-i]; Buf[Len-i]=A; }
	for (i=0;i<Len/2;i++) { A=Buf[i+1]; Buf[i+1]=Buf[Len/2+i+1]; Buf[Len/2+i+1]=A; }
    return Len+1;
}

int Txt_HSF1(char *Buf,int Len)
{
int i;
char Buf1[2000];
char A;
 
    if ((Len<=0)||(Len>=1000)) return Len;

    // 為了與舊版 Compatible 而保留 for Server
	//for (j=0;j<Len;j++) if (Buf[j]=='|') break;
	//if ( ((Buf[0]!=0x30)&&(j<=3)) || (j>=Len) || ((j<Len)&&(j>=9)) ) return Len;
	
    for (i=0;i<Len;i++) Buf1[i]=Buf[i]; Buf[0]='?';
	for (i=0;i<Len;i++) Buf[i+1]=char(Buf1[i]-Buf[i]);
	for (i=0;i<Len/2;i++) { A=Buf[2*i+1]; Buf[2*i+1]=Buf[2*i+2]; Buf[2*i+2]=A; }
	for (i=0;i<Len/2;i++) { A=Buf[i+1]; Buf[i+1]=Buf[Len-i]; Buf[Len-i]=A; }
	for (i=0;i<Len/2;i++) { A=Buf[i+1]; Buf[i+1]=Buf[Len/2+i+1]; Buf[Len/2+i+1]=A; }
    return Len+1;
}

int HSF_Txt(char *Buf,int Len1)
{
int i,Len;
char Buf1[2000];
char   A;

    if ((Buf[0]!='?')||(Len1<=1)||(Len1>=1000)) return Len1;

    Len=Len1-1;
    for (i=0;i<Len1;i++) Buf1[i]=Buf[i]; 
	for (i=0;i<Len/2;i++) { A=Buf1[i+1]; Buf1[i+1]=Buf1[Len/2+i+1]; Buf1[Len/2+i+1]=A; }
	for (i=0;i<Len/2;i++) { A=Buf1[i+1]; Buf1[i+1]=Buf1[Len-i]; Buf1[Len-i]=A; }
    for (i=0;i<Len/2;i++) { A=Buf1[2*i+1]; Buf1[2*i+1]=Buf1[2*i+2]; Buf1[2*i+2]=A; }
    for (i=0;i<Len;i++) Buf[i]=char(Buf1[i+1]+Buf1[i]);
	
    return Len;
}

int Hsf_recv(SOCKET Socket, char *Buf, int Len, int Flag)
{
	int i, Len11;
	sockaddr_in Addr;

	i = recv(Socket, Buf, Len, Flag); 	
	if (i>=0) Buf[i] = 0;
	if ( (i<=0)||(Buf[0]!='?') ) 
	{  
		Len11 = sizeof(sockaddr); 
		getsockname(Socket, (sockaddr *)&Addr, &Len11); 
		Write_Log("recv", Buf, &Addr);
		return i;  
	}
	else   
	{ 
		HSF_Txt((char *)Buf, i); Buf[i-1] = 0;
		Len11 = sizeof(sockaddr); 
		getsockname(Socket, (sockaddr *)&Addr, &Len11); 
		Write_Log("recv", Buf, &Addr);
		return i-1; 
	}
}

int Hsf_send(SOCKET Socket, char *Buf1, int Len, int Flag)
{
	char Buf[2000];
	int i, j, Len11;
	sockaddr_in Addr;

    if (Len>=sizeof(Buf)) return -1; 
	else memcpy(Buf, Buf1, Len); Buf[Len] = 0;
    Len11 = sizeof(sockaddr); 
	getsockname(Socket, (sockaddr *)&Addr, &Len11); 
	Write_Log("send", Buf, &Addr);
    j = Txt_HSF((char *)Buf, Len); 
	i = send(Socket, Buf, j, Flag); 
	return i-1;  
}

int Html_Txt(char *S1)
{
	return 0;
}
int Txt_Html(char *S1)
{
	return 0;
}

int Html_send(SOCKET Socket,char *Buf1,int Len,int Flag,unsigned short g_Proxy_Port)
{
char Buf[2000];
int i;

    //Len11=sizeof(sockaddr); getpeername(Socket,(sockaddr *)&Addr,&Len11);
	Buf1[Len]=(char)(g_Proxy_Port&0xFF); Buf1[Len+1]=(char)((g_Proxy_Port>>8)&0xFF); Len+=2;

    if ((Len>=sizeof(Buf))||(Len>=2000)) return -1; else memcpy(Buf,Buf1,Len); Buf[Len]=0;
    //getsockname(Socket,(sockaddr *)&Addr,&Len11); Write_Log("send",Buf,&Addr);
    //j=Txt_HSF((char *)Buf,Len); 
	//Buf2=Base64Encode((char *)Buf,j);
	//sprintf(Buf,"<html> %s </html>",Buf2);
	i=send(Socket,Buf,Len/*strlen(Buf)*/,Flag); return i;  
}

int Html_recv(SOCKET Socket,char *Buf,int Len,int Flag)
{
int i;
static char Buf1[2000],*Buf2;

    i=recv(Socket,Buf,Len,Flag); if (i>=0) Buf[i]=0; if (i>=2000) return -1;
	//sscanf(Buf,"<html> %s </html>",Buf1);
	//Buf2=Base64Decode(Buf1,&Len22);
	//if ( (Len22<=0)||(Buf2[0]!='?') ) 
	//{  Len11=sizeof(sockaddr); getsockname(Socket,(sockaddr *)&Addr,&Len11); Write_Log("recv",Buf,&Addr);
	//   return i;  }
	//else   
	//{  HSF_Txt((char *)Buf2,Len22); Buf[Len22-1]=0;
    //   Len11=sizeof(sockaddr); getsockname(Socket,(sockaddr *)&Addr,&Len11); Write_Log("recv",Buf,&Addr);
	//   return Len22-1; }
	return i;
}

// ==== UDP ====
int Hsf_recvfrom(SOCKET Socket,char *Buf,int Len,int Flag,sockaddr *lpAddr,int *lpLen)
{
int i;
    i=recvfrom(Socket,Buf,Len,Flag,lpAddr,lpLen); if (i>=0) Buf[i]=0;
	if ( (i<=0)||(Buf[0]!='?') ) 
	{  Write_Log("recvfrom",Buf,(sockaddr_in *)lpAddr);
	   return i; }
	else   
	{  HSF_Txt((char *)Buf,i); Buf[i-1]=0;
       Write_Log("recvfrom",Buf,(sockaddr_in *)lpAddr);
	   return i-1; } 
}
int Hsf_recvfrom1(SOCKET Socket,char *Buf,int Len,int Flag,sockaddr *lpAddr,int *lpLen,BOOL *lpFlag)
{
int i;
    i=recvfrom(Socket,Buf,Len,Flag,lpAddr,lpLen); if (i>=0) Buf[i]=0;
	if ( (i<=0)||(Buf[0]!='?') ) 
	{  *lpFlag=FALSE; 
       Write_Log("recvfrom1",Buf,(sockaddr_in *)lpAddr);
	   return i; }
	else   
	{  HSF_Txt((char *)Buf,i); *lpFlag=TRUE; Buf[i-1]=0;
	   Write_Log("recvfrom1",Buf,(sockaddr_in *)lpAddr);
       return i-1; } 
}
int Hsf_sendto(SOCKET Socket,char *Buf1,int Len,int Flag,sockaddr *lpAddr,int SLen)
{
static char Buf[2000];
int i,j;

    if (Len>=sizeof(Buf)) return -1; else memcpy(Buf,Buf1,Len); Buf[Len]=0;
    Write_Log("sendto",Buf,(sockaddr_in *)lpAddr);
    j=Txt_HSF((char *)Buf,Len);	i=sendto(Socket,Buf,j,Flag,lpAddr,SLen); return i-1;
}
int Hsf_sendto1(SOCKET Socket,char *Buf1,int Len,int Flag,sockaddr *lpAddr,int SLen,BOOL SFlag)
{
static char Buf[2000];
int i,j;

    Write_Log("sendto1",Buf1,(sockaddr_in *)lpAddr);
    if (Len>=sizeof(Buf)) return -1; else memcpy(Buf,Buf1,Len); Buf[Len]=0;
    if (SFlag==FALSE)
	{  i=sendto(Socket,Buf,Len,Flag,lpAddr,SLen);
       return i;  }	
	else
	{  j=Txt_HSF((char *)Buf,Len);
	   i=sendto(Socket,Buf,j,Flag,lpAddr,SLen);
       return i-1;  }
}

// ========================================================================
// Security Process 
// ========================================================================
// 因為 Server 必須放在 Real IP 環境下, 因此 R_IP=VIP,
// 在 Get_Total 時, 於 wParam 只能取得 V_IP, 而 R_IP需藉由 LS 回復告知
// 因此 在 Location 之 Get_Total 只能假設 V_IP = R_IP
UINT Get_Total(WPARAM wParam,char *Head,char *IDD,char *VIP,UINT Year,UINT Month,UINT Date)
{
int         Len;
sockaddr_in Addr;
//CTime       Time1;
UINT        R_IPNo,V_IPNo,PortNo,PNo,Total;
HOSTENT     *lpHostEnt;
char        S1[100];

			// ==== Get Local_IP ====
			Len=sizeof(sockaddr);
			getsockname(wParam,(sockaddr *)&Addr,&Len);
			R_IPNo=inet_addr(inet_ntoa(Addr.sin_addr));
			if (R_IPNo==0)
			{  gethostname(S1,sizeof(S1)-1);
			   lpHostEnt=gethostbyname(S1);
			   if (lpHostEnt!=NULL) 
				  R_IPNo=inet_addr(inet_ntoa(*(LPIN_ADDR)(lpHostEnt->h_addr))); 
			   else R_IPNo=0;
			}
			V_IPNo=inet_addr(VIP);
			PortNo=(UINT)(Addr.sin_port>>8)+(UINT)((Addr.sin_port&0xFF)<<8);
			PNo=(UINT)atol(IDD);

            // ==== For Security ====
		    //Time1=CTime::GetCurrentTime();
			//Date=(UINT)Time1.GetDay();
			//Month=(UINT)Time1.GetMonth();
			//Year=(UINT)Time1.GetYear();
			Total=HSF_Security_Set(Head,Year,Month,Date,R_IPNo,V_IPNo,PortNo,PNo,0);
			return Total;
}

BOOL Check_Total(WPARAM wParam,char *Command22,char *Security22,char *ID22,char *V_IP,char *Addrr,int *Ret,UINT Year,UINT Month,UINT Date)
{
int         Len;//,i;
sockaddr_in Addr;
//CTime       Time1;
UINT        R_IPNo,V_IPNo,PortNo,PNo,Total,Pass;
char        Password[100];//,S1[500];
BOOL        Abc;

            Ret[0]=0;
			// ==== Get Local_IP ====
            Len=sizeof(sockaddr);
			getpeername(wParam,(sockaddr *)&Addr,&Len);
			R_IPNo=inet_addr(inet_ntoa(Addr.sin_addr));
			V_IPNo=inet_addr(V_IP);
			PortNo=(UINT)(Addr.sin_port>>8)+(UINT)((Addr.sin_port&0xFF)<<8);
			PNo=(UINT)atol(ID22);

            // ==== For Security ====
		    //Time1=CTime::GetCurrentTime();
			//Date=(UINT)Time1.GetDay();
			//Month=(UINT)Time1.GetMonth();
			//Year=(UINT)Time1.GetYear();
			Total=(UINT)atol(Security22);

			// ((==02/021/022)&&(Login))
			// ==== SoftPhone 在 Login 時才需要使用 Password ====
			if ( ((!strcmp(Command22,"02"))||(strcmp(Command22,"021"))||(strcmp(Command22,"022"))) &&
				 ((Addrr!=NULL)&&(strlen(Addrr)==0)) )//&&
				 //((ID22[0]=='2')&&(strlen(ID22)>=PHONE_DIGNUM))&&
				 //(lpCDMS!=NULL)  )
			{  //Ret[0]=lpCDMS->Get_Password(ID22,Password); // -2: inActive, -3: Account Invalid
			   Pass=(UINT)atol(Password); }
			else Pass=0;

			Abc=HSF_Security_Check(Command22,Year,Month,Date,R_IPNo,V_IPNo,PortNo,PNo,Pass,Total);
//{  sprintf(S1,"%s [%u %u %u] [%u %u %u] %u %u T=%u\r\n",Command22,Year,Month,Date,R_IPNo,V_IPNo,PortNo,PNo,Pass,Total);
//SetWindowText(S1); }
			return Abc;
}



UINT HSF_Security_Set(char *Command,UINT Year,UINT Month,UINT Date,UINT R_IP,UINT V_IP,UINT Port,UINT No,UINT Password)
{
UINT Total,i;
BYTE *Point;
char S1[256];

    // 1. Check Date, IP, Port, No
    if (!strcmp(Command,"025")) R_IP=0; // 025 Command 用以 Check NAT IP, 故 R_IP 事先未知
    Point=(BYTE *)&Total;
	if (Password==0)
	{  if (R_IP==V_IP) { Total=Year*Month+Date*R_IP-Port*No+(Year|R_IP)-(~(Month*Port))+(Date|No)+Date*((~Port)>>4)-((~Date)&24)*(Port^12345); Total|=121177524; Total^=100591153; }
	   else            { Total=Year*Month+Date*R_IP-V_IP*No+(Year|R_IP)-(~(Month*V_IP))+(Date|No)-Date*((~V_IP)>>6)+(Date^46)*((~V_IP)|54321); Total&=220641440; Total+=200612159; }
	}
	else
	{  if (R_IP==V_IP) { Total=Password*Month   +Date*R_IP-Port*No+(Year|R_IP)-(~(Month*Port))+(Date|No)+Date*((~Port)>>4)-((~Date)&24)*(Port^12345); Total|=200612159; Total^=220641440; }
	   else            { Total=Year    *Password+Date*R_IP-V_IP*No+(Year|R_IP)-(~(Month*V_IP))+(Date|No)-Date*((~V_IP)>>6)+(Date^46)*((~V_IP)|54321); Total&=100591153; Total+=121177524; }
	}

	// 2. Check Command Format
	strcpy(S1,Command);
	for (i=0;i<strlen(S1);i++) 
	{   if      ((S1[i]>='A')&&(S1[i]<='Z')) { S1[i]-='A'; S1[i]%=10; S1[i]+='0'; }
	    else if ((S1[i]>='a')&&(S1[i]<='z')) { S1[i]-='a'; S1[i]%=10; S1[i]+='0'; }
		else if (S1[i]<'0') S1[i]='0';
		else if (S1[i]>'9') S1[i]='9';  }

	// 3. Summation
	Total+=atol(S1)*Date|No;
	for (i=0;i<strlen(Command);i++) Total+=(UINT)Command[i]<<(i*3);
    Total=((UINT)Point[0]<<24)+(UINT)Point[1]+((UINT)Point[2]<<16)+((UINT)Point[3]<<8);
	if (Password!=0) Total+=Password^R_IP;

	return Total;
}
BOOL HSF_Security_Check(char *Command,UINT Year,UINT Month,UINT Date,UINT R_IP,UINT V_IP,UINT Port,UINT No,UINT Password,UINT Total)
{
UINT ODate,OMonth,OYear,Total1;

     // Estimate Current Date
     ODate=Date; OMonth=Month; OYear=Year;
	 if ((Total1=HSF_Security_Set(Command,OYear,OMonth,ODate,R_IP,V_IP,Port,No,Password))==Total) return TRUE;

	 // Estimate Previous Date
	 ODate=Date; OMonth=Month; OYear=Year;
	 if (ODate>=2) ODate--;
	 else
	 {  OMonth--; if (OMonth<=0) { OMonth=12; OYear--; }
		if ((OMonth==1)||(OMonth==3)||(OMonth==5)||(OMonth==7)||(OMonth==8)||(OMonth==10)||(OMonth==12)) ODate=31;
	    else if ((OMonth==4)||(OMonth==6)||(OMonth==9)||(OMonth==11)) ODate=30;
		else
		{  if ((OYear%400)==0) ODate=29; else if ((OYear%100)==0) ODate=28; else if ((OYear%4)==0) ODate=29; else ODate=28; }
	 }
	 if (Total1=HSF_Security_Set(Command,OYear,OMonth,ODate,R_IP,V_IP,Port,No,Password)==Total) return TRUE;

	 // Estimate Next Date
	 ODate=Date; OMonth=Month; OYear=Year;
	 if (OMonth==2) 
	 {  if ((OYear%400)==0)      { if (ODate<29) ODate++; else { ODate=1; OMonth++; } }
	    else if ((OYear%100)==0) { if (ODate<28) ODate++; else { ODate=1; OMonth++; } }
        else if ((OYear%4)==0)   { if (ODate<29) ODate++; else { ODate=1; OMonth++; } }
		else                     { if (ODate<28) ODate++; else { ODate=1; OMonth++; } }	 }
	 else if ((OMonth==4)||(OMonth==6)||(OMonth==9)||(OMonth==11)) { if (ODate<30) ODate++; else { ODate=1; OMonth++; } }
     else	 {  if (ODate<31) ODate++; else { ODate=1; OMonth++; } }
	 if (OMonth>12) { OMonth=1; OYear++; }
	 if (HSF_Security_Set(Command,OYear,OMonth,ODate,R_IP,V_IP,Port,No,Password)==Total) return TRUE;

	 // Verify Failure
	 return FALSE;
}
