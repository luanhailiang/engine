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

typedef struct var_p_s{
	char *var;
	void *p;
}var_p_t;

config_t *
get_config(){
	static config_t *config = NULL;
	if(config != NULL){
		return config;
	}
	config = calloc(1,sizeof(config_t));
	return config;
}

void
set_work_id(int id){
	config_t * config = get_config();
	config->work_id = id;
	printf("Worker id is :%d\n",id);
}

void
set_gate_id(int gate){
	config_t * config = get_config();
	config->gate_id = gate;
	printf("System link gate is :%d\n",gate);
}

void
set_cfg_dir(char *file){
	config_t * config = get_config();
	size_t size = strlen(file);
    char *string = malloc (size + 1);
    memcpy (string, file, size);
    config->config_file = string;
    printf("System using configure:%s\n",file);
}

void
_get_lua_value(lua_State *L, char *var, void *p){
	lua_getglobal(L, var);
	if(lua_isnumber(L, -1)){
		*(int*)p = (int)lua_tonumber(L,-1);
//		printf("cfg: %-20s = %d\n",var,*(int*)p);
	}else{
		*(char**)p = (char *)lua_tostring(L, -1);
//		printf("cfg: %-20s = %s\n",var,*(char**)p);
	}
	lua_pop(L, 1);
}

void
load_config(){
	char *file;
	config_t *cfg = get_config();
	lua_State *L=luaL_newstate();

	luaL_openlibs(L);
	file = cfg->config_file;
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
	var_p_t list[] = {
			{"eth_name",			&(cfg->eth_name)},
			{"gate_client_port",	&(cfg->gate_client_port)},
			{"heart_beat_time" ,	&(cfg->heart_beat_time)},

			{"worker_start_pub",	&(cfg->worker_start_pub)},
			{"gate_work_router",	&(cfg->gate_work_router)},
			{"master_gate_pub", 	&(cfg->master_gate_pub)},
			{"master_gate_rep", 	&(cfg->master_gate_rep)},
			{"master_work_pub", 	&(cfg->master_work_pub)},
			{"master_work_rep", 	&(cfg->master_work_rep)},

			{"master_lua_file",		&(cfg->master_lua_file)},
			{"master_lua_path",		&(cfg->master_lua_path)},
			{"master_lua_cpath",	&(cfg->master_lua_cpath)},

			{"master_lua_beat",		&(cfg->master_lua_beat)},
			{"master_lua_gate",		&(cfg->master_lua_gate)},
			{"master_lua_worker",	&(cfg->master_lua_worker)},

			{"worker_lua_file",		&(cfg->worker_lua_file)},
			{"worker_lua_path",		&(cfg->worker_lua_path)},
			{"worker_lua_cpath",	&(cfg->worker_lua_cpath)},

			{"worker_lua_beat",		&(cfg->worker_lua_beat)},
			{"worker_lua_master",	&(cfg->worker_lua_master)},
			{"worker_lua_player",	&(cfg->worker_lua_player)},
			{"worker_lua_worker",	&(cfg->worker_lua_worker)},

			{NULL,NULL}
	};
	int i;
	for(i = 0; list[i].var != NULL; i++){
		_get_lua_value(L,list[i].var,list[i].p);
	}
	cfg->ip = get_ip(cfg->eth_name);
	//lua_close(L);
}
