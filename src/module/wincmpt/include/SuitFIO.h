#ifndef __SUITFIO_H__
#define __SUITFIO_H__

// for size_t
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

///////////////////////////////////////////////////////////////////////////////////////
// FILE

// Position used as reference for the offset
extern int SFIO_SEEK_SET;
extern int SFIO_SEEK_CUR;
extern int SFIO_SEEK_END;

// The FILE IO function pointers
void*  sfio_open(const char* filename, const char* mode);
int    sfio_close(void* stream);
size_t sfio_read(void* buf, size_t size, size_t count, void* stream);
size_t sfio_write(const void* buf, size_t size, size_t count, void* stream);
int    sfio_flush(void* stream);
int    sfio_seek(void* stream, long offset, int origin);
long   sfio_tell(void* stream);
void   sfio_rewind(void* stream);
int    sfio_eof(void* stream);

// Setting the own implement FILE-IO function pointers
void sfio_set_origin(int vseek_set, int vseek_cur, int vseek_end);
void sfio_set_fopen(void* (*fopen_func)(const char*, const char*));
void sfio_set_fclose(int (*fclose_func)(void*));
void sfio_set_fread(size_t (*fread_func)(void*, size_t, size_t, void*));
void sfio_set_fwrite(size_t (*fwrite_func)(const void*, size_t, size_t, void*));
void sfio_set_fflush(int (*fflush_func)(void*));
void sfio_set_fseek(int (*fseek_func)(void*, long, int));
void sfio_set_ftell(long (*ftell_func)(void*));
void sfio_set_rewind(void (*rewind_func)(void*));
void sfio_set_feof(int (*feof_func)(void*));

///////////////////////////////////////////////////////////////////////////////////////
// DIR

#define SDT_UNKNOWN	0
#define SDT_FILE	1
#define SDT_LINK	2
#define SDT_DIR		3
typedef struct
{
	unsigned char d_type;
	const char* d_name;
}
sfio_info;

// The DIR IO function pointers
void* sfio_opendir(const char* path);
int sfio_readdir(void* dir, sfio_info* out);
int sfio_closedir(void* dir);

// Setting the own implement DIR IO function pointers
void sfio_set_opendir(void* (*opendir_func)(const char*));
void sfio_set_readdir(int (*readdir_func)(void*, sfio_info*));
void sfio_set_closedir(int (*closedir_func)(void*));

#ifdef __cplusplus
}
#endif

#endif
