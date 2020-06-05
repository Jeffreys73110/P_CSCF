#ifndef __SRAMALLOC_H__
#define __SRAMALLOC_H__

#include <stddef.h>

#if defined(_MSC_VER)
#include <stdlib.h>
#elif defined(__MACH__)
#include <stdlib.h>
#elif defined(__FreeBSD__)
#include <stdlib.h>
#elif defined(__linux__)
#include <stdlib.h>
#elif defined(ANDROID)
#include <stdlib.h>
#else
#endif	// OS

#ifdef __cplusplus
extern "C" {
#endif

int sram_is_init(void); // return 0/1
int sram_poollink(void* ptr, size_t size);
int sram_partial_poollink(void* ptr[], size_t msize[], unsigned int num);

void* sram_malloc(size_t size);
void* sram_calloc(size_t num, size_t size);
void* sram_realloc(void* ptr, size_t size);
void  sram_free(void* ptr);

// for debug
void  sram_usage(unsigned int* freeNum, unsigned int* useNum, unsigned int* fatalNum);
#define TEST_SRAM_USAGE(f,d,str) \
if(1){ unsigned int x,y,z; const char *s=str; \
sram_usage(&x, &y, &z); \
if (s==0) f("[%d] SRAM usage: free=%d-Bytes; occupy=%d-Bytes; fatal=%d-Bytes;\r\n", d, x, y, z); \
else f("[%d][%s] SRAM usage: free=%d-Bytes; occupy=%d-Bytes; fatal=%d-Bytes;\r\n", d, s, x, y, z);}

#ifdef __cplusplus
}
#endif

#endif
