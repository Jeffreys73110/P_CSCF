/*
 * FIPS 180-2 SHA-224/256/384/512 implementation
 * Last update: 02/02/2007
 * Issue date:  04/30/2005
 *
 * Copyright (C) 2005, 2007 Olivier Gay <olivier.gay@a3.epfl.ch>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the project nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE PROJECT AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE PROJECT OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#ifndef CSHA2_H
#define CSHA2_H

#ifndef SHA_64BITS_OP
#define SHA_64BITS_OP 0
#endif
#ifndef SHA224_DIGEST_SIZE
#define SHA224_DIGEST_SIZE ( 224 / 8)
#endif
#ifndef SHA256_DIGEST_SIZE
#define SHA256_DIGEST_SIZE ( 256 / 8)
#endif
#ifndef SHA384_DIGEST_SIZE
#define SHA384_DIGEST_SIZE ( 384 / 8)
#endif
#ifndef SHA512_DIGEST_SIZE
#define SHA512_DIGEST_SIZE ( 512 / 8)
#endif
#ifndef SHA256_BLOCK_SIZE
#define SHA256_BLOCK_SIZE  ( 512 / 8)
#endif
#ifndef SHA512_BLOCK_SIZE
#define SHA512_BLOCK_SIZE  (1024 / 8)
#endif
#ifndef SHA384_BLOCK_SIZE
#define SHA384_BLOCK_SIZE  SHA512_BLOCK_SIZE
#endif
#ifndef SHA224_BLOCK_SIZE
#define SHA224_BLOCK_SIZE  SHA256_BLOCK_SIZE
#endif

typedef struct 
{
	unsigned int tot_len;
	unsigned int len;
	unsigned char block[2 * SHA256_BLOCK_SIZE];
	unsigned int h[8];
} 
sha256_obj;
typedef sha256_obj sha224_obj;

#if SHA_64BITS_OP
typedef struct 
{
	unsigned int tot_len;
	unsigned int len;
	unsigned char block[2 * SHA512_BLOCK_SIZE];
	unsigned long long h[8];
} 
sha512_obj;
typedef sha512_obj sha384_obj;
#endif // SHA_64BITS_OP

//////////////////////////////////////////////////////////////////////////////////
// API

#ifdef __cplusplus
extern "C" {
#endif

void csha224_init(sha224_obj *ctx);
void csha224_update(sha224_obj *ctx, const unsigned char *message,
                    unsigned int len);
void csha224_final(sha224_obj *ctx, unsigned char *digest);
void csha224(const unsigned char *message, unsigned int len,
             unsigned char *digest);
    
void csha256_init(sha256_obj * ctx);
void csha256_update(sha256_obj *ctx, const unsigned char *message,
                    unsigned int len);
void csha256_final(sha256_obj *ctx, unsigned char *digest);
void csha256(const unsigned char *message, unsigned int len,
             unsigned char *digest);

#if SHA_64BITS_OP
void csha384_init(sha384_obj *ctx);
void csha384_update(sha384_obj *ctx, const unsigned char *message,
                    unsigned int len);
void csha384_final(sha384_obj *ctx, unsigned char *digest);
void csha384(const unsigned char *message, unsigned int len,
             unsigned char *digest);
    
void csha512_init(sha512_obj *ctx);
void csha512_update(sha512_obj *ctx, const unsigned char *message,
                    unsigned int len);
void csha512_final(sha512_obj *ctx, unsigned char *digest);
void csha512(const unsigned char *message, unsigned int len,
             unsigned char *digest);
#endif // SHA_64BITS_OP
    
#ifdef __cplusplus
}
#endif

#endif /* !SHA2_H */

// =============================================================================================================
/*
 * HMAC-SHA-224/256/384/512 implementation
 * Last update: 06/15/2005
 * Issue date:  06/15/2005
 *
 * Copyright (C) 2005 Olivier Gay <olivier.gay@a3.epfl.ch>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the project nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE PROJECT AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE PROJECT OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#ifndef CHMAC_SHA2_H
#define CHMAC_SHA2_H

typedef struct 
{
	sha224_obj ctx_inside;
	sha224_obj ctx_outside;
        
	/* for hmac_reinit */
	sha224_obj ctx_inside_reinit;
	sha224_obj ctx_outside_reinit;
        
	unsigned char block_ipad[SHA224_BLOCK_SIZE];
	unsigned char block_opad[SHA224_BLOCK_SIZE];
} 
hmac_sha224_obj;
    
typedef struct 
{
	sha256_obj ctx_inside;
	sha256_obj ctx_outside;
        
	/* for hmac_reinit */
	sha256_obj ctx_inside_reinit;
	sha256_obj ctx_outside_reinit;
        
	unsigned char block_ipad[SHA256_BLOCK_SIZE];
	unsigned char block_opad[SHA256_BLOCK_SIZE];
} 
hmac_sha256_obj;

#if SHA_64BITS_OP
typedef struct 
{ 
	sha384_obj ctx_inside;
	sha384_obj ctx_outside;
        
	/* for hmac_reinit */
	sha384_obj ctx_inside_reinit;
	sha384_obj ctx_outside_reinit;
        
	unsigned char block_ipad[SHA384_BLOCK_SIZE];
	unsigned char block_opad[SHA384_BLOCK_SIZE];
} 
hmac_sha384_obj;
    
typedef struct 
{
	sha512_obj ctx_inside;
	sha512_obj ctx_outside;
        
	/* for hmac_reinit */
	sha512_obj ctx_inside_reinit;
	sha512_obj ctx_outside_reinit;
        
	unsigned char block_ipad[SHA512_BLOCK_SIZE];
	unsigned char block_opad[SHA512_BLOCK_SIZE];
} 
hmac_sha512_obj;
#endif // SHA_64BITS_OP

//////////////////////////////////////////////////////////////////////////////////
// API

#ifdef __cplusplus
extern "C" {
#endif

void hmac_csha224_init(hmac_sha224_obj *ctx, const unsigned char *key,
                       unsigned int key_size);
void hmac_csha224_reinit(hmac_sha224_obj *ctx);
void hmac_csha224_update(hmac_sha224_obj *ctx, const unsigned char *message,
                         unsigned int message_len);
void hmac_csha224_final(hmac_sha224_obj *ctx, unsigned char *mac,
                        unsigned int mac_size);
void hmac_csha224(const unsigned char *key, unsigned int key_size,
                  const unsigned char *message, unsigned int message_len,
                  unsigned char *mac, unsigned mac_size);
    
void hmac_csha256_init(hmac_sha256_obj *ctx, const unsigned char *key,
                       unsigned int key_size);
void hmac_csha256_reinit(hmac_sha256_obj *ctx);
void hmac_csha256_update(hmac_sha256_obj *ctx, const unsigned char *message,
                         unsigned int message_len);
void hmac_csha256_final(hmac_sha256_obj *ctx, unsigned char *mac,
                        unsigned int mac_size);
void hmac_csha256(const unsigned char *key, unsigned int key_size,
                  const unsigned char *message, unsigned int message_len,
                  unsigned char *mac, unsigned mac_size);

#if SHA_64BITS_OP
void hmac_csha384_init(hmac_sha384_obj *ctx, const unsigned char *key,
                       unsigned int key_size);
void hmac_csha384_reinit(hmac_sha384_obj *ctx);
void hmac_csha384_update(hmac_sha384_obj *ctx, const unsigned char *message,
                         unsigned int message_len);
void hmac_csha384_final(hmac_sha384_obj *ctx, unsigned char *mac,
                        unsigned int mac_size);
void hmac_csha384(const unsigned char *key, unsigned int key_size,
                  const unsigned char *message, unsigned int message_len,
                  unsigned char *mac, unsigned mac_size);
    
void hmac_csha512_init(hmac_sha512_obj *ctx, const unsigned char *key,
                       unsigned int key_size);
void hmac_csha512_reinit(hmac_sha512_obj *ctx);
void hmac_csha512_update(hmac_sha512_obj *ctx, const unsigned char *message,
                         unsigned int message_len);
void hmac_csha512_final(hmac_sha512_obj *ctx, unsigned char *mac,
                        unsigned int mac_size);
void hmac_csha512(const unsigned char *key, unsigned int key_size,
                  const unsigned char *message, unsigned int message_len,
                  unsigned char *mac, unsigned mac_size);
#endif // SHA_64BITS_OP

#ifdef __cplusplus
}
#endif

#endif /* !HMAC_SHA2_H */
