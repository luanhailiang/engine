/*
 * gate.c
 * worker handle message with gate
 *  Created on: Aug 28, 2012
 *      Author: luan
 */

#include <zmq.h>
#include <stddef.h>

void *g_context = NULL;
void *g_gate = NULL;

void
init_gate_connect(char *gate_addr){
	int rc;
	char id[5];
	config_t *cfg;
	cfg = get_config();
	sprintf(id,"%03d",cfg->work_id);
	g_context = zmq_init (1);
	g_gate = zmq_socket (g_context, ZMQ_DEALER);
    zmq_setsockopt (g_gate, ZMQ_IDENTITY, id, strlen(id));
    rc = zmq_connect (g_gate, gate_addr);
    assert(rc == 0);
}

