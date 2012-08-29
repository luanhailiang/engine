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

void * g_worker;

void
init_worker_bind(){
	int rc;
	void *context;
	char addr[30];
	config_t *cfg;

	cfg = get_config();
    context = s_context();
    g_worker = zmq_socket (context, ZMQ_ROUTER);
    sprintf(addr,"tcp://*:%d",(cfg->router_port+cfg->gate_id));
    rc = zmq_bind (g_worker, addr);
    assert(rc == 0);
    printf("Gate router listen on %s ready\n",addr);
}

void
send_message(char *work, char *id, char *msg){
	s_sendm(g_worker,work);
	s_sendm(g_worker,id);
	s_send(g_worker,msg);
}

char *
recv_message(){
    int64_t more;           //  Multipart detection
    size_t more_size = sizeof (more);
    zmq_getsockopt (g_worker, ZMQ_RCVMORE, &more, &more_size);
	return s_recv(g_worker);
}
