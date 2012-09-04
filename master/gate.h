/*
 * gate.h
 *
 *  Created on: Sep 4, 2012
 *      Author: luan
 */

#ifndef GATE_H_
#define GATE_H_

void init_gate_rep();
void init_gate_pub();
void send_message_gate(char *id, char *msg);
char *recv_message_gate();
void back_message_gate(char *msg);
void *get_gate_rep();

#endif /* GATE_H_ */
