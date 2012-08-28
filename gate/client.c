/*
 * client.c
 * handle message from client
 *  Created on: Jul 31, 2012
 *      Author: luan
 */

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <signal.h>

#include "client.h"
#include "../share/config.h"

int g_epollfd;
interactive_t g_sockets[MAX_LINK] = {0};

/**
 * clear break socket
 */
static void
remove_interactive(interactive_t *ip){
	struct epoll_event ev;
	memset(&ev,0,sizeof(ev));
	if(epoll_ctl(g_epollfd, EPOLL_CTL_DEL, ip->fd , &ev)==-1){
		perror("epoll_ctl:del");
	}
	if(close(ip->fd) == -1){
		perror("socket close");
	}
	ip->flag = S_LINKDEAD;
}
/**
 * flush message in send buff
 */
static int
flush_message(interactive_t *ip){
	int bytes;
	int length;
	int num = 0;
	if(ip->flag & S_LINKDEAD){
		fprintf(stderr,"flush_message:Try flush a closed socket\n");
		return 0;
	}
	while(ip->send_len != 0){
		if (ip->send_bgn < ip->send_end) {
		    length = ip->send_end - ip->send_bgn;
		} else {
		    length = MAX_BUFF - ip->send_bgn;
		}
		bytes = send(ip->fd,ip->send_buf+ip->send_bgn,length,0);
		if(!bytes){
			remove_interactive(ip);
			return 0;
		}
		if(bytes == -1){
			perror("flush_message:send");
			/* Try again */
			if(EAGAIN == errno){
				return 1;
			}
			remove_interactive(ip);
			return 0;
		}
		ip->send_bgn = (ip->send_bgn + bytes) % MAX_BUFF;
		ip->send_len -= bytes;
	}
	return 1;
}

/**
 * receive message from socket
 */
static void
recv_message(interactive_t *ip){
	int bytes;
	int space;
	space = MAX_BUFF - ip->recv_end;
	/* check if we need more space */
	if(space < MAX_BUFF / 16){
		if (ip->recv_bgn > 0) {
		    memmove(ip->recv_buf, ip->recv_buf + ip->recv_bgn,
		    		ip->recv_end - ip->recv_bgn);
		    space += ip->recv_bgn;
		    ip->recv_end -= ip->recv_bgn;
		    ip->recv_bgn = 0;
		}
		 /* the user is sending too much data.  flush it */
		if (space < MAX_BUFF / 16) {
			fprintf(stderr,"%d receive too much data clear it: %d byte\n",
					ip->fd,ip->recv_end-ip->recv_bgn);
		    ip->recv_bgn = ip->recv_end = 0;
		    space = MAX_BUFF;
		}
	}
	bytes = recv(ip->fd,ip->recv_buf+ip->recv_end,space,0);
	/* tried to read from closing fd */
	if(!bytes){
		remove_interactive(ip);
		return;
	}
	/* error read from fd */
	if(bytes == -1){
		perror("recv_message:recv");
		/* Try again */
		if(EAGAIN == errno){
			return;
		}
		remove_interactive(ip);
		return;
	}
	ip->recv_end += bytes;
}
/**
 * clean receive buff illegal char
 */
static int
clean_buf(interactive_t *ip){
    /* skip null input */
    while (ip->recv_bgn < ip->recv_end && !*(ip->recv_buf + ip->recv_bgn)){
    	ip->recv_bgn++;
    }
    /* if we've advanced beyond the end of the buffer, reset it */
    if (ip->recv_bgn >= ip->recv_end) {
    	ip->recv_bgn = ip->recv_end = 0;
    }
    return (ip->recv_end > ip->recv_bgn);
}

/**
 * socket set nonblocking
 */
static int
set_nonblocking(int fd){
	int opts;

	opts=fcntl(fd,F_GETFL);
	if(opts<0){
		perror("set nonblocking getfl");
		return -1;
	}

	opts = opts|O_NONBLOCK;
	if(fcntl(fd,F_SETFL,opts)<0){
		perror("set nonblocking setfl");
		return -1;
	}
	return 1;
}
/**
 * socket break check
 */
static int
set_keepalive (int fd){
	int alive, idle, intv, cnt;

	alive = 1;
	if (setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, &alive,sizeof alive) != 0){
		perror ("set keepalive");
		return -1;
	}

	idle = 10;
	if (setsockopt (fd, SOL_TCP, TCP_KEEPIDLE, &idle, sizeof idle) != 0){
		perror("set keepalive idle");
		return -1;
	}

	intv = 5;
	if (setsockopt (fd, SOL_TCP, TCP_KEEPINTVL, &intv, sizeof intv) != 0){
		perror("set keepalive intv");
		return -1;
	}

	cnt = 3;
	if (setsockopt (fd, SOL_TCP, TCP_KEEPCNT, &cnt, sizeof cnt) != 0){
		perror("set keepalive cnt");
		return -1;
	}
	return 0;
}
/**
 * let bind port useful when restart
 */
static int
set_reuseaddr(int fd){
	int optval = 1;
    if(setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1){
    	perror("set socket reuseaddr");
    	return -1;
    }
    return 1;
}

static void
accept_client(int listen){
	int addrlen;
	int conn_sock;
	struct epoll_event ev;
	struct sockaddr_in local;
	addrlen = sizeof(local);
    conn_sock = accept(listen,(struct sockaddr *) &local, &addrlen);
    if (conn_sock == -1) {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    if(conn_sock >= MAX_EVENTS){
    	fprintf(stderr,"fds amount more than %d\n",MAX_EVENTS);
    	close(conn_sock);
    	return;
    }
    memset(&g_sockets[conn_sock],0,sizeof(interactive_t));
    g_sockets[conn_sock].fd = conn_sock;
    g_sockets[conn_sock].type = SK_TYPE_CLIENT;
    g_sockets[conn_sock].flag = S_LOGINING;
    g_sockets[conn_sock].addr = local;
    set_nonblocking(conn_sock);
    set_keepalive(conn_sock);
    ev.events = EPOLLIN;
    ev.data.fd = conn_sock;
    if (epoll_ctl(g_epollfd, EPOLL_CTL_ADD, conn_sock, &ev) == -1) {
        perror("epoll_ctl: conn_sock");
        exit(EXIT_FAILURE);
    }
}
/**
 * handle all message
 */
static void
process_client(int fd){
	interactive_t *ip;
	ip = &g_sockets[fd];
	/* Maybe closed by send error */
	if(ip->flag & S_LINKDEAD){
		return;
	}
	recv_message(ip);
}
/**
 * init listen socket
 */
void
init_client_bind(){
	int myport;
	int listen_sock;
	struct epoll_event ev;
	struct sockaddr_in my_addr;
	/* set listen port */
	config_t *cfg = get_config();
   	myport = cfg->gate_port;
	/* set pipe signal */
    if(signal(SIGPIPE, SIG_IGN) == SIG_ERR){
    	perror("signal");
		exit(EXIT_FAILURE);
    }
	/* create socket */
	listen_sock = socket(PF_INET, SOCK_STREAM, 0);
	if(listen_sock == -1){
		perror("create socket");
		exit(EXIT_FAILURE);
	}
	/* set socket nonblocking */
	if(set_nonblocking(listen_sock) == -1){
		exit(EXIT_FAILURE);
	}
	/* set listen port reuse */
	if(set_reuseaddr(listen_sock) == -1){
		exit(EXIT_FAILURE);
	}
	/* bind socket on port */
	memset(&my_addr,0,sizeof(my_addr));
    my_addr.sin_family = PF_INET;
    my_addr.sin_port = htons(myport);
    my_addr.sin_addr.s_addr = INADDR_ANY;
    if(bind(listen_sock, (struct sockaddr *)&my_addr, sizeof(my_addr)) == -1){
        perror("bind socket");
        exit(EXIT_FAILURE);
    }
    /* start listen socket */
    if(listen(listen_sock, LISTEN_NUM) == -1){
        perror("listen socket");
        exit(EXIT_FAILURE);
    }
    /* create epoll */
    g_epollfd = epoll_create(MAX_EVENTS);
    if(g_epollfd == -1){
    	perror("epoll_create");
    	exit(EXIT_FAILURE);
    }
    /* add listen_sock to epoll control */
    ev.events = EPOLLIN;
    ev.data.fd = listen_sock;
    if (epoll_ctl(g_epollfd, EPOLL_CTL_ADD, listen_sock, &ev) == -1) {
        perror("epoll_ctl: listen_sock");
        exit(EXIT_FAILURE);
    }
    g_sockets[listen_sock].fd = listen_sock;
    g_sockets[listen_sock].type = SK_TYPE_LISTEN;
    printf("Client listen on %d ready\n",myport);
}

interactive_t *
process_io(struct epoll_event *ev){
	interactive_t *ip;
	ip = &g_sockets[ev->data.fd];
	switch(ip->type){
		case SK_TYPE_LISTEN:
			accept_client(ev->data.fd);
			return NULL;
		case SK_TYPE_CLIENT:
			process_client(ev->data.fd);
			return ip;
		default:
			//TODO log
			break;
	}
	return NULL;
}

/**
 * add message to client send buff
 */
void
add_message(interactive_t *ip, char *msg, int len){
	int i;
	if(ip->send_len+len > MAX_BUFF){
		if(!flush_message(ip)){
			fprintf(stderr,"add_message:Broken connection\n");
			return;
		}
		if(ip->send_len+len > MAX_BUFF){
			fprintf(stderr,"add_mesage:flush_message fail "
					"lose this message %d bytes\n",len);
			return;
		}
	}
#ifdef USE_MEMCPY
	if(ip->send_end+len > MAX_BUFF){
		memcpy(ip->send_buf+ip->send_end,msg,MAX_BUFF-ip->send_end);
		memcpy(ip->send_buf,msg+MAX_BUFF-ip->send_end,ip->send_end+len-MAX_BUFF);
	}else{
		memcpy(ip->send_buf+ip->send_end,msg,len);
	}
	ip->send_end = (ip->send_end + len) % MAX_BUFF;
	ip->send_len += len;
#else
	for(i = 0; i < len; i++){
		ip->send_buf[ip->send_end] = *(msg+i);
		ip->send_end = (ip->send_end + 1) % MAX_BUFF;
		ip->send_len++;
	}
#endif
	if(ip->send_len > MAX_BUFF/2){
		flush_message(ip);
	}
}
/**
 * get the first message from buff
 */
char *
first_cmd_in_buf(interactive_t *ip){
    char *p;
    char *temp;

    /* do standard input buffer cleanup */
    if (!clean_buf(ip)){
    	return 0;
    }
    /* search for the newline */
    for(p = ip->recv_buf+ip->recv_bgn; p<(ip->recv_buf+ip->recv_end); p++){
    	if(*p == '\n'){
    		break;
    	}
    }
    if(*p != '\n'){
    	return 0;
    }
    *p = '\0';
    temp = p+1;
    p = ip->recv_buf+ip->recv_bgn;
    ip->recv_bgn = temp - ip->recv_buf;
    return p;
}
