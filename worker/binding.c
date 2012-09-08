/*
 * binding.c
 *
 *  Created on: Sep 7, 2012
 *      Author: luan
 */



#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include "gate.h"
#include "worker.h"
#include "master.h"
#include "../share/gdef.h"
#include "../share/config.h"
#include "../share/lua_env.h"

static int
_message_master(lua_State *L){
	const char *msg = luaL_checkstring(L, 1);
	send_message_master(msg);
	return 0;
}

static int
_message_player(lua_State *L){
	char *id;
	char sid[5];
	if(lua_isnumber(L, 1)){
		sprintf(sid,"%03d",(int)lua_tonumber(L, 1));
		id = sid;
	}else{
		id = (char *)lua_tostring(L, 1);
	}
	const char *msg = luaL_checkstring(L, 2);
	send_message_gate(id, msg);
	return 0;
}

static int
_message_worker(lua_State *L){
	char *id;
	char sid[5];
	if(lua_isnumber(L, 1)){
		sprintf(sid,"%03d",(int)lua_tonumber(L, 1));
		id = sid;
	}else{
		id = (char *)lua_tostring(L, 1);
	}
	const char *msg = luaL_checkstring(L, 2);
	send_message_worker(id, msg);
	return 0;
}

static int
_message_all_worker(lua_State *L){
	const char *msg = luaL_checkstring(L, 1);
	send_message_worker("***", msg);
	return 0;
}

void
init_lua_binding(){
	config_t *cfg;
	cfg = get_config();

	init_lua_state(cfg->worker_lua_file,
			cfg->worker_lua_path,
			cfg->worker_lua_cpath);

	reg_function("message_master",_message_master);
	reg_function("message_player",_message_player);
	reg_function("message_worker",_message_worker);
	reg_function("message_all_worker",_message_all_worker);
}

void
call_player_message(char *id,char *msg){
	config_t *cfg;
	cfg = get_config();
	call_va(cfg->worker_lua_player,"ss>",id,msg);
}

void
call_worker_message(char *id,char *msg){
	config_t *cfg;
	cfg = get_config();
	call_va(cfg->worker_lua_worker,"ss>",id,msg);
}

void
call_master_message(char *msg){
	config_t *cfg;
	cfg = get_config();
	call_va(cfg->worker_lua_master,"s>",msg);
}
