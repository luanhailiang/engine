/*
 * option.c
 * handle arguments option
 *  Created on: Aug 29, 2012
 *      Author: luan
 */
#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>

#define __USE_GNU
#include <sched.h>

#include "config.h"
#include "option.h"

static void
_bind_cpu(int cpuidx){
	cpu_set_t mask;
	CPU_ZERO(&mask);
	CPU_SET(cpuidx,&mask);
	if (sched_setaffinity(0, sizeof(mask), &mask) == -1){
		printf("warning: could not set CPU affinity\n");
	}

	CPU_ZERO(&mask);
	if (sched_getaffinity(0, sizeof(mask), &mask) == -1){
		printf("warning: I could not get CPU affinity, continuing...\n");
	}else{
		if (CPU_ISSET(cpuidx, &mask) ){
			printf("This process %d is running on CPU: %d\n",getpid(),cpuidx);
		}
	}
}


void
handle_args_opt(int argc, char **argv, int type){
	int c;
	int gate = 0;
	int work = 0;
	while ((c = getopt(argc, argv, "c: b: w: g:")) != EOF){
		switch (c){
		case 'c':
			set_cfg_dir(optarg);
			break;
		case 'b':
			_bind_cpu(atoi(optarg));
			break;
		case 'w':
			work = atoi(optarg);
			set_work_id(work);
			break;
		case 'g':
			gate = atoi(optarg);
			set_gate_id(gate);
			break;
		default:
			printf("Unknown option '%c'.", c);
			break;
		}
	}
	switch(type){
	case OPT_MASTER:
		break;
	case OPT_WORKER:
		if(!work){
			fprintf(stderr,"Process no work id opt: -w[1-n]\n");
			exit(EXIT_FAILURE);
		}
		/* no break */
	case OPT_GATE:
		if(!gate){
			fprintf(stderr,"Process no gate id opt: -g[1-n]\n");
			exit(EXIT_FAILURE);
		}
		break;
	}
}
