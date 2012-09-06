/*
 * config.h
 *
 *  Created on: Aug 28, 2012
 *      Author: luan
 */

#ifndef CONFIG_H_
#define CONFIG_H_


typedef struct config_s{
	int gate_id;
	int work_id;

	int gate_client_port;
	int heart_beat_time;

	char *ip;
	char *eth_name;
	char *config_file;


	char *worker_start_pub;
	char *gate_work_router;

	char *master_gate_pub;
	char *master_gate_rep;
	char *master_work_pub;
	char *master_work_rep;


	char *master_lua_file;
	char *master_lua_path;
	char *master_lua_cpath;

	char *master_lua_beat;
	char *master_lua_gate;
	char *master_lua_worker;


	char *worker_lua_file;
	char *worker_lua_path;
	char *worker_lua_cpath;

	char *worker_lua_beat;
	char *worker_lua_master;
	char *worker_lua_player;
	char *worker_lua_worker;

}config_t;

void load_config();
config_t *get_config();
void set_work_id(int id);
void set_gate_id(int gate);
void set_cfg_dir(char *file);

#endif /* CONFIG_H_ */
