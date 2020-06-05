#ifndef __STRING_PATCH_H__
#define __STRING_PATCH_H__

#include <stddef.h>

#if defined(_MSC_VER)
#include <string.h>
#elif defined(__MACH__)
#include <string.h>
#elif defined(__FreeBSD__)
#include <string.h>
#elif defined(__linux__)
#include <string.h>
#elif defined(ANDROID)
#include <string.h>
#else
#endif	// OS

#ifdef __cplusplus
extern "C" {
#endif

char*	patch_strtok(char *s, const char *delim);
int		patch_strcspn(char *string, const char *chars);
size_t	patch_strlen(const char *str);
char*	patch_strstr(const char *str1, const char *str2);
char*	patch_strchr(const char *str, int character);
char*	patch_strcat(char *dst, const char *src);
int		patch_strcmp(const char *str1, const char *str2);
char*	patch_strcpy(char *dst, const char *src);
int		patch_strncmp(const char *str1, const char *str2, size_t num);
char*	patch_strncpy(char *dst, const char *src, size_t num);
int		patch_strcasecmp(const char *s1, const char *s2);
int		patch_strncasecmp(const char *s1, const char *s2, size_t n);
//
int		patch_memcmp(const void *ptr1, const void *ptr2, size_t num);
void*	patch_memcpy(void *dst, const void *src, size_t num);
void*	patch_memset(void *ptr, int value, size_t num);
void*	patch_memmove(void *dst, const void *src, size_t num);

char*	patch_safe_strcat(char *dst, const char *src, unsigned int *dst_zise);

#ifdef __cplusplus
}
#endif

#endif
