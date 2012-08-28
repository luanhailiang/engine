/*
 * message.c
 * swap message between client and worker
 *  Created on: Jul 31, 2012
 *      Author: luan
 */

#include <zmq.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include "client.h"
#include "worker.h"

static void
handle_client_message(interactive_t *ip, char *cmd){
	//TODO handle message
	switch(ip->flag){
		case S_LINKDEAD:
			break;
		case S_LOGINING:
			break;
		case S_WAITID:
			break;
		case S_READY:
			break;
	}
	send_message("A", "123", cmd);
}

static void
handle_worker_message(char *msg){
	//TODO handle message
	printf("-->%s\n",msg);
}

void
start_loop(){
	int n;
	int nfds;
	char *msg;
	interactive_t *ip;
	struct epoll_event events[MAX_EVENTS];
    /* start loop */
	printf("process start loop handle message\n");
    while(1){
        nfds = epoll_wait(g_epollfd, events, MAX_EVENTS, 1000);
        if (nfds == -1) {
        	if(errno == EINTR){
        		continue;
        	}
            perror("epoll_pwait");
            exit(EXIT_FAILURE);
        }
        for(n = 0; n < nfds; ++n) {
        	ip = process_io(&events[n]);
        	if(ip == NULL){
        		continue;
        	}
        	while((msg = first_cmd_in_buf(ip))){
        		handle_client_message(ip,msg);
        	}
        }
        while((msg = recv_message()) != NULL){
        	handle_worker_message(msg);
        	free(msg);
        }
    }
}
