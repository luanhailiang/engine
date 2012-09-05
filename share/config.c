/*
 * config.c
 * handle config
 *  Created on: Aug 28, 2012
 *      Author: luan
 */

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include <stdlib.h>
#include <stddef.h>
#include <string.h>

#include "config.h"
#include "utils.h"

config_t *
get_config(){
	static config_t *config = NULL;
	if(config != NULL){
		return config;
	}
	config = calloc(1,sizeof(config_t));
	return config;
}

void set_work_id(int id){
	config_t * config = get_config();
	config->work_id = id;
	printf("Worker id is :%d\n",id);
}

void set_gate_id(int gate){
	config_t * config = get_config();
	config->gate_id = gate;
	printf("System link gate is :%d\n",gate);
}

void set_cfg_dir(char *file){
	config_t * config = get_config();
	size_t size = strlen(file);
    char *string = malloc (size + 1);
    memcpy (string, file, size);
    config->config_file = string;
    printf("System using configure:%s\n",file);
}

void
load_config(){
	char *file;
	config_t *config = get_config();
	lua_State *L=luaL_newstate();
	luaL_openlibs(L);
	file = config->config_file;
	if(file == NULL){
		fprintf(stderr,"No configure file\n");
		exit(EXIT_FAILURE);
	}
	int err = luaL_dofile(L, file);
	if (err) {
		fprintf(stderr,"%s\n",lua_tostring(L,-1));
		lua_close(L);
		exit(EXIT_FAILURE);
	}
	lua_getglobal(L,"gate_client_port");
	config->gate_client_port = (int)lua_tonumber(L,-1);
	lua_pop(L,1);

	lua_getglobal(L,"heart_beat_time");
	config->heart_beat_time = (int)lua_tonumber(L,-1);
	lua_pop(L,1);

	lua_getglobal(L,"worker_start_pub");
	config->worker_start_pub = (char *)lua_tostring(L,-1);
	lua_pop(L,1);
	lua_getglobal(L,"gate_work_router");
	config->gate_work_router = (char *)lua_tostring(L,-1);
	lua_pop(L,1);

	lua_getglobal(L,"master_gate_pub");
	config->master_gate_pub = (char *)lua_tostring(L,-1);
	lua_pop(L,1);
	lua_getglobal(L,"master_gate_rep");
	config->master_gate_rep = (char *)lua_tostring(L,-1);
	lua_pop(L,1);
	lua_getglobal(L,"master_work_pub");
	config->master_work_pub = (char *)lua_tostring(L,-1);
	lua_pop(L,1);
	lua_getglobal(L,"master_work_rep");
	config->master_work_rep = (char *)lua_tostring(L,-1);
	lua_pop(L,1);

	lua_getglobal(L,"eth_name");
	config->eth_name = (char *)lua_tostring(L,-1);
	lua_pop(L,1);
	//lua_close(L);
	config->ip = get_ip(config->eth_name);
}
