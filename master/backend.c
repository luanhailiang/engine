/*
 * backend.c
 *
 *  Created on: Aug 29, 2012
 *      Author: luan
 */

#include "gate.h"
#include "worker.h"
#include "binding.h"
#include "../share/gdef.h"
#include "../share/config.h"


static void
_sig_alrm(int sigo){
}
static int
_ualarm (int usecs, int reload) {
    struct itimerval new, old;
    new.it_interval.tv_usec = reload % 1000000;
    new.it_interval.tv_sec = reload / 1000000;

    new.it_value.tv_usec = usecs % 1000000;
    new.it_value.tv_sec = usecs / 1000000;

    if (setitimer(ITIMER_REAL, &new, &old) == 0)
    	return old.it_value.tv_sec * 1000000 + old.it_value.tv_usec;
    /* else */
    return -1;
}

static void
_handle_gate_message(char *id, char * msg){
	//TODO handle message
	call_gate_message(id,msg);
}
static void
_handle_worker_message(char *id, char * msg){
	//TODO handle message
	call_worker_message(id,msg);
}
void backend(){
	msg_t *msgs;
	void *gate_router;
	void *work_router;
	config_t *cfg;

    if(signal(SIGALRM,_sig_alrm) == SIG_ERR){
    	exit(1);
    }
    cfg = get_config();
    _ualarm(cfg->heart_beat_time,cfg->heart_beat_time);

	init_work_pub();
	init_work_router();

	init_gate_pub();
	init_gate_router();

	init_lua_binding();

	gate_router = get_gate_router();
	work_router = get_work_router();

	// Initialize poll set
	zmq_pollitem_t items [] = {
		{ gate_router, 0, ZMQ_POLLIN, 0 },
		{ work_router, 0, ZMQ_POLLIN, 0 }
	};
	// Process messages from both sockets
	while (1) {
		zmq_poll (items, 2, 1000);
		if (items [0].revents & ZMQ_POLLIN) {
	        while((msgs = recv_message_gate()) != NULL){
	        	_handle_gate_message(msgs->id,msgs->msg);
	        }
		}
		if (items [1].revents & ZMQ_POLLIN) {
	        while((msgs = recv_message_work()) != NULL){
	        	_handle_worker_message(msgs->id,msgs->msg);
	        }
		}
	}
}
