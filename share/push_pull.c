/*
 * push.c
 *
 *  Created on: Aug 28, 2012
 *      Author: luan
 */

#include <zmq.h>
#include "pzmq.h"

void *
zmq_push_init(void *context, char *addr){
	int rc;
    void *push;
    push = zmq_socket (context, ZMQ_PUSH);
    rc = zmq_connect (push, addr);
	assert(rc == 0);
	printf("zmq_push:%s is ready\n",addr);
    return push;
}

int
zmq_push_msg(void *push, char *msg){
	return s_send (push, msg);
}


void *
zmq_pull_init(void *context, char *addr){
	int rc;
	void *pull;
	pull = zmq_socket (context, ZMQ_PULL);
	rc = zmq_bind (pull, addr);
	assert(rc == 0);
	printf("zmq_pull:%s is ready\n",addr);
	return pull;
}

char *
zmq_pull_msg(void *pull){
	return s_recv(pull);
}
