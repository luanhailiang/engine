/*
 * binding.h
 *
 *  Created on: Sep 7, 2012
 *      Author: luan
 */

#ifndef BINDING_H_
#define BINDING_H_

void init_lua_binding();
void call_gate_message(char *id, char *msg);
void call_worker_message(char *id, char *msg);

#endif /* BINGING_H_ */
