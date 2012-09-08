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

void *
get_gate_router(){
	return g_router;
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

msg_t *
recv_message_gate(){
	static msg_t msg = {NULL,NULL};
	if(msg.id != NULL){
		free(msg.id);
		msg.id=NULL;
	}
	if(msg.msg != NULL){
		free(msg.msg);
		msg.msg=NULL;
	}
	msg.id = s_recv(g_router);
	if(msg.id == NULL){
		return NULL;
	}
	msg.msg = s_recv(g_router);
	if(msg.msg == NULL){
		return NULL;
	}
	return &msg;
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


