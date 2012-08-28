/*
 * worker.c
 * gate handle message with worker
 *  Created on: Jul 31, 2012
 *      Author: luan
 */
#include <zmq.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "../share/pzmq.h"
#include "../share/config.h"

void * g_context;
void * g_client;

void
init_worker_bind(){
	int rc;
	config_t *cfg;
	cfg = get_config();
    g_context = zmq_init (1);
    g_client = zmq_socket (g_context, ZMQ_ROUTER);
    rc = zmq_bind (g_client, cfg->gate_router);
    assert(rc == 0);
    printf("Worker listen on %s ready\n",cfg->gate_router);
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
	return s_recv(g_client);
}
