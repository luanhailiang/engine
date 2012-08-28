/*
 * config.c
 *
 *  Created on: Aug 28, 2012
 *      Author: luan
 */

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include <stdlib.h>
#include <stddef.h>

#include "config.h"

config_t *
get_config(){
	static config_t *config = NULL;
	if(config != NULL){
		return config;
	}
	config = malloc(sizeof(config_t));
	return config;
}

void
load_config(char *file){
	config_t *config = get_config();
	lua_State *L=luaL_newstate();
	luaL_openlibs(L);
	int err = luaL_dofile(L, file);
	if (err) {
		fprintf(stderr,"%s\n",lua_tostring(L,-1));
		lua_close(L);
		exit(EXIT_FAILURE);
	}
	lua_getglobal(L,"gate_port");
	config->gate_port = (int)lua_tonumber(L,-1);
	lua_pop(L,1);
	lua_getglobal(L,"gate_addr");
	config->gate_addr = (char *)lua_tostring(L,-1);
	lua_pop(L,1);
	lua_getglobal(L,"gate_router");
	config->gate_router = (char *)lua_tostring(L,-1);
	lua_pop(L,1);
	lua_getglobal(L,"master_pub");
	config->master_pub = (char *)lua_tostring(L,-1);
	lua_pop(L,1);
	lua_getglobal(L,"master_pull");
	config->master_pull = (char *)lua_tostring(L,-1);
	lua_pop(L,1);
	lua_close(L);
}
