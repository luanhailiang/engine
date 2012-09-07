/*
 * worker.c
 * master handle message with worker
 *  Created on: Aug 28, 2012
 *      Author: luan
 */


#include "../share/gdef.h"
#include "../share/pzmq.h"
#include "../share/config.h"

static void *g_pub = NULL;
static void *g_router = NULL;

static char *g_last = NULL;

void *
get_work_router(){
	return g_router;
}

void
clear_last_work(){
	if(g_last != NULL){
		free(g_last);
		g_last = NULL;
	}
}

void
pub_message_work(const char *msg){
	s_send(g_pub,msg);
}

void
send_message_work(char *id, const char *msg){
	s_sendm(g_router,id);
	s_send(g_router,msg);
}

char *
recv_message_work(){
	if(g_last != NULL){
		free(g_last);
		g_last = NULL;
	}
	g_last = s_recv(g_router);
	return s_recv(g_router);
}

void
back_message_work(const char *msg){
	assert(g_last != NULL);
	s_sendm(g_router,g_last);
	s_send(g_router,msg);
}


void
init_work_router(){
	int rc;
	void *context;
	config_t *cfg;

	cfg = get_config();
    context = s_context();
    assert(cfg->master_work_router != NULL);
    g_router = zmq_socket (context, ZMQ_ROUTER);
    rc = zmq_bind (g_router, cfg->master_work_router);
    assert(rc == 0);
    printf("Master work reply bind on %s ready\n",cfg->master_work_router);
}

void
init_work_pub(){
	int rc;
	void *context;
	config_t *cfg;

	cfg = get_config();
    context = s_context();
    assert(cfg->master_work_pub != NULL);
    g_pub = zmq_socket (context, ZMQ_PUB);
    rc = zmq_bind (g_pub, cfg->master_work_pub);
    assert(rc == 0);
    printf("Master work publish bind on %s ready\n",cfg->master_work_pub);
}
