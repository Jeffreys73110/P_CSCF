#ifndef __CRC32_H__
#define __CRC32_H__

typedef enum  
{
	// It can be used at the media type data like MPEG2
	// Note: The initial CRC32 value of MPEG2 is 0xffffffff
	MPEG_CMODE=0,
	// Secure Shell, it can be used at most situation like file data, network protocol... etc.
	SSH_CMODE=1,
	// Checksum
	SUM_CMODE=2
}  
CRC_CTYPE;

#ifdef __cplusplus
extern "C" {
#endif

unsigned int GetCrc32(CRC_CTYPE style, unsigned int crc, const void *buf, unsigned int len);

#ifdef __cplusplus
}
#endif

#endif
