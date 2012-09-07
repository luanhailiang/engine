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

int client=0;
int master=0;

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
		default:
			break;
	}
	client++;
	send_message_master(cmd);
}

static void
_handle_worker_message(char *msg){
	//TODO handle message
	printf("Gate from worker : %s\n",msg);
}

static void
_handle_master_message(char *msg){
	//TODO handle message
	master++;
	if(!(master%1000)){
		printf(" %10d %10d %10d\n",client-master,client,master);
	}
//	printf("Gate from master : %s\n",msg);
}

void
start_loop(){
	int n;
	int nfds;
	char *msg;

	void *master_sub;
	void *worker_pull;
	void *master_dealer;

	interactive_t *ip;
	struct epoll_event events[MAX_EVENTS];

	//connect master
	init_master_connect();
	//start listen client
	init_client_bind();
	//start listen worker
	init_worker_pull();
	init_worker_router();

	master_sub = get_master_sub();
	worker_pull = get_worker_pull();
	master_dealer = get_master_dealer();

	// Initialize poll set
	zmq_pollitem_t items [] = {
		{ master_sub, 0, ZMQ_POLLIN, 0 },
		{ worker_pull, 0, ZMQ_POLLIN, 0 },
		{ master_dealer, 0, ZMQ_POLLIN, 0 }
	};
    /* start loop */
	printf("process start loop handle message\n");
    while(1){
		zmq_poll (items, 3, 1);
		if (items [0].revents & ZMQ_POLLIN) {
	        while((msg = recv_message_master()) != NULL){
	        	_handle_master_message(msg);
	        	free(msg);
	        }
		}
		if (items [1].revents & ZMQ_POLLIN) {
	        while((msg = recv_message_worker()) != NULL){
	        	_handle_worker_message(msg);
	        	free(msg);
	        }
		}
		if (items [2].revents & ZMQ_POLLIN) {
	        while((msg = back_message_master()) != NULL){
	        	_handle_master_message(msg);
	        	free(msg);
	        }
		}
        nfds = epoll_wait(g_epollfd, events, MAX_EVENTS, 0);
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
    }
}
