/*
 * zmqdef.h
 *
 *  Created on: Jul 31, 2012
 *      Author: luan
 */

#ifndef ZMQDEF_H_
#define ZMQDEF_H_

#include <zmq.h>

//  zmq command
#define CMD_READY          "\001"
#define CMD_REQUEST        "\002"
#define CMD_REPLY          "\003"
#define CMD_DISCONNECT     "\005"

static char *zmq_commands [] = {
    NULL,
    "READY",
    "REQUEST",
    "REPLY",
    "DISCONNECT",
};

#endif /* ZMQDEF_H_ */
