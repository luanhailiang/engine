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
#include "../share/gdef.h"
#include "../share/config.h"
#include "../share/lua_env.h"


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
	send_message_work(id, msg);
	return 0;
}

static int
_message_all_gate(lua_State *L){
	const char *msg = luaL_checkstring(L, 1);
	pub_message_gate(msg);
	return 0;
}

static int
_message_all_worker(lua_State *L){
	const char *msg = luaL_checkstring(L, 1);
	pub_message_work(msg);
	return 0;
}

void
init_lua_binding(){
	config_t *cfg;
	cfg = get_config();

	init_lua_state(cfg->master_lua_file,
			cfg->master_lua_path,
			cfg->master_lua_cpath);

	reg_function("message_worker",_message_worker);
	reg_function("message_all_gate",_message_all_gate);
	reg_function("message_all_worker",_message_all_worker);
}

void
call_gate_message(char *msg){
	char *back;
	config_t *cfg;
	cfg = get_config();
	call_va(cfg->master_lua_gate,"s>s",msg,&back);
	back_message_gate(back);
}
void
call_worker_message(char *msg){
	char *back;
	config_t *cfg;
	cfg = get_config();
	call_va(cfg->master_lua_worker,"s>s",msg,&back);
	back_message_work(back);
}
