/*
 * worker.h
 *
 *  Created on: Sep 4, 2012
 *      Author: luan
 */

#ifndef WORKER_H_
#define WORKER_H_

void init_work_rep();
void init_work_pub();
void send_message_work(char *id, char *msg);
char *recv_message_work();
void back_message_work(char *msg);
void *get_work_rep();

#endif /* WORKER_H_ */
