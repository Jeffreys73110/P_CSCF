#ifndef WINCMPT_SRC_INCLUDE_SIMPLEBASE64_H_
#define WINCMPT_SRC_INCLUDE_SIMPLEBASE64_H_

#ifdef __cplusplus
extern "C" {
#endif

int ToBase64Simple(const unsigned char* pSrc, int nLenSrc, char* pDst, int nLenDst); // append zero byte at the end
int FromBase64Simple(const char* pSrc, int nLenSrc, char* pDst, int nLenDst);
int IsBase64Simple(const char* str);

#ifdef __cplusplus
}
#endif

#endif /* WINCMPT_SRC_INCLUDE_SIMPLEBASE64_H_ */
