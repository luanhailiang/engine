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


void *
get_work_router(){
	return g_router;
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

msg_t *
recv_message_work(){
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
