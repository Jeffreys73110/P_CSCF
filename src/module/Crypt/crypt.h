#ifndef CRYPT_H
#define CRYPT_H

#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
//#include "includes.h"
//#include "StartSip.h"
// #ifdef __cplusplus
// extern "C" {
// #endif
// // #include "net/netall.h"
// // #include "net/netsock.h"
// #ifdef __cplusplus
// }
// #endif

#define	INT8U	unsigned char
#define	INT16U	unsigned int


typedef enum
{
	SIP_ENCRYPT_AVS=0,
	SIP_ENCRYPT_WALKSUN,
	SIP_ENCRYPT_SEAWOLF,
	SIP_ENCRYPT_SEAWOLF_N2C,
	SIP_ENCRYPT_GX_XOR,

	SIP_ENCRYPT_CUSTOMER_DEF_0=0xe0,
	SIP_ENCRYPT_CUSTOMER_DEF_1,
	SIP_ENCRYPT_CUSTOMER_DEF_2,
	SIP_ENCRYPT_CUSTOMER_DEF_3,
	SIP_ENCRYPT_CUSTOMER_DEF_4,
	SIP_ENCRYPT_CUSTOMER_DEF_5,
	SIP_ENCRYPT_CUSTOMER_DEF_6,
	SIP_ENCRYPT_CUSTOMER_DEF_7,
	SIP_ENCRYPT_CUSTOMER_DEF_8,
	SIP_ENCRYPT_CUSTOMER_DEF_9,
	
	SIP_ENCRYPT_UNKNOW=0xff
}SIP_ENCRYPT_TYPE;

typedef enum
{
	RTP_ENCRYPT_WALKSUN=0,
	RTP_ENCRYPT_SEAWOLF,
	RTP_ENCRYPT_SEAWOLF_N2C,
	RTP_ENCRYPT_GX_XOR,
	
	RTP_ENCRYPT_CUSTOMER_DEF_0=0xe0,
	RTP_ENCRYPT_CUSTOMER_DEF_1,
	RTP_ENCRYPT_CUSTOMER_DEF_2,
	RTP_ENCRYPT_CUSTOMER_DEF_3,
	RTP_ENCRYPT_CUSTOMER_DEF_4,
	RTP_ENCRYPT_CUSTOMER_DEF_5,
	RTP_ENCRYPT_CUSTOMER_DEF_6,
	RTP_ENCRYPT_CUSTOMER_DEF_7,
	RTP_ENCRYPT_CUSTOMER_DEF_8,
	RTP_ENCRYPT_CUSTOMER_DEF_9,
	
	RTP_ENCRYPT_UNKNOW=0xff
}RTP_ENCRYPT_TYPE;

extern SIP_ENCRYPT_TYPE sipEncryptFunctionSelect;
extern RTP_ENCRYPT_TYPE rtpEncryptFunctionSelect;

typedef struct
{
	unsigned short   EncryptMethod;
	unsigned short   Length;
}N2C_MAPHEADER;


typedef	enum
{
	Sec_Success = 0,
	Encode_MsgLen_Invalid,
	Encode_MsgBuf_Invalid,
	Encode_MapTable_Invalid,
	Encode_MapSize_Invalid,
	Encode_MapSize_TooSmall,	

	Decode_MsgLen_Invalid,
	Decode_MsgBuf_Invalid,
	Decode_MapTable_Invalid,
	Decode_MapSize_Invalid,
	Decode_EncrypMethod_Invalid,
	Decode_MsgBodyLength_Invalid,	
	Decode_MapSize_TooSmall,		

}N2C_Sec_ErrorCode;

#ifdef __cplusplus
extern "C" {
#endif

void SIP_TKPT_Secret(const char * tkpt_packet, int len, char SCNo, char Factor, unsigned char *result);
int SIP_TKPT_DeSecret(const char * tkpt_packet, int length, unsigned char *result);
int sip_encrypt(const char *sip_packet, unsigned char *result);
int sip_decrypt(const char *encryptstr, const int packetLength, char *decryptstr);
int check_sippacket(const char *sipcryptpacket, const int packetLength);
int WallkersunRtpEncrypt(const unsigned char* rtp_pkt, const int nPktLen, unsigned char* result);
int WallkersunRtpDecrypt(const unsigned char* encrypt_pkt, const int nPktLen, unsigned char* decrypt);

N2C_Sec_ErrorCode N2C_Map_Encode_Algorithm(const char *msg_buffer,
										   unsigned short len,
										   char *encode_buffer,
										   unsigned int *actuallen,
										   const char *EncodeMapTable,
										   int MapSize,
										   unsigned short EncryptMethod);

N2C_Sec_ErrorCode N2C_Map_Decode_Algorithm(const char *msg_ptr,
										   unsigned short len,
										   char *decode_buffer,
										   unsigned int *actuallen,
										   const char *DecodeMapTable,
										   int MapSize,
										   unsigned short EncryptMethod);

int SeaWolf_sip_encrypt(const char *sip_packet, unsigned char *result);
int SeaWolf_sip_decrypt(const char *encryptstr, const int packetLength, char *decryptstr);
int SeaWolf_rtp_encrypt(const unsigned char *sip_packet, const int packetlen, unsigned char *result);
int SeaWolf_rtp_decrypt(const unsigned char *encryptstr, const int packetLength, unsigned char *decryptstr);

int InitSecurityMap(void);
int SeaWolf_sip_encrypt_N2C(const char *sip_packet, unsigned char *result);
int SeaWolf_sip_decrypt_N2C(const char *encryptstr, const int packetLength, char *decryptstr);
int SeaWolf_rtp_encrypt_N2C(const unsigned char *rtp_packet, const int packetlen, unsigned char *result);
int SeaWolf_rtp_decrypt_N2C(const unsigned char *encryptstr, const int packetLength, unsigned char *decryptstr);
int SeaWolf_check_sippacket(const char *sipcryptpacket, const int packetLength);

typedef struct
{
	unsigned char key;
	unsigned char *packet;
} t_walkersun_rtp_encpacket;

int walkersun_sip_encrypt(const char *sip_packet, unsigned char *result);
int walkersun_sip_decrypt(const char *encryptstr, const int packetLength, char *decryptstr);
int walkersun_check_sippacket(const char *sipcryptpacket, const int packetLength);
void walkersunEncMsg(INT8U* pMsg, INT16U msgLen, unsigned char *result);
unsigned char* walkersun_rtp_decrtphdr(t_walkersun_rtp_encpacket* epack);
void walkersun_rtp_encrtphdr(unsigned char* rtppack, t_walkersun_rtp_encpacket* epack);
unsigned char krand(void);


// don't change following define
typedef int (*RTP_ENCRYPT_FUN)(const unsigned char* rtp_pkt, const int nPktLen, unsigned char* result);			// return encrypt data length
typedef int (*RTP_DECRYPT_FUN)(const unsigned char* encrypt_pkt, const int nPktLen, unsigned char* decrypt);	// return decrypt data length
typedef int (*SIP_ENCRYPT_FUN)(const char* sip_pkt, unsigned char* result);
typedef int (*SIP_DECRYPT_FUN)(const char* encrypt_pkt, const int nPktLen, char* decrypt);
typedef int (*SIP_CHECK_SIP_ENCRYPT_FUN)(const char* encrypt_pkt, const int nPktLen);
SIP_ENCRYPT_FUN getSipCryptFun(void);
SIP_DECRYPT_FUN getSipDeCryptFun(void);
SIP_CHECK_SIP_ENCRYPT_FUN getSipCryptCheckFun(void);
RTP_ENCRYPT_FUN getRtpCryptFun(void);
RTP_DECRYPT_FUN getRtpDeCryptFun(void);
SIP_ENCRYPT_TYPE getSipCryptType(void);
RTP_ENCRYPT_TYPE getRtpCryptType(void);

int encrypt_xor(char *input, char *output, int len);
int GxXor_sip_encrypt(const char *sip_packet, unsigned char *result);
int GxXor_sip_decrypt(const char *encryptstr, const int packetLength, char *decryptstr);
int GxXor_rtp_encrypt(const unsigned char *rtp_packet, const int packetlen, unsigned char *result);
int GxXor_rtp_decrypt(const unsigned char *encryptstr, const int packetLength, unsigned char *decryptstr);
int GxXor_check_sippacket(const char *sipcryptpacket, const int packetLength);

#ifdef __cplusplus
}
#endif

#endif
