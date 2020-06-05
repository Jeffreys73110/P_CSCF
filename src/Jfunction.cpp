#ifndef __JFUNCTION_CPP__
#define __JFUNCTION_CPP__

#include "Jfunction.h"

// PrintCalClockTime
double CalClockTime(bool start_flag)
{
	static clock_t start_ck, end_ck;

	if (!start_flag)
	{
		start_ck = clock();
		return 0;
	}
	else
	{
		end_ck = clock();
		return ((double)(end_ck - start_ck)) / CLOCKS_PER_SEC;
	}
}

// PrintBinary
#define PrintBinary_ENABLE true
int PrintBinary(const unsigned char *msg, int len)
{
#if PrintBinary_ENABLE
	for (int i = 0, j = 0, l = 0; i < len; i += 16)
	{
		printf("  ");
		for (j = 0, l = std::min(len - i, 8); j < l; j++)
			printf("%02x ", (unsigned char)msg[i + j]);
		if (l <= 0)
			continue;
		printf(" ");
		for (j = 0, l = std::min(len - i - 8, 8); j < l; j++)
			printf("%02x ", (unsigned char)msg[i + j + 8]);
		printf("\n");
	}
	printf("\n\n");
#endif
	return 0;
}

// ExtractIPAddr
char *ExtractIPAddr(int af, ip *iphdr)
{
	static char strAddr[128] = "";
	char src[INET_ADDRSTRLEN + 1], dst[INET_ADDRSTRLEN + 1];
	inet_ntop(af, &iphdr->ip_src, src, INET_ADDRSTRLEN);
	inet_ntop(af, &iphdr->ip_dst, dst, INET_ADDRSTRLEN);

	snprintf(strAddr, sizeof(strAddr), "%s --> %s", src, dst);
	return strAddr;
}

// Myprintf
int MyPrintf(const char *fmt, ...)
{
	char s[1024];
	va_list ap;

	va_start(ap, fmt);
	vsnprintf(s, sizeof(s), fmt, ap);
	va_end(ap);
	puts(s);

	return 0;
}

// TCP flag
char *ExtractTCPFlag(tcphdr *tcp)
{
	static char strflag[32] = "";

	if (!tcp)
		return strflag;

	snprintf(strflag, sizeof(strflag), "%s%s%s%s%s%s",
			 (tcp->fin) ? "FIN" : "",
			 (tcp->syn) ? ",SYN" : "",
			 (tcp->rst) ? ",RST" : "",
			 (tcp->psh) ? ",PSH" : "",
			 (tcp->ack) ? ",ACK" : "",
			 (tcp->urg) ? ",URG" : "");

	return strflag;
}

#endif