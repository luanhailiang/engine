


#include <stdio.h>
#include <errno.h>
#include <signal.h>
#include <stdlib.h>

#include "client.h"
#include "worker.h"

void shut_down(int sig){
	printf("System shut down by Ctrl+c : %d\n",sig);
	exit(0);
}

int
main(int argc, char **argv){
	signal(SIGINT, shut_down);
	//start listen client
	init_client_bind();
	//start listen worker
	init_worker_bind();
	//start handle message
	start_loop();
}
