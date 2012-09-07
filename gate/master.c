/*
 * master.c
 * gate handle message with master
 *  Created on: Aug 29, 2012
 *      Author: luan
 */



#include "../share/gdef.h"
#include "../share/pzmq.h"
#include "../share/config.h"

static void *g_sub = NULL;
static void *g_dealer = NULL;

void *
get_master_sub(){
	return g_sub;
}

void *
get_master_dealer(){
	return g_dealer;
}

void
send_message_master(char *msg){
	s_send(g_dealer,msg);
}

char *
recv_message_master(){
	return s_recv(g_sub);
}

char *
back_message_master(){
	return s_recv(g_dealer);
}


void
init_master_dealer(){
	int rc;
	char id[5];
	void *context;
	config_t *cfg;

	cfg = get_config();
    context = s_context();
    assert(cfg->gate_id != 0);
    sprintf(id,"%03d",cfg->gate_id);
    assert(cfg->master_gate_router != NULL);
    g_dealer = zmq_socket (context, ZMQ_DEALER);
    zmq_setsockopt (g_dealer, ZMQ_IDENTITY, id, strlen(id));
    rc = zmq_connect (g_dealer, cfg->master_gate_router);
    assert(rc == 0);
    printf("Worker master request connect on %s ready\n",cfg->master_gate_router);
}

void
init_master_sub(){
	int rc;
	void *context;
	config_t *cfg;

	cfg = get_config();
    context = s_context();
    assert(cfg->master_gate_pub != NULL);
    g_sub = zmq_socket (context, ZMQ_SUB);
    zmq_setsockopt (g_sub, ZMQ_SUBSCRIBE, "", 0);
    rc = zmq_connect (g_sub, cfg->master_gate_pub);
    assert(rc == 0);
    printf("Worker master subscribe connect on %s ready\n",cfg->master_gate_pub);
}

void
init_master_connect(){
	init_master_dealer();
	init_master_sub();
}


