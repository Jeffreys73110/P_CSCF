#ifndef __JFUNCTION_H__
#define __JFUNCTION_H__

#include <time.h>		// clock
#include <stdio.h>		// printf
#include <stdarg.h>		// vsnprintf
#include <netinet/ip.h> // ip
#include <arpa/inet.h>	// INET_ADDRSTRLEN
#include <algorithm>	// std::min
// #include <sys/socket.h>
// #include <sys/select.h>
// #include <netinet/ip.h>
#include <netinet/tcp.h>
// #include <netinet/udp.h>
// #include <netinet/in.h>
// #include <netinet/ether.h>

// PrintProbe
#define PrintProb_ENABLE true
#define PrintProb()       \
	if (PrintProb_ENABLE) \
		printf("%d of %s\n", __LINE__, __FILE__);

#define PrintCalClockTime_ENABLE false
#if PrintCalClockTime_ENABLE
#define printf_ClockTime(tag_string, start_flag) printf("%s -- tick count in sec: %f (%d %s)\n", tag_string, CalClockTime(start_flag), __LINE__, __FILE__)
#else
#define printf_ClockTime(tag_string, start_flag) \
	{                                            \
	}
#endif

double CalClockTime(bool start_flag = false);
int PrintBinary(const unsigned char *msg, int len);
char *ExtractIPAddr(int af, ip *iphdr);
int MyPrintf(const char *fmt, ...);
char *ExtractTCPFlag(tcphdr *tcp);

#endif