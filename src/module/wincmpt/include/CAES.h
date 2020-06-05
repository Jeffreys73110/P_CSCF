#ifndef WINCMPT_SRC_INCLUDE_CAES_H_
#define WINCMPT_SRC_INCLUDE_CAES_H_

#ifndef AES_MAXNR
#define AES_MAXNR 14
#endif

#ifndef AES_BLOCK_SIZE
#define AES_BLOCK_SIZE 16
#endif

// This should be a hidden type, but EVP requires that the size be known
typedef struct
{
    unsigned int rd_key[4 *(AES_MAXNR + 1)];
    int rounds;
}
CAES_KEY;

// encrypt & decrypt key
typedef struct
{
	CAES_KEY enc;
	CAES_KEY dec;
}
CAES_KEYS;

///////////////////////////////////////////////////////////////////////////////////////////////
// API

#ifdef __cplusplus
extern "C" {
#endif

int CAES_set_encrypt_key(const unsigned char *userKey, const int bits, CAES_KEY *key);
int CAES_set_decrypt_key(const unsigned char *userKey, const int bits, CAES_KEY *key);
void CAES_encrypt(const unsigned char *in, unsigned char *out, const CAES_KEY *key);
void CAES_decrypt(const unsigned char *in, unsigned char *out, const CAES_KEY *key);

int CAesKeyInit(const void* in, unsigned int len, int bits, CAES_KEYS *key); // ECB
unsigned int CAesEncrypt(const void *in, unsigned int len, void *out, unsigned int size, CAES_KEYS *key); // ECB
unsigned int CAesDecrypt(const void *in, unsigned int len, void *out, unsigned int size, CAES_KEYS *key); // ECB

unsigned int CAesCbcEncrypt(void       *in, unsigned int len, const void *iv, unsigned int vlen, void *out, unsigned int size, CAES_KEYS *key); // *in will be effected by padding bytes
unsigned int CAesCbcDecrypt(const void *in, unsigned int len, const void *iv, unsigned int vlen, void *out, unsigned int size, CAES_KEYS *key);

#ifdef __cplusplus
}
#endif

#endif /* WINCMPT_SRC_INCLUDE_CAES_H_ */
