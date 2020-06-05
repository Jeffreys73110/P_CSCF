#ifndef __SCTP_CLIENT_H__
#define __SCTP_CLIENT_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/sctp.h>
#include <arpa/inet.h>
#include <pthread.h>
// #include"s1ap_decode.h"
// #include"s1ap_encode.h"
// #include"mme.h"
#include <unistd.h>
#include <iostream>
// #include "s1ap.h"

// #include <sys/epoll.h>
// #include <fcntl.h>
// #include <errno.h>
// #include <iostream>
// #include <stdarg.h>

void sctp_readit(void *vfdp);
void sctp_connect(int &fd, struct sockaddr_in *addr);
int sctp_disconnect(int socket);
int sctp_send_data(int fd, iovec *iov, size_t iovlen);

#endif // __SCTP_CLIENT_H__