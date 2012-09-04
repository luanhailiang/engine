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
handle_args_opt(int argc, char **argv){
	int c;
	int gate = 0;
	int work = 0;
	while ((c = getopt(argc, argv, "c: b:")) != EOF){
		switch (c){
		case 'c':
			set_cfg_dir(optarg);
			break;
		case 'b':
			_bind_cpu(atoi(optarg));
			break;
		default:
			printf("Unknown option '%c'.", c);
			break;
		}
	}
}
