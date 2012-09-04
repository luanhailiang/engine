/*
 * gate.c
 *
 *  Created on: Aug 29, 2012
 *      Author: luan
 */

#include "../share/gdef.h"
#include "../share/pzmq.h"
#include "../share/config.h"

static void g_rep = NULL;
static void g_pub = NULL;


void
init_gate_rep(){
	int rc;
	void *context;
	config_t *cfg;

	cfg = get_config();
    context = s_context();
    assert(cfg->master_gate_rep == NULL);
    g_rep = zmq_socket (context, ZMQ_REP);
    rc = zmq_bind (g_rep, cfg->master_gate_rep);
    assert(rc == 0);
    printf("Master gate publish bind on %s ready\n",cfg->master_gate_rep);
}

void
init_gate_pub(){
	int rc;
	void *context;
	config_t *cfg;

	cfg = get_config();
    context = s_context();
    assert(cfg->master_gate_pub == NULL);
    g_pub = zmq_socket (context, ZMQ_PUB);
    rc = zmq_bind (g_pub, cfg->master_gate_pub);
    assert(rc == 0);
    printf("Master gate publish bind on %s ready\n",cfg->master_gate_pub);
}

void
send_message_gate(char *id, char *msg){
	s_sendm(g_pub,id);
	s_send(g_pub,msg);
}

char *
recv_message_gate(){
	return s_recv(g_rep);
}
void
back_message_gate(char msg){
	s_send(g_rep,msg);
}
