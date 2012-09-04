/*
 * worker.c
 * worker handle message with worker
 *  Created on: Aug 28, 2012
 *      Author: luan
 */

#include "../share/gdef.h"
#include "../share/config.h"

static void *g_pub = NULL;
static void *g_sub = NULL;

void
init_worker_pub(){
	int rc;
	char addr[50];
	void *context;
	config_t *cfg;

	cfg = get_config();
    context = s_context();
    assert(cfg->work_id == 0);
    assert(cfg->worker_start_pub == NULL);
    sprintf(addr,cfg->worker_start_pub,cfg->work_id);
    g_pub = zmq_socket (context, ZMQ_PUB);
    rc = zmq_bind (g_pub, addr);
    assert(rc == 0);
    printf("Worker worker pub bind on %s ready\n",addr);
}
void
init_worker_sub(){
	char id[5];
	void *context;
	config_t *cfg;

	cfg = get_config();
    context = s_context();
    assert(cfg->work_id == 0);
    sprintf(id,"%03d",cfg->work_id);
    g_sub = zmq_socket (context, ZMQ_SUB);
    zmq_setsockopt (context, ZMQ_SUBSCRIBE, "***", 3);
	zmq_setsockopt (context, ZMQ_SUBSCRIBE, id, strlen(id));
	printf("Worker worker sub is ready\n");
}
void
worker_sub_connect(char *addr){
	assert(g_sub == NULL);
	zmq_connect (g_sub, addr);
}
void
send_message_worker(char *id, char *msg){
	s_sendm(g_pub,id);
	s_send(g_pub,msg);
}

char *
recv_message_worker(){
	return s_recv(g_sub);
}
