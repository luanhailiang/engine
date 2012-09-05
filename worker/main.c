/*
 * main.c
 *
 *  Created on: Jul 31, 2012
 *      Author: luan
 */

#include "backend.h"
#include "../share/gdef.h"
#include "../share/pzmq.h"
#include "../share/option.h"
#include "../share/config.h"

int main(int argc, char **argv){

	//handle arguments
	handle_args_opt(argc,argv);
	//load configure data
	load_config();

	//start loop
	backend();
}
