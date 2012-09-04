/*
 * worker.h
 *
 *  Created on: Sep 5, 2012
 *      Author: luan
 */

#ifndef WORKER_H_
#define WORKER_H_

void init_worker_pub();
void init_worker_sub();
void worker_sub_connect(char *addr);
void send_message_worker(char *id, char *msg);
char * recv_message_worker();
void * get_worker_sub();

#endif /* WORKER_H_ */
