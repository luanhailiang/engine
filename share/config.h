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

	char *worker_start_pub;
	char *gate_work_router;

	char *master_gate_pub;
	char *master_gate_rep;
	char *master_work_pub;
	char *master_work_rep;

	char *config_file;

	char *eth_name;
	char *ip;
}config_t;

void load_config();
config_t *get_config();
void set_work_id(int id);
void set_gate_id(int gate);
void set_cfg_dir(char *file);

#endif /* CONFIG_H_ */
