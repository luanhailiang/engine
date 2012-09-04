/*
 * master.h
 *
 *  Created on: Sep 4, 2012
 *      Author: luan
 */

#ifndef MASTER_H_
#define MASTER_H_

void send_message_master(char *msg);
char *recv_message_master();
char *back_message_master();
char *wait_message_master();
void init_master_req();
void init_master_sub();
void init_master_connect();

#endif /* MASTER_H_ */
