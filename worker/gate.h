/*
 * gate.h
 *
 *  Created on: Sep 5, 2012
 *      Author: luan
 */

#ifndef GATE_H_
#define GATE_H_

void init_gate_connect();
void send_message_gate(char *id,const char *msg);
char *recv_message_gate();
void *get_gate_dealer();

#endif /* GATE_H_ */
