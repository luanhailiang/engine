/*
 * master.c
 * gate handle message with master
 *  Created on: Aug 29, 2012
 *      Author: luan
 */



#include "../share/gdef.h"
#include "../share/pzmq.h"
#include "../share/config.h"

static void *g_req = NULL;
static void *g_sub = NULL;

void
send_message_master(char *msg){
	s_send(g_req,msg);
}

char *
recv_message_master(){
	return s_recv(g_sub);
}

char *
back_message_master(){
	return s_recv(g_req);
}

char *
wait_message_master(){
	return s_recvb(g_req);
}

void
init_master_req(){
	int rc;
	void *context;
	config_t *cfg;

	cfg = get_config();
    context = s_context();
    assert(cfg->master_gate_rep != NULL);
    g_req = zmq_socket (context, ZMQ_REQ);
    rc = zmq_connect (g_req, cfg->master_gate_rep);
    assert(rc == 0);
    printf("Worker master request connect on %s ready\n",cfg->master_gate_rep);
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
	init_master_req();
	init_master_sub();
}


