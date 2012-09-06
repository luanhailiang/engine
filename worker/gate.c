/*
 * gate.c
 * worker handle message with gate
 *  Created on: Aug 28, 2012
 *      Author: luan
 */


#include "../share/gdef.h"
#include "../share/pzmq.h"
#include "../share/config.h"

static void *g_gate = NULL;

void *
get_gate_dealer(){
	return g_gate;
}

void
send_message_gate(char *id, char *msg){
	s_sendm(g_gate,id);
	s_send(g_gate,msg);
}

char *
recv_message_gate(){
    int64_t more;           //  Multipart detection
    size_t more_size = sizeof (more);
    zmq_getsockopt (g_gate, ZMQ_RCVMORE, &more, &more_size);
	return s_recv(g_gate);
}

void
init_gate_connect(){
	int rc;
	char id[5];
	void *context;

	config_t *cfg;
	cfg = get_config();
	assert(cfg->work_id != 0);
	sprintf(id,"%03d",cfg->work_id);
	context = s_context();
	g_gate = zmq_socket (context, ZMQ_DEALER);
    zmq_setsockopt (g_gate, ZMQ_IDENTITY, id, strlen(id));
    rc = zmq_connect (g_gate, cfg->gate_work_router);
    assert(rc == 0);
}
