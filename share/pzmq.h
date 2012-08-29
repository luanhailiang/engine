/*
 * pzmq.h
 * package of czmq
 *  Created on: Jul 31, 2012
 *      Author: luan
 */

#ifndef PZMQ_H_
#define PZMQ_H_
void *s_context();
inline char * s_recv (void *socket);
inline int s_send (void *socket, char *string);
inline int s_sendm (void *socket, char *string);

#endif /* PZMQ_H_ */
