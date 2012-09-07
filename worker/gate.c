/*
 * gate.c
 * worker handle message with gate
 *  Created on: Aug 28, 2012
 *      Author: luan
 */


#include "../share/gdef.h"
#include "../share/pzmq.h"
#include "../share/config.h"

static void *g_push = NULL;
static void *g_dealer = NULL;

static void *g_last = NULL;

void *
get_gate_dealer(){
	return g_dealer;
}

void
send_message_gate(char *id,const char *msg){
	s_sendm(g_push,id);
	s_send(g_push,msg);
}

char *
recv_message_gate(){
	if(g_last != NULL){
		free(g_last);
		g_last=NULL;
	}
	g_last = s_recv(g_dealer);
	return s_recv(g_dealer);
}
void
back_message_gate(const char *msg){
	assert(g_last != NULL);
	s_sendm(g_push,g_last);
	s_send(g_push,msg);
}
void
init_gate_dealer(){
	int rc;
	char id[5];
	void *context;

	config_t *cfg;
	cfg = get_config();
	assert(cfg->work_id != 0);
	sprintf(id,"%03d",cfg->work_id);
	context = s_context();
	g_dealer = zmq_socket (context, ZMQ_DEALER);
    zmq_setsockopt (g_dealer, ZMQ_IDENTITY, id, strlen(id));
    rc = zmq_connect (g_dealer, cfg->gate_work_router);
    assert(rc == 0);
}
void
init_gate_push(){
	int rc;
	void *context;

	config_t *cfg;
	cfg = get_config();
	context = s_context();
	g_push = zmq_socket (context, ZMQ_PUSH);
    rc = zmq_connect (g_push, cfg->gate_work_pull);
    assert(rc == 0);
}
void
init_gate_connect(){
	init_gate_dealer();
	init_gate_push();
}
