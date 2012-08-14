/*
 * worker.h
 *
 *  Created on: Jul 31, 2012
 *      Author: luan
 */

#ifndef WORKER_H_
#define WORKER_H_

void init_worker_bind();
void send_message(char *work, char *id, char *msg);
char *recv_message();


#endif /* WORKER_H_ */
