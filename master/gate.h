/*
 * gate.h
 *
 *  Created on: Sep 4, 2012
 *      Author: luan
 */

#ifndef GATE_H_
#define GATE_H_

#include "../share/gdef.h"

void init_gate_router();
void init_gate_pub();
void pub_message_gate(const char *msg);
void send_message_gate(char *id, const char *msg);
msg_t *recv_message_gate();
void *get_gate_router();

#endif /* GATE_H_ */
