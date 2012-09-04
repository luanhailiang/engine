/*
 * message.c
 * swap message between client and worker
 *  Created on: Jul 31, 2012
 *      Author: luan
 */


#include "client.h"
#include "worker.h"
#include "master.h"
#include "../share/gdef.h"

static void
_handle_client_message(interactive_t *ip, char *cmd){
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
	send_message_worker("001", "123", cmd);
}

static void
_handle_worker_message(char *msg){
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
	//connect master
	init_master_connect();
	//start listen client
	init_client_bind();
	//start listen worker
	init_worker_router();
    /* start loop */
	printf("process start loop handle message\n");
    while(1){
        nfds = epoll_wait(g_epollfd, events, MAX_EVENTS, 1);
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
        		_handle_client_message(ip,msg);
        	}
        }
        while((msg = recv_message_worker()) != NULL){
        	_handle_worker_message(msg);
        	free(msg);
        }
    }
}
