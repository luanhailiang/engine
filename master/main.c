/*
 * main.c
 * master handle
 *  Created on: Aug 28, 2012
 *      Author: luan
 */


#include "../share/gdef.h"

static void
_shut_down(int sig){
	printf("System shut down by Ctrl+c : %d\n",sig);
	exit(0);
}


int
main(int argc, char **argv){
	//add signal handle
	signal(SIGINT, _shut_down);
}
