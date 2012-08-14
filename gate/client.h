/*
 * client.h
 *
 *  Created on: Jul 31, 2012
 *      Author: luan
 */

#ifndef CLIENT_H_
#define CLIENT_H_

#include <arpa/inet.h>
#include <sys/epoll.h>

#define USE_MEMCPY

#define S_LINKDEAD		0x01
#define S_LOGINING		0x02
#define S_WAITID		0x04
#define S_READY			0x08

#define MY_PORT			7878
#define LISTEN_NUM		128
#define MAX_EVENTS		256
#define MAX_BUFF		8192

//socket type
#define SK_TYPE_LISTEN	1
#define SK_TYPE_CLIENT	2

typedef struct interactive_s{
	int fd;								/* socket fd */
	int type;							/* socket link type */
	int flag;							/* socket link statue */
	char work[10];						/* work process */
	char wkid[10];						/* work process id */
	struct sockaddr_in addr;			/* client address */
	int recv_bgn;						/* receive message begin */
	int recv_end;						/* receive message end */
	int send_bgn;						/* send message begin */
	int send_end;						/* send message end */
	int send_len;						/* send message length */
	char recv_buf[MAX_BUFF];			/* receive message buff */
	char send_buf[MAX_BUFF];			/* send message buff */
} interactive_t;

extern int g_epollfd;

void init_client_bind();
interactive_t * process_io(struct epoll_event *ev);
char *first_cmd_in_buf(interactive_t *ip);
void add_message(interactive_t *ip, char *msg, int len);


#endif /* CLIENT_H_ */
