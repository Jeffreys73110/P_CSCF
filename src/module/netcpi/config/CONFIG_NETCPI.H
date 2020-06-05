/*****************************************************************************
 netcpi-src/NetCMod.h Configuration (set of defines)
 ====================================================
 To setting compiler for different network module
*****************************************************************************/

#ifndef __CONFIG_NETCPI_H__
#define __CONFIG_NETCPI_H__

// -------------------------------------------------------------------------

// lib user define
#define MAX_LIB_USER		0x7F			// 0x7
#define BIT_OFFSET_USER		24				// 28
#define MASK_LIB_USER		0x00FFFFFF		// 0x0FFFFFFF
#define NULL_MIXSOCKET		-1

// -------------------------------------------------------------------------

// avoid LiteOS define __linux__
#ifdef __LITEOS__
#undef __linux__
#endif

// Compiler for Windows
#if defined(_MSC_VER)
#define	KQUEUE_MODULE	0		// kqueue module
#define	SELECT_MODULE	1		// select module
#define	EPOLL_MODULE	0		// epoll module
#define	POLL_MODULE		0		// poll module
#define	IOCP_MODULE		1		// iocp module
#define	RTOS_MODULE		0		// RTOS module

// Compiler for OS X
#elif defined(__MACH__)
#define	KQUEUE_MODULE	1		// kqueue module
#define	SELECT_MODULE	1		// select module
#define	EPOLL_MODULE	0		// epoll module
#define	POLL_MODULE		1		// poll module
#define	IOCP_MODULE		0		// iocp module
#define	RTOS_MODULE		0		// RTOS module

// Compiler for Free BSD
#elif defined(__FreeBSD__)
#define	KQUEUE_MODULE	1		// kqueue module
#define	SELECT_MODULE	1		// select module
#define	EPOLL_MODULE	0		// epoll module
#define	POLL_MODULE		1		// poll module
#define	IOCP_MODULE		0		// iocp module
#define	RTOS_MODULE		0		// RTOS module

// Compiler for Android
#elif defined(__ANDROID__)
#define	KQUEUE_MODULE	0		// kqueue module
#define	SELECT_MODULE	1		// select module
#define	EPOLL_MODULE	1		// epoll module
#define	POLL_MODULE		1		// poll module
#define	IOCP_MODULE		0		// iocp module
#define	RTOS_MODULE		0		// RTOS module

// Compiler for Linux
#elif defined(__linux__)
#define	KQUEUE_MODULE	0		// kqueue module
#define	SELECT_MODULE	1		// select module
#define	EPOLL_MODULE	1		// epoll module
#define	POLL_MODULE		1		// poll module
#define	IOCP_MODULE		0		// iocp module
#define	RTOS_MODULE		0		// RTOS module

// Compiler for other OS
#else
#define	KQUEUE_MODULE	0		// kqueue module
#define	SELECT_MODULE	0		// select module
#define	EPOLL_MODULE	0		// epoll module
#define	POLL_MODULE		0		// poll module
#define	IOCP_MODULE		0		// iocp module
#define	RTOS_MODULE		1		// RTOS module

#endif	// OS

// -------------------------------------------------------------------------

// RTOS SUPPORT_MULTI_LIB_USER configure
#if RTOS_MODULE==1
#if	defined(RTOS_OV798) || \
    defined(RTOS_XXXXX)
#define SUPPORT_MULTI_LIB_USER
#endif
#endif

#endif
