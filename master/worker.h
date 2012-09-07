/*
 * worker.h
 *
 *  Created on: Sep 4, 2012
 *      Author: luan
 */

#ifndef WORKER_H_
#define WORKER_H_

void init_work_router();
void init_work_pub();
void pub_message_work(const char *msg);
void send_message_work(char *id, const char *msg);
char *recv_message_work();
void back_message_work(const char *msg);
void *get_work_router();

#endif /* WORKER_H_ */
