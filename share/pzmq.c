/*
 * pzmq.c
 *
 *  Created on: Jul 31, 2012
 *      Author: luan
 */
#include <zmq.h>
#include <string.h>
#include <stdlib.h>

void *
s_context(){
	static void *context = NULL;
	if(context != NULL){
		return context;
	}
	context = zmq_init (1);
	return context;
}

inline char *
s_recv (void *socket) {
    zmq_msg_t message;
    zmq_msg_init (&message);
    if (zmq_recvmsg (socket, &message, ZMQ_DONTWAIT) < 0)
        return (NULL);
    int size = zmq_msg_size (&message);
    char *string = malloc (size + 1);
    memcpy (string, zmq_msg_data (&message), size);
    zmq_msg_close (&message);
    string [size] = 0;
    return (string);
}


inline int
s_send (void *socket,const char *string) {
    int rc;
    zmq_msg_t message;
    zmq_msg_init_size (&message, strlen (string));
    memcpy (zmq_msg_data (&message), string, strlen (string));
    rc = zmq_sendmsg (socket, &message, 0);
    zmq_msg_close (&message);
    return (rc);
}

inline int
s_sendm (void *socket,const char *string) {
    int rc;
    zmq_msg_t message;
    zmq_msg_init_size (&message, strlen (string));
    memcpy (zmq_msg_data (&message), string, strlen (string));
    rc = zmq_sendmsg (socket, &message, ZMQ_SNDMORE);
    zmq_msg_close (&message);
    return (rc);
}

