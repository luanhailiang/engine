/*
 * config.h
 *
 *  Created on: Aug 28, 2012
 *      Author: luan
 */

#ifndef CONFIG_H_
#define CONFIG_H_


typedef struct config_s{
	int gate_port;
	char *gate_addr;
	char *gate_router;
	char *master_pub;
	char *master_pull;
}config_t;

void load_config(char *file);
config_t *get_config();

#endif /* CONFIG_H_ */
