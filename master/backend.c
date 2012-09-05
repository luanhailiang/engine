/*
 * backend.c
 *
 *  Created on: Aug 29, 2012
 *      Author: luan
 */

#include "gate.h"
#include "worker.h"
#include "../share/gdef.h"

void backend(){
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
			zmq_msg_init (&message);
			zmq_recvmsg (gate_rep, &message, 0);
			zmq_sendmsg (gate_rep, &message, 0);
			// Process task
			printf("GATE:%s\n",(char *)zmq_msg_data (&message));
			zmq_msg_close (&message);
		}
		if (items [1].revents & ZMQ_POLLIN) {
			zmq_msg_init (&message);
			zmq_recvmsg (work_rep, &message, 0);
			// Process weather update
			printf("WORK:%s\n",(char *)zmq_msg_data (&message));
			zmq_msg_close (&message);
		}
		//send_message_gate("luanqibazao");
	}
}
