/*
 * gate.c
 *
 *  Created on: Aug 29, 2012
 *      Author: luan
 */

#include "../share/gdef.h"
#include "../share/pzmq.h"
#include "../share/config.h"

static void *g_pub = NULL;
static void *g_router = NULL;

static char *g_last = NULL;

void *
get_gate_router(){
	return g_router;
}
void
clear_last_gate(){
	if(g_last != NULL){
		free(g_last);
		g_last=NULL;
	}
}

void
pub_message_gate(const char *msg){
	s_send(g_pub,msg);
}

void
send_message_gate(char *id, const char *msg){
	s_sendm(g_router,id);
	s_send(g_router,msg);
}

char *
recv_message_gate(){
	if(g_last != NULL){
		free(g_last);
		g_last=NULL;
	}
	g_last = s_recv(g_router);
	return s_recv(g_router);
}

void
back_message_gate(const char *msg){
	assert(g_last != NULL);
	s_sendm(g_router,g_last);
	s_send(g_router,msg);
}

void
init_gate_router(){
	int rc;
	void *context;
	config_t *cfg;

	cfg = get_config();
    context = s_context();
    assert(cfg->master_gate_router != NULL);
    g_router = zmq_socket (context, ZMQ_ROUTER);
    rc = zmq_bind (g_router, cfg->master_gate_router);
    assert(rc == 0);
    printf("Master gate reply bind on %s ready\n",cfg->master_gate_router);
}

void
init_gate_pub(){
	int rc;
	void *context;
	config_t *cfg;

	cfg = get_config();
    context = s_context();
    assert(cfg->master_gate_pub != NULL);
    g_pub = zmq_socket (context, ZMQ_PUB);
    rc = zmq_bind (g_pub, cfg->master_gate_pub);
    assert(rc == 0);
    printf("Master gate publish bind on %s ready\n",cfg->master_gate_pub);
}


