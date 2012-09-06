/*
 * worker.c
 * master handle message with worker
 *  Created on: Aug 28, 2012
 *      Author: luan
 */


#include "../share/gdef.h"
#include "../share/pzmq.h"
#include "../share/config.h"

static void *g_rep = NULL;
static void *g_pub = NULL;

void *
get_work_rep(){
	return g_rep;
}

void
send_message_work(char *id, char *msg){
	s_sendm(g_pub,id);
	s_send(g_pub,msg);
}

char *
recv_message_work(){
	return s_recv(g_rep);
}

void
back_message_work(char *msg){
	s_send(g_rep,msg);
}


void
init_work_rep(){
	int rc;
	void *context;
	config_t *cfg;

	cfg = get_config();
    context = s_context();
    assert(cfg->master_work_rep != NULL);
    g_rep = zmq_socket (context, ZMQ_REP);
    rc = zmq_bind (g_rep, cfg->master_work_rep);
    assert(rc == 0);
    printf("Master work reply bind on %s ready\n",cfg->master_work_rep);
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
