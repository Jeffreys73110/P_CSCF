#ifndef __DATAHEXSTRING_H__
#define __DATAHEXSTRING_H__

#ifdef __cplusplus
extern "C"{
#endif

int mmData2HexString(const void* data, int dlen, char* out, int size);
int mmHexString2Data(const char* data, int dlen, void* out, int size);

#ifdef __cplusplus
}
#endif

#endif
