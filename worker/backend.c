/*
 * backend.c
 *
 *  Created on: Aug 29, 2012
 *      Author: luan
 */



#include "gate.h"
#include "worker.h"
#include "master.h"
#include "../share/gdef.h"

void backend(){
	void *master_rep;
	void *master_sub;
	void *worker_sub;
	void *gate_dealer;

	//connect master
	init_master_connect();

	init_gate_connect();
	init_worker_pub();
	init_worker_sub();

	master_sub = get_master_sub();
	master_rep = get_master_req();
	worker_sub = get_worker_sub();
	gate_dealer = get_gate_dealer();

	// Initialize poll set
	zmq_pollitem_t items [] = {
		{ master_sub, 0, ZMQ_POLLIN, 0 },
		{ master_rep, 0, ZMQ_POLLIN, 0 },
		{ worker_sub, 0, ZMQ_POLLIN, 0 },
		{ gate_dealer, 0, ZMQ_POLLIN, 0 }
	};
	// Process messages from both sockets
	while (1) {
		zmq_msg_t message;
		zmq_poll (items, 4, -1);
		if (items [0].revents & ZMQ_POLLIN) {
			zmq_msg_init (&message);
			zmq_recvmsg (master_sub, &message, 0);
			// Process task
			printf("MASTER_SUB:%s\n",(char *)zmq_msg_data (&message));
			zmq_msg_close (&message);
		}
		if (items [1].revents & ZMQ_POLLIN) {
			zmq_msg_init (&message);
			zmq_recvmsg (master_rep, &message, 0);
			// Process weather update
			printf("MASTER_REP:%s\n",(char *)zmq_msg_data (&message));
			zmq_msg_close (&message);
		}
		if (items [2].revents & ZMQ_POLLIN) {
			zmq_msg_init (&message);
			zmq_recvmsg (worker_sub, &message, 0);
			// Process weather update
			printf("WORKER_SUB:%s\n",(char *)zmq_msg_data (&message));
			zmq_msg_close (&message);
		}
		if (items [3].revents & ZMQ_POLLIN) {
			zmq_msg_init (&message);
			zmq_recvmsg (gate_dealer, &message, 0);
			// Process weather update
			printf("GATE_DEALER:%s\n",(char *)zmq_msg_data (&message));
			zmq_msg_close (&message);
		}
	}
}
