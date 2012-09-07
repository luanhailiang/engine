/*
 * binding.h
 *
 *  Created on: Sep 7, 2012
 *      Author: luan
 */

#ifndef BINDING_H_
#define BINDING_H_

void init_lua_binding();
void call_player_message(char *msg);
void call_worker_message(char *msg);
void call_master_message(char *msg);

#endif /* BINDING_H_ */
