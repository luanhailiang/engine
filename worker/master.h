/*
 * master.h
 *
 *  Created on: Sep 5, 2012
 *      Author: luan
 */

#ifndef MASTER_H_
#define MASTER_H_

void send_message_master(const char *msg);
char * recv_message_master();
char * back_message_master();
void init_master_dealer();
void init_master_sub();
void init_master_connect();
void *get_master_sub();
void *get_master_dealer();


#endif /* MASTER_H_ */
