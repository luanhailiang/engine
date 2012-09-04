/*
 * error.h
 *
 *  Created on: Sep 4, 2012
 *      Author: luan
 */

#ifndef ERROR_H_
#define ERROR_H_

void	log_msg(const char *, ...);			/* {App misc_source} */
void	log_open(const char *);
void	log_quit(const char *, ...);
void	log_ret(const char *, ...);
void	log_sys(const char *, ...);

#endif /* ERROR_H_ */
