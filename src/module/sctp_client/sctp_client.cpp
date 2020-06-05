#include "sctp_client.h"

// #define uchar_t	unsinged char
#define MAX_STREAM 64

// global variables
pthread_t g_tid = 0;
int g_threadValid = 0;

#define BUFLEN 100
// static void set_sctp_event(struct sctp_event_subscribe* ses){
// 	ses->sctp_data_io_event=1;
// 	ses->sctp_shutdown_event=1;
// /*	ses->sctp_association_event=1;
// 	ses->sctp_address_event=1;
// 	ses->sctp_send_failure_event=1;
// 	ses->sctp_peer_error_event=1;
// 	ses->sctp_partial_delivery_event=1;
// 	ses->sctp_adaptation_layer_event=1;
// 	ses->sctp_authentication_event=1;
// */}

// Refer to https://docs.oracle.com/cd/E19253-01/817-4415/sockets-27/index.html
void sctp_readit(void *vfdp)
{
	int fd;
	ssize_t n;
	char buf[BUFLEN];
	struct msghdr msg[1];
	struct iovec iov[1];
	struct cmsghdr *cmsg;
	struct sctp_sndrcvinfo *sri;
	char cbuf[sizeof(*cmsg) + sizeof(*sri)];
	union sctp_notification *snp;

	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

	fd = *(int *)vfdp;

	/* Initialize the message header for receiving */
	memset(msg, 0, sizeof(*msg));
	msg->msg_control = cbuf;
	msg->msg_controllen = sizeof(*cmsg) + sizeof(*sri);
	msg->msg_flags = 0;
	cmsg = (struct cmsghdr *)cbuf;
	sri = (struct sctp_sndrcvinfo *)(cmsg + 1);
	iov->iov_base = buf;
	iov->iov_len = BUFLEN;
	msg->msg_iov = iov;
	msg->msg_iovlen = 1;


	while ((n = recvmsg(fd, msg, 0)) > 0)
	// if ((n = recvmsg(fd, msg, 0)) > 0)
	{
		/* Intercept notifications here */
		if (msg->msg_flags & MSG_NOTIFICATION)
		{
			snp = (union sctp_notification *)buf;
			printf("[ Receive notification type %u ]\n",
				   0); // snp->sn_type);
			// continue;
			// break;
		}
		msg->msg_control = cbuf;
		msg->msg_controllen = sizeof(*cmsg) + sizeof(*sri);
		printf("[ Receive (%ld bytes): stream = %hu, ssn = %hu, "
			   "flags = %hx, ppid = %d ]\n",
			   n,
			   sri->sinfo_stream, sri->sinfo_ssn, sri->sinfo_flags,
			   0); // sri->sinfo_ppid);
	}

	if (n < 0)
	{
		perror("recv");
		exit(1);
	}

	close(fd);
	exit(0);
}

void sctp_connect(int &fd, struct sockaddr_in *addr)
{
	// int     fd;
	uint8_t buf[BUFLEN];
	ssize_t n;
	int perr;
	struct cmsghdr *cmsg;
	struct sctp_sndrcvinfo *sri;
	char cbuf[sizeof(*cmsg) + sizeof(*sri)];
	struct msghdr msg[1];
	struct iovec iov[1];
	int ret;
	struct sctp_initmsg initmsg;
	struct sctp_event_subscribe events;

	/* Create a one-one SCTP socket */
	if ((fd = socket(AF_INET, SOCK_STREAM, IPPROTO_SCTP)) == -1)
	{
		perror("socket");
		// exit(1);
		return;
	}
	else
		printf("create client socket successfully\n");

	/*
     * We are interested in association change events and we want
     * to get sctp_sndrcvinfo in each receive.
     */
	events.sctp_association_event = 1;
	events.sctp_data_io_event = 1;
	ret = setsockopt(fd, IPPROTO_SCTP, SCTP_EVENTS, &events,
					 sizeof(events));
	if (ret < 0)
	{
		perror("setsockopt SCTP_EVENTS");
		// exit(1);
	}
	printf("%d of %s\n", __LINE__, __FILE__);

	/*
     * Set the SCTP stream parameters to tell the other side when
     * setting up the association.
     */
	memset(&initmsg, 0, sizeof(struct sctp_initmsg));
	initmsg.sinit_num_ostreams = MAX_STREAM;
	initmsg.sinit_max_instreams = MAX_STREAM;
	initmsg.sinit_max_attempts = MAX_STREAM;
	ret = setsockopt(fd, IPPROTO_SCTP, SCTP_INITMSG, &initmsg,
					 sizeof(struct sctp_initmsg));
	if (ret < 0)
	{
		perror("setsockopt SCTP_INITMSG");
		// exit(1);
		return;
	}

	/*
     * Set the SCTP stream parameters to tell the other side when
     * setting up the association.
     */
	memset(&initmsg, 0, sizeof(struct sctp_initmsg));
	initmsg.sinit_num_ostreams = MAX_STREAM;
	initmsg.sinit_max_instreams = MAX_STREAM;
	initmsg.sinit_max_attempts = MAX_STREAM;
	ret = setsockopt(fd, IPPROTO_SCTP, SCTP_INITMSG, &initmsg,
					 sizeof(struct sctp_initmsg));
	if (ret < 0)
	{
		perror("setsockopt SCTP_INITMSG");
		// exit(1);
		return;
	}

	printf("connect to %s:%d\n", inet_ntoa(addr->sin_addr), ntohs(addr->sin_port));
	if (connect(fd, (struct sockaddr *)addr, sizeof(*addr)) == -1)
	{
		perror("connect");
		// exit(1);
		return;
	}
	else
		printf("connected\n");

	// /* Initialize the message header structure for sending. */
	// memset(msg, 0, sizeof (*msg));
	// iov->iov_base = buf;
	// msg->msg_iov = iov;
	// msg->msg_iovlen = 1;
	// msg->msg_control = cbuf;
	// msg->msg_controllen = sizeof (*cmsg) + sizeof (*sri);
	// // msg->msg_flags |= MSG_XPG4_2;

	// memset(cbuf, 0, sizeof (*cmsg) + sizeof (*sri));
	// cmsg = (struct cmsghdr *)cbuf;
	// sri = (struct sctp_sndrcvinfo *)(cmsg + 1);

	// cmsg->cmsg_len = sizeof (*cmsg) + sizeof (*sri);
	// cmsg->cmsg_level = IPPROTO_SCTP;
	// cmsg->cmsg_type  = SCTP_SNDRCV;

	// sri->sinfo_ppid   = 1;
	// /* Start sending to stream 0. */
	// sri->sinfo_stream = 0;

	/* Create a thread to receive network traffic. */
	g_threadValid = pthread_create(&g_tid, NULL, (void *(*)(void *))sctp_readit, &fd);

	if (g_threadValid != 0)
	{
		fprintf(stderr, "pthread_create: %d\n", g_threadValid);
		// exit(1);
		return;
	}

	// printf("%d of %s\n",__LINE__,__FILE__);
	// /* Read from stdin and then send to the echo server. */
	// while ((n = read(fileno(stdin), buf, BUFLEN)) > 0) {
	// 	printf("%d of %s\n",__LINE__,__FILE__);
	//     iov->iov_len = n;
	//     if (sendmsg(fd, msg, 0) < 0) {
	//         perror("sendmsg");
	//         exit(1);
	//     }
	//     /* Send the next message to a different stream. */
	//     sri->sinfo_stream = (sri->sinfo_stream + 1) % MAX_STREAM;
	// 	printf("%d of %s\n",__LINE__,__FILE__);
	// }

	printf("%d of %s\n", __LINE__, __FILE__);
}

int sctp_disconnect(int socket)
{
	if (g_threadValid)
		pthread_cancel(g_tid);
	if (socket >= 0)
		close(socket);

	return 0;
}

int sctp_send_data(int fd, iovec *iov, size_t iovlen)
{
	uint8_t buf[1024];
	struct cmsghdr *cmsg;
	struct sctp_sndrcvinfo *sri;
	char cbuf[sizeof(*cmsg) + sizeof(*sri)];
	struct msghdr msg[1];
	// struct iovec   iov[1];
	struct sctp_initmsg initmsg;
	struct sctp_event_subscribe events;
	int ret;

	/* Initialize the message header structure for sending. */
	memset(msg, 0, sizeof(*msg));
	// iov->iov_base = buf;
	msg->msg_iov = iov;
	msg->msg_iovlen = iovlen;
	msg->msg_control = cbuf;
	msg->msg_controllen = sizeof(*cmsg) + sizeof(*sri);
	// msg->msg_flags |= MSG_XPG4_2;

	memset(cbuf, 0, sizeof(*cmsg) + sizeof(*sri));
	cmsg = (struct cmsghdr *)cbuf;
	sri = (struct sctp_sndrcvinfo *)(cmsg + 1);

	cmsg->cmsg_len = sizeof(*cmsg) + sizeof(*sri);
	cmsg->cmsg_level = IPPROTO_SCTP;
	cmsg->cmsg_type = SCTP_SNDRCV;

	sri->sinfo_ppid = 1;
	/* Start sending to stream 0. */
	sri->sinfo_stream = 0;

	//--- send message
	printf("%d of %s\n", __LINE__, __FILE__);
	// iov->iov_len = n;
	if (sendmsg(fd, msg, 0) < 0)
	{
		perror("sendmsg");
		return -1;
	}
	/* Send the next message to a different stream. */
	sri->sinfo_stream = (sri->sinfo_stream + 1) % MAX_STREAM;
	printf("%d of %s\n", __LINE__, __FILE__);

	// /* Read from stdin and then send to the echo server. */
	// while ((n = read(fileno(stdin), buf, BUFLEN)) > 0) {
	// 	printf("%d of %s\n",__LINE__,__FILE__);

	// }

	return 0;
}

//--- example
// int main(int argc, char** argv)
// {
// 	//--- Create a client connection to server
// 	int	fd=0;
// 	struct sockaddr_in addr[1];

// 	char data[] = "sctp test";
// 	iovec	iov = {data, strlen(data)};
// 	addr->sin_family = AF_INET;
// 	addr->sin_addr.s_addr = inet_addr("10.102.81.100");
//     addr->sin_port = htons(36412);

// 	printf("ready to connect to %s:%d\n", inet_ntoa(addr->sin_addr), ntohs(addr->sin_port));
// 	sctp_connect(fd, addr);

// 	send_data(sip_cqi1_fd, &iov, 1);
// 	printf("send data = %s\n", data);

// 	sleep(3);
// 	disconnect(fd);
// 	printf("close\n");
// 	return 0;
// }