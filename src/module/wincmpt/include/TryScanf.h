#ifndef _TRY_SCANF_H_
#define _TRY_SCANF_H_

#include "TrySleep.h"

#define	SCANF_MAX_LEN		100		// max string length
#define REFLASH_SHOW_TIME	20		// m-second

typedef struct _TRY_SCANF_STRUCT
{
	int			npos;
	char		buffer[SCANF_MAX_LEN+1];
	char		show; // bool
	TRY_SLEEP	trysleep;
}
TRY_SCANF, *LPTRY_SCANF;

#ifdef __cplusplus
extern "C" {
#endif

// only work for RTOS system
void	TryScanfInit(LPTRY_SCANF obj); // [defult] perticks := DURATION_PER_TICKS
void	TryScanfInit2(LPTRY_SCANF obj, unsigned int perticks);
char*	TryScanf(LPTRY_SCANF obj);

#ifdef __cplusplus
}
#endif

#endif
