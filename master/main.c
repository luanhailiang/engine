/*
 * main.c
 * master handle
 *  Created on: Aug 28, 2012
 *      Author: luan
 */

#include "backend.h"
#include "../share/gdef.h"
#include "../share/option.h"
#include "../share/config.h"

static void
_shut_down(int sig){
	printf("System shut down by Ctrl+c : %d\n",sig);
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

	//start loop
	backend();
}
