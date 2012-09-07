/*
 * lua_env.h
 *
 *  Created on: Sep 6, 2012
 *      Author: luan
 */

#ifndef LUA_ENV_H_
#define LUA_ENV_H_


lua_State *get_lua_state();
void init_lua_state(char *file, char *path, char *cpath);
void reg_function(char *name, void *func);
void call_va (const char *func, const char *sig, ...);

#endif /* LUA_ENV_H_ */
