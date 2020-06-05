#ifndef __CINIR_H__
#define __CINIR_H__

// copyright: https://github.com/benhoyt/inih/

// stop parser return value
#define STOP_INI_PARSER	100

// custom string-based I/O fgets()
typedef char* (*ini_reader)(char* str, int num, void* stream);

// INI file parser callback
typedef int (*ini_handler)(void* user, const char* section,
                           const char* name, const char* value,
                           int lineno);

#ifdef __cplusplus
extern "C" {
#endif

int ini_parse_stream(ini_reader reader, void* stream, ini_handler handler, void* user);
int ini_parse_string(const char* string, ini_handler handler, void* user);
#define ini_parse_stop() return STOP_INI_PARSER;

#ifdef __cplusplus
}
#endif

#endif
