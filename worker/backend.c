/*
 * backend.c
 *
 *  Created on: Aug 29, 2012
 *      Author: luan
 */



#include "gate.h"
#include "worker.h"
#include "master.h"
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
_handle_master_message(char *msg){
	//TODO handle message
	call_master_message(msg);
}
static void
_handle_worker_message(char *msg){
	//TODO handle message
	call_worker_message(msg);
}
static void
_handle_player_message(char *msg){
	//TODO handle message
	call_player_message(msg);
}

void backend(){
	char *msg;

	void *master_dealer;
	void *master_sub;
	void *worker_sub;
	void *gate_dealer;
	config_t *cfg;

    if(signal(SIGALRM,_sig_alrm) == SIG_ERR){
    	exit(1);
    }
    cfg = get_config();
    _ualarm(cfg->heart_beat_time,cfg->heart_beat_time);

	//Initialize socket connects
	init_master_connect();
	init_gate_connect();
	init_worker_pub();
	init_worker_sub();

	init_lua_binding();

	master_sub = get_master_sub();
	master_dealer = get_master_dealer();
	worker_sub = get_worker_sub();
	gate_dealer = get_gate_dealer();

	// Initialize poll set
	zmq_pollitem_t items [] = {
		{ master_sub, 0, ZMQ_POLLIN, 0 },
		{ master_dealer, 0, ZMQ_POLLIN, 0 },
		{ worker_sub, 0, ZMQ_POLLIN, 0 },
		{ gate_dealer, 0, ZMQ_POLLIN, 0 }
	};
	// Process messages from both sockets
	while (1) {
		zmq_poll (items, 4, 1000);
		if (items [0].revents & ZMQ_POLLIN) {
	        while((msg = recv_message_master()) != NULL){
	        	_handle_master_message(msg);
	        	free(msg);
	        }
		}
		if (items [1].revents & ZMQ_POLLIN) {
	        while((msg = back_message_master()) != NULL){
	        	_handle_master_message(msg);
	        	free(msg);
	        }
		}
		if (items [2].revents & ZMQ_POLLIN) {
	        while((msg = recv_message_worker()) != NULL){
	        	_handle_worker_message(msg);
	        	free(msg);
	        }
		}
		if (items [3].revents & ZMQ_POLLIN) {
	        while((msg = recv_message_gate()) != NULL){
	        	_handle_player_message(msg);
	        	free(msg);
	        }
		}
	}
}
