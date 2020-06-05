#ifndef __UNIXTIMEDATE_H__
#define __UNIXTIMEDATE_H__

typedef struct _Utm
{
	int tm_sec;		// seconds [0,61]
	int tm_min;		// minutes [0,59]
	int tm_hour;	// hour [0,23]
	int tm_mday;	// day of month [1,31]
	int tm_mon;		// month of year [0,11]
	int tm_year;	// years since 1900
	int tm_wday;	// day of week [0,6] (Sunday = 0)
	int tm_yday;	// day of year [0,365]
	int tm_isdst;	// daylight savings flag
	long tm_gmtoff;         // Seconds east of UTC
	const char *tm_zone;	// Time zone abbreviation
}
Utm;

typedef enum
{
	UTM_GMT=0,
	UTM_UTC=1
}
UtmZoneName;

#ifdef __cplusplus
extern "C" {
#endif

void SetUtmZone(UtmZoneName name, int tz); // Only work for undefined OS
Utm	 *GetUtm(const long unixtime, Utm *tmbuf);
Utm	 *GetLocalUtm(const long unixtime, Utm *tmbuf);
char *GetUtmStr(const Utm *utm, char *tmbuf);
long MakeUnixTime(Utm *tmbuf);

const char *Utm_GetWeekFullName(int x);
const char *Utm_GetWeekShortName(int x);
const char *Utm_GetMonthFullName(int x);
const char *Utm_GetMonthShortName(int x);

#ifdef __cplusplus
}
#endif

#endif
