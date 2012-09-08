/*
 * worker.h
 *
 *  Created on: Jul 31, 2012
 *      Author: luan
 */

#ifndef WORKER_H_
#define WORKER_H_

#include "../share/gdef.h"

void *get_worker_pull();
void init_worker_pull();
void init_worker_router();
void send_message_worker(char *work, char *id, char *msg);
msg_t *recv_message_worker();


#endif /* WORKER_H_ */
