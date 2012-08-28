/*
 * pub_sub.h
 *
 *  Created on: Aug 28, 2012
 *      Author: luan
 */

#ifndef PUB_SUB_H_
#define PUB_SUB_H_

void *zmq_pub_init(void *context, char *addr);
int zmq_pub_msg(void* pub, char *bgn, char *msg);

void *zmq_sub_init(void *context, char *addr);
void zmq_sub_set(void *sub, char *bgn);
char *zmq_sub_msg(void *sub);

#endif /* PUB_SUB_H_ */
