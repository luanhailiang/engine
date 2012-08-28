/*
 * main.c
 *
 *  Created on: Jul 31, 2012
 *      Author: luan
 */
#include <zmq.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>

#include "../share/pzmq.h"

static void
sig_alrm(int sigo){
	time_t t;
	t=time(&t);
	printf("signal:%d time:%s\n",sigo,ctime(&t));
}
int _ualarm (int usecs, int reload) {
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
int main(){
	int rc;
	char *msg;
    void *context = zmq_init (1);
    void *worker = zmq_socket (context, ZMQ_DEALER);
    zmq_setsockopt (worker, ZMQ_IDENTITY, "A", 1);
    rc = zmq_connect (worker, "ipc://routing.ipc");
    assert(rc == 0);
    if(signal(SIGALRM,sig_alrm) == SIG_ERR){
    	exit(1);
    }
    _ualarm(100000,1000000);
    printf("connect ok start loop : %d\n",rc);
    while(1){
    	//TODO handle message
        zmq_pollitem_t items [] = {
            { worker, 0, ZMQ_POLLIN, 0 },
        };
        zmq_poll (items, 1, 0);
        s_send (worker, "luanhailiang");
        if (items [0].revents & ZMQ_POLLIN) {
            while((msg = s_recv(worker)) != NULL){
            	s_sendm (worker, "***");
                s_send (worker, msg);
                s_send (worker, msg);
            	free(msg);
            }
        }
    }
    zmq_close (worker);
    zmq_term (context);
}
