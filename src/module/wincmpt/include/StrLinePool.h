#ifndef BUILD__WIN_COMPILER_STRLINE_H_
#define BUILD__WIN_COMPILER_STRLINE_H_

typedef struct
{
	char *spool, *S, *E, *R;
	unsigned int msize;
	unsigned char missing;
	unsigned char linking;
}
StrLinePool;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// API

#ifdef __cplusplus
extern "C" {
#endif

int slp_malloc(StrLinePool* obj, unsigned int lines, unsigned int slen);
int slp_melink(StrLinePool* obj, char* mem, unsigned int msize);
int slp_mefree(StrLinePool* obj);

void slp_putstr(StrLinePool* obj, const char* str);
int slp_readstr(StrLinePool* obj, char* out, unsigned int rlen, unsigned char* missing);

const char* slp_getstr_p1(StrLinePool* obj);
const char* slp_getstr_p2(StrLinePool* obj);

#ifdef __cplusplus
}
#endif

#endif
