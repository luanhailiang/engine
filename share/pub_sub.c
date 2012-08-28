/*
 * pub.c
 *
 *  Created on: Aug 28, 2012
 *      Author: luan
 */

#include <zmq.h>
#include "pzmq.h"

void *
zmq_pub_init(void *context, char *addr){
	int rc;
	void *pub;
	pub = zmq_socket (context, ZMQ_PUB);
	rc = zmq_bind (pub, addr);
	assert(rc == 0);
	printf("zmq_pub:%s is ready\n",addr);
	return pub;
}

int
zmq_pub_msg(void* pub, char *bgn, char *msg){
	int rc;
	char *buf;
	buf = malloc (strlen (bgn)+strlen (msg) + 1);
	sprintf(buf,"%s %s",bgn,msg);
	rc = s_send (pub, buf);
	free(buf);
	return rc;
}


void *
zmq_sub_init(void *context, char *addr){
	int rc;
	void *sub;
	sub = zmq_socket (context, ZMQ_SUB);
	rc = zmq_connect (sub, addr);
	assert(rc == 0);
	printf("zmq_sub:%s is ready\n",addr);
	return sub;
}

void
zmq_sub_set(void *sub, char *bgn){
	zmq_setsockopt (sub, ZMQ_SUBSCRIBE, bgn, strlen (bgn));
}

char *
zmq_sub_msg(void *sub){
	return s_recv(sub);
}
