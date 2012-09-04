/*
 * master.c
 * worker handle message with master
 *  Created on: Aug 28, 2012
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
    assert(cfg->master_work_rep != NULL);
    g_req = zmq_socket (context, ZMQ_REQ);
    rc = zmq_connect (g_req, cfg->master_gate_rep);
    assert(rc == 0);
    printf("Worker master sub connect on %s ready\n",cfg->master_work_rep);
}

void
init_master_sub(){
	int rc;
	char id[5];
	void *context;
	config_t *cfg;

	cfg = get_config();
    context = s_context();
    assert(cfg->work_id != 0);
    sprintf(id,"%03d",cfg->work_id);
    assert(cfg->master_work_pub != NULL);
    g_sub = zmq_socket (context, ZMQ_SUB);
    zmq_setsockopt (context, ZMQ_SUBSCRIBE, "***", 3);
	zmq_setsockopt (context, ZMQ_SUBSCRIBE, id, strlen(id));
    rc = zmq_connect (g_sub, cfg->master_work_pub);
    assert(rc == 0);
    printf("Worker master sub connect on %s ready\n",cfg->master_work_pub);
}

void
init_master_connect(){
	char *msg;
	init_master_req();
	send_message_master("query my id");
	msg = wait_message_master();
	printf("------------>%s\n",msg);
	return;
	init_master_sub();
}
void *
get_master_sub(){
	return g_sub;
}
void *
get_master_req(){
	return g_req;
}
