#ifndef WINCMPT_SRC_INCLUDE_MURMURHASH_H_
#define WINCMPT_SRC_INCLUDE_MURMURHASH_H_

#ifdef __cplusplus
extern "C" {
#endif

void MurmurHash3_x86_32 (const void *key, const int len, const unsigned int seed, void *out); // out: uint32_t
void MurmurHash3_x86_128(const void *key, const int len, const unsigned int seed, void *out); // out: uint32_t[4]
void MurmurHash3_x64_128(const void *key, const int len, const unsigned int seed, void *out); // out: uint64_t[2]

#ifdef __cplusplus
}
#endif

#endif // WINCMPT_SRC_INCLUDE_MURMURHASH_H_
