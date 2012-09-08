/*
 * gdef.h
 *
 *  Created on: Aug 28, 2012
 *      Author: luan
 */

#ifndef GDEF_H_
#define GDEF_H_

#include <zmq.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <assert.h>
#include <signal.h>

#include <time.h>
#include <sys/time.h>


typedef struct msg_s{
	char *id;
	char *msg;
}msg_t;

#endif /* GDEF_H_ */
