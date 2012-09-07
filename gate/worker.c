/*
 * worker.c
 * gate handle message with worker
 *  Created on: Jul 31, 2012
 *      Author: luan
 */


#include "../share/gdef.h"
#include "../share/pzmq.h"
#include "../share/config.h"

static void *g_pull = NULL;
static void *g_router = NULL;

static char *g_fd = NULL;

void *
get_worker_pull(){
	return g_pull;
}

void
send_message_worker(char *work, char *id, char *msg){
	s_sendm(g_router,work);
	s_sendm(g_router,id);
	s_send(g_router,msg);
}

char *
recv_message_worker(){
	if(g_fd != NULL){
		free(g_fd);
		g_fd = NULL;
	}
	g_fd = s_recv(g_pull);
	return s_recv(g_pull);
}

void
init_worker_router(){
	int rc;
	void *context;
	config_t *cfg;

	cfg = get_config();
    context = s_context();
    g_router = zmq_socket (context, ZMQ_ROUTER);
    rc = zmq_bind (g_router, cfg->gate_work_router);
    assert(rc == 0);
    printf("Gate router listen on %s ready\n",cfg->gate_work_router);
}
void
init_worker_pull(){
	int rc;
	void *context;

	config_t *cfg;
	cfg = get_config();
	context = s_context();
	g_pull = zmq_socket (context, ZMQ_PULL);
    rc = zmq_bind (g_pull, cfg->gate_work_pull);
    assert(rc == 0);
}
