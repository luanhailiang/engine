/*
 * option.h
 *
 *  Created on: Aug 29, 2012
 *      Author: luan
 */

#ifndef OPTION_H_
#define OPTION_H_

#define OPT_MASTER	1
#define OPT_GATE	2
#define OPT_WORKER	3

void handle_args_opt(int argc, char **argv, int type);

#endif /* OPTION_H_ */
