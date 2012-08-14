/*
 * worker.c
 * handle message with worker
 *  Created on: Jul 31, 2012
 *      Author: luan
 */
#include <zmq.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "../share/pzmq.h"

void * g_context;
void * g_client;

void
init_worker_bind(){
	int rc;
    g_context = zmq_init (1);
    g_client = zmq_socket (g_context, ZMQ_ROUTER);
    rc = zmq_bind (g_client, "ipc://routing.ipc");
    assert(rc == 0);
    printf("Worker listen on routing.ipc ready\n");
}

void
send_message(char *work, char *id, char *msg){
	s_sendm(g_client,work);
	s_sendm(g_client,id);
	s_send(g_client,msg);
}

char *
recv_message(){
    int64_t more;           //  Multipart detection
    size_t more_size = sizeof (more);
    zmq_getsockopt (g_client, ZMQ_RCVMORE, &more, &more_size);
    printf("--->%ld\n",(long)more);
	return s_recv(g_client);
}
