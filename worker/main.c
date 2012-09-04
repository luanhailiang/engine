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

#include "backend.h"
#include "../share/pzmq.h"
#include "../share/option.h"
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
int main(int argc, char **argv){
    if(signal(SIGALRM,_sig_alrm) == SIG_ERR){
    	exit(1);
    }
    _ualarm(100000,1000000);

	//handle arguments
	handle_args_opt(argc,argv);
	//load configure data
	load_config();

	//start loop
	backend();
}
