
#include "swap.h"
#include "client.h"
#include "worker.h"
#include "../share/gdef.h"
#include "../share/option.h"
#include "../share/config.h"

static void
_shut_down(int sig){
	printf("Gate shut down by Ctrl+c : %d\n",sig);
	exit(0);
}

int
main(int argc, char **argv){
	//add signal handle
	signal(SIGINT, _shut_down);
	//handle arguments
	handle_args_opt(argc,argv);
	//load configure data
	load_config();
	//start listen client
	init_client_bind();
	//start listen worker
	init_worker_router();
	//start handle message
	start_loop();
}
