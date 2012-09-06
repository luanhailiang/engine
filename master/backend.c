/*
 * backend.c
 *
 *  Created on: Aug 29, 2012
 *      Author: luan
 */

#include "gate.h"
#include "worker.h"
#include "../share/gdef.h"

static void
_handle_gate_message(char * msg){
	//TODO handle message
	printf("Master from gate : %s\n",msg);
}
static void
_handle_worker_message(char * msg){
	//TODO handle message
	printf("Master from worker : %s\n",msg);
}
void backend(){
	char *msg;

	void *gate_rep;
	void *work_rep;


	init_work_rep();
	init_work_pub();

	init_gate_rep();
	init_gate_pub();

	gate_rep = get_gate_rep();
	work_rep = get_work_rep();

	// Initialize poll set
	zmq_pollitem_t items [] = {
		{ gate_rep, 0, ZMQ_POLLIN, 0 },
		{ work_rep, 0, ZMQ_POLLIN, 0 }
	};
	// Process messages from both sockets
	while (1) {
		zmq_msg_t message;
		zmq_poll (items, 2, 1000);
		if (items [0].revents & ZMQ_POLLIN) {
	        while((msg = recv_message_gate()) != NULL){
	        	_handle_gate_message(msg);
	        	free(msg);
	        }
		}
		if (items [1].revents & ZMQ_POLLIN) {
	        while((msg = recv_message_work()) != NULL){
	        	_handle_worker_message(msg);
	        	free(msg);
	        }
		}
	}
}
