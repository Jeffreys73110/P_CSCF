/*****************************************************************************
Compiler for different network module

written by
   Bing-Jhih Yao, last updated 12/23/2014
*****************************************************************************/

#ifndef __NETCMOD_H__
#define __NETCMOD_H__

// Module code define
#define	CWINIOCP		0		// iocp class
#define	CKQUEUE			1		// kqueue class
#define	CSELECT			2		// select class
#define	CEPOLL			3		// epoll class
#define	CPOLL			4		// poll class
#define	CRTOS			5		// RTOS system

#define GET_NET_MODULE_STR(v) \
(v==CWINIOCP)?"CWINIOCP":\
(v==CKQUEUE)?"CKQUEUE":\
(v==CSELECT)?"CSELECT":\
(v==CEPOLL)?"CEPOLL":\
(v==CPOLL)?"CPOLL":\
(v==CRTOS)?"CRTOS":\
"NULL"

// config file for Compiler
#include "../config/CONFIG_NETCPI.H"

#endif	// __NETCMOD_H__
