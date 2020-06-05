#ifndef _TRY_SLEEP_H_
#define _TRY_SLEEP_H_

#define DURATION_PER_TICKS	10 // m-second

typedef struct _TRY_SLEEP_STRUCT
{
	char			first;				// bool
	unsigned int	pre_ticks;			// pre. ticks
	unsigned int	duration_ticks;		// total ticks
	unsigned int	duration_per_ticks;	// m-second per ticks
}
TRY_SLEEP, *LPTRY_SLEEP;

#ifdef __cplusplus
extern "C" {
#endif

void			TrySleepInit(LPTRY_SLEEP obj, unsigned int perticks);
unsigned int	TrySleep(LPTRY_SLEEP obj, unsigned int Vticks, unsigned int msec);

#ifdef __cplusplus
}
#endif

#endif
